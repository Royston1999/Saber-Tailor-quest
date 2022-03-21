#include "SaberTailorHelper.hpp"

namespace IncrementHelper{
    std::string Round (float val, int precision){
        val = (val > -0.001 && val < 0.001) ? 0 : val;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << val;
        std::string Out = stream.str();
        return Out;
    }

    float fixDumbNumberThing(float num){
        if (num > -0.05 && num < 0.04) num = 0.0f;
        return num;
    }

    custom_types::Helpers::Coroutine forceUpdateSliderText(SaberTailor::IncrementSlider* slider, std::string text)
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForFixedUpdate::New_ctor());
        co_yield nullptr;

        while (to_utf8(csstrtostr(slider->sliderComponent->text->get_text())).compare(text)!=0){
            slider->sliderComponent->text->set_text(text);
            co_yield nullptr;
        }
        co_return;
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
            settings->dyn_controllerPosition()->set_value(UnityEngine::Vector3(-(SaberTailorMain::config.leftPosX)/100, SaberTailorMain::config.leftPosY/100, SaberTailorMain::config.leftPosZ/100));
            settings->dyn_controllerRotation()->set_value(UnityEngine::Vector3(SaberTailorMain::config.leftRotX, -(SaberTailorMain::config.leftRotY), SaberTailorMain::config.leftRotZ));
            settings->Save();
            settings->Load(true);
        }
        // from right hand settings 
        if (hand == 1){
            settings->dyn_controllerPosition()->set_value(UnityEngine::Vector3(SaberTailorMain::config.rightPosX/100, SaberTailorMain::config.rightPosY/100, SaberTailorMain::config.rightPosZ/100));
            settings->dyn_controllerRotation()->set_value(UnityEngine::Vector3(SaberTailorMain::config.rightRotX, SaberTailorMain::config.rightRotY, SaberTailorMain::config.rightRotZ));
            settings->Save();
            settings->Load(true);
        }
    }
    void importFromBaseGame(int hand){
        GlobalNamespace::MainSettingsModelSO* settings = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainSettingsModelSO*>());
        if (hand == 0){
            setFloat(getConfig().config, "leftPosX", -(settings->dyn_controllerPosition()->get_value().x) * 100);
            setFloat(getConfig().config, "leftPosY", settings->dyn_controllerPosition()->get_value().y * 100);
            setFloat(getConfig().config, "leftPosZ", settings->dyn_controllerPosition()->get_value().z * 100);
            getConfig().Write();
            setInt(getConfig().config, "leftRotX", settings->dyn_controllerRotation()->get_value().x);
            setInt(getConfig().config, "leftRotY", -(settings->dyn_controllerRotation()->get_value().y));
            setInt(getConfig().config, "leftRotZ", settings->dyn_controllerRotation()->get_value().z);
            getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        }
        if (hand == 1){
            setFloat(getConfig().config, "rightPosX", settings->dyn_controllerPosition()->get_value().x * 100);
            setFloat(getConfig().config, "rightPosY", settings->dyn_controllerPosition()->get_value().y * 100);
            setFloat(getConfig().config, "rightPosZ", settings->dyn_controllerPosition()->get_value().z * 100);
            getConfig().Write();
            setInt(getConfig().config, "rightRotX", settings->dyn_controllerRotation()->get_value().x);
            setInt(getConfig().config, "rightRotY", settings->dyn_controllerRotation()->get_value().y);
            setInt(getConfig().config, "rightRotZ", settings->dyn_controllerRotation()->get_value().z);
            getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        }
    }
}