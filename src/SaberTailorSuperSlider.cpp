#include "SaberTailorSuperSlider.hpp"
#include "custom-types/shared/delegate.hpp"
#include "System/Action.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Resources.hpp"
#include "HMUI/CustomFormatRangeValuesSlider.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "BGLib/Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include <cmath>
#include "SaberTailorSuperSlider.hpp"
#include "HMUI/ImageView.hpp"
#include "assets.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "System/Action_2.hpp"
#include "UnityEngine/Time.hpp"
#include "HMUI/ButtonBinder.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::EventSystems;
using namespace HMUI;
using namespace TMPro;
using namespace SaberTailor;


DEFINE_TYPE(SaberTailor, SaberTailorSuperSlider);
DEFINE_TYPE(SaberTailor, PointerEventsHandler);

namespace SaberTailor {
    void SaberTailorSuperSlider::Start() {
        if (!superIncButton || !superDecButton) return;
        ____buttonBinder->ClearBindings();
        std::function<void()> onSuperInc = [this](){ this->SetNormalizedValue(this->NormalizeValue(get_value() + 10 * increment)); };
        std::function<void()> onSuperDec = [this](){ this->SetNormalizedValue(this->NormalizeValue(get_value() - 10 * increment)); };
        std::function<void()> onInc = [this](){ this->SetNormalizedValue(this->NormalizeValue(get_value() + increment)); };
        std::function<void()> onDec = [this](){ this->SetNormalizedValue(this->NormalizeValue(get_value() - increment)); };
        ____incButton->get_onClick()->AddListener(custom_types::MakeDelegate<UnityEngine::Events::UnityAction*>(onInc));
        ____decButton->get_onClick()->AddListener(custom_types::MakeDelegate<UnityEngine::Events::UnityAction*>(onDec));
        superIncButton->get_onClick()->AddListener(custom_types::MakeDelegate<UnityEngine::Events::UnityAction*>(onSuperInc));
        superDecButton->get_onClick()->AddListener(custom_types::MakeDelegate<UnityEngine::Events::UnityAction*>(onSuperDec));

        UpdateVisuals();
    }

    void SaberTailorSuperSlider::Init() {
        GetSuperSliderPrefab();
    }

    std::string correctText(std::string originalText) {
        std::string corrected = "";
        std::string oopsies = "-+."; // the special characters i care about readjusting
        for (int i = 0; i < originalText.length(); i++) {
            if (oopsies.find(originalText[i]) != std::string::npos) {
                std::string hmm = i == 0 ? "<cspace=-0.3em> " : "<cspace=-0.2em> "; 
                corrected += (hmm + std::string{originalText[i]} + "</cspace>");
                if (i < originalText.length() -1) corrected += "<cspace=-0.27em>  </cspace>";
            }
            else corrected += originalText[i];
        }
        return corrected;
    }

    StringW SaberTailorSuperSlider::TextForValue(float value) {
        if (formatter) return correctText(formatter(value));
        return fmt::format("{:.1f}", value);
    }

    float getRatioClamped(float val) {
        float ratio = val / 2.0f;
        return std::clamp<float>(ratio, 0.0f, 1.0f);
    }

    float lerp(float a, float b, float t) {
        return a + (b - a) * std::clamp<float>(t, 0.0f, 1.0f);
    }

    void SaberTailorSuperSlider::LateUpdate() {
        if (!_pressed) return;
        float t = 3 * Time::get_deltaTime() * getRatioClamped(Time::get_time() - _pressedTime);
        _currentValue = lerp(_currentValue, _targetValue, t);
        set_value(_currentValue);
        float tempCurrent = get_value();
        if (onChange && hasChanged(tempCurrent)) onChange(tempCurrent);
    }

    void SaberTailorSuperSlider::OnSliderPointerDown(PointerEventData* eventData) {
        if (!this->IsActive() || !this->IsInteractable() || eventData->get_button() != PointerEventData::InputButton::Left) return;
        _currentValue = _targetValue = get_value();
        _pressed = true;
        _pressedTime = Time::get_time();
    }

    void SaberTailorSuperSlider::OnSliderPointerUp() {
        _currentValue = _targetValue = get_value();
        _pressed = false;
    }

