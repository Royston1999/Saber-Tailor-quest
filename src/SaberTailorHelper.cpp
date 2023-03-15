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
            SetSliderPosText(SaberTailor::UI::LeftHand::posX, GET_VALUE(leftHandPosition).x);
            SetSliderPosText(SaberTailor::UI::LeftHand::posY, GET_VALUE(leftHandPosition).y);
            SetSliderPosText(SaberTailor::UI::LeftHand::posZ, GET_VALUE(leftHandPosition).z);
            SaberTailor::UI::LeftHand::rotX->sliderComponent->text->set_text(std::to_string((int)(GET_VALUE(leftHandRotation).x)) + " deg");
            SaberTailor::UI::LeftHand::rotY->sliderComponent->text->set_text(std::to_string((int)(GET_VALUE(leftHandRotation).y)) + " deg");
            SaberTailor::UI::LeftHand::rotZ->sliderComponent->text->set_text(std::to_string((int)(GET_VALUE(leftHandRotation).z)) + " deg");
        }
        else{
            SetSliderPosText(SaberTailor::UI::RightHand::posX, GET_VALUE(rightHandPosition).x);
            SetSliderPosText(SaberTailor::UI::RightHand::posY, GET_VALUE(rightHandPosition).y);
            SetSliderPosText(SaberTailor::UI::RightHand::posZ, GET_VALUE(rightHandPosition).z);
            SaberTailor::UI::RightHand::rotX->sliderComponent->text->set_text(std::to_string((int)(GET_VALUE(rightHandRotation).x)) + " deg");
            SaberTailor::UI::RightHand::rotY->sliderComponent->text->set_text(std::to_string((int)(GET_VALUE(rightHandRotation).y)) + " deg");
            SaberTailor::UI::RightHand::rotZ->sliderComponent->text->set_text(std::to_string((int)(GET_VALUE(rightHandRotation).z)) + " deg");
        }
        
        co_return;
    }

    void SetIncrementText(QuestUI::IncrementSetting* setting, std::string text){
        setting->GetComponentsInChildren<TMPro::TextMeshProUGUI*>()[1]->SetText(text);
    }

    float getPosIncrement(){
        return ((float)(GET_VALUE(saberPosIncrement)));
    }

    void SetSliderPosText(SaberTailor::IncrementSlider* slider, float value){
        int realValue = std::round(value);
        std::string unit = GET_VALUE(saberPosDisplayUnit);
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
        SET_VALUE(leftHandPosition, toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentLeftHandPosition));
        SET_VALUE(leftHandRotation, toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentLeftHandRotation));
    }
    void revertRightHand(bool toZero){
        SET_VALUE(rightHandPosition, toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentRightHandPosition));
        SET_VALUE(rightHandRotation, toZero ? UnityEngine::Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentRightHandRotation));
    }
}

namespace TransferHelper{
    void mirrorToLeft(){
        auto rPos = GET_VALUE(rightHandPosition);
        auto rRot = GET_VALUE(rightHandRotation);
        SET_VALUE(leftHandPosition, UnityEngine::Vector3(-rPos.x, rPos.y, rPos.z));
        SET_VALUE(leftHandRotation, UnityEngine::Vector3(rRot.x, -rRot.y, -rRot.z));
    }
    void mirrorToRight(){
        auto lPos = GET_VALUE(leftHandPosition);
        auto lRot = GET_VALUE(leftHandRotation);
        SET_VALUE(rightHandPosition, UnityEngine::Vector3(-lPos.x, lPos.y, lPos.z));
        SET_VALUE(rightHandRotation, UnityEngine::Vector3(lRot.x, -lRot.y, -lRot.z));
    }
    void exportToBaseGame(int hand){
        GlobalNamespace::MainSettingsModelSO* settings = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainSettingsModelSO*>());
        // from left hand settings
        if (hand == 0){
            settings->controllerPosition->set_value(UnityEngine::Vector3(GET_VALUE(leftHandPosition))/1000 + UnityEngine::Vector3(-GET_VALUE(leftHandPosition).x/500, 0, 0));
            settings->controllerRotation->set_value(GET_VALUE(leftHandRotation) + UnityEngine::Vector3(0, -GET_VALUE(leftHandPosition).y * 2, 0));
            settings->Save();
            settings->Load(true);
        }
        // from right hand settings 
        if (hand == 1){
            settings->controllerPosition->set_value(UnityEngine::Vector3(GET_VALUE(rightHandPosition))/1000);
            settings->controllerRotation->set_value(UnityEngine::Vector3(GET_VALUE(rightHandRotation)));
            settings->Save();
            settings->Load(true);
        }
    }
    void importFromBaseGame(int hand){
        GlobalNamespace::MainSettingsModelSO* settings = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainSettingsModelSO*>());
        auto lPos = settings->controllerPosition->get_value() * 1000;
        auto lRot = settings->controllerRotation->get_value();
        if (hand == 0){
            SET_VALUE(leftHandPosition, UnityEngine::Vector3(-lPos.x, lPos.y, lPos.z));
            SET_VALUE(leftHandRotation, UnityEngine::Vector3(lRot.x, -lRot.y, lRot.z));
        }
        if (hand == 1){
            SET_VALUE(rightHandPosition, lPos);
            SET_VALUE(rightHandRotation, lRot);
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