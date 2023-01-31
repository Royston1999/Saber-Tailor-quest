#include "UI/Tabs/ControllerSettingsHelper.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"
#include "AxisDisplay.hpp"
#include "GlobalNamespace/VRController.hpp"

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace ControllerSettingsHelper;

namespace SaberTailor::Tabs::CSH{
    Toggle* enableSettingsOverride;
    Toggle* enableAxisArrows;
    Toggle* mirrorZRot;
    Toggle* axisInReplay;

    GameObject* CreateCSHSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Controller Settings Helper");
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);
        
        enableAxisArrows = CreateToggle(container->get_transform(), "Enable Axis Arrows in Menu", SaberTailorMain::config.spawnAxisDisplay, [](bool value) {
            setBool(SaberTailorMain::config.currentlyLoadedConfig, "axisEnabled", value, false);
            if(SaberTailorMain::config.spawnAxisDisplay){
                for (auto& controller : Resources::FindObjectsOfTypeAll<VRController*>()) AxisDisplay::CreateAxes(controller->get_transform());   
            }
        });
        AddHoverHint(enableAxisArrows->get_gameObject(), "Spawns axes that track the controller's movement and rotation");
        
        enableSettingsOverride = CreateToggle(container->get_transform(), "Custom Controller Settings Method", SaberTailorMain::config.overrideSettingsMethod, [](bool value) {
            setBool(SaberTailorMain::config.currentlyLoadedConfig, "overrideSettingsMethod", value, false);
        } );
        AddHoverHint(enableSettingsOverride->get_gameObject(), "Changes the order in which the positions and rotations are applied to the controller");

        mirrorZRot = CreateToggle(container->get_transform(), "Mirror Z Rotations for Left", SaberTailorMain::config.mirrorZRot, [](bool value) {
            setBool(SaberTailorMain::config.currentlyLoadedConfig, "mirrorZRot", value, false);
        } );
        AddHoverHint(mirrorZRot->get_gameObject(), "Fixes left hand Z Rotation when using base game settings. INEFFECTIVE WHEN SABER TAILOR IS ENABLED");

        axisInReplay = CreateToggle(container->get_transform(), "Axis Arrows in Replay", SaberTailorMain::config.axisInReplay, [](bool value) {
            setBool(SaberTailorMain::config.currentlyLoadedConfig, "axisInReplay", value, false);
        } );
        AddHoverHint(axisInReplay->get_gameObject(), "Enables the axis arrows to be viewed in replays");
        
        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}