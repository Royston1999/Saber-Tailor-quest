#include "IncrementSlider.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "HMUI/TimeSlider.hpp"
#include "HMUI/RangeValuesTextSlider.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "UnityEngine/TextAnchor.hpp"
#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "TMPro/FontStyles.hpp"

DEFINE_TYPE(SaberTailor, IncrementSlider);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace TMPro;
using namespace HMUI;
using namespace GlobalNamespace;

QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view name, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange)
    {

        auto gameObject = GameObject::New_ctor(il2cpp_utils::newcsstr("CUNT"));
        gameObject->AddComponent<LayoutElement*>();
        auto sliderSetting = gameObject->AddComponent<QuestUI::SliderSetting*>();

        auto timeSliderTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TimeSlider*>(), [](auto s) { 
            if (to_utf8(csstrtostr(s->get_name())) != "RangeValuesTextSlider") return false;
            auto parent1 = s->get_transform()->get_parent();
            if (!parent1) return false; 
            return to_utf8(csstrtostr(parent1->get_name())) == "SongStart";
            });

        sliderSetting->slider = Object::Instantiate(timeSliderTemplate, gameObject->get_transform(), false);
        sliderSetting->Setup(minValue, maxValue, value, increment, 0.0f, onValueChange);
        static auto QuestUISlider_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUISlider");
        sliderSetting->slider->set_name(QuestUISlider_cs);
        sliderSetting->slider->GetComponentInChildren<TextMeshProUGUI*>()->set_enableWordWrapping(false);

        auto rectTransform = reinterpret_cast<RectTransform*>(sliderSetting->slider->get_transform());
        rectTransform->set_anchorMin(Vector2(1, 0));
        rectTransform->set_anchorMax(Vector2(1, 1));
        rectTransform->set_sizeDelta(Vector2(54.0f, 0.0f));
        rectTransform->set_pivot(Vector2(1, 0.5f));
        rectTransform->set_anchoredPosition(Vector2(0.75f, 0.0f));
        rectTransform->set_localScale({1.0f, 1.0f, 0.0f});

        gameObject->GetComponent<RectTransform*>()->set_sizeDelta(Vector2(40, 8));
        gameObject->get_transform()->set_parent(parent);
        gameObject->get_transform()->set_localScale({1.0f, 1.0f, 0.0f});
        gameObject->GetComponent<LayoutElement*>()->set_minWidth(rectTransform->get_sizeDelta().x);
        gameObject->SetActive(true);
        return sliderSetting;
    }

SaberTailor::IncrementSlider* SaberTailor::IncrementSlider::CreateIncrementSlider(UnityEngine::Transform* parent, std::string_view text, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange){
    auto valueControllerTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<FormattedFloatListSettingsValueController*>(), [](auto x) { return to_utf8(csstrtostr(x->get_name())) == "VRRenderingScale"; });

    FormattedFloatListSettingsValueController* baseSetting = Object::Instantiate(valueControllerTemplate, parent, false);
    static auto QuestUISliderSetting_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUISliderSetting");
    baseSetting->set_name(QuestUISliderSetting_cs);

    auto mainCanvas = baseSetting->get_gameObject();
    static auto ValuePicker_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("ValuePicker");
    Object::Destroy(mainCanvas->get_transform()->Find(ValuePicker_cs)->get_gameObject());
    Object::Destroy(baseSetting);

    mainCanvas->set_active(false);

    auto gameObject = GameObject::New_ctor(il2cpp_utils::newcsstr("CUNT"));
    gameObject->AddComponent<LayoutElement*>();

    SaberTailor::IncrementSlider* incSlider = gameObject->AddComponent<SaberTailor::IncrementSlider*>();

    auto* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(gameObject->get_transform());
    layout->set_spacing(-1.0f);

    incSlider->leftButton = QuestUI::BeatSaberUI::CreateUIButton(layout->get_transform(), "", "DecButton", {0, 0}, {5.9f, 8.0f}, [incSlider, increment](){
        float x = incSlider->sliderComponent->get_value() - increment;
        incSlider->sliderComponent->set_value(x);
        incSlider->sliderComponent->OnValueChange(incSlider->sliderComponent->get_value());
    });
    incSlider->sliderComponent = CreateSliderSetting(layout->get_transform(), "", increment, value, minValue, maxValue, onValueChange);
    incSlider->sliderComponent->slider->enableDragging = false;
    incSlider->rightButton = QuestUI::BeatSaberUI::CreateUIButton(layout->get_transform(), "", "IncButton", {0, 0}, {8.0f, 8.0f}, [incSlider, increment](){
        float x = incSlider->sliderComponent->get_value() + increment;
        incSlider->sliderComponent->set_value(x);
        incSlider->sliderComponent->OnValueChange(incSlider->sliderComponent->get_value());
    });

    reinterpret_cast<UnityEngine::RectTransform*>(incSlider->leftButton->get_transform())->set_pivot({1.0f, 0.5f});
    reinterpret_cast<UnityEngine::RectTransform*>(incSlider->rightButton->get_transform())->set_pivot({1.0f, 0.5f});

    static auto NameText_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("NameText"); 
    auto nameText = mainCanvas->get_transform()->Find(NameText_cs)->get_gameObject();
    Polyglot::LocalizedTextMeshProUGUI* localizedText = nameText->GetComponent<Polyglot::LocalizedTextMeshProUGUI*>();
    localizedText->set_enabled(false);
    localizedText->set_Key(System::String::_get_Empty());

    auto text1 = nameText->GetComponent<TextMeshProUGUI*>();
    text1->set_text(il2cpp_utils::newcsstr(text));
    gameObject->get_transform()->SetParent(mainCanvas->get_transform(), false);
    gameObject->GetComponent<LayoutElement*>()->set_preferredWidth(40);
    gameObject->GetComponent<RectTransform*>()->set_sizeDelta({40, 10});
    mainCanvas->GetComponent<LayoutElement*>()->set_preferredWidth(90);
    mainCanvas->get_transform()->set_localScale({1.0f, 1.0f, 0.0f});
    mainCanvas->get_transform()->SetParent(parent, false);
    mainCanvas->set_active(true);
    return incSlider;
}