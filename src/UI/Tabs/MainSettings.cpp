#include "UI/Tabs/MainSettings.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"
#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SaberTailorRightHand.hpp"
#include "UI/ImportExportModal.hpp"

using namespace BSML::Lite;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace SaberTailor::Tabs::MainSettings{

    BSML::IncrementSetting* saberPosIncrement;
    BSML::IncrementSetting* saberRotIncrement;
    BSML::ToggleSetting* enableSaberTailor;
    BSML::ToggleSetting* enableBaseGameAdjustment;
    BSML::DropdownListSetting* incrementUnits;
    BSML::DropdownListSetting* displayUnits;
    SafePtr<UI::ImportExportModal> importExportModal;

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
        SaberTailor::UI::RightHand::posX->UpdateVisuals();
        SaberTailor::UI::RightHand::posY->UpdateVisuals();
        SaberTailor::UI::RightHand::posZ->UpdateVisuals();
        SaberTailor::UI::LeftHand::posX->UpdateVisuals();
        SaberTailor::UI::LeftHand::posY->UpdateVisuals();
        SaberTailor::UI::LeftHand::posZ->UpdateVisuals();
    }

    GameObject* CreateMainSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Saber Tailor", {0, 0}, {0, 7});
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);

        enableSaberTailor = CreateToggle(container->get_transform(), "Enable Saber Tailor Grip Adjustment", GET_VALUE(isGripModEnabled), [](bool value) {
            SET_VALUE(isGripModEnabled, value);
            getPlatformHelper()->RefreshControllersReference();
        });
        AddHoverHint(enableSaberTailor->get_gameObject(), "Turn on Saber Tailor Override");

        // enableBaseGameAdjustment = CreateToggle(container->get_transform(), "Enable Base Game Adjustment Mode", true, [](bool value){
        // });
        // enableBaseGameAdjustment->set_enabled(false);
        // AddHoverHint(enableBaseGameAdjustment->get_gameObject(), "No Male, this toggle does not work. It does nothing. It's purely there to pretend like I am making a full port of the PC version of Saber Tailor. Thank you for your understanding");
        static std::vector<std::string_view> incUnitOptions = {"cm", "mm"};
        incrementUnits = CreateDropdown(container->get_transform(), "Position Increment Units", GET_VALUE(saberPosIncUnit), incUnitOptions, [](StringW inc){
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

        static std::vector<std::string_view> displayUnitsVals{"cm", "inches", "miles", "nmi"};
        displayUnits = CreateDropdown(container->get_transform(), "Saber Pos. Display Unit", GET_VALUE(saberPosDisplayUnit), displayUnitsVals, [](StringW inc){
            std::string unit = static_cast<std::string>(inc);
            SET_VALUE(saberPosDisplayUnit, unit);
            updatePosDisplayValues();
        });
        AddHoverHint(displayUnits->get_gameObject(), "Changes what unit of measurement is displayed for the position values (this does not affect the incrementing)");

        if (SaberTailorMain::config.isAprilFools) AddHoverHint(CreateToggle(container->get_transform(), "Disable April Fools Funny", !SaberTailorMain::config.isAprilFools, [](bool value) {
            SaberTailorMain::config.isAprilFools = !value;
        } )->get_gameObject(), "not sorry LMAO");

        importExportModal = UI::ImportExportModal::New_ctor(container->get_transform());

        HorizontalLayoutGroup* somebuttons = CreateHorizontalLayoutGroup(container->get_transform());
        somebuttons->set_childControlWidth(true); somebuttons->set_childForceExpandWidth(false); somebuttons->set_childForceExpandHeight(false);
        somebuttons->set_childControlHeight(true);
        somebuttons->set_childAlignment(TextAnchor::MiddleCenter);
        auto* importButton = CreateUIButton(somebuttons->get_transform(), "Import from Base Game", [](){
            importExportModal->Show(true);
        });
        AddHoverHint(importButton->get_gameObject(), "Choose a base game controller settings profile to import into the current saber tailor profile");
        importButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);
        auto* exportButton = CreateUIButton(somebuttons->get_transform(), "Export to Base Game", [](){
            importExportModal->Show(false);
        });
        AddHoverHint(exportButton->get_gameObject(), "Choose a base game controller settings profile to export the current saber tailor profile into");
        exportButton->get_gameObject()->GetComponentInChildren<LayoutElement*>()->set_preferredWidth(44.5f);

        IncrementHelper::SetIncrementText(saberPosIncrement, std::to_string(GET_VALUE(saberPosIncValue)) + " " + GET_VALUE(saberPosIncUnit));
        IncrementHelper::SetIncrementText(saberRotIncrement, std::to_string(GET_VALUE(saberRotIncrement)) + " deg");

        return TabHelper::AdjustedScrollContainerObject(container, true);
    }
}