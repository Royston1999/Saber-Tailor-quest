#include "UI/SaberTailorRightHand.hpp"
#include "UI/SettingsFlowCoordinator.hpp"
#include "SaberTailorHelper.hpp"
#include "Utils/UIUtils.hpp"
#include "UnityEngine/RectOffset.hpp"

DEFINE_TYPE(SaberTailor::Views, SaberTailorRightHand);

UnityEngine::UI::VerticalLayoutGroup* rightsabercontainer;

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace custom_types::Helpers;
using namespace IncrementHelper;
using namespace SaberTailor::UI::RightHand;
using namespace BSML::Lite;

namespace SaberTailor::UI::RightHand{
    SaberTailor::SaberTailorSuperSlider* posX;
    SaberTailor::SaberTailorSuperSlider* posY;
    SaberTailor::SaberTailorSuperSlider* posZ;
    SaberTailor::SaberTailorSuperSlider* rotX;
    SaberTailor::SaberTailorSuperSlider* rotY;
    SaberTailor::SaberTailorSuperSlider* rotZ;
    void UpdateSliderValues(){
        posX->set_value(GET_VALUE(rightHandPosition).x);
        posY->set_value(GET_VALUE(rightHandPosition).y);
        posZ->set_value(GET_VALUE(rightHandPosition).z);
        rotX->set_value(GET_VALUE(rightHandRotation).x);
        rotY->set_value(GET_VALUE(rightHandRotation).y);
        rotZ->set_value(GET_VALUE(rightHandRotation).z);
        getPlatformHelper()->RefreshControllersReference();
    }
}

void SaberTailor::Views::SaberTailorRightHand::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {

    if (firstActivation) {
        UIUtils::AddHeader(get_transform(), "Right Hand Settings", UnityEngine::Color(0.188f, 0.620f, 1.0f, 1.0f));
        rightsabercontainer = CreateVerticalLayoutGroup(get_transform());
        rightsabercontainer->set_childForceExpandHeight(false);
        rightsabercontainer->set_childControlHeight(true);
        rightsabercontainer->set_spacing(2.8f);
        rightsabercontainer->set_padding(RectOffset::New_ctor(0, 0, 1, 2));
        StartCoroutine(CoroutineHelper::New(CreateSliders()));
    }
    GET_VALUE(currentRightHandPosition) = Vector3(GET_VALUE(rightHandPosition));
    GET_VALUE(currentRightHandRotation) = Vector3(GET_VALUE(rightHandRotation));
}

custom_types::Helpers::Coroutine SaberTailor::Views::SaberTailorRightHand::CreateSliders(){

    HorizontalLayoutGroup* somebuttons = CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
    somebuttons->set_childControlWidth(true); somebuttons->set_childForceExpandWidth(false); somebuttons->set_childForceExpandHeight(false);
    somebuttons->set_childControlHeight(true);
    somebuttons->set_childAlignment(TextAnchor::MiddleCenter);
    auto* mirrorLeft = CreateUIButton(somebuttons->get_transform(), "Mirror to Left Hand", [](){
        TransferHelper::mirrorToLeft();
        SaberTailor::UI::LeftHand::UpdateSliderValues();
    });
    AddHoverHint(mirrorLeft->get_gameObject(), "Copies the position and rotation values into the left hand");
    mirrorLeft->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(90.0f);

    auto posFormat = [](float val) { return IncrementHelper::formatSliderPosText(val); };
    auto rotFormat = [](float val) { return fmt::format("{0:.0f} deg", val); };

    posX = SaberTailorSuperSlider::CreateSuperSlider(rightsabercontainer->get_transform(), "Position X", -500, 500, GET_VALUE(saberPosIncrement), GET_VALUE(rightHandPosition).x, posFormat, [](float value){
        SET_VALUE(rightHandPosition.x, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    posY = SaberTailorSuperSlider::CreateSuperSlider(rightsabercontainer->get_transform(), "Position Y", -500, 500, GET_VALUE(saberPosIncrement), GET_VALUE(rightHandPosition).y, posFormat, [](float value){
        SET_VALUE(rightHandPosition.y, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    posZ = SaberTailorSuperSlider::CreateSuperSlider(rightsabercontainer->get_transform(), "Position Z", -500, 500, GET_VALUE(saberPosIncrement), GET_VALUE(rightHandPosition).z, posFormat, [](float value){
        SET_VALUE(rightHandPosition.z, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    rotX = SaberTailorSuperSlider::CreateSuperSlider(rightsabercontainer->get_transform(), "Rotation X", -180, 180, GET_VALUE(saberRotIncrement), GET_VALUE(rightHandRotation).x, rotFormat, [](float value){
        int x = std::round(value);
        SET_VALUE(rightHandRotation.x, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    rotY = SaberTailorSuperSlider::CreateSuperSlider(rightsabercontainer->get_transform(), "Rotation Y", -180, 180, GET_VALUE(saberRotIncrement), GET_VALUE(rightHandRotation).y, rotFormat, [](float value){
        int x = std::round(value);
        SET_VALUE(rightHandRotation.y, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;
    rotZ = SaberTailorSuperSlider::CreateSuperSlider(rightsabercontainer->get_transform(), "Rotation Z", -180, 180, GET_VALUE(saberRotIncrement), GET_VALUE(rightHandRotation).z, rotFormat, [](float value){
        int x = std::round(value);
        SET_VALUE(rightHandRotation.z, (int)std::round(value));
        getPlatformHelper()->RefreshControllersReference();
    });
    co_yield nullptr;

    auto* bottomButtons = CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
    bottomButtons->set_childControlWidth(true); bottomButtons->set_childForceExpandWidth(false); bottomButtons->set_childForceExpandHeight(false);
    bottomButtons->set_childControlHeight(true);
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
    co_return;
}