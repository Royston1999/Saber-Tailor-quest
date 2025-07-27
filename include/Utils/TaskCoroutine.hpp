#pragma once

#include "System/Threading/Tasks/Task.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "custom-types/shared/delegate.hpp"
#include <atomic>
#include <coroutine>
#include <bsml/shared/BSML/MainThreadScheduler.hpp>
#include <exception>
#include <type_traits>
#include <stack>

template<typename T>
concept CSharpTask = std::is_base_of_v<System::Threading::Tasks::Task, T>;

template <CSharpTask T> class CSharpTaskAwaiter;

enum class TaskType {
    INVALID, VOID, SINGLE, MULTI
};

template<typename... TArgs> concept NonVoid = !(std::is_same_v<void, TArgs> || ...);

template<typename... TRetArgs> struct task_type { 
    static constexpr TaskType type = NonVoid<TRetArgs...> ? TaskType::MULTI : TaskType::INVALID; 
};
template<typename T> struct task_type<T> { 
    static constexpr TaskType type = TaskType::SINGLE; 
};
template<> struct task_type<void> { 
    static constexpr TaskType type = TaskType::VOID; 
};

template<typename... TRetArgs> struct task_promise;
template<> struct task_promise<void>;
template <typename... TRetArgs> class TaskCoroutineAwaiter;

struct resumer_coroutine {
    struct resumer_promise {
        void unhandled_exception() noexcept {}
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        resumer_coroutine get_return_object() noexcept { return { std::coroutine_handle<resumer_promise>::from_promise(*this) }; }
        void return_void() {}
    };
    using promise_type = resumer_promise;
    std::coroutine_handle<resumer_promise> handle;
};

template<typename... TRetArgs>
requires (task_type<TRetArgs...>::type != TaskType::INVALID)
class task_coroutine_internal {

    template <typename... Args> friend class promise_internal;

    public:
    using promise_type = task_promise<TRetArgs...>;

    ~task_coroutine_internal() {
        if (!handle) return;
        handle.promise().ref_count--;
        if (!(handle.promise().ref_count) && handle.done()) { handle.destroy(); }
    }

    task_coroutine_internal(const task_coroutine_internal& copy) : handle(copy.handle) { handle.promise().ref_count++; };
    task_coroutine_internal(task_coroutine_internal&& other) noexcept : handle(other.handle) { other.handle = nullptr; }

    protected:
    task_coroutine_internal(std::coroutine_handle<promise_type> handle) : handle(handle) { handle.promise().ref_count++; }
    std::coroutine_handle<promise_type> handle;

    void internal_wait() const { while (!handle.done()) continue; }
};

template<typename... TRetArgs>
struct task_coroutine : public task_coroutine_internal<TRetArgs...> {
    auto await_result() {
        this->internal_wait();
        if constexpr(task_type<TRetArgs...>::type == TaskType::SINGLE) return std::get<TRetArgs...>(this->handle.promise().get_future().get());
        else if constexpr(task_type<TRetArgs...>::type == TaskType::MULTI) return this->handle.promise().get_future().get();
    }
};

template<>
struct task_coroutine<void> : public task_coroutine_internal<void> {
    void wait() { this->internal_wait(); }
};

struct coro_stack {
    std::stack<std::coroutine_handle<>> handles;
    void add_handle(std::coroutine_handle<> handle) {
        handles.push(handle);
    }
    std::coroutine_handle<> pop_handle() {
        if (handles.empty()) return nullptr;
        std::coroutine_handle<> handle = handles.top();
        handles.pop();
        return handle;
    }
};

inline resumer_coroutine resume_coroutines(coro_stack& routines) {
    while (auto handle = routines.pop_handle()) handle.resume();
    co_return;
}

struct suspend_conditional {
    bool ready;
    coro_stack& coros;
    constexpr bool await_ready() const noexcept { return ready; }
    constexpr std::coroutine_handle<> await_suspend(std::coroutine_handle<> complete) const noexcept {
        switch (coros.handles.size()) {
            case 0: return std::noop_coroutine(); // if nothing to resume then do nothing
            case 1: return coros.pop_handle(); // if only 1 coro to resume then pass that directly
            default: return resume_coroutines(coros).handle; // if multiple, create resumer routine to resume them one by one 
        }
    }
    constexpr void await_resume() const noexcept {}
};

