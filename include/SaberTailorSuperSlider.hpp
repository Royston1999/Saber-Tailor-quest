#pragma once

#include "HMUI/RangeValuesTextSlider.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/EventSystems/IEventSystemHandler.hpp"
#include "UnityEngine/EventSystems/IPointerDownHandler.hpp"
#include "UnityEngine/EventSystems/IPointerUpHandler.hpp"
#include "UnityEngine/EventSystems/PointerEventData.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor, SaberTailorSuperSlider, HMUI::RangeValuesTextSlider) {
    
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, LateUpdate);
    DECLARE_OVERRIDE_METHOD_MATCH(StringW, TextForValue, &HMUI::RangeValuesTextSlider::TextForValue, float);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, superIncButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, superDecButton);
    DECLARE_STATIC_METHOD(void, Init);
    DECLARE_INSTANCE_METHOD(void, OnSliderPointerDown, UnityEngine::EventSystems::PointerEventData* eventData);
    DECLARE_INSTANCE_METHOD(void, OnSliderPointerUp);

    public:
    std::function<std::string(float)> formatter = nullptr;
    std::function<void(float)> onChange = nullptr;
    float increment;
    static SaberTailorSuperSlider* CreateSuperSlider(UnityEngine::Transform* parent, std::string name, int min, int max, float increment, float value, std::function<std::string(float)> formatter, std::function<void(float)> onChange);
    private:
    static UnityEngine::GameObject* GetSuperSliderPrefab();
    float _targetValue;
    float _pressed;
    float _pressedTime;
    float _currentValue;
    std::function<bool(float)> hasChanged = nullptr;
};

#define INTERFACES \
    UnityEngine::EventSystems::IPointerDownHandler*, \
    UnityEngine::EventSystems::IPointerUpHandler*, \
    UnityEngine::EventSystems::IEventSystemHandler*

DECLARE_CLASS_CODEGEN_INTERFACES(SaberTailor, PointerEventsHandler, UnityEngine::MonoBehaviour, INTERFACES) {

    DECLARE_OVERRIDE_METHOD_MATCH(void, OnPointerDown, &UnityEngine::EventSystems::IPointerDownHandler::OnPointerDown, UnityEngine::EventSystems::PointerEventData*);
    DECLARE_OVERRIDE_METHOD_MATCH(void, OnPointerUp, &UnityEngine::EventSystems::IPointerUpHandler::OnPointerUp, UnityEngine::EventSystems::PointerEventData*);
    DECLARE_INSTANCE_FIELD(SaberTailorSuperSlider*, superSlider);
    DECLARE_INSTANCE_METHOD(void, Start);
};

#undef INTERFACES
