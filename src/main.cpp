#include "main.hpp"
#include "UI/SettingsFlowCoordinator.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/delegate.hpp"
#include "AxisDisplay.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "System/Action_1.hpp"
#include "Zenject/DiContainer.hpp"
#include "UnityEngine/XR/XRNode.hpp"
#include "GlobalNamespace/SaberModelContainer.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "UnityEngine/Mesh.hpp"
#include "bs-utils/shared/utils.hpp"

//april fools stuff
#include <ctime>
#include "GlobalNamespace/BeatmapObjectManager.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/GameplayLevelSceneTransitionEvents.hpp"
#include "System/Action.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/MainSettingsModelSO.hpp"
#include "GlobalNamespace/BeatmapDataCache.hpp"

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"

using namespace UnityEngine;
using namespace GlobalNamespace;

SaberTailorConfig SaberTailorMain::config;  
SaberTailor::Views::SettingsViewController* SaberTailorMain::saberTailorMainSettingsPage = nullptr;
SaberTailor::Views::SaberTailorLeftHand* SaberTailorMain::saberTailorLeftHand = nullptr;
SaberTailor::Views::SaberTailorRightHand* SaberTailorMain::saberTailorRightHand = nullptr;

ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
AprilFools::RandomSaber randomSaber;
// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

void SaberTailorMain::loadConfig() {
    getConfig().Load();
    ConfigHelper::LoadConfig(config, getConfig().config);
    auto now = std::time(0);
    auto* ltm = std::localtime(&now);
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    SaberTailorMain::config.isAprilFools = (month == 4 && day == 1);
    getLogger().info("Month: %i, Day: %i", month, day);
    getLogger().info("Is April Fools?: %i", (int)SaberTailorMain::config.isAprilFools);
}

MAKE_HOOK_MATCH(ControllerTransform, &OculusVRHelper::AdjustControllerTransform, void, OculusVRHelper* self, XR::XRNode node, Transform* transform, Vector3 position, Vector3 rotation) 
{  
    // right controller
    if (node == XR::XRNode::RightHand)
    {
        if(SaberTailorMain::config.isEnabled) // overrides base game settings with the saber tailor config (right hand)
        {
            position = Vector3(SaberTailorMain::config.rightHandPosition) /1000;
            rotation = Vector3(SaberTailorMain::config.rightHandRotation);
        }
        if (SaberTailorMain::config.isAprilFools && transform->get_name() == "RightHand"){
            rotation.x += randomSaber.rightXRot.first;
            rotation.y += randomSaber.rightYRot.first;
            rotation.z += randomSaber.rightZRot.first;
            position.z += randomSaber.zPos;
        }
        if(SaberTailorMain::config.overrideSettingsMethod) // overrides the order in which the settings are applied to the controller
        {
            transform->Rotate(Vector3(0, 0, rotation.z));
			transform->Translate(position);
			transform->Rotate(Vector3(rotation.x, rotation.y, 0));
            ControllerTransform(self, node, transform, Vector3(0.0f, 0.0f, -0.055f), Vector3(40, 0, 0));
        }
        else ControllerTransform(self, node, transform, position, rotation); // runs the original method which applies the settings in normal order
    }
    // left controller
    if (node == XR::XRNode::LeftHand)
    {
        if (SaberTailorMain::config.isEnabled) // overrides base game settings with the saber tailor config (left hand)
        {
            position = Vector3(SaberTailorMain::config.leftHandPosition) /1000;
            rotation = Vector3(SaberTailorMain::config.leftHandRotation);
        }
        else if (SaberTailorMain::config.mirrorZRot) rotation.z = -rotation.z; // imagine the base game not having bugs
        if (SaberTailorMain::config.isAprilFools && transform->get_name() == "LeftHand"){
            rotation.x += randomSaber.leftXRot.first;
            rotation.y += randomSaber.leftYRot.first;
            rotation.z += randomSaber.leftZRot.first;
            position.z += randomSaber.zPos;
        }
        if(SaberTailorMain::config.overrideSettingsMethod) // overrides the order in which the settings are applied to the controller
        {
            transform->Rotate(Vector3(0, 0, rotation.z));
			transform->Translate(position);
			transform->Rotate(Vector3(rotation.x, rotation.y, 0));
            ControllerTransform(self, node, transform, Vector3(0.0f, 0.0f, -0.055f), Vector3(40, 0, 0));
        }
        else ControllerTransform(self, node, transform, position, rotation); // runs the original method which applies the settings in normal order
    }
}

