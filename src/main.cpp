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
#include "metacore/shared/game.hpp"
#include "bsml/shared/BSML.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

//april fools stuff
#include <ctime>
#include "GlobalNamespace/BeatmapObjectManager.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/GameplayLevelSceneTransitionEvents.hpp"
#include "System/Action.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"

#include "conditional-dependencies/shared/main.hpp"
#include "GlobalNamespace/PauseController.hpp"
#include "GlobalNamespace/IGamePause.hpp"
#include "Utils/EasyDelegate.hpp"
#include "SaberTailorSuperSlider.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/IVRPlatformHelper.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"
#include "GlobalNamespace/ControllersTransformSettingsViewController.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"

#include "SaberTailorControllerTransformOffset.hpp"

#include "System/Nullable_1.hpp"

#include "bsml/shared/Helpers/getters.hpp"
#include "bsml/shared/Helpers/creation.hpp"
#include "GlobalNamespace/SettingsFlowCoordinator.hpp"
#include "GlobalNamespace/MainSettingsMenuViewController.hpp"

using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace ControllerSettingsHelper;

SaberTailorConfig SaberTailorMain::config;  
bool isReplay = false;
bool metalitReplay = false;

modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
AprilFools::RandomSaber randomSaber;
// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
const Paper::ConstLoggerContext<12UL>& getLogger() {
    static constexpr auto Logger = Paper::ConstLoggerContext("sabertailor");
    return Logger;
}

// Called at the early stages of game loading
SABER_TAILOR_EXPORT_FUNC void setup(CModInfo& info) {
    info = modInfo.to_c();

    getConfig().Load();
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
    getLogger().info("Month: {0}, Day: {1}", month, day);
    getLogger().info("Is April Fools?: {}", (int)SaberTailorMain::config.isAprilFools);
}

IVRPlatformHelper* getPlatformHelper() {
    return BSML::Helpers::GetIVRPlatformHelper();
}

SaberTailor::SaberTailorControllerTransformOffset* getSaberTailorOffset() {
    static SafePtrUnity<SaberTailor::SaberTailorControllerTransformOffset> offset;
    if (offset) return offset.ptr();
    auto off = GameObject::New_ctor("SaberTailorControllerTransformOffset")->AddComponent<SaberTailor::SaberTailorControllerTransformOffset*>();
    Object::DontDestroyOnLoad(off);
    offset = off;
    return offset.ptr();
}

static Vector3 operator/(Vector3 a, float_t b) { return Vector3::op_Division(a,b); }
static Vector3 operator+=(Vector3& a, Vector3 b) { return a = Vector3::op_Addition(a, b); }
static void operator*=(Quaternion& a, Quaternion b) { a = Quaternion::op_Multiply(a, b); }

