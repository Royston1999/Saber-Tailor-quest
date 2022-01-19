#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor::Views, SettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(HMUI::FlowCoordinator*, flowCoordinator);

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, leftButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, rightButton);
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

public:

    UnityEngine::UI::Button* CreateUIViewControllerButton(UnityEngine::Transform* parent, std::string title, HMUI::ViewController* viewController);
)