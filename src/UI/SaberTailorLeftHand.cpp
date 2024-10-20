#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SettingsFlowCoordinator.hpp"
#include "SaberTailorHelper.hpp"
#include "Utils/UIUtils.hpp"
#include "HMUI/RangeValuesTextSlider.hpp"
#include "UnityEngine/RectOffset.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorLeftHand);

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace custom_types::Helpers;
using namespace IncrementHelper;
using namespace SaberTailor::UI::LeftHand;
using namespace BSML::Lite;

UnityEngine::UI::VerticalLayoutGroup* leftsabercontainer;

namespace SaberTailor::UI::LeftHand{
    SaberTailor::SaberTailorSuperSlider* posX;
    SaberTailor::SaberTailorSuperSlider* posY;
    SaberTailor::SaberTailorSuperSlider* posZ;
    SaberTailor::SaberTailorSuperSlider* rotX;
    SaberTailor::SaberTailorSuperSlider* rotY;
    SaberTailor::SaberTailorSuperSlider* rotZ;
    void UpdateSliderValues(){
        posX->set_value(GET_VALUE(leftHandPosition).x);
        posY->set_value(GET_VALUE(leftHandPosition).y);
        posZ->set_value(GET_VALUE(leftHandPosition).z);
        rotX->set_value(GET_VALUE(leftHandRotation).x);
        rotY->set_value(GET_VALUE(leftHandRotation).y);
        rotZ->set_value(GET_VALUE(leftHandRotation).z);
        getPlatformHelper()->RefreshControllersReference();
    }
}

void SaberTailor::Views::SaberTailorLeftHand::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    
    if (firstActivation) {
        UIUtils::AddHeader(get_transform(), "Left Hand Settings", Color(0.941f, 0.188f, 0.188f, 1.0f));
        leftsabercontainer = CreateVerticalLayoutGroup(get_transform());
        leftsabercontainer->set_childForceExpandHeight(false);
        leftsabercontainer->set_childControlHeight(true);
        leftsabercontainer->set_spacing(2.8f);
        leftsabercontainer->set_padding(RectOffset::New_ctor(0, 0, 1, 2));
        StartCoroutine(CoroutineHelper::New(CreateSliders()));
    }
    GET_VALUE(currentLeftHandPosition) = Vector3(GET_VALUE(leftHandPosition));
    GET_VALUE(currentLeftHandRotation) = Vector3(GET_VALUE(leftHandRotation));  
}

custom_types::Helpers::Coroutine SaberTailor::Views::SaberTailorLeftHand::CreateSliders(){

    HorizontalLayoutGroup* somebuttons = CreateHorizontalLayoutGroup(leftsabercontainer->get_transform());
    somebuttons->set_childControlWidth(true); somebuttons->set_childForceExpandWidth(false); somebuttons->set_childForceExpandHeight(false);
    somebuttons->set_childControlHeight(true);
    somebuttons->set_childAlignment(TextAnchor::MiddleCenter);
    auto* mirrorRight = CreateUIButton(somebuttons->get_transform(), "Mirror to Right", [](){
        TransferHelper::mirrorToRight();
        SaberTailor::UI::RightHand::UpdateSliderValues();
    });
    AddHoverHint(mirrorRight->get_gameObject(), "Copies the position and rotation values into the right hand");
    mirrorRight->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    auto* exportButton = CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
        TransferHelper::exportToBaseGame(0);
        getPlatformHelper()->RefreshControllersReference();
    });
    AddHoverHint(exportButton->get_gameObject(), "Takes the current left hand settings and copies them into the base game settings");
    exportButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    auto posFormat = [](float val) { return IncrementHelper::formatSliderPosText(val); };
    auto rotFormat = [](float val) { return fmt::format("{0:.0f} deg", val); };

    posX = SaberTailorSuperSlider::CreateSuperSlider(leftsabercontainer->get_transform(), "Position X", -100, 100, GET_VALUE(saberPosIncrement), GET_VALUE(leftHandPosition).x, posFormat, [](float value){
        SET_VALUE(leftHandPosition.x, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    posY = SaberTailorSuperSlider::CreateSuperSlider(leftsabercontainer->get_transform(), "Position Y", -100, 100, GET_VALUE(saberPosIncrement), GET_VALUE(leftHandPosition).y, posFormat, [](float value){
        SET_VALUE(leftHandPosition.y, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    posZ = SaberTailorSuperSlider::CreateSuperSlider(leftsabercontainer->get_transform(), "Position Z", -100, 100, GET_VALUE(saberPosIncrement), GET_VALUE(leftHandPosition).z, posFormat, [](float value){
        SET_VALUE(leftHandPosition.z, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    rotX = SaberTailorSuperSlider::CreateSuperSlider(leftsabercontainer->get_transform(), "Rotation X", -180, 180, GET_VALUE(saberRotIncrement), GET_VALUE(leftHandRotation).x, rotFormat, [](float value){
        int x = std::round(value);
        SET_VALUE(leftHandRotation.x, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    rotY = SaberTailorSuperSlider::CreateSuperSlider(leftsabercontainer->get_transform(), "Rotation Y", -180, 180, GET_VALUE(saberRotIncrement), GET_VALUE(leftHandRotation).y, rotFormat, [](float value){
        int x = std::round(value);
        SET_VALUE(leftHandRotation.y, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    rotZ = SaberTailorSuperSlider::CreateSuperSlider(leftsabercontainer->get_transform(), "Rotation Z", -180, 180, GET_VALUE(saberRotIncrement), GET_VALUE(leftHandRotation).z, rotFormat, [](float value){
        int x = std::round(value);
        SET_VALUE(leftHandRotation.z, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;

    auto* bottomButtons = CreateHorizontalLayoutGroup(leftsabercontainer->get_transform());
    bottomButtons->set_childControlWidth(true); bottomButtons->set_childForceExpandWidth(false); bottomButtons->set_childForceExpandHeight(false);
    bottomButtons->set_childControlHeight(true);
    bottomButtons->set_childAlignment(TextAnchor::MiddleCenter);
    auto* revertButton = CreateUIButton(bottomButtons->get_transform(), "Revert", [](){
        PosRotHelper::revertLeftHand(false);
        UpdateSliderValues();
    });
    AddHoverHint(revertButton->get_gameObject(), "Reverts all of the values back to what they were when the settings were last opened (or to the values of the last profile change)");
    revertButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    auto* resetButton = CreateUIButton(bottomButtons->get_transform(), "Reset Values", [](){
        PosRotHelper::revertLeftHand(true);
        UpdateSliderValues();
    });
    AddHoverHint(resetButton->get_gameObject(), "Resets all of the values to 0");
    resetButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
    co_return;
}