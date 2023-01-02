#pragma once

#include <string>
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs-string.hpp"
#include "UnityEngine/Vector3.hpp"

#define GET(obj, fieldName, method, required) auto itr = obj.FindMember(fieldName.data()); \
if (itr == obj.MemberEnd()) { \
    if (required) { \
    } \
    return std::nullopt; \
} \
return itr->value.method()

std::optional<bool> getBool(rapidjson::Value& obj, std::string_view fieldName, bool required = false);
std::optional<bool> setBool(rapidjson::Value& obj, std::string_view fieldName,  bool value, bool required = false);
std::optional<int> getInt(rapidjson::Value& obj, std::string_view fieldName, bool required = false);
std::optional<int> setInt(rapidjson::Value& obj, std::string_view fieldName, int value, bool required = false);
std::optional<float> getFloat(rapidjson::Value& obj, std::string_view fieldName, bool required = false);
void setVectorObjectValue(rapidjson::Value& obj, std::string_view vectorField, std::string_view coord, int value);
void setVectorObject(rapidjson::Value& obj, std::string vectorField, UnityEngine::Vector3 vector);
void setString(ConfigDocument& obj, std::string stringField, std::string string);

class SaberTailorConfig {
public:

    ConfigDocument currentlyLoadedConfig;
    std::string currentConfigName = "";

    bool isEnabled;
    bool spawnAxisDisplay;
    bool overrideSettingsMethod;
    bool mirrorZRot;

    UnityEngine::Vector3 rightHandPosition;
    UnityEngine::Vector3 leftHandPosition;
    UnityEngine::Vector3 rightHandRotation;
    UnityEngine::Vector3 leftHandRotation;

    UnityEngine::Vector3 currentRightHandPosition;
    UnityEngine::Vector3 currentLeftHandPosition;
    UnityEngine::Vector3 currentRightHandRotation;
    UnityEngine::Vector3 currentLeftHandRotation;

    bool isAprilFools;

    int saberPosIncMultiplier = 1;
    int saberPosIncrement = 1;
    int saberRotIncrement = 1;
    std::string saberPosIncUnit = "mm";
    std::string saberPosDisplayValue = "cm";

    bool enableScaleSettings;
    int saberLength;
    int saberGirth;
    bool scaleHitbox;

    bool enableTrailSettings;
    bool enableSaberTrails;
    int trailDuration;
    int trailGranularity;
    int whitestepDuration;
};

class ConfigHelper {
public:
    static bool LoadConfig(SaberTailorConfig& con, ConfigDocument& config);
    static bool LoadConfigNew(SaberTailorConfig& con, ConfigDocument& config);
    static void LoadConfigFile(std::string fileName);
    static std::vector<StringW> GetExistingConfigs();
    static void WriteToConfigFile(std::string file);
    static void DeleteFile(std::string fileName);
    static void CreateNewConfigFile(std::string fileName, std::string configToCopyFrom);
    static void ConvertOldConfig();
    static bool HasConfigWithName(std::string configName);
};