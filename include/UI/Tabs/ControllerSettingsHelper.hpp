#pragma once

#include "questui/shared/BeatSaberUI.hpp"

namespace SaberTailor::Tabs::CSH{
    UnityEngine::GameObject* CreateCSHSettings(UnityEngine::Transform* parent);
}
namespace SaberTailor::Tabs::CSH{
    extern UnityEngine::UI::Toggle* enableSettingsOverride;
    extern UnityEngine::UI::Toggle* enableAxisArrows;
    extern UnityEngine::UI::Toggle* mirrorZRot;
    extern UnityEngine::UI::Toggle* axisInReplay;
    }