MAKE_HOOK_MATCH(ControllerTransform, static_cast<bool(VRController::*)(ByRef<Pose>)>(&VRController::TryGetControllerOffset), bool, VRController* self, ByRef<Pose> poseOffset) {
    
    if (!(GET_VALUE(isGripModEnabled) || GET_VALUE(offsetApplicationMethod) == SaberTailor::ApplicationMethod::ElectroMethod || SaberTailorMain::config.isAprilFools)) return ControllerTransform(self, poseOffset);

    GlobalNamespace::VRControllerTransformOffset* controllerOffset = GET_VALUE(isGripModEnabled) ? getSaberTailorOffset() : self->____transformOffset;
    if (!controllerOffset) controllerOffset = getSaberTailorOffset();

    bool isRightHand = self->get_node() == XR::XRNode::RightHand;
    Pose controllerPose = isRightHand ? 
        Pose(controllerOffset->get_rightPositionOffset(), Quaternion::Euler(controllerOffset->get_rightRotationOffset())) : 
        Pose(controllerOffset->get_leftPositionOffset(), Quaternion::Euler(controllerOffset->get_leftRotationOffset()));
    
    SaberTailor::ApplicationMethod applicationMethod = GET_VALUE(offsetApplicationMethod);
    UnityW<Transform> anchor = self->get_viewAnchorTransform();
    if (applicationMethod != SaberTailor::ApplicationMethod::ElectroMethod) {
        VRController::TryGetControllerOffset(self->____vrPlatformHelper, controllerOffset, self->____node, ByRef(controllerPose));
    }
    else if(applicationMethod == SaberTailor::ApplicationMethod::ElectroMethod) {
        anchor->SetLocalPositionAndRotation(Vector3::get_zero(), Quaternion::get_identity());
        anchor->Rotate({0, 0, controllerPose.rotation.get_eulerAngles().z});
        anchor->Translate(controllerPose.position);
        anchor->Rotate({controllerPose.rotation.get_eulerAngles().x, controllerPose.rotation.get_eulerAngles().y, 0});
        controllerPose = Pose(anchor->get_localPosition(), anchor->get_localRotation());
    }

    if (SaberTailorMain::config.isAprilFools) {
        if (self->get_transform()->get_name() == "RightHand")
            controllerPose.rotation *= Quaternion::Euler(randomSaber.rightXRot.first, randomSaber.rightYRot.first, randomSaber.rightZRot.first);
        else if (self->get_transform()->get_name() == "LeftHand")
            controllerPose.rotation *= Quaternion::Euler(randomSaber.leftXRot.first, randomSaber.leftYRot.first, randomSaber.leftZRot.first);
        controllerPose.position.z += randomSaber.zPos;
    }
    *poseOffset = controllerPose;
    return true;
}

