#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "main.hpp"
#include "HMUI/FlowCoordinator.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor, SettingsFlowCoordinator, HMUI::FlowCoordinator,

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);
)