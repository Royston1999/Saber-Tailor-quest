#include "SaberTailorLeftHand.hpp"
#include "SettingsFlowCoordinator.hpp"
#include "SaberTailorConfig.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "SaberTailorHelper.hpp"
#include "IncrementSlider.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include <cmath>
#include "UnityEngine/WaitForFixedUpdate.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorLeftHand);

UnityEngine::UI::VerticalLayoutGroup* leftsabercontainer;
SaberTailor::IncrementSlider* posX;
SaberTailor::IncrementSlider* posY;
SaberTailor::IncrementSlider* posZ;
SaberTailor::IncrementSlider* rotX;
SaberTailor::IncrementSlider* rotY;
SaberTailor::IncrementSlider* rotZ;
Array<TMPro::TextMeshProUGUI*>* incText;

void SaberTailor::Views::SaberTailorLeftHand::DidActivate(
    bool firstActivation,
    bool addedToHierarchy,
    bool screenSystemEnabling
) {
    using namespace GlobalNamespace;
    using namespace UnityEngine;
    using namespace QuestUI::BeatSaberUI;
    using namespace UnityEngine::UI;

    if (firstActivation) {
        leftsabercontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        HorizontalLayoutGroup* somebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(leftsabercontainer->get_transform());
        CreateUIButton(somebuttons->get_transform(), "Mirror to Right", [](){
            TransferHelper::mirrorToRight();
        });
        CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
            TransferHelper::exportToBaseGame(0);
        });
        posX = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer->get_transform(), "Position X", 0.1f, SaberTailorMain::config.leftPosX, -10, 10, [](float value){
            setFloat(getConfig().config, "leftPosX", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            posX->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(il2cpp_utils::newcsstr(IncrementHelper::Round(SaberTailorMain::config.leftPosX, 1) + " cm"));
        });
        posY = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer->get_transform(), "Position Y", 0.1f, SaberTailorMain::config.leftPosY, -10, 10, [](float value){
            setFloat(getConfig().config, "leftPosY", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            posY->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(il2cpp_utils::newcsstr(IncrementHelper::Round(SaberTailorMain::config.leftPosY, 1) + " cm"));
        });
        posZ = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer->get_transform(), "Position Z", 0.1f, SaberTailorMain::config.leftPosZ, -10, 10, [](float value){
            setFloat(getConfig().config, "leftPosZ", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            posZ->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(il2cpp_utils::newcsstr(IncrementHelper::Round(SaberTailorMain::config.leftPosZ, 1) + " cm"));
        });
        rotX = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer->get_transform(), "Rotation X", 1, SaberTailorMain::config.leftRotX, -180, 180, [](float value){
            int x = std::round(value);
            setInt(getConfig().config, "leftRotX", x); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            rotX->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(il2cpp_utils::newcsstr(std::to_string(SaberTailorMain::config.leftRotX) + " deg"));
        });
        rotY = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer->get_transform(), "Rotation Y", 1, SaberTailorMain::config.leftRotY, -180, 180, [](float value){
            int x = std::round(value);
            setInt(getConfig().config, "leftRotY", x); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            rotY->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(il2cpp_utils::newcsstr(std::to_string(SaberTailorMain::config.leftRotY) + " deg"));
        });

        rotZ = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer->get_transform(), "Rotation Z", 1, SaberTailorMain::config.leftRotZ, -180, 180, [](float value){
            int x = std::round(value);
            setInt(getConfig().config, "leftRotZ", x); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            rotZ->sliderComponent->slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_text(il2cpp_utils::newcsstr(std::to_string(SaberTailorMain::config.leftRotZ) + " deg"));
        });

        HorizontalLayoutGroup* somemorebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(leftsabercontainer->get_transform());
        CreateUIButton(somemorebuttons->get_transform(), "Cancel", [](){
            PosRotHelper::revertLeftHand();
            QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return to_utf8(csstrtostr(x->get_name())) == "BackButton"; })->get_gameObject()->SetActive(true);
            SaberTailorMain::config.okButtonPressed = true;
        }); 
        CreateUIButton(somemorebuttons->get_transform(), "Confirm", "PlayButton", [](){
            QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return to_utf8(csstrtostr(x->get_name())) == "BackButton"; })->get_gameObject()->SetActive(true);
            SaberTailorMain::config.okButtonPressed = true;
        });
    }
        QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return to_utf8(csstrtostr(x->get_name())) == "BackButton"; })->get_gameObject()->SetActive(false);
        posX->sliderComponent->set_value(SaberTailorMain::config.leftPosX);
        posY->sliderComponent->set_value(SaberTailorMain::config.leftPosY);
        posZ->sliderComponent->set_value(SaberTailorMain::config.leftPosZ);
        rotX->sliderComponent->set_value(SaberTailorMain::config.leftRotX);
        rotY->sliderComponent->set_value(SaberTailorMain::config.leftRotY);
        rotZ->sliderComponent->set_value(SaberTailorMain::config.leftRotZ);

        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(IncrementHelper::forceUpdateSliderText(posX, IncrementHelper::Round(SaberTailorMain::config.leftPosX, 1) + " cm"))));
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(IncrementHelper::forceUpdateSliderText(posY, IncrementHelper::Round(SaberTailorMain::config.leftPosY, 1) + " cm"))));
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(IncrementHelper::forceUpdateSliderText(posZ, IncrementHelper::Round(SaberTailorMain::config.leftPosZ, 1) + " cm"))));
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(IncrementHelper::forceUpdateSliderText(rotX, std::to_string(SaberTailorMain::config.leftRotX) + " deg"))));
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(IncrementHelper::forceUpdateSliderText(rotY, std::to_string(SaberTailorMain::config.leftRotY) + " deg"))));
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(IncrementHelper::forceUpdateSliderText(rotZ, std::to_string(SaberTailorMain::config.leftRotZ) + " deg"))));
        
        SaberTailorMain::config.currentPosXLeft = SaberTailorMain::config.leftPosX;
        SaberTailorMain::config.currentPosYLeft = SaberTailorMain::config.leftPosY;
        SaberTailorMain::config.currentPosZLeft = SaberTailorMain::config.leftPosZ;
        SaberTailorMain::config.currentRotXLeft = SaberTailorMain::config.leftRotX;
        SaberTailorMain::config.currentRotYLeft = SaberTailorMain::config.leftRotY;
        SaberTailorMain::config.currentRotZLeft = SaberTailorMain::config.leftRotZ;
    
}