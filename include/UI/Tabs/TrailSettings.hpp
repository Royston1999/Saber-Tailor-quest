#pragma once

#include "bsml/shared/BSML-Lite.hpp"

namespace SaberTailor::Tabs::Trail{
    UnityEngine::GameObject* CreateTrailSettings(UnityEngine::Transform* parent);
}
namespace SaberTailor::Tabs::Trail{
    extern BSML::IncrementSetting* duration;
    extern BSML::IncrementSetting* whitestep;
    extern BSML::ToggleSetting* enableTrailSettings;
    extern BSML::ToggleSetting* enableSaberTrails;
    extern BSML::IncrementSetting* granularity;
}