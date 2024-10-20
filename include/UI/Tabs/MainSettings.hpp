#pragma once

#include "bsml/shared/BSML-Lite.hpp"

namespace SaberTailor::Tabs::MainSettings{
    UnityEngine::GameObject* CreateMainSettings(UnityEngine::Transform* parent);
}

namespace SaberTailor::Tabs::MainSettings{
    extern BSML::IncrementSetting* saberPosIncrement;
    extern BSML::IncrementSetting* saberRotIncrement;
    extern BSML::ToggleSetting* enableSaberTailor;
    extern BSML::ToggleSetting* enableBaseGameAdjustment;
    extern BSML::DropdownListSetting* incrementUnits;
    extern BSML::DropdownListSetting* displayUnits;
    void updatePosIncrements(int inc);
    void updateRotIncrements(int inc);
}
