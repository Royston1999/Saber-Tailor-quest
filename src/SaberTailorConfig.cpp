#include <tuple>
#include "SaberTailorConfig.hpp"
#include "main.hpp"
#include <filesystem>

using namespace rapidjson;
using namespace UnityEngine;

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
    ConfigHelper::WriteToConfigFile(SaberTailorMain::config.currentConfigName);
    return true;
}

std::optional<int> setInt(Value& obj, std::string_view fieldName, int value, bool required) {
    auto itr = obj.FindMember(fieldName.data());
    if (itr == obj.MemberEnd()) {
        if (required) {
        }
    }
    itr->value.SetInt(value);
    ConfigHelper::WriteToConfigFile(SaberTailorMain::config.currentConfigName);
    return true;
}

void setString(ConfigDocument& obj, std::string stringField, std::string string){
    char buffer[string.length()+5]; int len = sprintf(buffer, "%s", string.c_str());
    obj.FindMember(stringField)->value.SetString(buffer, len, obj.GetAllocator());
    ConfigHelper::WriteToConfigFile(SaberTailorMain::config.currentConfigName);
}

UnityEngine::Vector3 LoadVectorObject(ConfigDocument& config, std::string vectorObj){
    auto object = config.FindMember(vectorObj);
    if (object == config.MemberEnd()) return UnityEngine::Vector3::get_zero();
    int x = object->value.FindMember("x")->value.GetInt();
    int y = object->value.FindMember("y")->value.GetInt();
    int z = object->value.FindMember("z")->value.GetInt();
    return UnityEngine::Vector3(x, y, z);
}

bool ConfigHelper::LoadConfigNew(SaberTailorConfig& con, ConfigDocument& config) {
    int temp = getInt(config, "ConfigVersion").value_or(0);
    con.enableScaleSettings = getBool(config, "IsSaberScaleModEnabled").value_or(false);
    con.scaleHitbox = getBool(config, "SaberScaleHitbox").value_or(false);
    con.saberLength = getInt(config, "SaberLength").value_or(0);
    con.saberGirth = getInt(config, "SaberGirth").value_or(0);
    con.enableTrailSettings = getBool(config, "IsTrailModEnabled").value_or(false);
    con.enableSaberTrails = getBool(config, "IsTrailEnabled").value_or(false);
    con.trailDuration = getInt(config, "TrailDuration").value_or(0);
    con.trailGranularity = getInt(config, "TrailGranularity").value_or(0);
    con.whitestepDuration = getInt(config, "TrailWhiteSectionDuration").value_or(0);
    con.isEnabled = getBool(config, "IsGripModEnabled").value_or(false);
    
    con.rightHandPosition = LoadVectorObject(config, "GripRightPosition");
    con.leftHandPosition = LoadVectorObject(config, "GripLeftPosition");
    con.rightHandRotation = LoadVectorObject(config, "GripRightRotation");
    con.leftHandRotation = LoadVectorObject(config, "GripLeftRotation");
    
    bool temp2 = getBool(config, "ModifyMenuHiltGrip").value_or(false);
    bool wtfEvenIsThisSetting = getBool(config, "UseBaseGameAdjustmentMode").value_or(true);
    con.saberPosIncMultiplier = getInt(config, "SaberPosIncValue").value_or(0);
    con.saberPosIncrement = getInt(config, "SaberPosIncrement").value_or(0);
    con.saberRotIncrement = getInt(config, "SaberRotIncrement").value_or(0);
    con.saberPosIncUnit = config.FindMember("SaberPosIncUnit")->value.GetString();
    con.saberPosDisplayValue = config.FindMember("SaberPosDisplayUnit")->value.GetString();

    if (!config.HasMember("axisEnabled")) {
        config.AddMember("axisEnabled", false, config.GetAllocator());
        config.AddMember("overrideSettingsMethod", false, config.GetAllocator());
        config.AddMember("mirrorZRot", false, config.GetAllocator());
        ConfigHelper::WriteToConfigFile(SaberTailorMain::config.currentConfigName);
    }
    if (!config.HasMember("axisInReplay")){
        config.AddMember("axisInReplay", false, config.GetAllocator());
        ConfigHelper::WriteToConfigFile(SaberTailorMain::config.currentConfigName);
    }
        
    con.mirrorZRot = getBool(config, "mirrorZRot").value_or(false);
    con.spawnAxisDisplay = getBool(config, "axisEnabled").value_or(false);
    con.overrideSettingsMethod = getBool(config, "overrideSettingsMethod").value_or(false);
    con.axisInReplay = getBool(config, "axisInReplay").value_or(false);
    return true;
}

