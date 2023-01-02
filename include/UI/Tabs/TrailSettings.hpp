#pragma once

#include "questui/shared/BeatSaberUI.hpp"

namespace SaberTailor::Tabs::Trail{
    UnityEngine::GameObject* CreateTrailSettings(UnityEngine::Transform* parent);
}
namespace SaberTailor::Tabs::Trail{
    extern QuestUI::IncrementSetting* duration;
    extern QuestUI::IncrementSetting* whitestep;
    extern UnityEngine::UI::Toggle* enableTrailSettings;
    extern UnityEngine::UI::Toggle* enableSaberTrails;
    extern QuestUI::IncrementSetting* granularity;
}