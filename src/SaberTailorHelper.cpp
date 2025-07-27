#include "SaberTailorHelper.hpp"
#include "GlobalNamespace/MainSystemInit.hpp"
#include "BeatSaber/GameSettings/GameSettings.hpp"
#include "GlobalNamespace/QuestAppInit.hpp"
#include "GlobalNamespace/SettingsApplicatorSO.hpp"
#include "Utils/EasyDelegate.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "System/Threading/Tasks/Task.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/RectOffset.hpp"

using namespace UnityEngine;
using namespace GlobalNamespace;

namespace IncrementHelper{
    std::string Round (float val, int precision){
        // val = (val > -0.001 && val < 0.001) ? 0 : val;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << val;
        std::string Out = stream.str();
        return Out;
    }

    void SetIncrementText(BSML::IncrementSetting* setting, std::string text){
        setting->GetComponentsInChildren<TMPro::TextMeshProUGUI*>()[1]->set_text(text);
    }

    float getPosIncrement(){
        return ((float)(GET_VALUE(saberPosIncrement)));
    }

    std::string formatSliderPosText(float value) {
        std::string unit = GET_VALUE(saberPosDisplayUnit);
        if (unit == "inches") return fmt::format("{0:.3f} inches", value/25.4f);
        else if (unit == "miles") return fmt::format("{0:.10f} miles", value/1609340.0f);
        else if (unit == "nmi") return fmt::format("{0:.10f} nmi", value/1852000.0f);
        else return fmt::format("{0:.1f} cm", value/10.0f);
    }
}

namespace PosRotHelper{
    void revertLeftHand(bool toZero){
        SET_VALUE(leftHandPosition, toZero ? Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentLeftHandPosition));
        SET_VALUE(leftHandRotation, toZero ? Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentLeftHandRotation));
    }
    void revertRightHand(bool toZero){
        SET_VALUE(rightHandPosition, toZero ? Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentRightHandPosition));
        SET_VALUE(rightHandRotation, toZero ? Vector3(0.0f, 0.0f, 0.0f) : GET_VALUE(currentRightHandRotation));
    }
}

static Vector3 operator+(Vector3 a, Vector3 b) { return Vector3::op_Addition(a,b); }
static Vector3 operator-(Vector3 a, Vector3 b) { return Vector3::op_Subtraction(a,b); }
static Vector3 operator*(Vector3 a, float_t b) { return Vector3::op_Multiply(a,b); }
static Vector3 operator/(Vector3 a, float_t b) { return Vector3::op_Division(a,b); }

namespace TransferHelper{
    void mirrorToLeft(){
        auto rPos = GET_VALUE(rightHandPosition);
        auto rRot = GET_VALUE(rightHandRotation);
        SET_VALUE(leftHandPosition, Vector3(-rPos.x, rPos.y, rPos.z));
        SET_VALUE(leftHandRotation, Vector3(rRot.x, -rRot.y, -rRot.z));
    }
    void mirrorToRight(){
        auto lPos = GET_VALUE(leftHandPosition);
        auto lRot = GET_VALUE(leftHandRotation);
        SET_VALUE(rightHandPosition, Vector3(-lPos.x, lPos.y, lPos.z));
        SET_VALUE(rightHandRotation, Vector3(lRot.x, -lRot.y, -lRot.z));
    }
    void exportToBaseGame(int hand){
        // auto appInit = Resources::FindObjectsOfTypeAll<QuestAppInit*>().front_or_default();
        // auto settings = appInit->____mainSystemInit->____mainSettingsHandler;
        // auto controllers = settings->get_instance()->get_controllerSettings();
        // // from left hand settings
        // if (hand == 0){
        //     Vector3 leftHandPos = GET_VALUE(leftHandPosition) / 1000;
        //     Vector3 leftHandRot = GET_VALUE(leftHandRotation);
        //     controllers->set_positionOffset(Vector3(-leftHandPos.x, leftHandPos.y, leftHandPos.z));
        //     controllers->set_rotationOffset(Vector3(leftHandRot.x, -leftHandRot.y, -leftHandRot.z));
        // }
        // // from right hand settings 
        // else if (hand == 1){
        //     controllers->set_positionOffset(Vector3(GET_VALUE(rightHandPosition))/1000);
        //     controllers->set_rotationOffset(Vector3(GET_VALUE(rightHandRotation)));
        // }
        // appInit->____settingsApplicator->ApplyMainSettings(settings->get_instance());
        // auto task = settings->SaveAsync(true);
        // BSML::MainThreadScheduler::ScheduleUntil([task]() { return task->get_IsCompleted(); }, [settings](){ settings->LoadAsync(); });
    }
    void importFromBaseGame(int hand){
    //     auto controllers = Resources::FindObjectsOfTypeAll<MainSystemInit*>().front_or_default()->____mainSettingsHandler->get_instance()->get_controllerSettings();

    //     auto lPos = controllers->get_positionOffset() * 1000;
    //     auto lRot = controllers->get_rotationOffset();
    //     if (hand == 0){
    //         SET_VALUE(leftHandPosition, Vector3(-lPos.x, lPos.y, lPos.z));
    //         SET_VALUE(leftHandRotation, Vector3(lRot.x, -lRot.y, -lRot.z));
    //     }
    //     if (hand == 1){
    //         SET_VALUE(rightHandPosition, lPos);
    //         SET_VALUE(rightHandRotation, lRot);
    //     }
    }
}

namespace TabHelper{
    GameObject* AdjustedScrollContainerObject(GameObject* container, bool active){
        BSML::ExternalComponents* externalComponents = container->GetComponent<BSML::ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(Vector2(0.0f, -7.0f));
        scrollTransform->set_sizeDelta(Vector2(0.0f, 0.0f));
        scrollTransform->get_gameObject()->SetActive(active);
        return scrollTransform->get_gameObject();
    }
}