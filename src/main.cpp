#include "main.hpp"
#include "SettingsFlowCoordinator.hpp"
#include <iomanip>
#include <sstream>
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "AxisDisplay.hpp"
#include "System/Math.hpp"
#include "HMUI/ImageView.hpp"
#include "GlobalNamespace/ScoreFormatter.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/TextAnchor.hpp"
#include "UnityEngine/Transform.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "System/Action_1.hpp"
#include "Zenject/DiContainer.hpp"
#include "UnityEngine/XR/XRNode.hpp"

using namespace UnityEngine;
using namespace GlobalNamespace;

SaberTailorConfig SaberTailorMain::config;  
SaberTailor::Views::SettingsViewController* SaberTailorMain::saberTailorMainSettingsPage = nullptr;
static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

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
}

MAKE_HOOK_MATCH(ControllerTransform, &OculusVRHelper::AdjustControllerTransform, void, OculusVRHelper* self, XR::XRNode node, Transform* transform, Vector3 position, Vector3 rotation) {  
    if (node == XR::XRNode::RightHand)
    {
        if(SaberTailorMain::config.isEnabled){
            position = Vector3((SaberTailorMain::config.rightPosX)/100, (SaberTailorMain::config.rightPosY)/100, (SaberTailorMain::config.rightPosZ)/100);
            rotation = Vector3((float)(SaberTailorMain::config.rightRotX), (float)(SaberTailorMain::config.rightRotY), (float)(SaberTailorMain::config.rightRotZ));
        }
        if(SaberTailorMain::config.overrideSettingsMethod){
            transform->Rotate(Vector3(0, 0, rotation.z));
			transform->Translate(position);
			transform->Rotate(Vector3(rotation.x, rotation.y, 0));
        }
        else ControllerTransform(self, node, transform, position, rotation);
    }
    if (node == XR::XRNode::LeftHand)
    {
        if (SaberTailorMain::config.isEnabled){
            position = Vector3((SaberTailorMain::config.leftPosX)/100, (SaberTailorMain::config.leftPosY)/100, (SaberTailorMain::config.leftPosZ)/100);
            rotation = Vector3((float)(SaberTailorMain::config.leftRotX), (float)(SaberTailorMain::config.leftRotY), (float)(SaberTailorMain::config.leftRotZ));
        }
        else if (SaberTailorMain::config.mirrorZRot){
            rotation.z = -rotation.z;
        }
        if(SaberTailorMain::config.overrideSettingsMethod){
            transform->Rotate(Vector3(0, 0, rotation.z));
			transform->Translate(position);
			transform->Rotate(Vector3(rotation.x, rotation.y, 0));
        }
        else ControllerTransform(self, node, transform, position, rotation);
    }
}

MAKE_HOOK_MATCH(MenuTransitionsHelper_RestartGame, &MenuTransitionsHelper::RestartGame, void, MenuTransitionsHelper* self, System::Action_1<Zenject::DiContainer*>* finishCallback)
{
    Object::Destroy(SaberTailorMain::saberTailorMainSettingsPage);
    SaberTailorMain::saberTailorMainSettingsPage = nullptr;
    MenuTransitionsHelper_RestartGame(self, finishCallback);
}

MAKE_HOOK_MATCH(AxisOnStart, &MainMenuViewController::DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    AxisOnStart(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation && SaberTailorMain::config.spawnAxisDisplay){
        Array<VRController*>* controllers = (Resources::FindObjectsOfTypeAll<VRController*>());
        for (int i = 0; i<controllers->get_Length(); i++){
            if ((*controllers)[i]->get_node() == XR::XRNode::LeftHand)(*controllers)[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
            if ((*controllers)[i]->get_node() == XR::XRNode::RightHand)(*controllers)[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
        }
    }
}


// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    SaberTailorMain::loadConfig();
    QuestUI::Init();
    custom_types::Register::AutoRegister();
    QuestUI::Register::RegisterModSettingsFlowCoordinator<SaberTailor::SettingsFlowCoordinator*>(modInfo); // display in settings
    QuestUI::Register::RegisterMainMenuModSettingsFlowCoordinator<SaberTailor::SettingsFlowCoordinator*>(modInfo); // display in menu
    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    INSTALL_HOOK(getLogger(), ControllerTransform);
    INSTALL_HOOK(getLogger(), MenuTransitionsHelper_RestartGame);
    INSTALL_HOOK(getLogger(), AxisOnStart);
    getLogger().info("Installed all hooks!");
}