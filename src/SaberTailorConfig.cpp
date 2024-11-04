#include <tuple>
#include "SaberTailorConfig.hpp"
#include "main.hpp"
#include <filesystem>

using namespace rapidjson;
using namespace UnityEngine;

void ConfigHelper::CreateBlankConfig(std::string name){
    SaberTailor::SaberTailorProfileConfig newConfig;
    std::string filePath = getDataDir(modInfo) + name + ".json";
    writefile(filePath, CreateJSONString(newConfig));
    ReadFromFile(filePath, SaberTailorMain::config.currentConfig);
}

std::string ConfigHelper::GetDefaultConfigName() {
    std::vector<std::string> configs = ConfigHelper::GetExistingConfigs();
    for (auto& name : configs) if (toLower(name) == "default") return name;
    return "";
}

bool ThisStupidFuckingQuestChildTriedToDeleteTheDefaultConfig(){
    return !ConfigHelper::HasConfigWithName("Default");
}

std::string ConfigHelper::toLower(std::string s){
    for (char& c : s) c = std::tolower(c);
    return s;
}

bool ConfigHelper::HasConfigWithName(std::string configName){
    std::vector<std::string> configs = ConfigHelper::GetExistingConfigs();
    for (auto& name : configs) if (toLower(name) == toLower(configName)) return true;
    return false;
}