rapidjson::Value CreateVectorObject(UnityEngine::Vector3 values){
    Document::ValueType vector(kObjectType);
    vector.AddMember("x", (int)std::round(values.x), SaberTailorMain::config.currentlyLoadedConfig.GetAllocator());
    vector.AddMember("y", (int)std::round(values.y), SaberTailorMain::config.currentlyLoadedConfig.GetAllocator());
    vector.AddMember("z", (int)std::round(values.z), SaberTailorMain::config.currentlyLoadedConfig.GetAllocator());
    return vector;
}

void setVectorObjectValue(rapidjson::Value& obj, std::string_view vectorField, std::string_view coord, int value){
    auto itr = obj.FindMember(vectorField.data())->value.GetObject();
    itr.FindMember(coord.data())->value.SetInt(value);
    ConfigHelper::WriteToConfigFile(SaberTailorMain::config.currentConfigName);
}

void setVectorObject(rapidjson::Value& obj, std::string vectorField, UnityEngine::Vector3 vector){
    auto value = CreateVectorObject(vector);
    obj.FindMember(vectorField)->value.Swap(value);
    ConfigHelper::WriteToConfigFile(SaberTailorMain::config.currentConfigName);  
}

void CreateBlankConfig(std::string name){
    ConfigDocument &config = SaberTailorMain::config.currentlyLoadedConfig;
    config.SetObject();
    config.RemoveAllMembers();
    config.AddMember("ConfigVersion", 5, config.GetAllocator());
    config.AddMember("IsSaberScaleModEnabled", false, config.GetAllocator());
    config.AddMember("SaberScaleHitbox", false, config.GetAllocator());
    config.AddMember("SaberLength", 100, config.GetAllocator());
    config.AddMember("SaberGirth", 100, config.GetAllocator());
    config.AddMember("IsTrailModEnabled", false, config.GetAllocator());
    config.AddMember("IsTrailEnabled", true, config.GetAllocator());
    config.AddMember("TrailDuration", 400, config.GetAllocator());
    config.AddMember("TrailGranularity", 60, config.GetAllocator());
    config.AddMember("TrailWhiteSectionDuration", 100, config.GetAllocator());
    config.AddMember("IsGripModEnabled", false, config.GetAllocator());

    config.AddMember("GripLeftPosition", CreateVectorObject(Vector3::get_zero()), config.GetAllocator());
    config.AddMember("GripRightPosition", CreateVectorObject(Vector3::get_zero()), config.GetAllocator());
    config.AddMember("GripLeftRotation", CreateVectorObject(Vector3::get_zero()), config.GetAllocator());
    config.AddMember("GripRightRotation", CreateVectorObject(Vector3::get_zero()), config.GetAllocator());
    config.AddMember("GripLeftOffset", CreateVectorObject(Vector3::get_zero()), config.GetAllocator()); // what the fuck are these
    config.AddMember("GripRightOffset", CreateVectorObject(Vector3::get_zero()), config.GetAllocator()); // these are borderline useless

    config.AddMember("ModifyMenuHiltGrip", true, config.GetAllocator());
    config.AddMember("UseBaseGameAdjustmentMode", true, config.GetAllocator());
    config.AddMember("SaberPosIncrement", 1, config.GetAllocator());
    config.AddMember("SaberPosIncValue", 1, config.GetAllocator());
    config.AddMember("SaberRotIncrement", 1, config.GetAllocator());
    config.AddMember("SaberPosIncUnit", "mm", config.GetAllocator());
    config.AddMember("SaberPosDisplayUnit", "cm", config.GetAllocator());

    config.AddMember("axisEnabled", false, config.GetAllocator());
    config.AddMember("overrideSettingsMethod", false, config.GetAllocator());
    config.AddMember("mirrorZRot", false, config.GetAllocator());

    ConfigHelper::WriteToConfigFile(name);
}

bool ThisStupidFuckingQuestChildTriedToDeleteTheDefaultConfig(){
    std::vector<StringW> configs = ConfigHelper::GetExistingConfigs();
    for (auto& name : configs) if (name == "Default") return false;
    return true;
}

std::string toLower(std::string s){
    for (char& c : s) c = std::tolower(c);
    return s;
}

bool ConfigHelper::HasConfigWithName(std::string configName){
    std::vector<StringW> configs = ConfigHelper::GetExistingConfigs();
    for (auto& name : configs) if (toLower(static_cast<std::string>(name)) == toLower(configName)) return true;
    return false;
}

