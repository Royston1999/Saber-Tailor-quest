#pragma once

#include <string>
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "config-utils/shared/config-utils.hpp"

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
std::optional<float> setFloat(rapidjson::Value& obj, std::string_view fieldName, float value, bool required = false);

class SaberTailorConfig {
public:
    bool isEnabled;
    bool spawnAxisDisplay;
    bool overrideSettingsMethod;
    bool mirrorZRot;

    float rightPosX;
    float rightPosY;
    float rightPosZ;
    int rightRotX;
    int rightRotY;
    int rightRotZ;

    float leftPosX;
    float leftPosY;
    float leftPosZ;
    int leftRotX;
    int leftRotY;
    int leftRotZ;

    bool okButtonPressed = false;
    
    float currentPosXLeft;
    float currentPosYLeft;
    float currentPosZLeft;
    int currentRotXLeft;
    int currentRotYLeft;
    int currentRotZLeft;

    float currentPosXRight;
    float currentPosYRight;
    float currentPosZRight;
    int currentRotXRight;
    int currentRotYRight;
    int currentRotZRight;

};

class ConfigHelper {
public:
    static bool LoadConfig(SaberTailorConfig& con, ConfigDocument& config);
    static void CreateDefaultConfig(ConfigDocument& config);
};