MAKE_HOOK_MATCH(MenuTransitionsHelper_RestartGame, &MenuTransitionsHelper::RestartGame, void, MenuTransitionsHelper* self, System::Action_1<Zenject::DiContainer*>* finishCallback)
{
    // stops soft restart from ruining my life
    Object::Destroy(SaberTailorMain::saberTailorMainSettingsPage);
    SaberTailorMain::saberTailorMainSettingsPage = nullptr;
    SaberTailorMain::saberTailorLeftHand = nullptr;
    SaberTailorMain::saberTailorRightHand = nullptr;
    MenuTransitionsHelper_RestartGame(self, finishCallback);
}

MAKE_HOOK_MATCH(AxisOnStart, &MainMenuViewController::DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    // enables axis arrows on start up or after soft restart if turned on in config
    AxisOnStart(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation && SaberTailorMain::config.spawnAxisDisplay){
        ArrayW<VRController*> controllers = (Resources::FindObjectsOfTypeAll<VRController*>());
        for (int i = 0; i<controllers.Length(); i++){
            if (controllers[i]->get_node() == XR::XRNode::LeftHand)(controllers)[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
            if (controllers[i]->get_node() == XR::XRNode::RightHand)(controllers)[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
        }
    }
    if (firstActivation){
        AprilFools::Init();
    }
}

MAKE_HOOK_MATCH(SaberModelContainer_Start, &SaberModelContainer::Start, void, SaberModelContainer* self) {
    SaberModelContainer_Start(self);
    if (SceneManagement::SceneManager::GetActiveScene().get_name() == "GameCore") {
        if (SaberTailorMain::config.enableScaleSettings){
            Saber* saber = self->saber;
            auto saberBottom = Vector3::get_zero();
            auto saberTop = Vector3::get_zero();
            saberBottom = saberBottom + Vector3(saber->saberBladeBottomTransform->get_position());
            saberTop = saberTop + Vector3(saber->saberBladeTopTransform->get_position());
            float length = (float)(SaberTailorMain::config.saberLength) / 100.0f;
            float width = (float)(SaberTailorMain::config.saberGirth) / 100.0f;

            auto currentScale = self->get_transform()->get_localScale();
            self->get_transform()->set_localScale({currentScale.x * width, currentScale.y * width, currentScale.z * length});
            auto currentPos = self->get_transform()->get_position();
            self->get_transform()->set_position({currentPos.x, currentPos.y, currentPos.z + (length - 1)/6});

            if (!SaberTailorMain::config.scaleHitbox){
                // the trails are bound to this and idk how to fix it when saber length is adjusted :pain:
                saber->saberBladeBottomTransform->set_position(saberBottom);
                saber->saberBladeTopTransform->set_position(saberTop);
                bs_utils::Submission::enable(modInfo);
            }
            else bs_utils::Submission::disable(modInfo); 
        }
        else bs_utils::Submission::enable(modInfo);
        if (SaberTailorMain::config.enableTrailSettings){
            auto trail = self->get_transform()->GetComponentInChildren<SaberModelController*>()->saberTrail;
            trail->trailDuration = (float)SaberTailorMain::config.trailDuration/1000.0f;
            trail->whiteSectionMaxDuration = (float)SaberTailorMain::config.whitestepDuration/1000.0f;
            trail->granularity = SaberTailorMain::config.trailGranularity;
            trail->trailRenderer->set_enabled(SaberTailorMain::config.enableSaberTrails);
        }
    }
}

// April Fools Stuff

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameplayCoreSceneSetupData_ctor, "", "GameplayCoreSceneSetupData", ".ctor", void, GameplayCoreSceneSetupData* self, IDifficultyBeatmap* difficultyBeatmap, IPreviewBeatmapLevel* previewBeatmapLevel, GameplayModifiers* gameplayModifiers, PlayerSpecificSettings* playerSpecificSettings, PracticeSettings* practiceSettings, bool useTestNoteCutSoundEffects, EnvironmentInfoSO* environmentInfo, ColorScheme* colorScheme, MainSettingsModelSO* mainSettingsModel, BeatmapDataCache* beatmapDataCache)
{
    GameplayCoreSceneSetupData_ctor(self, difficultyBeatmap, previewBeatmapLevel, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects, environmentInfo, colorScheme, mainSettingsModel, beatmapDataCache);
    if (SaberTailorMain::config.isAprilFools){
        randomSaber.zPos -= 0.003;
        AprilFools::generateRandomSaberMovementsForSession();
    }
}
// MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameplayLevelSceneTransitionEvents_ctor, "", "GameplayLevelSceneTransitionEvents", ".ctor", void, GameplayLevelSceneTransitionEvents* self, StandardLevelScenesTransitionSetupDataSO* standardLevelScenesTransitionSetupData, MissionLevelScenesTransitionSetupDataSO* missionLevelScenesTransitionSetupData, MultiplayerLevelScenesTransitionSetupDataSO* multiplayerLevelScenesTransitionSetupData)
// {
//     GameplayLevelSceneTransitionEvents_ctor(self, standardLevelScenesTransitionSetupData, missionLevelScenesTransitionSetupData, multiplayerLevelScenesTransitionSetupData);
//     if (SaberTailorMain::config.isAprilFools){
//         self->add_anyGameplayLevelDidFinishEvent(custom_types::MakeDelegate<System::Action*>(classof(System::Action*),  static_cast<std::function<void()>>([](){AprilFools::Init();})));
//     }
// }
MAKE_HOOK_MATCH(NoteMissed, &BeatmapObjectManager::HandleNoteControllerNoteWasMissed, void, BeatmapObjectManager* self, NoteController* noteController)
{
    NoteMissed(self, noteController);
    if (SaberTailorMain::config.isAprilFools){
        int hand = noteController->get_noteData()->get_colorType() == ColorType::ColorA ? 0 : 1;
        AprilFools::updateValuesOnMiss(hand);
    }
}

MAKE_HOOK_MATCH(NoteCut, &BeatmapObjectManager::HandleNoteControllerNoteWasCut, void, BeatmapObjectManager* self, NoteController* noteController, ByRef<NoteCutInfo> noteCutInfo)
{
    NoteCut(self, noteController, noteCutInfo);
    if(SaberTailorMain::config.isAprilFools && !noteCutInfo.heldRef.directionOK){
        int hand = noteCutInfo.heldRef.saberType.value;
        AprilFools::updateValuesOnMiss(hand);
    }
}


// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    Modloader::requireMod("Qclaws");
    il2cpp_functions::Init();
    SaberTailorMain::loadConfig();
    QuestUI::Init();
    custom_types::Register::AutoRegister();
    QuestUI::Register::RegisterModSettingsFlowCoordinator<SaberTailor::SettingsFlowCoordinator*>(modInfo); // display in settings
    QuestUI::Register::RegisterMainMenuModSettingsFlowCoordinator<SaberTailor::SettingsFlowCoordinator*>(modInfo); // display in menu
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), ControllerTransform);
    INSTALL_HOOK(getLogger(), MenuTransitionsHelper_RestartGame);
    INSTALL_HOOK(getLogger(), AxisOnStart);
    INSTALL_HOOK(getLogger(), NoteMissed);
    INSTALL_HOOK(getLogger(), NoteCut);
    INSTALL_HOOK(getLogger(), GameplayCoreSceneSetupData_ctor);
    INSTALL_HOOK(getLogger(), SaberModelContainer_Start);
    // INSTALL_HOOK(getLogger(), GameplayLevelSceneTransitionEvents_ctor);
    getLogger().info("Installed all hooks!");
}