MAKE_HOOK_MATCH(AxisOnStart, &MainMenuViewController::DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    // enables axis arrows on start up or after soft restart if turned on in config
    AxisOnStart(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (!firstActivation) return;
    AprilFools::Init();
    if (SaberTailorMain::config.currentConfig.axisEnabled){
        for (auto& controller : Resources::FindObjectsOfTypeAll<VRController*>()) AxisDisplay::CreateAxes(controller->get_viewAnchorTransform());
    }
    SaberTailor::SaberTailorSuperSlider::Init();
    
    // auto mainSettings = BSML::Helpers::GetMainFlowCoordinator()->____settingsFlowCoordinator->____mainSettingsMenuViewController;
    // mainSettings->____settingsSubMenuInfos[3]->____viewController = BSML::Helpers::CreateViewController<SaberTailor::Views::SettingsViewController*>();
}

MAKE_HOOK_MATCH(SaberModelContainer_Start, &SaberModelContainer::Start, void, SaberModelContainer* self) {
    SaberModelContainer_Start(self);
    if (SceneManagement::SceneManager::GetActiveScene().get_name() == "GameCore") {
        if (SaberTailorMain::config.currentConfig.isSaberScaleModEnabled){
            Saber* saber = self->____saber;
            auto saberBottom = Vector3::get_zero();
            auto saberTop = Vector3::get_zero();
            saberBottom = Vector3::op_Addition(saberBottom, Vector3(saber->____saberBladeBottomTransform->get_position()));
            saberTop = Vector3::op_Addition(saberTop, Vector3(saber->____saberBladeTopTransform->get_position()));
            float length = (float)(SaberTailorMain::config.currentConfig.saberLength) / 100.0f;
            float width = (float)(SaberTailorMain::config.currentConfig.saberGirth) / 100.0f;

            auto currentScale = self->get_transform()->get_localScale();
            self->get_transform()->set_localScale({currentScale.x * width, currentScale.y * width, currentScale.z * length});
            auto currentPos = self->get_transform()->get_position();
            self->get_transform()->set_position({currentPos.x, currentPos.y, currentPos.z + (length - 1)/6});

            if (!SaberTailorMain::config.currentConfig.saberScaleHitbox){
                // the trails are bound to this and idk how to fix it when saber length is adjusted :pain:
                saber->____saberBladeBottomTransform->set_position(saberBottom);
                saber->____saberBladeTopTransform->set_position(saberTop);
                MetaCore::Game::SetScoreSubmission(modInfo.id, true);
            }
            else MetaCore::Game::SetScoreSubmission(modInfo.id, false);
        }
        else MetaCore::Game::SetScoreSubmission(modInfo.id, true);
        if (SaberTailorMain::config.currentConfig.isTrailModEnabled){
            auto trail = self->get_transform()->GetComponentInChildren<SaberModelController*>()->____saberTrail;
            trail->____trailDuration = (float)SaberTailorMain::config.currentConfig.trailDuration/1000.0f;
            trail->____whiteSectionMaxDuration = (float)SaberTailorMain::config.currentConfig.trailWhiteSectionDuration/1000.0f;
            trail->____granularity = SaberTailorMain::config.currentConfig.trailGranularity;
            trail->____trailRenderer->set_enabled(SaberTailorMain::config.currentConfig.isTrailEnabled);
        }
        if (isReplay && SaberTailorMain::config.currentConfig.axisInReplay){
            AxisDisplay::CreateAxes(self->get_transform());
        }
    }
}

MAKE_HOOK_MATCH(ReplayCheck, &StandardLevelScenesTransitionSetupDataSO::InitAndSetupScenes, void, StandardLevelScenesTransitionSetupDataSO* self, PlayerSpecificSettings* playerSpecificSettings, StringW backButtonText, bool startPaused) {
    ReplayCheck(self, playerSpecificSettings, backButtonText, startPaused);
    auto metalit = CondDeps::Find<bool>("replay", "IsInReplay");
    if (self->get_gameMode() == "Replay" || (metalit.has_value() && (*metalit)())) isReplay = true;
}

// April Fools Stuff

MAKE_HOOK_MATCH(GameplayCoreSceneSetupData_ctor,
    static_cast<void(GameplayCoreSceneSetupData::*)
    (
        ByRef<BeatmapKey>, BeatmapLevel*, GameplayModifiers*,
        PlayerSpecificSettings*, PracticeSettings*, bool,
        EnvironmentInfoSO*, EnvironmentInfoSO*, ColorScheme*,
        SettingsManager*, AudioClipAsyncLoader*, BeatmapDataLoader*,
        BeatmapLevelsEntitlementModel*, bool, bool,
        EnvironmentsListModel*, System::Nullable_1<RecordingToolManager_SetupData>
    )>(&GameplayCoreSceneSetupData::_ctor),
    void,
    GameplayCoreSceneSetupData* self,
    ByRef<BeatmapKey> beatmapKey, BeatmapLevel* beatmapLevel, GameplayModifiers* gameplayModifiers,
    PlayerSpecificSettings* playerSpecificSettings, PracticeSettings* practiceSettings, bool useTestNoteCutSoundEffects,
    EnvironmentInfoSO* targetEnvironmentInfo, EnvironmentInfoSO* originalEnvironmentInfo, ColorScheme* colorScheme,
    SettingsManager* settingsManager, AudioClipAsyncLoader* audioClipAsyncLoader, BeatmapDataLoader* beatmapDataLoader,
    BeatmapLevelsEntitlementModel* beatmapLevelsEntitlementModel, bool enableBeatmapDataCaching, bool allowNullBeatmapLevelData,
    EnvironmentsListModel* environmentsListModel, System::Nullable_1<RecordingToolManager_SetupData> recordingToolData
) {
    GameplayCoreSceneSetupData_ctor(
        self, beatmapKey, beatmapLevel, gameplayModifiers,
        playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects,
        targetEnvironmentInfo, originalEnvironmentInfo, colorScheme,
        settingsManager, audioClipAsyncLoader, beatmapDataLoader,
        beatmapLevelsEntitlementModel, enableBeatmapDataCaching,
        allowNullBeatmapLevelData, environmentsListModel, recordingToolData
    );
    if (SaberTailorMain::config.isAprilFools){
        randomSaber.zPos -= 0.003;
        AprilFools::generateRandomSaberMovementsForSession();
    }
}
MAKE_HOOK_MATCH(GameplayLevelSceneTransitionEvents_ctor, &GameplayLevelSceneTransitionEvents::_ctor, void, GameplayLevelSceneTransitionEvents* self, StandardLevelScenesTransitionSetupDataSO* standardLevelScenesTransitionSetupData, MissionLevelScenesTransitionSetupDataSO* missionLevelScenesTransitionSetupData, MultiplayerLevelScenesTransitionSetupDataSO* multiplayerLevelScenesTransitionSetupData)
{
    GameplayLevelSceneTransitionEvents_ctor(self, standardLevelScenesTransitionSetupData, missionLevelScenesTransitionSetupData, multiplayerLevelScenesTransitionSetupData);    
    self->add_anyGameplayLevelDidFinishEvent(EasyDelegate::MakeDelegate<System::Action*>([](){isReplay = false;}));
}

MAKE_HOOK_MATCH(NoteMissed, &BeatmapObjectManager::HandleNoteControllerNoteWasMissed, void, BeatmapObjectManager* self, NoteController* noteController)
{
    NoteMissed(self, noteController);
    if (SaberTailorMain::config.isAprilFools){
        switch (noteController->get_noteData()->get_colorType()) {
            case ColorType::ColorA:
                AprilFools::updateValuesOnMiss(0); break;
            case ColorType::ColorB:
                AprilFools::updateValuesOnMiss(1); break;
            case ColorType::None:
                break;
            default:
                break;
        }
    }
}

MAKE_HOOK_MATCH(NoteCut, &BeatmapObjectManager::HandleNoteControllerNoteWasCut, void, BeatmapObjectManager* self, NoteController* noteController, ByRef<NoteCutInfo> noteCutInfo)
{
    NoteCut(self, noteController, noteCutInfo);
    if(SaberTailorMain::config.isAprilFools && !noteCutInfo.heldRef.directionOK){
        int hand = noteCutInfo.heldRef.saberType.value__;
        AprilFools::updateValuesOnMiss(hand);
    }
}

MAKE_HOOK_MATCH(PauseController_Start, &PauseController::Start, void, PauseController* self){
    PauseController_Start(self);
    if (!isReplay) return;
    std::function<void(bool)> func = [](bool value){
        for (auto& axis : Resources::FindObjectsOfTypeAll<AxisDisplay*>()) 
            if (axis->get_name() == "AxisDisplayReplay") axis->get_gameObject()->set_active(value);
    };   
    self->____gamePause->add_didPauseEvent(EasyDelegate::MakeDelegate<System::Action*>([func](){func(false);}));
    self->____gamePause->add_didResumeEvent(EasyDelegate::MakeDelegate<System::Action*>([func](){func(true);}));
}

// Called later on in the game loading - a good time to install function hooks
SABER_TAILOR_EXPORT_FUNC void late_load() {
    il2cpp_functions::Init();
    getMainConfig().Init(modInfo);
    SaberTailorMain::loadConfig();
    BSML::Init();
    custom_types::Register::AutoRegister();
    BSML::Register::RegisterMainMenuFlowCoordinator("Saber Tailor", "edit saber stuff", csTypeOf(SaberTailor::SettingsFlowCoordinator*));
    getLogger().info("Installing hooks...");
    INSTALL_HOOK_ORIG(getLogger(), ControllerTransform);
    INSTALL_HOOK(getLogger(), AxisOnStart);
    INSTALL_HOOK(getLogger(), NoteMissed);
    INSTALL_HOOK(getLogger(), NoteCut);
    INSTALL_HOOK(getLogger(), GameplayCoreSceneSetupData_ctor);
    INSTALL_HOOK(getLogger(), SaberModelContainer_Start);
    // INSTALL_HOOK(getLogger(), MenuTransitionsHelper_StartStandardLevel);
    INSTALL_HOOK(getLogger(), GameplayLevelSceneTransitionEvents_ctor);
    INSTALL_HOOK(getLogger(), PauseController_Start);
    INSTALL_HOOK(getLogger(), ReplayCheck);
    getLogger().info("Installed all hooks!");
}