#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "main.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor, SettingsFlowCoordinator, HMUI::FlowCoordinator,
    DECLARE_INSTANCE_FIELD(SaberTailor::Views::SettingsViewController*, settingsViewController);
    DECLARE_INSTANCE_FIELD(SaberTailor::Views::SaberTailorLeftHand*, leftHand);
    DECLARE_INSTANCE_FIELD(SaberTailor::Views::SaberTailorRightHand*, rightHand);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);
)