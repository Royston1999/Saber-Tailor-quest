#pragma once

#include <string>
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs-string.hpp"
#include "UnityEngine/Vector3.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "rapidjson-macros/shared/macros.hpp"

DECLARE_CONFIG(MainConfig, 
    CONFIG_VALUE(currentConfig, std::string, "currentConfig", "Default");
)

namespace SaberTailor {

DECLARE_JSON_CLASS(Vector3,
    NAMED_VALUE_DEFAULT(int, x, 0, "x");
    NAMED_VALUE_DEFAULT(int, y, 0, "y");
    NAMED_VALUE_DEFAULT(int, z, 0, "z");
    CONVERSION(Vector3,
        x = round(other.x); y = round(other.y); z = round(other.z);,
        (x, y, z)
    )
    Vector3() = default;
    Vector3(int x1, int y1, int z1) : x(x1), y(y1), z(z1) {}
)

DECLARE_JSON_CLASS(SaberTailorProfileConfig,

    NAMED_VALUE_DEFAULT(int, configVersion, 5, "ConfigVersion");
    NAMED_VALUE_DEFAULT(bool, isSaberScaleModEnabled, false, "IsSaberScaleModEnabled");
    NAMED_VALUE_DEFAULT(bool, saberScaleHitbox, false, "SaberScaleHitbox");
    NAMED_VALUE_DEFAULT(int, saberLength, 100, "SaberLength");
    NAMED_VALUE_DEFAULT(int, saberGirth, 100, "SaberGirth");
    NAMED_VALUE_DEFAULT(bool, isTrailModEnabled, false, "IsTrailModEnabled");
    NAMED_VALUE_DEFAULT(bool, isTrailEnabled, true, "IsTrailEnabled");
    NAMED_VALUE_DEFAULT(int, trailDuration, 400, "TrailDuration");
    NAMED_VALUE_DEFAULT(int, trailGranularity, 60, "TrailGranularity");
    NAMED_VALUE_DEFAULT(int, trailWhiteSectionDuration, 100, "TrailWhiteSectionDuration");
    NAMED_VALUE_DEFAULT(bool, isGripModEnabled, false, "IsGripModEnabled");
    NAMED_VALUE_DEFAULT(Vector3, leftHandPosition, Vector3(), "GripLeftPosition");
    NAMED_VALUE_DEFAULT(Vector3, leftHandRotation, Vector3(), "GripRightPosition");
    NAMED_VALUE_DEFAULT(Vector3, rightHandPosition, Vector3(), "GripLeftRotation");
    NAMED_VALUE_DEFAULT(Vector3, rightHandRotation, Vector3(), "GripRightRotation");
    NAMED_VALUE_DEFAULT(Vector3, gripLeftOffset, Vector3(), "GripLeftOffset");
    NAMED_VALUE_DEFAULT(Vector3, gripRightOffset, Vector3(), "GripRightOffset");
    NAMED_VALUE_DEFAULT(bool, modifyMenuHiltGrip, false, "ModifyMenuHiltGrip");
    NAMED_VALUE_DEFAULT(bool, useBaseGameAdjustmentMode, true, "UseBaseGameAdjustmentMode");
    NAMED_VALUE_DEFAULT(int, saberPosIncrement, 1, "SaberPosIncrement");
    NAMED_VALUE_DEFAULT(int, saberPosIncValue, 1, "SaberPosIncValue");
    NAMED_VALUE_DEFAULT(int, saberRotIncrement, 1, "SaberRotIncrement");
    NAMED_VALUE_DEFAULT(std::string, saberPosIncUnit, "mm", "SaberPosIncUnit");
    NAMED_VALUE_DEFAULT(std::string, saberPosDisplayUnit, "cm", "SaberPosDisplayUnit");
    NAMED_VALUE_DEFAULT(bool, axisEnabled, false, "axisEnabled");
    NAMED_VALUE_DEFAULT(bool, overrideSettingsMethod, false, "overrideSettingsMethod");
    NAMED_VALUE_DEFAULT(bool, mirrorZRot, false, "mirrorZRot");
    NAMED_VALUE_DEFAULT(bool, axisInReplay, false, "axisInReplay");
    
    public:
        UnityEngine::Vector3 currentLeftHandPosition;
        UnityEngine::Vector3 currentLeftHandRotation;
        UnityEngine::Vector3 currentRightHandPosition;
        UnityEngine::Vector3 currentRightHandRotation;
)
}

#define SET_VALUE(field, value) \
SaberTailorMain::config.currentConfig.field = value; \
ConfigHelper::WriteToConfigFile(getMainConfig().currentConfig.GetValue()) \

#define GET_VALUE(field) SaberTailorMain::config.currentConfig.field

class SaberTailorConfig {
public:
    SaberTailor::SaberTailorProfileConfig currentConfig;
    bool isAprilFools;
};

class ConfigHelper {
public:
    static bool LoadConfig(SaberTailorConfig& con, ConfigDocument& config);
    static void LoadConfigFile(std::string fileName);
    static std::vector<StringW> GetExistingConfigs();
    static void WriteToConfigFile(std::string file, bool reloadConfig = true);
    static void DeleteFile(std::string fileName);
    static void CreateNewConfigFile(std::string fileName, std::string configToCopyFrom);
    static void ConvertOldConfig();
    static bool HasConfigWithName(std::string configName);
    static void CreateBlankConfig(std::string name);
private:
    template<JSONClassDerived T>
    static std::string CreateJSONString(const T& toSerialize);
};