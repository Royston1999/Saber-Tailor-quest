#pragma once

#include <string>
#include <unordered_map>
#include "modloader/shared/modloader.hpp"
// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/byref.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Color.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/register.hpp"
#include <string>
#include <iostream>

#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "config-utils/shared/config-utils.hpp"
#include "UnityEngine/Color.hpp"

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