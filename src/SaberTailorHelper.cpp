#include "SaberTailorHelper.hpp"

namespace IncrementHelper{
    std::string Round (float val, int precision){
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << val;
        std::string Out = stream.str();
        return Out;
    }

    void setIncrementText(QuestUI::IncrementSetting* increment, float num){
        if (num > -0.05 && num < 0.04) num = 0.0f;
        Array<TMPro::TextMeshProUGUI*>* incText = increment->get_transform()->GetComponentsInChildren<TMPro::TextMeshProUGUI*>();
        (*incText)[1]->SetText(il2cpp_utils::newcsstr(Round(num, 1) + " cm"));
    }

    void setRotIncrementText(QuestUI::IncrementSetting* increment, int num){
        Array<TMPro::TextMeshProUGUI*>* incText = increment->get_transform()->GetComponentsInChildren<TMPro::TextMeshProUGUI*>();
        (*incText)[1]->SetText(il2cpp_utils::newcsstr(std::to_string(num) + " deg"));
    }

    float fixDumbNumberThing(float num){
        if (num > -0.05 && num < 0.04) num = 0.0f;
        return num;
    }
}

namespace PosRotHelper{
    void revertLeftHand(){
        setFloat(getConfig().config, "leftPosX", SaberTailorMain::config.currentPosXLeft);
        setFloat(getConfig().config, "leftPosY", SaberTailorMain::config.currentPosYLeft);
        setFloat(getConfig().config, "leftPosZ", SaberTailorMain::config.currentPosZLeft);
        getConfig().Write();
        setInt(getConfig().config, "leftRotX", SaberTailorMain::config.currentRotXLeft);
        setInt(getConfig().config, "leftRotY", SaberTailorMain::config.currentRotYLeft);
        setInt(getConfig().config, "leftRotZ", SaberTailorMain::config.currentRotZLeft);
        getConfig().Write();
        ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
    }
    void revertRightHand(){
        setFloat(getConfig().config, "rightPosX", SaberTailorMain::config.currentPosXRight);
        setFloat(getConfig().config, "rightPosY", SaberTailorMain::config.currentPosYRight);
        setFloat(getConfig().config, "rightPosZ", SaberTailorMain::config.currentPosZRight);
        getConfig().Write();
        setInt(getConfig().config, "rightRotX", SaberTailorMain::config.currentRotXRight);
        setInt(getConfig().config, "rightRotY", SaberTailorMain::config.currentRotYRight);
        setInt(getConfig().config, "rightRotZ", SaberTailorMain::config.currentRotZRight);
        getConfig().Write();
        ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
    }
}

namespace TransferHelper{
    void mirrorToLeft(){
        setFloat(getConfig().config, "leftPosX", -SaberTailorMain::config.rightPosX);
        setFloat(getConfig().config, "leftPosY", SaberTailorMain::config.rightPosY);
        setFloat(getConfig().config, "leftPosZ", SaberTailorMain::config.rightPosZ);
        getConfig().Write();
        setInt(getConfig().config, "leftRotX", SaberTailorMain::config.rightRotX);
        setInt(getConfig().config, "leftRotY", -SaberTailorMain::config.rightRotY);
        setInt(getConfig().config, "leftRotZ", -SaberTailorMain::config.rightRotZ);
        getConfig().Write();
        ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
    }
    void mirrorToRight(){
        setFloat(getConfig().config, "rightPosX", -SaberTailorMain::config.leftPosX);
        setFloat(getConfig().config, "rightPosY", SaberTailorMain::config.leftPosY);
        setFloat(getConfig().config, "rightPosZ", SaberTailorMain::config.leftPosZ);
        getConfig().Write();
        setInt(getConfig().config, "rightRotX", SaberTailorMain::config.leftRotX);
        setInt(getConfig().config, "rightRotY", -SaberTailorMain::config.leftRotY);
        setInt(getConfig().config, "rightRotZ", -SaberTailorMain::config.leftRotZ);
        getConfig().Write();
        ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
    }
    void exportToBaseGame(int hand){
        GlobalNamespace::MainSettingsModelSO* settings = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainSettingsModelSO*>());
        // from left hand settings
        if (hand == 0){
            settings->controllerPosition->value.x = -(SaberTailorMain::config.leftPosX)/100;
            settings->controllerPosition->value.y = SaberTailorMain::config.leftPosY/100;
            settings->controllerPosition->value.z = SaberTailorMain::config.leftPosZ/100;
            settings->controllerRotation->value.x = SaberTailorMain::config.leftRotX;
            settings->controllerRotation->value.y = -(SaberTailorMain::config.leftRotY);
            settings->controllerRotation->value.z = SaberTailorMain::config.leftRotZ;
            settings->Save();
            settings->Load(true);
        }
        // from right hand settings 
        if (hand == 1){
            settings->controllerPosition->value.x = SaberTailorMain::config.rightPosX/100;
            settings->controllerPosition->value.y = SaberTailorMain::config.rightPosY/100;
            settings->controllerPosition->value.z = SaberTailorMain::config.rightPosZ/100;
            settings->controllerRotation->value.x = SaberTailorMain::config.rightRotX;
            settings->controllerRotation->value.y = SaberTailorMain::config.rightRotY;
            settings->controllerRotation->value.z = SaberTailorMain::config.rightRotZ;
            settings->Save();
            settings->Load(true);
        }
    }
    void importFromBaseGame(int hand){
        GlobalNamespace::MainSettingsModelSO* settings = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainSettingsModelSO*>());
        if (hand == 0){
            setFloat(getConfig().config, "leftPosX", -(settings->controllerPosition->value.x) * 100);
            setFloat(getConfig().config, "leftPosY", settings->controllerPosition->value.y * 100);
            setFloat(getConfig().config, "leftPosZ", settings->controllerPosition->value.z * 100);
            getConfig().Write();
            setInt(getConfig().config, "leftRotX", settings->controllerRotation->value.x);
            setInt(getConfig().config, "leftRotY", -(settings->controllerRotation->value.y));
            setInt(getConfig().config, "leftRotZ", settings->controllerRotation->value.z);
            getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        }
        if (hand == 1){
            setFloat(getConfig().config, "rightPosX", settings->controllerPosition->value.x * 100);
            setFloat(getConfig().config, "rightPosY", settings->controllerPosition->value.y * 100);
            setFloat(getConfig().config, "rightPosZ", settings->controllerPosition->value.z * 100);
            getConfig().Write();
            setInt(getConfig().config, "rightRotX", settings->controllerRotation->value.x);
            setInt(getConfig().config, "rightRotY", settings->controllerRotation->value.y);
            setInt(getConfig().config, "rightRotZ", settings->controllerRotation->value.z);
            getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        }
    }
}