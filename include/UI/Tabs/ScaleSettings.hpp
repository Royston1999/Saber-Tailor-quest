#pragma once

#include "questui/shared/BeatSaberUI.hpp"

namespace SaberTailor::Tabs::Scale{
    UnityEngine::GameObject* CreateScaleSettings(UnityEngine::Transform* parent);
}
namespace SaberTailor::Tabs::Scale{
    extern QuestUI::IncrementSetting* length;
    extern QuestUI::IncrementSetting* width;
    extern UnityEngine::UI::Toggle* enableScaleSettings;
    extern UnityEngine::UI::Toggle* scaleHitbox;
}