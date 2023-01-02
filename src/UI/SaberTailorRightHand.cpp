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

namespace SaberTailor::UI::RightHand{
    SaberTailor::IncrementSlider* posX;
    SaberTailor::IncrementSlider* posY;
    SaberTailor::IncrementSlider* posZ;
    SaberTailor::IncrementSlider* rotX;
    SaberTailor::IncrementSlider* rotY;
    SaberTailor::IncrementSlider* rotZ;
    void UpdateSliderValues(){
        using namespace IncrementHelper;
        posX->sliderComponent->set_value(SaberTailorMain::config.rightHandPosition.x);
        posY->sliderComponent->set_value(SaberTailorMain::config.rightHandPosition.y);
        posZ->sliderComponent->set_value(SaberTailorMain::config.rightHandPosition.z);
        rotX->sliderComponent->set_value(SaberTailorMain::config.rightHandRotation.x);
        rotY->sliderComponent->set_value(SaberTailorMain::config.rightHandRotation.y);
        rotZ->sliderComponent->set_value(SaberTailorMain::config.rightHandRotation.z);
        SetSliderPosText(posX, SaberTailorMain::config.rightHandPosition.x);
        SetSliderPosText(posY, SaberTailorMain::config.rightHandPosition.y);
        SetSliderPosText(posZ, SaberTailorMain::config.rightHandPosition.z);
        rotX->sliderComponent->text->set_text(std::to_string((int)SaberTailorMain::config.rightHandRotation.x) + " deg");
        rotY->sliderComponent->text->set_text(std::to_string((int)SaberTailorMain::config.rightHandRotation.y) + " deg");
        rotZ->sliderComponent->text->set_text(std::to_string((int)SaberTailorMain::config.rightHandRotation.z) + " deg");
    }

    void notTheBestCode(){
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(posX, SaberTailorMain::config.rightHandPosition.x, true)));
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(posY, SaberTailorMain::config.rightHandPosition.y, true)));
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(posZ, SaberTailorMain::config.rightHandPosition.z, true)));
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(rotX, SaberTailorMain::config.rightHandRotation.x, false)));
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(rotY, SaberTailorMain::config.rightHandRotation.y, false)));
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(forceUpdateSliderText(rotZ, SaberTailorMain::config.rightHandRotation.z, false)));  
    }
}

void SaberTailor::Views::SaberTailorRightHand::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {

    if (firstActivation) {
        UIUtils::AddHeader(get_transform(), "Right Hand Settings", UnityEngine::Color(0.188f, 0.620f, 1.0f, 1.0f));
        rightsabercontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(CreateSliders()));
        SaberTailorMain::config.currentRightHandPosition = Vector3(SaberTailorMain::config.rightHandPosition);
        SaberTailorMain::config.currentRightHandRotation = Vector3(SaberTailorMain::config.rightHandRotation);
        return;
    }
    notTheBestCode();
}

custom_types::Helpers::Coroutine SaberTailor::Views::SaberTailorRightHand::CreateSliders(){
    HorizontalLayoutGroup* somebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(rightsabercontainer->get_transform());
    QuestUI::BeatSaberUI::CreateUIButton(somebuttons->get_transform(), "Mirror to Left", [](){
        TransferHelper::mirrorToLeft();
        SaberTailor::UI::LeftHand::UpdateSliderValues();
    });
    QuestUI::BeatSaberUI::CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
        TransferHelper::exportToBaseGame(1);
    });
    posX = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Position X", SaberTailorMain::config.saberPosIncrement, SaberTailorMain::config.rightHandPosition.x, -100, 100, [](float value){
        setVectorObjectValue(SaberTailorMain::config.currentlyLoadedConfig, "GripRightPosition", "x", std::round(value));
        SetSliderPosText(posX, SaberTailorMain::config.rightHandPosition.x);
        getLogger().info("%.1f", posX->increment);
    });
    co_yield nullptr;
    posY = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Position Y", SaberTailorMain::config.saberPosIncrement, SaberTailorMain::config.rightHandPosition.y, -100, 100, [](float value){
        setVectorObjectValue(SaberTailorMain::config.currentlyLoadedConfig, "GripRightPosition", "y", std::round(value));
        SetSliderPosText(posY, SaberTailorMain::config.rightHandPosition.y);
    });
    co_yield nullptr;
    posZ = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Position Z", SaberTailorMain::config.saberPosIncrement, SaberTailorMain::config.rightHandPosition.z, -100, 100, [](float value){
        setVectorObjectValue(SaberTailorMain::config.currentlyLoadedConfig, "GripRightPosition", "z", std::round(value));
        SetSliderPosText(posZ, SaberTailorMain::config.rightHandPosition.z);
    });
    co_yield nullptr;
    rotX = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Rotation X", SaberTailorMain::config.saberRotIncrement, SaberTailorMain::config.rightHandRotation.x, -180, 180, [](float value){
        int x = std::round(value);
        setVectorObjectValue(SaberTailorMain::config.currentlyLoadedConfig, "GripRightRotation", "x", std::round(value));
        rotX->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;
    rotY = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Rotation Y", SaberTailorMain::config.saberRotIncrement, SaberTailorMain::config.rightHandRotation.y, -180, 180, [](float value){
        int x = std::round(value);
        setVectorObjectValue(SaberTailorMain::config.currentlyLoadedConfig, "GripRightRotation", "y", std::round(value));
        rotY->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;
    rotZ = SaberTailor::IncrementSlider::CreateIncrementSlider(rightsabercontainer, "Rotation Z", SaberTailorMain::config.saberRotIncrement, SaberTailorMain::config.rightHandRotation.z, -180, 180, [](float value){
        int x = std::round(value);
        setVectorObjectValue(SaberTailorMain::config.currentlyLoadedConfig, "GripRightRotation", "z", std::round(value));
        rotZ->sliderComponent->text->set_text(std::to_string(x) + " deg");
    });
    co_yield nullptr;

    QuestUI::BeatSaberUI::CreateUIButton(rightsabercontainer->get_transform(), "Revert", [](){
        PosRotHelper::revertRightHand();
        SaberTailor::UI::RightHand::UpdateSliderValues();
    }); 
    posX->increment = getPosIncrement();
    posY->increment = getPosIncrement();
    posZ->increment = getPosIncrement();
    notTheBestCode();
    co_return;
}