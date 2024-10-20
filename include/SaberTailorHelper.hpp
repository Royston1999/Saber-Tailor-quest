#pragma once

#include "bsml/shared/BSML-Lite.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Vector3.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SaberTailorRightHand.hpp"
#include "UnityEngine/WaitForFixedUpdate.hpp"
#include <sstream>
#include <iomanip>
#include "bsml/shared/BSML/Components/ExternalComponents.hpp"
#include "main.hpp"

namespace IncrementHelper{
    std::string Round(float val, int precision);
    void SetIncrementText(BSML::IncrementSetting* setting, std::string text);
    float getPosIncrement();
    std::string formatSliderPosText(float value);
}
namespace PosRotHelper{
    void revertLeftHand(bool toZero);
    void revertRightHand(bool toZero);
}
namespace TransferHelper{
    void mirrorToLeft();
    void mirrorToRight();
    void exportToBaseGame(int hand);
    void importFromBaseGame(int hand);
}
namespace TabHelper{
    UnityEngine::GameObject* AdjustedScrollContainerObject(UnityEngine::GameObject* container, bool active);
}