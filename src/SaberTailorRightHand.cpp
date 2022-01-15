#include "SaberTailorRightHand.hpp"
#include "SettingsFlowCoordinator.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "SaberTailorHelper.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorRightHand);

UnityEngine::UI::VerticalLayoutGroup* rightsabercontainer;
QuestUI::IncrementSetting* posXRight;
QuestUI::IncrementSetting* posYRight;
QuestUI::IncrementSetting* posZRight;
QuestUI::IncrementSetting* rotXRight;
QuestUI::IncrementSetting* rotYRight;
QuestUI::IncrementSetting* rotZRight;
Array<TMPro::TextMeshProUGUI*>* incTextRight;

void SaberTailor::Views::SaberTailorRightHand::DidActivate(
    bool firstActivation,
    bool addedToHierarchy,
    bool screenSystemEnabling
) {
    using namespace GlobalNamespace;
    using namespace UnityEngine;
    using namespace QuestUI::BeatSaberUI;
    using namespace UnityEngine::UI;

    if (firstActivation) {
        rightsabercontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        HorizontalLayoutGroup* somebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
        CreateUIButton(somebuttons->get_transform(), "Mirror to Left", [](){
            TransferHelper::mirrorToLeft();
        });
        CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
            TransferHelper::exportToBaseGame(1);
        });
        posXRight = CreateIncrementSetting(rightsabercontainer->get_transform(), "Position X", 1, 0.1f, SaberTailorMain::config.rightPosX, -10.0f, 10.0f, [](float value){
            IncrementHelper::setIncrementText(posXRight, value);
            float num = IncrementHelper::fixDumbNumberThing(value);
            setFloat(getConfig().config, "rightPosX", num); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        posYRight = CreateIncrementSetting(rightsabercontainer->get_transform(), "Position Y", 1, 0.1f, SaberTailorMain::config.rightPosY, -10.0f, 10.0f, [](float value){
            IncrementHelper::setIncrementText(posYRight, value);
            float num = IncrementHelper::fixDumbNumberThing(value);
            setFloat(getConfig().config, "rightPosY", num); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        posZRight = CreateIncrementSetting(rightsabercontainer->get_transform(), "Position Z", 1, 0.1f, SaberTailorMain::config.rightPosZ, -10.0f, 10.0f, [](float value){
            IncrementHelper::setIncrementText(posZRight, value);
            float num = IncrementHelper::fixDumbNumberThing(value);
            setFloat(getConfig().config, "rightPosZ", num); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        rotXRight = CreateIncrementSetting(rightsabercontainer->get_transform(), "Rotation X", 0, 1.0f, SaberTailorMain::config.rightRotX, -180.0f, 180.0f, [](float value){
            IncrementHelper::setRotIncrementText(rotXRight, value);
            setInt(getConfig().config, "rightRotX", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        rotYRight = CreateIncrementSetting(rightsabercontainer->get_transform(), "Rotation Y", 0, 1.0f, SaberTailorMain::config.rightRotY, -180.0f, 180.0f, [](float value){
            IncrementHelper::setRotIncrementText(rotYRight, value);
            setInt(getConfig().config, "rightRotY", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        rotZRight = CreateIncrementSetting(rightsabercontainer->get_transform(), "Rotation Z", 0, 1.0f, SaberTailorMain::config.rightRotZ, -180.0f, 180.0f, [](float value){
            IncrementHelper::setRotIncrementText(rotZRight, value);
            setInt(getConfig().config, "rightRotZ", value); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        });
        HorizontalLayoutGroup* somemorebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
        CreateUIButton(somemorebuttons->get_transform(), "Cancel", [](){
            PosRotHelper::revertRightHand();
            QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return to_utf8(csstrtostr(x->get_name())) == "BackButton"; })->get_gameObject()->SetActive(true);
            SaberTailorMain::config.okButtonPressed = true;
        }); 
        CreateUIButton(somemorebuttons->get_transform(), "Confirm", "PlayButton", [](){
            QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return to_utf8(csstrtostr(x->get_name())) == "BackButton"; })->get_gameObject()->SetActive(true);
            SaberTailorMain::config.okButtonPressed = true;
        });
    }
    QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(), [] (UnityEngine::UI::Button* x) { return to_utf8(csstrtostr(x->get_name())) == "BackButton"; })->get_gameObject()->SetActive(false);
    posXRight->CurrentValue = SaberTailorMain::config.rightPosX;
    posYRight->CurrentValue = SaberTailorMain::config.rightPosY;
    posZRight->CurrentValue = SaberTailorMain::config.rightPosZ;
    rotXRight->CurrentValue = SaberTailorMain::config.rightRotX;
    rotYRight->CurrentValue = SaberTailorMain::config.rightRotY;
    rotZRight->CurrentValue = SaberTailorMain::config.rightRotZ;

    IncrementHelper::setIncrementText(posXRight, SaberTailorMain::config.rightPosX);
    IncrementHelper::setIncrementText(posYRight, SaberTailorMain::config.rightPosY);
    IncrementHelper::setIncrementText(posZRight, SaberTailorMain::config.rightPosZ);
    IncrementHelper::setRotIncrementText(rotXRight, SaberTailorMain::config.rightRotX);
    IncrementHelper::setRotIncrementText(rotYRight, SaberTailorMain::config.rightRotY);
    IncrementHelper::setRotIncrementText(rotZRight, SaberTailorMain::config.rightRotZ);
    
    SaberTailorMain::config.currentPosXRight = SaberTailorMain::config.rightPosX;
    SaberTailorMain::config.currentPosYRight = SaberTailorMain::config.rightPosY;
    SaberTailorMain::config.currentPosZRight = SaberTailorMain::config.rightPosZ;
    SaberTailorMain::config.currentRotXRight = SaberTailorMain::config.rightRotX;
    SaberTailorMain::config.currentRotYRight = SaberTailorMain::config.rightRotY;
    SaberTailorMain::config.currentRotZRight = SaberTailorMain::config.rightRotZ;
}