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

    custom_types::Helpers::Coroutine forceUpdateSliderText(bool isLeftHand)
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForFixedUpdate::New_ctor());
        co_yield nullptr;
        
        if (isLeftHand){
            SetSliderPosText(SaberTailor::UI::LeftHand::posX, SaberTailorMain::config.leftHandPosition.x);
            SetSliderPosText(SaberTailor::UI::LeftHand::posY, SaberTailorMain::config.leftHandPosition.y);
            SetSliderPosText(SaberTailor::UI::LeftHand::posZ, SaberTailorMain::config.leftHandPosition.z);
            SaberTailor::UI::LeftHand::rotX->sliderComponent->text->set_text(std::to_string((int)(SaberTailorMain::config.leftHandRotation.x)) + " deg");
            SaberTailor::UI::LeftHand::rotY->sliderComponent->text->set_text(std::to_string((int)(SaberTailorMain::config.leftHandRotation.y)) + " deg");
            SaberTailor::UI::LeftHand::rotZ->sliderComponent->text->set_text(std::to_string((int)(SaberTailorMain::config.leftHandRotation.z)) + " deg");
        }
        else{
            SetSliderPosText(SaberTailor::UI::RightHand::posX, SaberTailorMain::config.rightHandPosition.x);
            SetSliderPosText(SaberTailor::UI::RightHand::posY, SaberTailorMain::config.rightHandPosition.y);
            SetSliderPosText(SaberTailor::UI::RightHand::posZ, SaberTailorMain::config.rightHandPosition.z);
            SaberTailor::UI::RightHand::rotX->sliderComponent->text->set_text(std::to_string((int)(SaberTailorMain::config.rightHandRotation.x)) + " deg");
            SaberTailor::UI::RightHand::rotY->sliderComponent->text->set_text(std::to_string((int)(SaberTailorMain::config.rightHandRotation.y)) + " deg");
            SaberTailor::UI::RightHand::rotZ->sliderComponent->text->set_text(std::to_string((int)(SaberTailorMain::config.rightHandRotation.z)) + " deg");
        }
        
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
    void revertLeftHand(bool toZero){
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftPosition", toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : SaberTailorMain::config.currentLeftHandPosition);
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripLeftRotation", toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : SaberTailorMain::config.currentLeftHandRotation);
    }
    void revertRightHand(bool toZero){
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightPosition", toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : SaberTailorMain::config.currentRightHandPosition);
        setVectorObject(SaberTailorMain::config.currentlyLoadedConfig, "GripRightRotation", toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : SaberTailorMain::config.currentRightHandRotation);
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