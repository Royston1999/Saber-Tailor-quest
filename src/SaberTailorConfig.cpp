#include <tuple>
#include "SaberTailorConfig.hpp"
#include "main.hpp"

using namespace rapidjson;

std::optional<bool> getBool(Value& obj, std::string_view fieldName, bool required) {
    GET(obj, fieldName, GetBool, required);
}

std::optional<int> getInt(Value& obj, std::string_view fieldName, bool required) {
    GET(obj, fieldName, GetInt, required);
}

std::optional<float> getFloat(Value& obj, std::string_view fieldName, bool required) {
    GET(obj, fieldName, GetFloat, required);
}

std::optional<bool> setBool(Value& obj, std::string_view fieldName, bool value, bool required) {
    auto itr = obj.FindMember(fieldName.data());
    if (itr == obj.MemberEnd()) {
        if (required) {
        }
    }
    itr->value.SetBool(value);
    return true;
}

std::optional<int> setInt(Value& obj, std::string_view fieldName, int value, bool required) {
    auto itr = obj.FindMember(fieldName.data());
    if (itr == obj.MemberEnd()) {
        if (required) {
        }
    }
    itr->value.SetInt(value);
    return true;
}

std::optional<float> setFloat(Value& obj, std::string_view fieldName, float value, bool required) {
    auto itr = obj.FindMember(fieldName.data());
    if (itr == obj.MemberEnd()) {
        if (required) {
        }
    }
    itr->value.SetFloat(value);
    return true;
}

bool ConfigHelper::LoadConfig(SaberTailorConfig& con, ConfigDocument& config) {
    if (!config.HasMember("isEnabled")) ConfigHelper::CreateDefaultConfig(config);
    if (!config.HasMember("axisEnabled")) {config.AddMember("axisEnabled", false, config.GetAllocator()); getConfig().Write();}
    con.isEnabled = getBool(config, "isEnabled").value_or(false);
    con.spawnAxisDisplay = getBool(config, "axisEnabled").value_or(false);
    con.overrideSettingsMethod = getBool(config, "overrideSettingsMethod").value_or(false);
    con.mirrorZRot = getBool(config, "mirrorZRot").value_or(false);
    con.rightPosX = getFloat(config, "rightPosX").value_or(0);
    con.rightPosY = getFloat(config, "rightPosY").value_or(0);
    con.rightPosZ = getFloat(config, "rightPosZ").value_or(0);
    con.rightRotX = getInt(config, "rightRotX").value_or(0);
    con.rightRotY = getInt(config, "rightRotY").value_or(0);
    con.rightRotZ = getInt(config, "rightRotZ").value_or(0);
    con.leftPosX = getFloat(config, "leftPosX").value_or(0);
    con.leftPosY = getFloat(config, "leftPosY").value_or(0);
    con.leftPosZ = getFloat(config, "leftPosZ").value_or(0);
    con.leftRotX = getInt(config, "leftRotX").value_or(0);
    con.leftRotY = getInt(config, "leftRotY").value_or(0);
    con.leftRotZ = getInt(config, "leftRotZ").value_or(0);
    return true;
}

void ConfigHelper::CreateDefaultConfig(ConfigDocument& config){
    config.SetObject();
    config.RemoveAllMembers();
    config.AddMember("isEnabled", false, config.GetAllocator());
    config.AddMember("axisEnabled", false, config.GetAllocator());
    config.AddMember("overrideSettingsMethod", false, config.GetAllocator());
    config.AddMember("mirrorZRot", false, config.GetAllocator());
    config.AddMember("rightPosX", 0, config.GetAllocator());
    config.AddMember("rightPosY", 0, config.GetAllocator());
    config.AddMember("rightPosZ", 0, config.GetAllocator());
    config.AddMember("rightRotX", 0, config.GetAllocator());
    config.AddMember("rightRotY", 0, config.GetAllocator());
    config.AddMember("rightRotZ", 0, config.GetAllocator());
    getConfig().Write();
    config.AddMember("leftPosX", 0, config.GetAllocator());
    config.AddMember("leftPosY", 0, config.GetAllocator());
    config.AddMember("leftPosZ", 0, config.GetAllocator());
    config.AddMember("leftRotX", 0, config.GetAllocator());
    config.AddMember("leftRotY", 0, config.GetAllocator());
    config.AddMember("leftRotZ", 0, config.GetAllocator());
    getConfig().Write();
}