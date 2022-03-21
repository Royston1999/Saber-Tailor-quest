#include "SaberTailorRightHand.hpp"
#include "SettingsFlowCoordinator.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "SaberTailorHelper.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorRightHand);

UnityEngine::UI::VerticalLayoutGroup* rightsabercontainer;
SaberTailor::IncrementSlider* posXRight;
SaberTailor::IncrementSlider* posYRight;
SaberTailor::IncrementSlider* posZRight;
SaberTailor::IncrementSlider* rotXRight;
SaberTailor::IncrementSlider* rotYRight;
SaberTailor::IncrementSlider* rotZRight;

void SaberTailor::Views::SaberTailorRightHand::DidActivate(
    bool firstActivation,
    bool addedToHierarchy,
    bool screenSystemEnabling
) {
    using namespace GlobalNamespace;
    using namespace UnityEngine;
    using namespace UnityEngine::UI;
    using namespace custom_types::Helpers;
    using namespace IncrementHelper;

    if (firstActivation) {
        rightsabercontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        HorizontalLayoutGroup* somebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
        QuestUI::BeatSaberUI::CreateUIButton(somebuttons->get_transform(), "Mirror to Left", [](){
            TransferHelper::mirrorToLeft();
        });
        QuestUI::BeatSaberUI::CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
            TransferHelper::exportToBaseGame(1);
        });
        posXRight = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer->get_transform(), "Position X", 0.1f, SaberTailorMain::config.rightPosX, -10, 10, [](float value){
            setFloat(getConfig().config, "rightPosX", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            posXRight->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(IncrementHelper::Round(SaberTailorMain::config.rightPosX, 1) + " cm");
        });
        posYRight = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer->get_transform(), "Position Y", 0.1f, SaberTailorMain::config.rightPosY, -10, 10, [](float value){
            setFloat(getConfig().config, "rightPosY", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            posYRight->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(IncrementHelper::Round(SaberTailorMain::config.rightPosY, 1) + " cm");
        });
        posZRight = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer->get_transform(), "Position Z", 0.1f, SaberTailorMain::config.rightPosZ, -10, 10, [](float value){
            setFloat(getConfig().config, "rightPosZ", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            posZRight->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(IncrementHelper::Round(SaberTailorMain::config.rightPosZ, 1) + " cm");
        });
        rotXRight = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer->get_transform(), "Rotation X", 1, SaberTailorMain::config.rightRotX, -180, 180, [](float value){
            int x = std::round(value);
            setInt(getConfig().config, "rightRotX", x); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            rotXRight->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(std::to_string(SaberTailorMain::config.rightRotX) + " deg");
        });
        rotYRight = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer->get_transform(), "Rotation Y", 1, SaberTailorMain::config.rightRotY, -180, 180, [](float value){
            int x = std::round(value);
            setInt(getConfig().config, "rightRotY", x); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            rotYRight->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(std::to_string(SaberTailorMain::config.rightRotY) + " deg");
        });

        rotZRight = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer->get_transform(), "Rotation Z", 1, SaberTailorMain::config.rightRotZ, -180, 180, [](float value){
            int x = std::round(value);
            setInt(getConfig().config, "rightRotZ", x); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            rotZRight->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(std::to_string(SaberTailorMain::config.rightRotZ) + " deg");
        });
        HorizontalLayoutGroup* somemorebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
        QuestUI::BeatSaberUI::CreateUIButton(somemorebuttons->get_transform(), "Cancel", [this](){
            PosRotHelper::revertRightHand();
            QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return x->get_name() == "BackButton"; })->get_gameObject()->SetActive(true);
            SaberTailorMain::config.okButtonPressed = true;
        }); 
        QuestUI::BeatSaberUI::CreateUIButton(somemorebuttons->get_transform(), "Confirm", "PlayButton", [](){
            QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return x->get_name() == "BackButton"; })->get_gameObject()->SetActive(true);
            SaberTailorMain::config.okButtonPressed = true;
        });
    }
    QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return x->get_name() == "BackButton"; })->get_gameObject()->SetActive(false);
    posXRight->sliderComponent->set_value(SaberTailorMain::config.rightPosX);
    posYRight->sliderComponent->set_value(SaberTailorMain::config.rightPosY);
    posZRight->sliderComponent->set_value(SaberTailorMain::config.rightPosZ);
    rotXRight->sliderComponent->set_value(SaberTailorMain::config.rightRotX);
    rotYRight->sliderComponent->set_value(SaberTailorMain::config.rightRotY);
    rotZRight->sliderComponent->set_value(SaberTailorMain::config.rightRotZ);

    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(posXRight, IncrementHelper::Round(SaberTailorMain::config.rightPosX, 1) + " cm")));
    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(posYRight, IncrementHelper::Round(SaberTailorMain::config.rightPosY, 1) + " cm")));
    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(posZRight, IncrementHelper::Round(SaberTailorMain::config.rightPosZ, 1) + " cm")));
    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(rotXRight, std::to_string(SaberTailorMain::config.rightRotX) + " deg")));
    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(rotYRight, std::to_string(SaberTailorMain::config.rightRotY) + " deg")));
    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(rotZRight, std::to_string(SaberTailorMain::config.rightRotZ) + " deg")));

    SaberTailorMain::config.currentPosXRight = SaberTailorMain::config.rightPosX;
    SaberTailorMain::config.currentPosYRight = SaberTailorMain::config.rightPosY;
    SaberTailorMain::config.currentPosZRight = SaberTailorMain::config.rightPosZ;
    SaberTailorMain::config.currentRotXRight = SaberTailorMain::config.rightRotX;
    SaberTailorMain::config.currentRotYRight = SaberTailorMain::config.rightRotY;
    SaberTailorMain::config.currentRotZRight = SaberTailorMain::config.rightRotZ;
}