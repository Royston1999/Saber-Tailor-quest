#pragma once

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/MainSettingsModelSO.hpp"
#include "UnityEngine/Resources.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "GlobalNamespace/Vector3SO.hpp"
#include "UnityEngine/Vector3.hpp"
#include <sstream>
#include <iomanip>
#include "main.hpp"

namespace IncrementHelper{
    std::string Round(float val, int precision);
    void setIncrementText(QuestUI::IncrementSetting* increment, float num);
    void setRotIncrementText(QuestUI::IncrementSetting* increment, int num);
    float fixDumbNumberThing(float num);
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