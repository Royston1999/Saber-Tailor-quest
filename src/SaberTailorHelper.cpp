#include "SaberTailorHelper.hpp"

namespace IncrementHelper{
    std::string Round (float val, int precision){
        // val = (val > -0.001 && val < 0.001) ? 0 : val;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << val;
        std::string Out = stream.str();
        return Out;
    }

    float fixDumbNumberThing(float num){
        if (num > -0.05 && num < 0.04) num = 0.0f;
        return num;
    }

    custom_types::Helpers::Coroutine forceUpdateSliderText(SaberTailor::IncrementSlider* slider, float value, bool posType)
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForFixedUpdate::New_ctor());
        co_yield nullptr;

        if (posType) SetSliderPosText(slider, value);
        else slider->sliderComponent->text->set_text(std::to_string((int)(value)) + " deg");
        
        co_return;
    }

    void SetIncrementText(QuestUI::IncrementSetting* setting, std::string text){
        setting->GetComponentsInChildren<TMPro::TextMeshProUGUI*>()[1]->SetText(text);
    }

    float getPosIncrement(){
        return ((float)(SaberTailorMain::config.saberPosIncrement));
    }

    void SetSliderPosText(SaberTailor::IncrementSlider* slider, float value){
        int realValue = std::round(value);
        std::string unit = SaberTailorMain::config.saberPosDisplayValue;
        std::string text;
        if (unit == "inches") text = Round(realValue/25.4f, 3) + " inches";
        else if (unit == "miles") text = Round(realValue/1609340.0f, 10) + " miles";
        else if (unit == "nmi") text = Round(realValue/1852000.0f, 10) + " nmi";
        else text = Round(realValue/10.0f, 1) + " cm";
        slider->sliderComponent->text->set_text(text);
    }
}

namespace PosRotHelper{
    void revertLeftHand(){
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftPosition", SaberTailorMain::config.currentLeftHandPosition);
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftRotation", SaberTailorMain::config.currentLeftHandRotation);
    }
    void revertRightHand(){
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightPosition", SaberTailorMain::config.currentRightHandPosition);
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightRotation", SaberTailorMain::config.currentRightHandRotation);
    }
}

namespace TransferHelper{
    void mirrorToLeft(){
        auto rPos = SaberTailorMain::config.rightHandPosition;
        auto rRot = SaberTailorMain::config.rightHandRotation;
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftPosition", UnityEngine::Vector3(-rPos.x, rPos.y, rPos.z));
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftRotation", UnityEngine::Vector3(rRot.x, -rRot.y, -rRot.z));
    }
    void mirrorToRight(){
        auto lPos = SaberTailorMain::config.leftHandPosition;
        auto lRot = SaberTailorMain::config.leftHandRotation;
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightPosition", UnityEngine::Vector3(-lPos.x, lPos.y, lPos.z));
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightRotation", UnityEngine::Vector3(lRot.x, -lRot.y, -lRot.z));
    }
    void exportToBaseGame(int hand){
        GlobalNamespace::MainSettingsModelSO* settings = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainSettingsModelSO*>());
        // from left hand settings
        if (hand == 0){
            settings->controllerPosition->set_value(SaberTailorMain::config.leftHandPosition/1000 + UnityEngine::Vector3(-SaberTailorMain::config.leftHandPosition.x/500, 0, 0));
            settings->controllerRotation->set_value(SaberTailorMain::config.leftHandRotation + UnityEngine::Vector3(0, -SaberTailorMain::config.leftHandPosition.y * 2, 0));
            settings->Save();
            settings->Load(true);
        }
        // from right hand settings 
        if (hand == 1){
            settings->controllerPosition->set_value(UnityEngine::Vector3(SaberTailorMain::config.rightHandPosition)/1000);
            settings->controllerRotation->set_value(UnityEngine::Vector3(SaberTailorMain::config.rightHandRotation));
            settings->Save();
            settings->Load(true);
        }
    }
    void importFromBaseGame(int hand){
        GlobalNamespace::MainSettingsModelSO* settings = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainSettingsModelSO*>());
        auto lPos = settings->controllerPosition->get_value() * 1000;
        auto lRot = settings->controllerRotation->get_value();
        if (hand == 0){
            setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftPosition", UnityEngine::Vector3(-lPos.x, lPos.y, lPos.z));
            setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftRotation", UnityEngine::Vector3(lRot.x, -lRot.y, lRot.z));
        }
        if (hand == 1){
            setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightPosition", lPos);
            setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightRotation", lRot);
        }
    }
}

namespace TabHelper{
    UnityEngine::GameObject* AdjustedScrollContainerObject(UnityEngine::GameObject* container, bool active){
        QuestUI::ExternalComponents* externalComponents = container->GetComponent<QuestUI::ExternalComponents*>();
        UnityEngine::RectTransform* scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -5.0f));
        scrollTransform->get_gameObject()->SetActive(active);
        return scrollTransform->get_gameObject();
    }
}