#pragma once

#include "bsml/shared/BSML-Lite.hpp"

namespace SaberTailor::Tabs::Scale{
    UnityEngine::GameObject* CreateScaleSettings(UnityEngine::Transform* parent);
}
namespace SaberTailor::Tabs::Scale{
    extern BSML::IncrementSetting* length;
    extern BSML::IncrementSetting* width;
    extern BSML::ToggleSetting* enableScaleSettings;
    extern BSML::ToggleSetting* scaleHitbox;
}