bool ConfigHelper::LoadConfig(SaberTailorConfig& con, ConfigDocument& config) {
    std::string dir = getDataDir(modInfo);
    if(!direxists(dir)) int made = mkpath(dir);
    if (config.HasMember("isEnabled")) ConfigHelper::ConvertOldConfig();
    else if (!config.HasMember("currentConfig")) {
        config.AddMember("currentConfig", "Default", config.GetAllocator()); getConfig().Write();
        CreateBlankConfig("Default");
        con.currentConfigName = "Default";
        }
    else {
        if (ThisStupidFuckingQuestChildTriedToDeleteTheDefaultConfig()) CreateBlankConfig("Default");
        con.currentConfigName = config.FindMember("currentConfig")->value.GetString();
        LoadConfigFile(con.currentConfigName);
    }
    return true;
}

void ConfigHelper::LoadConfigFile(std::string fileName){
    std::string dir = getDataDir(modInfo);
    std::string file = dir + fileName + ".json";

    if(!direxists(dir)) {
        int made = mkpath(dir);
        if(made < 0) {
            getLogger().info("Failed to make data directory");
            return;
        }
    }
    if(!fileexists(file)) {
        getLogger().info("file doesn't exist!");
        if (fileName == "Default") CreateBlankConfig(fileName);
        else return LoadConfigFile("Default");
    };
    ConfigDocument d;
    if(!parsejsonfile(d, file)) {
        getLogger().info("Failed to read map data");
        return LoadConfigFile("Default");
    }
    SaberTailorMain::config.currentConfigName = fileName;
    SaberTailorMain::config.currentlyLoadedConfig.Swap(d);
    setString(getConfig().config, "currentConfig", fileName); getConfig().Write();
    LoadConfigNew(SaberTailorMain::config, SaberTailorMain::config.currentlyLoadedConfig);
}

std::vector<StringW> ConfigHelper::GetExistingConfigs(){
    std::vector<StringW> directories;
    std::string directoryPath = getDataDir(modInfo);
    if(!std::filesystem::is_directory(directoryPath)) return directories;
    std::error_code ec;
    auto directory_iterator = std::filesystem::directory_iterator(directoryPath, std::filesystem::directory_options::none, ec);
    if (ec) getLogger().info("uh oh");
    for (auto const& entry : directory_iterator) {
        if(entry.is_regular_file()){
            std::string filePath = entry.path().string();
            std::string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
            std::string::size_type const p(base_filename.find_last_of('.'));
            std::string file_extension = base_filename.substr(p, base_filename.length());
            std::string file_without_extension = base_filename.substr(0, p);
            getLogger().info("%s", file_extension.c_str());
            if (file_extension == ".json")
                directories.push_back(file_without_extension);
        }
    }
    return directories;
}

void ConfigHelper::WriteToConfigFile(std::string fileName){
    std::string dir = getDataDir(modInfo);
    std::string file = dir + fileName + ".json";
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    SaberTailorMain::config.currentlyLoadedConfig.Accept(writer);
    std::string s = buffer.GetString();
    writefile(file, s);
    LoadConfigNew(SaberTailorMain::config, SaberTailorMain::config.currentlyLoadedConfig);
}

void ConfigHelper::DeleteFile(std::string fileName){
    std::string dir = getDataDir(modInfo);
    std::string file = dir + fileName + ".json";
    if(fileexists(file)) std::filesystem::remove(file);
    else getLogger().info("File not found!");
}

void ConfigHelper::CreateNewConfigFile(std::string fileName, std::string configToCopyFrom){
    std::string dir = getDataDir(modInfo);
    std::string file = dir + fileName + ".json";
    ConfigDocument d;
    if (configToCopyFrom == "") CreateBlankConfig(fileName);
    else{
        if(!parsejsonfile(d, dir + configToCopyFrom + ".json")) {getLogger().info("oh no"); return;}
        SaberTailorMain::config.currentlyLoadedConfig.Swap(d);
    }
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    SaberTailorMain::config.currentlyLoadedConfig.Accept(writer);
    std::string s = buffer.GetString();
    writefile(file, s);
}