    GameObject* SaberTailorSuperSlider::GetSuperSliderPrefab() {
        static SafePtrUnity<SaberTailorSuperSlider> superSlider;
        if (superSlider) return superSlider->get_transform()->get_parent()->get_gameObject();

        auto baseSliderPrefab = Resources::FindObjectsOfTypeAll<CustomFormatRangeValuesSlider*>().front_or_default([](CustomFormatRangeValuesSlider* slider) {
            auto one = slider->get_transform()->get_parent();
            if (!one || !one->___m_CachedPtr.m_value) return false;
            return one->get_name() == "PositionX";
        })->get_transform()->get_parent()->get_gameObject();

        GameObject* containerGo = Object::Instantiate(baseSliderPrefab).ptr();
        containerGo->set_name("SaberTailorSuperSliderPrefab");
        Object::Destroy(containerGo->get_transform()->GetComponentInChildren<BGLib::Polyglot::LocalizedTextMeshProUGUI*>());
        CustomFormatRangeValuesSlider* slider = containerGo->get_transform()->GetComponentInChildren<CustomFormatRangeValuesSlider*>();
        GameObject* sliderGo = slider->get_gameObject();
        Object::DestroyImmediate(slider);
        sliderGo->AddComponent<PointerEventsHandler*>();
        superSlider = sliderGo->AddComponent<SaberTailorSuperSlider*>();
        superSlider->get_gameObject()->set_name("SaberTailorSuperSliderComponent");

        superSlider->____incButton = slider->____incButton;
        superSlider->____decButton = slider->____decButton;
        superSlider->____valueText = slider->____valueText;
        superSlider->____handleRect = slider->____handleRect;
        superSlider->____containerRect = slider->____containerRect;
        superSlider->____handleGraphic = slider->____handleGraphic;
        superSlider->____tracker = slider->____tracker;
        superSlider->___m_Colors = slider->___m_Colors;
        superSlider->___m_SpriteState = slider->___m_SpriteState;
        superSlider->___m_TargetGraphic = slider->___m_TargetGraphic;
        superSlider->____handleSize = slider->____handleSize;
        superSlider->____separatorSize = slider->____separatorSize;
        superSlider->____valueSize = slider->____valueSize;

        superSlider->GetComponentInChildren<TextMeshProUGUI*>()->set_enableWordWrapping(false);
        
        superSlider->superIncButton = Object::Instantiate(superSlider->____incButton, superSlider->get_transform());
        superSlider->superDecButton = Object::Instantiate(superSlider->____decButton, superSlider->get_transform());

        superSlider->superIncButton->get_transform()->set_localPosition(Vector3::op_Addition(superSlider->superIncButton->get_transform()->get_localPosition(), Vector3(6.65f, 0.0f, 0.0f)));
        superSlider->superDecButton->get_transform()->set_localPosition(Vector3::op_Addition(superSlider->superDecButton->get_transform()->get_localPosition(), Vector3(-6.65f, 0.0f, 0.0f)));
        
        containerGo->GetComponent<LayoutElement*>()->set_preferredWidth(90.0f);

        RectTransform* rect = reinterpret_cast<RectTransform*>(superSlider->get_transform().ptr());
        rect->set_anchorMin({1, 0});
        rect->set_anchorMax({1, 1});
        rect->set_sizeDelta({61, 0});
        rect->set_pivot({1, 0.5f});
        rect->set_localPosition({-4.5f, 0, 0});

        auto superIncImgs = superSlider->superIncButton->GetComponentsInChildren<HMUI::ImageView*>();
        auto superDecImgs = superSlider->superDecButton->GetComponentsInChildren<HMUI::ImageView*>();
        auto incImgs = superSlider->____incButton->GetComponentsInChildren<HMUI::ImageView*>();
        auto decImgs = superSlider->____decButton->GetComponentsInChildren<HMUI::ImageView*>();

        auto bgSprite = superSlider->get_transform()->GetComponentInChildren<ImageView*>()->get_sprite();
        auto arrowSprite = BSML::Utilities::LoadSpriteRaw(IncludedAssets::arrow_png);

        incImgs[0]->set_sprite(bgSprite);       
        decImgs[0]->set_sprite(bgSprite);
        superIncImgs[1]->set_sprite(arrowSprite); 
        superDecImgs[1]->set_sprite(arrowSprite);

        superIncImgs[1]->____skew = 0.0f;
        superDecImgs[1]->____skew = 0.0f;
        incImgs[1]->____skew = 0.0f;
        decImgs[1]->____skew = 0.0f;

        superSlider->____valueText->set_richText(true);

        containerGo->set_active(false);
        return superSlider->get_transform()->get_parent()->get_gameObject();
    }

    SaberTailorSuperSlider* SaberTailorSuperSlider::CreateSuperSlider(Transform* parent, std::string name, int min, int max, float increment, float value, std::function<std::string(float)> formatter, std::function<void(float)> onChange) {
        GameObject* containerGo = Object::Instantiate(GetSuperSliderPrefab(), parent);
        containerGo->set_name("SaberTailorSuperSlider");
        containerGo->get_transform()->GetChild(0)->GetComponent<TextMeshProUGUI*>()->set_text(name);

        SaberTailorSuperSlider* superSlider = containerGo->get_transform()->GetChild(1)->GetComponent<SaberTailorSuperSlider*>();
        superSlider->____enableDragging = true;
        superSlider->____minValue = min;
        superSlider->____maxValue = max;
        superSlider->____numberOfSteps = (max - min) / 1 + 1; // yes we divide by one
        superSlider->formatter = formatter;
        superSlider->set_value(value);
        superSlider->onChange = onChange;
        superSlider->increment = increment;

        superSlider->hasChanged = [value](float newVal) {
            static float prevValue = value;
            if (std::abs(prevValue - newVal) < 0.001f) return false;
            prevValue = newVal;
            return true;
        };

        std::function<void(RangeValuesTextSlider*, float)> func = [superSlider](RangeValuesTextSlider* _, float newVal) {
            superSlider->_targetValue = newVal;
            if (!superSlider->_pressed && superSlider->hasChanged(newVal)) superSlider->onChange(newVal);
        };
        superSlider->add_valueDidChangeEvent(custom_types::MakeDelegate<System::Action_2<UnityW<RangeValuesTextSlider>, float>*>(func));

        containerGo->set_active(true);
        return superSlider;
    }

    void PointerEventsHandler::Start() {
        superSlider = GetComponent<SaberTailorSuperSlider*>();
    }

    void PointerEventsHandler::OnPointerDown(PointerEventData* eventData) {
        superSlider->OnSliderPointerDown(eventData);
    }

    void PointerEventsHandler::OnPointerUp(PointerEventData* eventData) {
        superSlider->OnSliderPointerUp();
    }
}