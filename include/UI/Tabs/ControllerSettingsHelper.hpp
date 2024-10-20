#pragma once

#include "bsml/shared/BSML-Lite.hpp"

namespace SaberTailor::Tabs::CSH{
    UnityEngine::GameObject* CreateCSHSettings(UnityEngine::Transform* parent);
}
namespace SaberTailor::Tabs::CSH{
    extern BSML::DropdownListSetting* offsetApplicationMethod;
    extern BSML::ToggleSetting* enableAxisArrows;
    extern BSML::ToggleSetting* mirrorZRot;
    extern BSML::ToggleSetting* axisInReplay;
    }