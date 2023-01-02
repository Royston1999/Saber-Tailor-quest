#include "UI/Tabs/ControllerSettingsHelper.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"
#include "AxisDisplay.hpp"
#include "GlobalNamespace/VRController.hpp"

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace GlobalNamespace;

namespace SaberTailor::Tabs::CSH{
    Toggle* enableSettingsOverride;
    Toggle* enableAxisArrows;
    Toggle* mirrorZRot;

    GameObject* CreateCSHSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Controller Settings Helper");
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);
        
        enableAxisArrows = CreateToggle(container->get_transform(), "Enable Axis Arrows in Menu", SaberTailorMain::config.spawnAxisDisplay, [](bool value) {
            setBool(SaberTailorMain::config.currentlyLoadedConfig, "axisEnabled", value, false);
            if(SaberTailorMain::config.spawnAxisDisplay){
                ArrayW<VRController*> controllers = Resources::FindObjectsOfTypeAll<VRController*>();
                for (int i = 0; i<controllers.Length(); i++){
                    if (controllers[i]->get_node() == XR::XRNode::LeftHand)controllers[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
                    if (controllers[i]->get_node() == XR::XRNode::RightHand)controllers[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
                }       
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
        
        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}