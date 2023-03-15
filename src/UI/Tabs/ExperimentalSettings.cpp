#include "UI/Tabs/ExperimentalSettings.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "UnityEngine/TextAnchor.hpp"

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;

TMPro::TextMeshProUGUI* countdownText;
UI::Button* startButton;
HMUI::ModalView* rotatorModal;

void fixAngles(Vector3& vector){
    int angles[] = {(int)std::round(vector.x), (int)std::round(vector.y), (int)std::round(vector.z)};
    for (auto& angle : angles){
        angle = angle % 360;
        if (std::abs(angle) > 180) angle = std::abs(angle) == angle ? angle-360 : angle+360;
    }
    vector = {(float)angles[0], (float)angles[1], (float)angles[2]};
}

void adjustControllerSettings(){
    auto controllers = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
    auto* dummyTransform = GameObject::New_ctor()->get_transform();
    for (auto& controller : controllers){
        if (controller->get_node() == XR::XRNode::LeftHand){
            auto x = controller->get_transform()->InverseTransformDirection(Quaternion::get_identity().get_eulerAngles());
            SET_VALUE(leftHandRotation, x);
            SaberTailor::UI::LeftHand::UpdateSliderValues();
        }
        else if (controller->get_node() == XR::XRNode::RightHand){ 
            auto localRot = GET_VALUE(rightHandRotation);
            auto localPos = GET_VALUE(rightHandPosition);
            auto rot = controller->get_transform()->get_localRotation().get_eulerAngles();
            // fixAngles(rot);
            auto toRotate = Vector3::get_zero(); 
            for (int i=0; i<100; i++){
                controller->get_transform()->Rotate(-toRotate);
                toRotate = Vector3(rot.y == 0 ? -rot.x : 0, -rot.y, rot.x == 0 ? -rot.z : 0);
                // getLogger().info("pre rot: %.2f, %.2f, %.2f", rot.x, rot.y, rot.z);
                controller->get_transform()->Rotate(toRotate);
                rot = controller->get_transform()->get_localRotation().get_eulerAngles();
                fixAngles(rot);
                // getLogger().info("post rot: %.2f, %.2f, %.2f", rot.x, rot.y, rot.z);
                if (std::abs(rot.z) <= 0.5 && std::abs(rot.y) <= 0.5) break;
            }
            // getLogger().info("settings angles: %.2f, %.2f, %.2f", localRot.x, localRot.y, localRot.z);
            fixAngles(toRotate);
            SET_VALUE(rightHandRotation, toRotate);
            SaberTailor::UI::RightHand::UpdateSliderValues();
        }
    }
    Object::Destroy(dummyTransform->get_gameObject());
}

custom_types::Helpers::Coroutine CountdownRoutine(){
    startButton->set_interactable(false);
    countdownText->set_alpha(1.0f);
    for (int i=5; i>0; i--){
        countdownText->SetText(std::to_string(i));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(1.0f));
        if (!rotatorModal->isShown) co_return;
    }
    if (!rotatorModal->isShown) co_return;
    SET_VALUE(rightHandRotation, UnityEngine::Vector3::get_zero());
    co_yield nullptr;
    adjustControllerSettings();
    countdownText->set_alpha(0.0f);
    startButton->set_interactable(true);
    co_return;
}

namespace SaberTailor::Tabs::Experimental{
    GameObject* CreateExperimentalSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Experimental");
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);
        std::function<void()> onClose = [](){
            countdownText->set_alpha(0.0f);
            startButton->set_interactable(true);
        };
        rotatorModal = CreateModal(container->get_transform(), UnityEngine::Vector2(90, 60), [onClose](HMUI::ModalView *modal) {onClose();}, true);
        auto* rotatorVert = CreateVerticalLayoutGroup(rotatorModal->get_transform());
        rotatorVert->get_gameObject()->GetComponentInChildren<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(82.0f);
        rotatorVert->set_childControlWidth(true); rotatorVert->set_childForceExpandWidth(false); rotatorVert->set_childForceExpandHeight(true);
        rotatorVert->set_childAlignment(TextAnchor::MiddleCenter);
        auto* title = CreateText(rotatorVert->get_transform(), "Saber Rotator", false);
        title->set_fontSize(8.0f); title->set_alignment(TMPro::TextAlignmentOptions::Center);
        auto* bodyText = CreateText(rotatorVert->get_transform(), "This tool aims to assist you in finding good saber rotations.\nUsage - press the button and then hold your arms out as if you are trying to swing directly in line with the outer lanes. once the countdown finishes, your settings will be updated.", false);
        bodyText->set_alignment(TMPro::TextAlignmentOptions::Center);
        bodyText->set_enableWordWrapping(true);
        countdownText = CreateText(rotatorVert->get_transform(), "5", false);
        countdownText->set_fontSize(10.0f);
        countdownText->set_alpha(0.0f);
        countdownText->set_alignment(TMPro::TextAlignmentOptions::Center);
        startButton = CreateUIButton(rotatorVert->get_transform(), "Start", "PracticeButton", Vector2::get_zero(), {25, 7}, [](){
            GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(CountdownRoutine()));
        });
        auto* element = startButton->get_gameObject()->GetComponentInChildren<UnityEngine::UI::LayoutElement*>();
        element->set_preferredWidth(35.0f);
        element->set_flexibleWidth(0);

        auto saberRotatorButton = CreateUIButton(container->get_transform(), "Doesn't Work", [](){
            rotatorModal->Show(true, true, nullptr);
        });
        saberRotatorButton->set_interactable(false);
        AddHoverHint(saberRotatorButton->get_gameObject(), "this probably doesn't work as intended");

        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}