void ConfigHelper::ConvertOldConfig(){
    ConfigDocument &config = SaberTailorMain::config.currentlyLoadedConfig;
    ConfigDocument &oldConfig = getConfig().config;
    config.SetObject();
    config.RemoveAllMembers();
    config.AddMember("ConfigVersion", 5, config.GetAllocator());
    config.AddMember("IsSaberScaleModEnabled", false, config.GetAllocator());
    config.AddMember("SaberScaleHitbox", false, config.GetAllocator());
    config.AddMember("SaberLength", 100, config.GetAllocator());
    config.AddMember("SaberGirth", 100, config.GetAllocator());
    config.AddMember("IsTrailModEnabled", false, config.GetAllocator());
    config.AddMember("IsTrailEnabled", true, config.GetAllocator());
    config.AddMember("TrailDuration", 400, config.GetAllocator());
    config.AddMember("TrailGranularity", 60, config.GetAllocator());
    config.AddMember("TrailWhiteSectionDuration", 100, config.GetAllocator());
    config.AddMember("IsGripModEnabled", getBool(oldConfig, "isEnabled").value_or(false), config.GetAllocator());

    float leftPosX = getFloat(oldConfig, "leftPosX").value_or(0); leftPosX = leftPosX < 0 ? (leftPosX * 10 - 0.5f) : (leftPosX * 10 + 0.5f);
    float leftPosY = getFloat(oldConfig, "leftPosY").value_or(0); leftPosY = leftPosY < 0 ? (leftPosY * 10 - 0.5f) : (leftPosY * 10 + 0.5f);
    float leftPosZ = getFloat(oldConfig, "leftPosZ").value_or(0); leftPosZ = leftPosZ < 0 ? (leftPosZ * 10 - 0.5f) : (leftPosZ * 10 + 0.5f);

    float rightPosX = getFloat(oldConfig, "rightPosX").value_or(0); rightPosX = rightPosX < 0 ? (rightPosX * 10 - 0.5f) : (rightPosX * 10 + 0.5f);
    float rightPosY = getFloat(oldConfig, "rightPosY").value_or(0); rightPosY = rightPosY < 0 ? (rightPosY * 10 - 0.5f) : (rightPosY * 10 + 0.5f);
    float rightPosZ = getFloat(oldConfig, "rightPosZ").value_or(0); rightPosZ = rightPosZ < 0 ? (rightPosZ * 10 - 0.5f) : (rightPosZ * 10 + 0.5f);

    int rightRotX = getInt(oldConfig, "rightRotX").value_or(0);
    int rightRotY = getInt(oldConfig, "rightRotY").value_or(0);
    int rightRotZ = getInt(oldConfig, "rightRotZ").value_or(0);

    int leftRotX = getInt(oldConfig, "leftRotX").value_or(0);
    int leftRotY = getInt(oldConfig, "leftRotY").value_or(0);
    int leftRotZ = getInt(oldConfig, "leftRotZ").value_or(0);
    
    config.AddMember("GripLeftPosition", CreateVectorObject(Vector3(leftPosX, leftPosY, leftPosZ)), config.GetAllocator());
    config.AddMember("GripRightPosition", CreateVectorObject(Vector3(rightPosX, rightPosY, rightPosZ)), config.GetAllocator());
    config.AddMember("GripLeftRotation", CreateVectorObject(Vector3(leftRotX, leftRotY, leftRotZ)), config.GetAllocator());
    config.AddMember("GripRightRotation", CreateVectorObject(Vector3(rightRotX, rightRotY, rightRotZ)), config.GetAllocator());
    config.AddMember("GripLeftOffset", CreateVectorObject(Vector3::get_zero()), config.GetAllocator()); // what the fuck are these
    config.AddMember("GripRightOffset", CreateVectorObject(Vector3::get_zero()), config.GetAllocator()); // these are borderline useless

    config.AddMember("ModifyMenuHiltGrip", true, config.GetAllocator());
    config.AddMember("UseBaseGameAdjustmentMode", true, config.GetAllocator());
    config.AddMember("SaberPosIncrement", 1, config.GetAllocator());
    config.AddMember("SaberPosIncValue", 1, config.GetAllocator());
    config.AddMember("SaberRotIncrement", 1, config.GetAllocator());
    config.AddMember("SaberPosIncUnit", "mm", config.GetAllocator());
    config.AddMember("SaberPosDisplayUnit", "cm", config.GetAllocator());

    config.AddMember("axisEnabled", getBool(oldConfig, "axisEnabled").value_or(false), config.GetAllocator());
    config.AddMember("overrideSettingsMethod", getBool(oldConfig, "overrideSettingsMethod").value_or(false), config.GetAllocator());
    config.AddMember("mirrorZRot", getBool(oldConfig, "mirrorZRot").value_or(false), config.GetAllocator());

    oldConfig.SetObject();
    oldConfig.RemoveAllMembers();
    oldConfig.AddMember("currentConfig", "Default", oldConfig.GetAllocator()); getConfig().Write();
    SaberTailorMain::config.currentConfigName = "Default";

    ConfigHelper::WriteToConfigFile("Default");
}