template<typename... TRetArgs>
struct promise_internal {

    std::atomic_int ref_count = 0;
    coro_stack coros;

    void unhandled_exception() noexcept { exception = std::current_exception(); }
    void rethrow_if_exception() {
        if (exception) std::rethrow_exception(exception);
    }

    task_coroutine<TRetArgs...> get_return_object() { return { std::coroutine_handle<task_promise<TRetArgs...>>::from_promise(*static_cast<task_promise<TRetArgs...>*>(this))}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    suspend_conditional final_suspend() noexcept {
        IL2CPP_CATCH_HANDLER(rethrow_if_exception();)
        return {!ref_count && coros.handles.empty(), coros}; 
    }

    template<typename T> auto await_transform(T&& t) { return std::forward<T>(t); }
    template<typename... Args> auto await_transform(task_coroutine<Args...>& t) { return TaskCoroutineAwaiter<Args...>{t.handle}; }
    template<typename... Args> auto await_transform(task_coroutine<Args...>&& t) { return TaskCoroutineAwaiter<Args...>{t.handle}; }

    template<CSharpTask T> auto await_transform(T* t) { return CSharpTaskAwaiter{t}; }    

    template<typename... Args>
    void* operator new(std::size_t n, const Args&...) {
        return il2cpp_functions::gc_alloc_fixed(n);
    }

    void operator delete(void* mem) noexcept {
        il2cpp_functions::gc_free_fixed(mem);
    }

    private:
    std::exception_ptr exception;
};

template<typename... TRetArgs>
struct task_promise : public promise_internal<TRetArgs...> {

    using TRet = std::tuple<TRetArgs...>;

    void return_value(const TRet& val) { prom.set_value(val); }

    task_promise() : future(prom.get_future()) {}
    std::shared_future<TRet> get_future() { return future; }

    private:
    std::promise<TRet> prom;
    std::shared_future<TRet> future;
};

template<>
struct task_promise<void> : public promise_internal<void> {
    void return_void() {}
};

template <typename... TRetArgs>
struct TaskCoroutineAwaiter {

    std::coroutine_handle<task_promise<TRetArgs...>> caller_handle;

    bool await_ready() { return caller_handle.done(); }

    void await_suspend(std::coroutine_handle<> handle) {
        caller_handle.promise().coros.add_handle(handle);
    }
    
    auto await_resume() {
        if constexpr(task_type<TRetArgs...>::type == TaskType::SINGLE) return std::get<TRetArgs...>(caller_handle.promise().get_future().get());
        else if constexpr(task_type<TRetArgs...>::type == TaskType::MULTI) return caller_handle.promise().get_future().get();
    }
};

template <CSharpTask T>
struct CSharpTaskAwaiter {

    template<CSharpTask U> struct TaskReturnType { using TRet = void; };
    template<typename U> struct TaskReturnType<System::Threading::Tasks::Task_1<U>> { using TRet = U; };
    
    using TRet = TaskReturnType<T>::TRet;

    CSharpTaskAwaiter(T* task) : _task(task) {}

    bool await_ready() { return _task->get_IsCompleted(); }
    
    void await_suspend(std::coroutine_handle<> handle) {
        static_cast<System::Threading::Tasks::Task*>(_task)->ContinueWith(
            custom_types::MakeDelegate<System::Action_1<System::Threading::Tasks::Task*>*>(
                std::function([handle](T* t) {
                    handle.resume();
                })
            )
        );
    }

    TRet await_resume() { 
        if constexpr(!std::is_same_v<TRet, void>) return _task->get_Result();
    }

    private:
    T* _task;
};

struct YieldMainThread {
    bool await_ready() { return BSML::MainThreadScheduler::CurrentThreadIsMainThread(); }
    void await_suspend(std::coroutine_handle<> handle) {
        BSML::MainThreadScheduler::Schedule([handle]() {
            handle.resume();
        });
    }
    void await_resume() { }
};

struct YieldNewCSharpThread {
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> handle) {
        il2cpp_utils::il2cpp_aware_thread([handle]() {
            handle.resume();
        }).detach();
    }
    void await_resume() { }
};