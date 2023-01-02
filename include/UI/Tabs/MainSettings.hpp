#pragma once

#include "questui/shared/BeatSaberUI.hpp"

namespace SaberTailor::Tabs::MainSettings{
    UnityEngine::GameObject* CreateMainSettings(UnityEngine::Transform* parent);
}

namespace SaberTailor::Tabs::MainSettings{
    extern QuestUI::IncrementSetting* saberPosIncrement;
    extern QuestUI::IncrementSetting* saberRotIncrement;
    extern UnityEngine::UI::Toggle* enableSaberTailor;
    extern UnityEngine::UI::Toggle* enableBaseGameAdjustment;
    extern HMUI::SimpleTextDropdown* incrementUnits;
    extern HMUI::SimpleTextDropdown* displayUnits;
    void updatePosIncrements(int inc);
    void updateRotIncrements(int inc);
}
