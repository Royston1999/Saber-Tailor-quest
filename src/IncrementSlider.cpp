#include "IncrementSlider.hpp"
#include "questui/shared/CustomTypes/Components/Settings/SliderSetting.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "HMUI/TimeSlider.hpp"
#include "HMUI/RangeValuesTextSlider.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "main.hpp"

DEFINE_TYPE(SaberTailor, IncrementSlider);

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
    
Transform* GetSubcontainer(VerticalLayoutGroup* vertical) {
    auto horizontal = CreateHorizontalLayoutGroup(vertical);
    horizontal->GetComponent<LayoutElement*>()->set_minHeight(8);
    horizontal->set_childForceExpandHeight(true);
    horizontal->set_childAlignment(TextAnchor::MiddleCenter);
    return horizontal->get_transform();
}

SaberTailor::IncrementSlider* SaberTailor::IncrementSlider::CreateIncrementSlider(VerticalLayoutGroup* vertical, std::string_view text, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange) {
    SaberTailor::IncrementSlider* incSlider = GameObject::New_ctor("SorryMetalitIYoinkedYourCode")->AddComponent<SaberTailor::IncrementSlider*>();
    incSlider->sliderComponent = CreateSliderSetting(GetSubcontainer(vertical)->get_transform(), text, 1, value, minValue, maxValue, onValueChange);
    incSlider->sliderComponent->slider->enableDragging = false;
    incSlider->increment = increment;
    incSlider->sliderComponent->timerResetValue = 0.0f;

    auto rectTransform = reinterpret_cast<RectTransform*>(incSlider->sliderComponent->slider->get_transform());
    rectTransform->set_anchoredPosition({-6, 0});
    rectTransform->set_sizeDelta({54.0f, 0.0f});

    incSlider->leftButton = CreateUIButton(incSlider->sliderComponent->get_transform(), "", "DecButton", {-17.5, 0}, {6, 8}, [incSlider](){
        float x = incSlider->sliderComponent->get_value() - incSlider->increment;
        incSlider->sliderComponent->set_value(x);
        incSlider->sliderComponent->OnValueChange(incSlider->sliderComponent->get_value());
    });
    incSlider->rightButton = CreateUIButton(incSlider->sliderComponent->get_transform(), "", "IncButton", {41, 0}, {8, 8}, [incSlider](){
        float x = incSlider->sliderComponent->get_value() + incSlider->increment;
        incSlider->sliderComponent->set_value(x);
        incSlider->sliderComponent->OnValueChange(incSlider->sliderComponent->get_value());
    });

    return incSlider;
}