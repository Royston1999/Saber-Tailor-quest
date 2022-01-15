#include "SaberTailorLeftHand.hpp"
#include "SettingsFlowCoordinator.hpp"
#include "SaberTailorConfig.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "SaberTailorHelper.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorLeftHand);

UnityEngine::UI::VerticalLayoutGroup* leftsabercontainer;
QuestUI::IncrementSetting* posX;
QuestUI::IncrementSetting* posY;
QuestUI::IncrementSetting* posZ;
QuestUI::IncrementSetting* rotX;
QuestUI::IncrementSetting* rotY;
QuestUI::IncrementSetting* rotZ;
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
        posX = CreateIncrementSetting(leftsabercontainer->get_transform(), "Position X", 1, 0.1f, SaberTailorMain::config.leftPosX, -10.0f, 10.0f, [](float value){
            IncrementHelper::setIncrementText(posX, value);
            float num = IncrementHelper::fixDumbNumberThing(value);
            setFloat(getConfig().config, "leftPosX", num); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        posY = CreateIncrementSetting(leftsabercontainer->get_transform(), "Position Y", 1, 0.1f, SaberTailorMain::config.leftPosY, -10.0f, 10.0f, [](float value){
            IncrementHelper::setIncrementText(posY, value);
            float num = IncrementHelper::fixDumbNumberThing(value);
            setFloat(getConfig().config, "leftPosY", num); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        posZ = CreateIncrementSetting(leftsabercontainer->get_transform(), "Position Z", 1, 0.1f, SaberTailorMain::config.leftPosZ, -10.0f, 10.0f, [](float value){
            IncrementHelper::setIncrementText(posZ, value);
            float num = IncrementHelper::fixDumbNumberThing(value);
            setFloat(getConfig().config, "leftPosZ", num); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        rotX = CreateIncrementSetting(leftsabercontainer->get_transform(), "Rotation X", 0, 1.0f, SaberTailorMain::config.leftRotX, -180.0f, 180.0f, [](float value){
            IncrementHelper::setRotIncrementText(rotX, value);
            setInt(getConfig().config, "leftRotX", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        rotY = CreateIncrementSetting(leftsabercontainer->get_transform(), "Rotation Y", 0, 1.0f, SaberTailorMain::config.leftRotY, -180.0f, 180.0f, [](float value){
            IncrementHelper::setRotIncrementText(rotY, value);
            setInt(getConfig().config, "leftRotY", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        rotZ = CreateIncrementSetting(leftsabercontainer->get_transform(), "Rotation Z", 0, 1.0f, SaberTailorMain::config.leftRotZ, -180.0f, 180.0f, [](float value){
            IncrementHelper::setRotIncrementText(rotZ, value);
            setInt(getConfig().config, "leftRotZ", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
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
        posX->CurrentValue = SaberTailorMain::config.leftPosX;
        posY->CurrentValue = SaberTailorMain::config.leftPosY;
        posZ->CurrentValue = SaberTailorMain::config.leftPosZ;
        rotX->CurrentValue = SaberTailorMain::config.leftRotX;
        rotY->CurrentValue = SaberTailorMain::config.leftRotY;
        rotZ->CurrentValue = SaberTailorMain::config.leftRotZ;

        IncrementHelper::setIncrementText(posX, SaberTailorMain::config.leftPosX);
        IncrementHelper::setIncrementText(posY, SaberTailorMain::config.leftPosY);
        IncrementHelper::setIncrementText(posZ, SaberTailorMain::config.leftPosZ);
        IncrementHelper::setRotIncrementText(rotX, SaberTailorMain::config.leftRotX);
        IncrementHelper::setRotIncrementText(rotY, SaberTailorMain::config.leftRotY);
        IncrementHelper::setRotIncrementText(rotZ, SaberTailorMain::config.leftRotZ);
        
        SaberTailorMain::config.currentPosXLeft = SaberTailorMain::config.leftPosX;
        SaberTailorMain::config.currentPosYLeft = SaberTailorMain::config.leftPosY;
        SaberTailorMain::config.currentPosZLeft = SaberTailorMain::config.leftPosZ;
        SaberTailorMain::config.currentRotXLeft = SaberTailorMain::config.leftRotX;
        SaberTailorMain::config.currentRotYLeft = SaberTailorMain::config.leftRotY;
        SaberTailorMain::config.currentRotZLeft = SaberTailorMain::config.leftRotZ;
    
}