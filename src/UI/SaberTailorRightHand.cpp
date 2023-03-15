#include "UI/SaberTailorRightHand.hpp"
#include "UI/SettingsFlowCoordinator.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "SaberTailorHelper.hpp"
#include "Utils/UIUtils.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorRightHand);

UnityEngine::UI::VerticalLayoutGroup* rightsabercontainer;

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace custom_types::Helpers;
using namespace IncrementHelper;
using namespace SaberTailor::UI::RightHand;
using namespace QuestUI::BeatSaberUI;

namespace SaberTailor::UI::RightHand{
    SaberTailor::IncrementSlider* posX;
    SaberTailor::IncrementSlider* posY;
    SaberTailor::IncrementSlider* posZ;
    SaberTailor::IncrementSlider* rotX;
    SaberTailor::IncrementSlider* rotY;
    SaberTailor::IncrementSlider* rotZ;
    void UpdateSliderValues(){
        posX->sliderComponent->set_value(GET_VALUE(rightHandPosition).x);
        posY->sliderComponent->set_value(GET_VALUE(rightHandPosition).y);
        posZ->sliderComponent->set_value(GET_VALUE(rightHandPosition).z);
        rotX->sliderComponent->set_value(GET_VALUE(rightHandRotation).x);
        rotY->sliderComponent->set_value(GET_VALUE(rightHandRotation).y);
        rotZ->sliderComponent->set_value(GET_VALUE(rightHandRotation).z);
        SetSliderPosText(posX, GET_VALUE(rightHandPosition).x);
        SetSliderPosText(posY, GET_VALUE(rightHandPosition).y);
        SetSliderPosText(posZ, GET_VALUE(rightHandPosition).z);
        rotX->sliderComponent->text->set_text(std::to_string((int)GET_VALUE(rightHandRotation).x) + " deg");
        rotY->sliderComponent->text->set_text(std::to_string((int)GET_VALUE(rightHandRotation).y) + " deg");
        rotZ->sliderComponent->text->set_text(std::to_string((int)GET_VALUE(rightHandRotation).z) + " deg");
    }
}

void SaberTailor::Views::SaberTailorRightHand::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {

    if (firstActivation) {
        UIUtils::AddHeader(get_transform(), "Right Hand Settings", UnityEngine::Color(0.188f, 0.620f, 1.0f, 1.0f));
        rightsabercontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(CreateSliders()));
    }
    else SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(false)));
    GET_VALUE(currentRightHandPosition) = Vector3(GET_VALUE(rightHandPosition));
    GET_VALUE(currentRightHandRotation) = Vector3(GET_VALUE(rightHandRotation));
}

custom_types::Helpers::Coroutine SaberTailor::Views::SaberTailorRightHand::CreateSliders(){

    HorizontalLayoutGroup* somebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
    somebuttons->set_childControlWidth(true); somebuttons->set_childForceExpandWidth(false); somebuttons->set_childForceExpandHeight(true);
    somebuttons->set_childAlignment(TextAnchor::MiddleCenter);
    auto* mirrorLeft = CreateUIButton(somebuttons->get_transform(), "Mirror to Left", [](){
        TransferHelper::mirrorToLeft();
        SaberTailor::UI::LeftHand::UpdateSliderValues();
    });
    AddHoverHint(mirrorLeft->get_gameObject(), "Copies the position and rotation values into the left hand");
    mirrorLeft->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    auto* exportButton = CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
        TransferHelper::exportToBaseGame(1);
    });
    AddHoverHint(exportButton->get_gameObject(), "Takes the current left hand settings and copies them into the base game settings");
    exportButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);

    posX = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Position X", GET_VALUE(saberPosIncrement), GET_VALUE(rightHandPosition).x, -100, 100, [](float value){
        SET_VALUE(rightHandPosition.x, (int)std::round(value));
        SetSliderPosText(posX, GET_VALUE(rightHandPosition).x);
    });
    co_yield nullptr;
    posY = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Position Y", GET_VALUE(saberPosIncrement), GET_VALUE(rightHandPosition).y, -100, 100, [](float value){
        SET_VALUE(rightHandPosition.y, (int)std::round(value));
        SetSliderPosText(posY, GET_VALUE(rightHandPosition).y);
    });
    co_yield nullptr;
    posZ = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Position Z", GET_VALUE(saberPosIncrement), GET_VALUE(rightHandPosition).z, -100, 100, [](float value){
        SET_VALUE(rightHandPosition.z, (int)std::round(value));
        SetSliderPosText(posZ, GET_VALUE(rightHandPosition).z);
    });
    co_yield nullptr;
    rotX = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Rotation X", GET_VALUE(saberRotIncrement), GET_VALUE(rightHandRotation).x, -180, 180, [](float value){
        int x = std::round(value);
        SET_VALUE(rightHandRotation.x, (int)std::round(value));
        rotX->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;
    rotY = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Rotation Y", GET_VALUE(saberRotIncrement), GET_VALUE(rightHandRotation).y, -180, 180, [](float value){
        int x = std::round(value);
        SET_VALUE(rightHandRotation.y, (int)std::round(value));
        rotY->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;
    rotZ = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Rotation Z", GET_VALUE(saberRotIncrement), GET_VALUE(rightHandRotation).z, -180, 180, [](float value){
        int x = std::round(value);
        SET_VALUE(rightHandRotation.z, (int)std::round(value));
        rotZ->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;

    auto* bottomButtons = CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
    bottomButtons->set_childControlWidth(true); bottomButtons->set_childForceExpandWidth(false); bottomButtons->set_childForceExpandHeight(true);
    bottomButtons->set_childAlignment(TextAnchor::MiddleCenter);
    auto* revertButton = CreateUIButton(bottomButtons->get_transform(), "Revert", [](){
        PosRotHelper::revertRightHand(false);
        UpdateSliderValues();
    });
    AddHoverHint(revertButton->get_gameObject(), "Reverts all of the values back to what they were when the settings were last opened (or to the values of the last profile change)");
    revertButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    auto* resetButton = CreateUIButton(bottomButtons->get_transform(), "Reset Values", [](){
        PosRotHelper::revertRightHand(true);
        UpdateSliderValues();
    });
    AddHoverHint(resetButton->get_gameObject(), "Resets all of the values to 0");
    resetButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);

    posX->increment = getPosIncrement();
    posY->increment = getPosIncrement();
    posZ->increment = getPosIncrement();
    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(false)));
    co_return;
}