#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "bsml/shared/BSML-Lite.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor::Views, SettingsViewController, HMUI::ViewController) {
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    public:
        custom_types::Helpers::Coroutine CreateTabs();
        std::vector<UnityEngine::GameObject*> tabs;
        int selectedTab;
};