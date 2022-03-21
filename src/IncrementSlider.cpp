#include "IncrementSlider.hpp"
#include "questui/shared/CustomTypes/Components/Settings/SliderSetting.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "HMUI/TimeSlider.hpp"
#include "HMUI/RangeValuesTextSlider.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"

DEFINE_TYPE(SaberTailor, IncrementSlider);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace TMPro;
using namespace HMUI;
using namespace GlobalNamespace;

QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view name, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange)
    {
        auto gameObject = GameObject::New_ctor("CUNT");
        gameObject->AddComponent<LayoutElement*>();
        auto sliderSetting = gameObject->AddComponent<QuestUI::SliderSetting*>();

        auto timeSliderTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TimeSlider*>(), [](auto s) { 
            if (s->get_name() != "RangeValuesTextSlider") return false;
            auto parent1 = s->get_transform()->get_parent();
            if (!parent1) return false; 
            return parent1->get_name() == "SongStart";
        });

        sliderSetting->slider = Object::Instantiate(timeSliderTemplate, gameObject->get_transform(), false);
        sliderSetting->Setup(minValue, maxValue, value, increment, 0.0f, onValueChange);
        sliderSetting->slider->set_name("QuestUISlider");
        sliderSetting->slider->GetComponentInChildren<TextMeshProUGUI*>()->set_enableWordWrapping(false);

        auto rectTransform = reinterpret_cast<RectTransform*>(sliderSetting->slider->get_transform());
        rectTransform->set_anchorMin(Vector2(1, 0));
        rectTransform->set_anchorMax(Vector2(1, 1));
        rectTransform->set_sizeDelta(Vector2(54.0f, 0.0f));
        rectTransform->set_pivot(Vector2(1, 0.5f));
        rectTransform->set_anchoredPosition(Vector2(0.76f, 0.0f));
        rectTransform->set_localScale({1.0f, 1.0f, 0.0f});

        gameObject->GetComponent<RectTransform*>()->set_sizeDelta(Vector2(40, 8));
        gameObject->get_transform()->set_parent(parent);
        gameObject->get_transform()->set_localScale({1.0f, 1.0f, 0.0f});
        gameObject->GetComponent<LayoutElement*>()->set_minWidth(rectTransform->get_sizeDelta().x);
        gameObject->SetActive(true);
        return sliderSetting;
    }

SaberTailor::IncrementSlider* SaberTailor::IncrementSlider::CreateIncrementSlider(UnityEngine::Transform* parent, std::string_view text, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange){
    auto valueControllerTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<FormattedFloatListSettingsValueController*>(), [](auto x) { return x->get_name() == "VRRenderingScale"; });

    FormattedFloatListSettingsValueController* baseSetting = Object::Instantiate(valueControllerTemplate, parent, false);
    baseSetting->set_name("QuestUISliderSetting");

    auto mainCanvas = baseSetting->get_gameObject();
    Object::Destroy(mainCanvas->get_transform()->Find("ValuePicker")->get_gameObject());
    Object::Destroy(baseSetting);

    mainCanvas->set_active(false);

    auto gameObject = GameObject::New_ctor("CUNT");
    gameObject->AddComponent<LayoutElement*>();

    SaberTailor::IncrementSlider* incSlider = gameObject->AddComponent<SaberTailor::IncrementSlider*>();

    auto* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(gameObject->get_transform());
    layout->set_spacing(-0.9f);

    incSlider->leftButton = QuestUI::BeatSaberUI::CreateUIButton(layout->get_transform(), "", "DecButton", {0, 0}, {6.0f, 8.0f}, [incSlider, increment](){
        float x = incSlider->sliderComponent->get_value() - increment;
        incSlider->sliderComponent->set_value(x);
        incSlider->sliderComponent->OnValueChange(incSlider->sliderComponent->get_value());
    });
    incSlider->sliderComponent = CreateSliderSetting(layout->get_transform(), "", increment, value, minValue, maxValue, onValueChange);
    incSlider->sliderComponent->slider->dyn__enableDragging() = false;
    incSlider->rightButton = QuestUI::BeatSaberUI::CreateUIButton(layout->get_transform(), "", "IncButton", {0, 0}, {8.0f, 8.0f}, [incSlider, increment](){
        float x = incSlider->sliderComponent->get_value() + increment;
        incSlider->sliderComponent->set_value(x);
        incSlider->sliderComponent->OnValueChange(incSlider->sliderComponent->get_value());
    });

    auto nameText = mainCanvas->get_transform()->Find("NameText")->get_gameObject();
    Polyglot::LocalizedTextMeshProUGUI* localizedText = nameText->GetComponent<Polyglot::LocalizedTextMeshProUGUI*>();
    localizedText->set_enabled(false);
    localizedText->set_Key(System::String::_get_Empty());

    auto text1 = nameText->GetComponent<TextMeshProUGUI*>();
    text1->set_text(text);
    gameObject->get_transform()->SetParent(mainCanvas->get_transform(), false);
    gameObject->GetComponent<LayoutElement*>()->set_preferredWidth(40);
    gameObject->GetComponent<RectTransform*>()->set_sizeDelta({40, 10});
    mainCanvas->GetComponent<LayoutElement*>()->set_preferredWidth(90);
    mainCanvas->get_transform()->set_localScale({1.0f, 1.0f, 0.0f});
    mainCanvas->get_transform()->SetParent(parent, false);
    mainCanvas->set_active(true);
    return incSlider;
}