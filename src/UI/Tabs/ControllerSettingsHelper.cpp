#include "UI/Tabs/ControllerSettingsHelper.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"
#include "AxisDisplay.hpp"
#include "GlobalNamespace/VRController.hpp"

using namespace BSML::Lite;
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace ControllerSettingsHelper;
namespace SaberTailor::Tabs::CSH{
    BSML::DropdownListSetting* offsetApplicationMethod;
    BSML::ToggleSetting* enableAxisArrows;
    BSML::ToggleSetting* mirrorZRot;
    BSML::ToggleSetting* axisInReplay;
    static std::vector<std::string_view> options = {"Default", "Pre Unity Update", "Electro Method"};

    GameObject* CreateCSHSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Controller Settings Helper", {0, 0}, {0, 7});
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);
        
        enableAxisArrows = CreateToggle(container->get_transform(), "Enable Axis Arrows in Menu", GET_VALUE(axisEnabled), [](bool value) {
            SET_VALUE(axisEnabled, value);
            if(GET_VALUE(axisEnabled)){
                for (auto& controller : Resources::FindObjectsOfTypeAll<VRController*>()) AxisDisplay::CreateAxes(controller->get_viewAnchorTransform());   
            }
        });
        AddHoverHint(enableAxisArrows->get_gameObject(), "Spawns axes that track the controller's movement and rotation");
        
        offsetApplicationMethod = CreateDropdown(container->get_transform(), "Controller Settings Application Method", options[(int)GET_VALUE(offsetApplicationMethod)], options, [](StringW value) {
            SET_VALUE(offsetApplicationMethod, (ApplicationMethod)(std::find(options.begin(), options.end(), value) - options.begin()));
            getPlatformHelper()->RefreshControllersReference();
        } );
        AddHoverHint(offsetApplicationMethod->get_gameObject(), "Changes the order in which the positions and rotations are applied to the controller");

        mirrorZRot = CreateToggle(container->get_transform(), "Mirror Z Rotations for Left", GET_VALUE(mirrorZRot), [](bool value) {
            SET_VALUE(mirrorZRot, value);
        } );
        AddHoverHint(mirrorZRot->get_gameObject(), "BeatGames has fixed this now but I have not removed the toggle yet!!!! I may remove it eventually lmao");

        axisInReplay = CreateToggle(container->get_transform(), "Axis Arrows in Replay", GET_VALUE(axisInReplay), [](bool value) {
            SET_VALUE(axisInReplay, value);
        } );
        AddHoverHint(axisInReplay->get_gameObject(), "Enables the axis arrows to be viewed in replays");
        
        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}