#include "UI/Tabs/MainSettings.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"
#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SaberTailorRightHand.hpp"

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;

namespace SaberTailor::Tabs::MainSettings{

    QuestUI::IncrementSetting* saberPosIncrement;
    QuestUI::IncrementSetting* saberRotIncrement;
    UnityEngine::UI::Toggle* enableSaberTailor;
    UnityEngine::UI::Toggle* enableBaseGameAdjustment;
    HMUI::SimpleTextDropdown* incrementUnits;
    HMUI::SimpleTextDropdown* displayUnits;

    void updatePosIncrements(int inc){
        SaberTailor::UI::LeftHand::posX->increment = inc;
        SaberTailor::UI::LeftHand::posY->increment = inc;
        SaberTailor::UI::LeftHand::posZ->increment = inc;
        SaberTailor::UI::RightHand::posX->increment = inc;
        SaberTailor::UI::RightHand::posY->increment = inc;
        SaberTailor::UI::RightHand::posZ->increment = inc;
    }

    void updateRotIncrements(int inc){
        SaberTailor::UI::LeftHand::rotX->increment = inc;
        SaberTailor::UI::LeftHand::rotY->increment = inc;
        SaberTailor::UI::LeftHand::rotZ->increment = inc;
        SaberTailor::UI::RightHand::rotX->increment = inc;
        SaberTailor::UI::RightHand::rotY->increment = inc;
        SaberTailor::UI::RightHand::rotZ->increment = inc;  
    }

    void updatePosDisplayValues(){
        IncrementHelper::SetSliderPosText(SaberTailor::UI::RightHand::posX, GET_VALUE(rightHandPosition).x);
        IncrementHelper::SetSliderPosText(SaberTailor::UI::RightHand::posY, GET_VALUE(rightHandPosition).y);
        IncrementHelper::SetSliderPosText(SaberTailor::UI::RightHand::posZ, GET_VALUE(rightHandPosition).z);
        IncrementHelper::SetSliderPosText(SaberTailor::UI::LeftHand::posX, GET_VALUE(leftHandPosition).x);
        IncrementHelper::SetSliderPosText(SaberTailor::UI::LeftHand::posY, GET_VALUE(leftHandPosition).y);
        IncrementHelper::SetSliderPosText(SaberTailor::UI::LeftHand::posZ, GET_VALUE(leftHandPosition).z);
    }

    GameObject* CreateMainSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Saber Tailor");
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);

        enableSaberTailor = CreateToggle(container->get_transform(), "Enable Saber Tailor Grip Adjustment", GET_VALUE(isGripModEnabled), [](bool value) {
                SET_VALUE(isGripModEnabled, value);
            });
            AddHoverHint(enableSaberTailor->get_gameObject(), "Turn on Saber Tailor Override");

        // enableBaseGameAdjustment = CreateToggle(container->get_transform(), "Enable Base Game Adjustment Mode", true, [](bool value){
        // });
        // enableBaseGameAdjustment->set_enabled(false);
        // AddHoverHint(enableBaseGameAdjustment->get_gameObject(), "No Male, this toggle does not work. It does nothing. It's purely there to pretend like I am making a full port of the PC version of Saber Tailor. Thank you for your understanding");

        incrementUnits = CreateDropdown(container->get_transform(), "Position Increment Units", GET_VALUE(saberPosIncUnit), {"cm", "mm"}, [](StringW inc){
            std::string unit = static_cast<std::string>(inc);
            SET_VALUE(saberPosIncUnit, unit);
            SET_VALUE(saberPosIncrement, GET_VALUE(saberPosIncValue) * (GET_VALUE(saberPosIncUnit) == "cm" ? 10 : 1));
            IncrementHelper::SetIncrementText(saberPosIncrement, std::to_string(GET_VALUE(saberPosIncValue)) + " " + GET_VALUE(saberPosIncUnit));
            updatePosIncrements(SaberTailorMain::config.currentConfig.saberPosIncrement);
        });
        AddHoverHint(incrementUnits->get_gameObject(), "Changes whether a single buttom press increments the position values in millimetres or centimetres");

        saberPosIncrement = CreateIncrementSetting(container->get_transform(), "Saber Position Increment", 0, 1, GET_VALUE(saberPosIncValue), 1, 10, [](float value){
            SET_VALUE(saberPosIncValue, (int)value);
            SET_VALUE(saberPosIncrement, GET_VALUE(saberPosIncValue) * (GET_VALUE(saberPosIncUnit) == "cm" ? 10 : 1));
            IncrementHelper::SetIncrementText(saberPosIncrement, std::to_string(GET_VALUE(saberPosIncValue)) + " " + GET_VALUE(saberPosIncUnit));
            updatePosIncrements(GET_VALUE(saberPosIncrement));
        });
        AddHoverHint(saberPosIncrement->get_gameObject(), "Changes how much each buttom press increments the position values");

        saberRotIncrement = CreateIncrementSetting(container->get_transform(), "Saber Rotation Increment", 0, 1, GET_VALUE(saberRotIncrement), 1, 10, [](float value){
            SET_VALUE(saberRotIncrement, (int)value);
            IncrementHelper::SetIncrementText(saberRotIncrement, std::to_string(GET_VALUE(saberRotIncrement)) + " deg");
            updateRotIncrements(value);
        });
        AddHoverHint(saberRotIncrement->get_gameObject(), "Changes how much each buttom press increments the rotation values");

        displayUnits = CreateDropdown(container->get_transform(), "Saber Pos. Display Unit", GET_VALUE(saberPosDisplayUnit), {"cm", "inches", "miles", "nmi"}, [](StringW inc){
            std::string unit = static_cast<std::string>(inc);
            SET_VALUE(saberPosDisplayUnit, unit);
            updatePosDisplayValues();
        });
        AddHoverHint(displayUnits->get_gameObject(), "Changes what unit of measurement is displayed for the position values (this does not affect the incrementing)");

        if (SaberTailorMain::config.isAprilFools) AddHoverHint(CreateToggle(container->get_transform(), "Disable April Fools Funny", !SaberTailorMain::config.isAprilFools, [](bool value) {
            SaberTailorMain::config.isAprilFools = !value;
        } )->get_gameObject(), "not sorry LMAO");

        AddHoverHint(CreateUIButton(container->get_transform(), "Import Base Game Settings", [](){
            TransferHelper::importFromBaseGame(0);
            TransferHelper::importFromBaseGame(1);
            SaberTailor::UI::RightHand::UpdateSliderValues();
            SaberTailor::UI::LeftHand::UpdateSliderValues();
        })->get_gameObject(), "Copies the current base game controller settings values into the currently loaded config");
        

        IncrementHelper::SetIncrementText(saberPosIncrement, std::to_string(GET_VALUE(saberPosIncValue)) + " " + GET_VALUE(saberPosIncUnit));
        IncrementHelper::SetIncrementText(saberRotIncrement, std::to_string(GET_VALUE(saberRotIncrement)) + " deg");

        return TabHelper::AdjustedScrollContainerObject(container, true);
    }
}