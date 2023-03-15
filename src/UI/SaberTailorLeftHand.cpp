#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SettingsFlowCoordinator.hpp"
#include "SaberTailorHelper.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "Utils/UIUtils.hpp"
#include "HMUI/RangeValuesTextSlider.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorLeftHand);

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace custom_types::Helpers;
using namespace IncrementHelper;
using namespace SaberTailor::UI::LeftHand;
using namespace QuestUI::BeatSaberUI;

UnityEngine::UI::VerticalLayoutGroup* leftsabercontainer;

namespace SaberTailor::UI::LeftHand{
    SaberTailor::IncrementSlider* posX;
    SaberTailor::IncrementSlider* posY;
    SaberTailor::IncrementSlider* posZ;
    SaberTailor::IncrementSlider* rotX;
    SaberTailor::IncrementSlider* rotY;
    SaberTailor::IncrementSlider* rotZ;
    void UpdateSliderValues(){
        posX->sliderComponent->set_value(GET_VALUE(leftHandPosition).x);
        posY->sliderComponent->set_value(GET_VALUE(leftHandPosition).y);
        posZ->sliderComponent->set_value(GET_VALUE(leftHandPosition).z);
        rotX->sliderComponent->set_value(GET_VALUE(leftHandRotation).x);
        rotY->sliderComponent->set_value(GET_VALUE(leftHandRotation).y);
        rotZ->sliderComponent->set_value(GET_VALUE(leftHandRotation).z);
        SetSliderPosText(posX, GET_VALUE(leftHandPosition).x);
        SetSliderPosText(posY, GET_VALUE(leftHandPosition).y);
        SetSliderPosText(posZ, GET_VALUE(leftHandPosition).z);
        rotX->sliderComponent->text->set_text(std::to_string((int)GET_VALUE(leftHandRotation).x) + " deg");
        rotY->sliderComponent->text->set_text(std::to_string((int)GET_VALUE(leftHandRotation).y) + " deg");
        rotZ->sliderComponent->text->set_text(std::to_string((int)GET_VALUE(leftHandRotation).z) + " deg");
    }
}

void SaberTailor::Views::SaberTailorLeftHand::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    
    if (firstActivation) {
        UIUtils::AddHeader(get_transform(), "Left Hand Settings", Color(0.941f, 0.188f, 0.188f, 1.0f));
        leftsabercontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(CreateSliders()));
    }
    else SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(true)));
    GET_VALUE(currentLeftHandPosition) = Vector3(GET_VALUE(leftHandPosition));
    GET_VALUE(currentLeftHandRotation) = Vector3(GET_VALUE(leftHandRotation));  
    
}

custom_types::Helpers::Coroutine SaberTailor::Views::SaberTailorLeftHand::CreateSliders(){

    HorizontalLayoutGroup* somebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(leftsabercontainer->get_transform());
    somebuttons->set_childControlWidth(true); somebuttons->set_childForceExpandWidth(false); somebuttons->set_childForceExpandHeight(true);
    somebuttons->set_childAlignment(TextAnchor::MiddleCenter);
    auto* mirrorRight = CreateUIButton(somebuttons->get_transform(), "Mirror to Right", [](){
        TransferHelper::mirrorToRight();
        SaberTailor::UI::RightHand::UpdateSliderValues();
    });
    AddHoverHint(mirrorRight->get_gameObject(), "Copies the position and rotation values into the right hand");
    mirrorRight->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    auto* exportButton = CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
        TransferHelper::exportToBaseGame(0);
    });
    AddHoverHint(exportButton->get_gameObject(), "Takes the current right hand settings and copies them into the base game settings");
    exportButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    posX = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer, "Position X", GET_VALUE(saberPosIncrement), GET_VALUE(leftHandPosition).x, -100, 100, [](float value){
        SET_VALUE(leftHandPosition.x, (int)std::round(value));
        SetSliderPosText(posX, GET_VALUE(leftHandPosition).x);
    });
    co_yield nullptr;
    posY = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer, "Position Y", GET_VALUE(saberPosIncrement), GET_VALUE(leftHandPosition).y, -100, 100, [](float value){
        SET_VALUE(leftHandPosition.y, (int)std::round(value));
        SetSliderPosText(posY, GET_VALUE(leftHandPosition).y);
    });
    co_yield nullptr;
    posZ = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer, "Position Z", GET_VALUE(saberPosIncrement), GET_VALUE(leftHandPosition).z, -100, 100, [](float value){
        SET_VALUE(leftHandPosition.z, (int)std::round(value));
        SetSliderPosText(posZ, GET_VALUE(leftHandPosition).z);
    });
    co_yield nullptr;
    rotX = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer, "Rotation X", GET_VALUE(saberRotIncrement), GET_VALUE(leftHandRotation).x, -180, 180, [](float value){
        int x = std::round(value);
        SET_VALUE(leftHandRotation.x, (int)std::round(value));
        rotX->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;
    rotY = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer, "Rotation Y", GET_VALUE(saberRotIncrement), GET_VALUE(leftHandRotation).y, -180, 180, [](float value){
        int x = std::round(value);
        SET_VALUE(leftHandRotation.y, (int)std::round(value));
        rotY->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;
    rotZ = SaberTailor::IncrementSlider::CreateIncrementSlider(leftsabercontainer, "Rotation Z", GET_VALUE(saberRotIncrement), GET_VALUE(leftHandRotation).z, -180, 180, [](float value){
        int x = std::round(value);
        SET_VALUE(leftHandRotation.z, (int)std::round(value));
        rotZ->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;

    auto* bottomButtons = CreateHorizontalLayoutGroup(leftsabercontainer->get_transform());
    bottomButtons->set_childControlWidth(true); bottomButtons->set_childForceExpandWidth(false); bottomButtons->set_childForceExpandHeight(true);
    bottomButtons->set_childAlignment(TextAnchor::MiddleCenter);
    auto* revertButton = CreateUIButton(bottomButtons->get_transform(), "Revert", [](){
        PosRotHelper::revertLeftHand(false);
        UpdateSliderValues();
    });
    AddHoverHint(revertButton->get_gameObject(), "Reverts all of the values back to what they were when the settings were last opened (or to the values of the last profile change)");
    revertButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    auto* resetButton = CreateUIButton(bottomButtons->get_transform(), "ResetValues", [](){
        PosRotHelper::revertLeftHand(true);
        UpdateSliderValues();
    });
    AddHoverHint(resetButton->get_gameObject(), "Resets all of the values to 0");
    resetButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);

    posX->increment = getPosIncrement();
    posY->increment = getPosIncrement();
    posZ->increment = getPosIncrement();
    SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(true)));
    co_return;
}