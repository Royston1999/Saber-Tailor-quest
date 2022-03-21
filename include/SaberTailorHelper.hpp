#pragma once

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/MainSettingsModelSO.hpp"
#include "UnityEngine/Resources.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "GlobalNamespace/Vector3SO.hpp"
#include "UnityEngine/Vector3.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "IncrementSlider.hpp"
#include "UnityEngine/WaitForFixedUpdate.hpp"
#include <sstream>
#include <iomanip>
#include "main.hpp"

namespace IncrementHelper{
    std::string Round(float val, int precision);
    float fixDumbNumberThing(float num);
    custom_types::Helpers::Coroutine forceUpdateSliderText(SaberTailor::IncrementSlider* slider, std::string text);
}
namespace PosRotHelper{
    void revertLeftHand();
    void revertRightHand();
}
namespace TransferHelper{
    void mirrorToLeft();
    void mirrorToRight();
    void exportToBaseGame(int hand);
    void importFromBaseGame(int hand);
}