bool ConfigHelper::LoadConfig(SaberTailorConfig& con, ConfigDocument& config) {
    std::string dir = getDataDir(modInfo);
    if(!direxists(dir)) int made = mkpath(dir);
    if (config.HasMember("isEnabled")) ConfigHelper::ConvertOldConfig();
    else {
        if (ThisStupidFuckingQuestChildTriedToDeleteTheDefaultConfig()) CreateBlankConfig("Default");
        LoadConfigFile(getMainConfig().currentConfig.GetValue());
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
    getMainConfig().currentConfig.SetValue(fileName);

    try {
        ReadFromFile(file, SaberTailorMain::config.currentConfig);
        CheckAndUpdateOldConfigVersion(file, SaberTailorMain::config.currentConfig);
    } catch(const std::exception& err) {
        return LoadConfigFile("Default");
    }
}

std::vector<std::string> ConfigHelper::GetExistingConfigs(){
    std::vector<std::string> directories;
    std::string directoryPath = getDataDir(modInfo);
    if(!std::filesystem::is_directory(directoryPath)) return directories;
    std::error_code ec;
    auto directory_iterator = std::filesystem::directory_iterator(directoryPath, std::filesystem::directory_options::none, ec);
    for (auto const& entry : directory_iterator) {
        if(!entry.is_regular_file()) continue;
        std::string file_extension = entry.path().extension().string();
        std::string raw_file_name = entry.path().filename().replace_extension().string();
        if (file_extension == ".json") directories.push_back(raw_file_name);
    }
    std::sort(directories.begin(), directories.end(), [](std::string& a, std::string& b) {
        if (toLower(a) == "default") return true;
        if (toLower(b) == "default") return false;
        return a < b;
    });
    return directories;
}

void ConfigHelper::WriteToConfigFile(std::string fileName, bool reloadConfig){
    std::string dir = getDataDir(modInfo);
    std::string file = dir + fileName + ".json";
    writefile(file, CreateJSONString(SaberTailorMain::config.currentConfig));
    if (reloadConfig) ReadFromFile(file, SaberTailorMain::config.currentConfig);
}


template<JSONClassDerived T>
std::string ConfigHelper::CreateJSONString(const T& toSerialize){
    rapidjson::Document document;
    document.SetObject();
    toSerialize.Serialize(document.GetAllocator()).Swap(document);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
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
    getMainConfig().currentConfig.SetValue(fileName);
    if (configToCopyFrom == "") CreateBlankConfig(fileName);
    else{
        try {
            ReadFromFile(dir + configToCopyFrom + ".json", SaberTailorMain::config.currentConfig);
        } catch(const std::exception& err) {
            return getLogger().info("oh no");
        }
        WriteToConfigFile(fileName, false);
    }
}

DECLARE_JSON_CLASS(LegacyConfig,
    NAMED_VALUE_OPTIONAL(bool, overrideSettingsMethod, "overrideSettingsMethod");
)

void ConfigHelper::CheckAndUpdateOldConfigVersion(std::string file, SaberTailor::SaberTailorProfileConfig& config) {
    if (config.configVersion == 6) return;
    config.configVersion = 6;
    LegacyConfig legacy = ReadFromFile<LegacyConfig>(file);
    if (legacy.overrideSettingsMethod.has_value()) { 
        config.offsetApplicationMethod = legacy.overrideSettingsMethod.value() ? SaberTailor::ApplicationMethod::ElectroMethod : SaberTailor::ApplicationMethod::PreUnityUpdate;
    }
    writefile(file, CreateJSONString(config));
    ReadFromFile(file, SaberTailorMain::config.currentConfig);
}

template<class T>
std::optional<T> GetValue(rapidjson::GenericObject<false, rapidjson::Value>& object, std::string name){
    auto itr = object.FindMember(name);
    if (itr != object.MemberEnd() && !itr->value.IsNull()) return std::optional<T>(itr->value.Get<T>());
    else return std::nullopt;
}

void ConfigHelper::ConvertOldConfig(){
    SaberTailor::SaberTailorProfileConfig convertedConfig;
    auto oldConfig = getConfig().config.GetObject();

    float leftPosX = GetValue<float>(oldConfig, "leftPosX").value_or(0);
    float leftPosY = GetValue<float>(oldConfig, "leftPosY").value_or(0);
    float leftPosZ = GetValue<float>(oldConfig, "leftPosZ").value_or(0);

    float rightPosX = GetValue<float>(oldConfig, "rightPosX").value_or(0);
    float rightPosY = GetValue<float>(oldConfig, "rightPosY").value_or(0);
    float rightPosZ = GetValue<float>(oldConfig, "rightPosZ").value_or(0);

    int rightRotX = GetValue<int>(oldConfig, "rightRotX").value_or(0);
    int rightRotY = GetValue<int>(oldConfig, "rightRotY").value_or(0);
    int rightRotZ = GetValue<int>(oldConfig, "rightRotZ").value_or(0);

    int leftRotX = GetValue<int>(oldConfig, "leftRotX").value_or(0);
    int leftRotY = GetValue<int>(oldConfig, "leftRotY").value_or(0);
    int leftRotZ = GetValue<int>(oldConfig, "leftRotZ").value_or(0);

    convertedConfig.leftHandPosition = Vector3(round(leftPosX * 10.0f), round(leftPosY * 10.0f), round(leftPosZ * 10.0f));
    convertedConfig.rightHandPosition = Vector3(round(rightPosX * 10.0f), round(rightPosY * 10.0f), round(rightPosZ * 10.0f));
    convertedConfig.leftHandRotation = Vector3(leftRotX, leftRotY, leftRotZ);
    convertedConfig.rightHandRotation = Vector3(rightRotX, rightRotY, rightRotZ);

    convertedConfig.axisEnabled = GetValue<bool>(oldConfig, "axisEnabled").value_or(false);
    convertedConfig.offsetApplicationMethod = GetValue<bool>(oldConfig, "overrideSettingsMethod").value_or(false) ? SaberTailor::ApplicationMethod::ElectroMethod : SaberTailor::ApplicationMethod::Default;
    convertedConfig.mirrorZRot = GetValue<bool>(oldConfig, "mirrorZRot").value_or(false);
    convertedConfig.isGripModEnabled = GetValue<bool>(oldConfig, "isEnabled").value_or(false);

    getConfig().config.SetObject();
    getConfig().config.RemoveAllMembers();
    getConfig().config.AddMember("currentConfig", "Default", getConfig().config.GetAllocator()); getConfig().Write();
    getMainConfig().currentConfig.SetValue("Default");

    std::string dir = getDataDir(modInfo);
    if(!direxists(dir)) mkpath(dir);
    std::string filePath = dir + "Default.json";
    writefile(filePath, CreateJSONString(convertedConfig));
    ReadFromFile(filePath, SaberTailorMain::config.currentConfig);
}