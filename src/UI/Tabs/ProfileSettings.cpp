#include "UI/Tabs/ProfileSettings.hpp"
#include "SaberTailorHelper.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UI/Tabs/MainSettings.hpp"
#include "UI/Tabs/ControllerSettingsHelper.hpp"
#include "UI/Tabs/ScaleSettings.hpp"
#include "UI/Tabs/TrailSettings.hpp"
#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SaberTailorRightHand.hpp"
#include "main.hpp"

using namespace BSML::Lite;
using namespace UnityEngine;
using namespace System::Collections::Generic;
template<class T>
using IReadOnlyList = System::Collections::Generic::IReadOnlyList_1<T>;

BSML::DropdownListSetting* createModalDropdown;
HMUI::InputFieldView* createModalInput;
BSML::ToggleSetting* copyConfigToggle;
std::string currentConfigName = "";
std::string newConfigName = "";
std::string configToCopyFrom;
std::vector<std::string> configNames;

void MirrorConfigValuesToUI(){
    using namespace SaberTailor::Tabs::MainSettings;
    using namespace SaberTailor::Tabs::Scale;
    using namespace SaberTailor::Tabs::Trail;
    using namespace SaberTailor::Tabs::CSH;
    enableSaberTailor->toggle->set_isOn(GET_VALUE(isGripModEnabled));
    // enableBaseGameAdjustment->set_isOn(true);
    saberPosIncrement->currentValue = GET_VALUE(saberPosIncValue);
    saberRotIncrement->currentValue = GET_VALUE(saberRotIncrement);
    incrementUnits->dropdown->SelectCellWithIdx(GET_VALUE(saberPosIncUnit == "cm" ? 0 : 1));
    std::string s = GET_VALUE(saberPosDisplayUnit);
    displayUnits->dropdown->SelectCellWithIdx(s == "cm" ? 0 : s == "inches" ? 1 : s == "miles" ? 2 : 3);
    IncrementHelper::SetIncrementText(saberPosIncrement, std::to_string(GET_VALUE(saberPosIncValue)) + " " + GET_VALUE(saberPosIncUnit));
    IncrementHelper::SetIncrementText(saberRotIncrement, std::to_string(GET_VALUE(saberRotIncrement)) + " deg");

    // enableSettingsOverride->toggle->set_isOn(GET_VALUE(overrideSettingsMethod));
    offsetApplicationMethod->dropdown->SelectCellWithIdx((int)GET_VALUE(offsetApplicationMethod));
    enableAxisArrows->toggle->set_isOn(GET_VALUE(axisEnabled));
    mirrorZRot->toggle->set_isOn(GET_VALUE(mirrorZRot));
    axisInReplay->toggle->set_isOn(GET_VALUE(axisInReplay));

    enableScaleSettings->toggle->set_isOn(GET_VALUE(isSaberScaleModEnabled));
    scaleHitbox->toggle->set_isOn(GET_VALUE(saberScaleHitbox));
    length->currentValue = GET_VALUE(saberLength);
    width->currentValue = GET_VALUE(saberGirth);
    IncrementHelper::SetIncrementText(width, std::to_string(GET_VALUE(saberGirth)) + "%");
    IncrementHelper::SetIncrementText(length, std::to_string(GET_VALUE(saberLength)) + "%");

    enableTrailSettings->toggle->set_isOn(GET_VALUE(isTrailModEnabled));
    enableSaberTrails->toggle->set_isOn(GET_VALUE(isTrailEnabled));
    duration->currentValue = GET_VALUE(trailDuration);
    whitestep->currentValue = GET_VALUE(trailWhiteSectionDuration);
    granularity->currentValue = GET_VALUE(trailGranularity);
    IncrementHelper::SetIncrementText(duration, IncrementHelper::Round((float)GET_VALUE(trailDuration)/1000.0f, 1) + " s");
    IncrementHelper::SetIncrementText(whitestep, IncrementHelper::Round((float)GET_VALUE(trailWhiteSectionDuration)/1000.0f, 1) + " s");
    SaberTailor::UI::RightHand::UpdateSliderValues();
    SaberTailor::UI::LeftHand::UpdateSliderValues();
    updatePosIncrements(GET_VALUE(saberPosIncrement));
    updateRotIncrements(GET_VALUE(saberRotIncrement));
    GET_VALUE(currentRightHandPosition) = Vector3(GET_VALUE(rightHandPosition));
    GET_VALUE(currentRightHandRotation) = Vector3(GET_VALUE(rightHandRotation));
    GET_VALUE(currentLeftHandPosition) = Vector3(GET_VALUE(leftHandPosition));
    GET_VALUE(currentLeftHandRotation) = Vector3(GET_VALUE(leftHandRotation));
}

ListW<System::Object*> CreateConfigNamesList(){
    auto list = ListW<System::Object*>::New();
    for (auto& configName : configNames) list->Add(static_cast<System::Object*>(StringW(configName).convert()));
    return list;
}

std::vector<std::string_view> ConfigNamesReference() {
    std::vector<std::string_view> help;
    for (std::string& s : configNames) help.emplace_back(s);
    return help;
}

namespace SaberTailor::Tabs::Profile{
    GameObject* CreateProfileSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Profiles", {0, 0}, {0, 7});
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);

        currentConfigName = getMainConfig().currentConfig.GetValue();
        configToCopyFrom = getMainConfig().currentConfig.GetValue();
        configNames = ConfigHelper::GetExistingConfigs();

        HMUI::ModalView* createModal = CreateModal(container->get_transform(), UnityEngine::Vector2(60, 40), []() {}, true);
        HMUI::ModalView* deleteModal = CreateModal(container->get_transform(), UnityEngine::Vector2(60, 40), []() {}, true);

        auto* deleteVert = CreateVerticalLayoutGroup(deleteModal->get_transform());
        deleteVert->get_gameObject()->GetComponentInChildren<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(45.0f);
        deleteVert->set_childControlWidth(true);
        auto* deleteText = CreateText(deleteVert->get_transform(), "Are you sure you want to delete Deez Nuts?");
        deleteText->set_enableWordWrapping(true);
        deleteText->set_alignment(TMPro::TextAlignmentOptions::Center);
        auto* deleteHoriz = CreateHorizontalLayoutGroup(deleteVert->get_transform());
        // deleteHoriz->set_spacing(4.0f);

        auto* createVert = CreateVerticalLayoutGroup(createModal->get_transform());
        createVert->get_gameObject()->GetComponentInChildren<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(45.0f);

        createModalInput = CreateStringSetting(createVert->get_transform(), "Profile Name", "", [](std::string name){
            newConfigName = name;
        });

        copyConfigToggle = CreateToggle(createVert->get_transform(), "Copy from existing Config?", false, [](bool value){
            createModalDropdown->get_transform()->get_gameObject()->SetActive(value);
            createModalDropdown->get_transform()->get_parent()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_alpha((int)value);
        });

        std::vector<std::string_view> initialNames(ConfigNamesReference());
        createModalDropdown = CreateDropdown(createVert->get_transform(), "Select Profile", getMainConfig().currentConfig.GetValue(), initialNames, [](StringW thing){
            configToCopyFrom = static_cast<std::string>(thing);
        });

        createModalDropdown->get_transform()->get_gameObject()->SetActive(false);
        createModalDropdown->get_transform()->get_parent()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_alpha(0);

        std::function<void(StringW)> dropdownAction = [](StringW profile){
            currentConfigName = static_cast<std::string>(profile);
            ConfigHelper::LoadConfigFile(profile);
            MirrorConfigValuesToUI();
        };

        auto* profileSelector = CreateDropdown(container->get_transform(), "Select Profile", getMainConfig().currentConfig.GetValue(), initialNames, dropdownAction);

        auto* confirmDeleteButton = CreateUIButton(deleteHoriz->get_transform(), "Yes", Vector2(0, 0), {20, 0}, [deleteModal, profileSelector, dropdownAction](){
            ConfigHelper::DeleteFile(currentConfigName);
            configNames = ConfigHelper::GetExistingConfigs();
            profileSelector->dropdown->SelectCellWithIdx(0);
            profileSelector->values = CreateConfigNamesList();
            profileSelector->UpdateChoices();
            dropdownAction(ConfigHelper::GetDefaultConfigName());
            createModalDropdown->dropdown->SelectCellWithIdx(0);
            createModalDropdown->values = CreateConfigNamesList();
            createModalDropdown->UpdateChoices();
            deleteModal->Hide(true, nullptr);
        });

        auto* createHoriz = CreateHorizontalLayoutGroup(createVert->get_transform());
        // createHoriz->set_spacing(4.0f);

        CreateUIButton(createHoriz->get_transform(), "Create", Vector2(0, 0), {20, 0}, [createModal, profileSelector, dropdownAction](){
            std::replace(newConfigName.begin(), newConfigName.end(), ' ', '_');
            if (newConfigName != "" && !ConfigHelper::HasConfigWithName(newConfigName)){
                bool copyingConfig = copyConfigToggle->toggle->get_isOn();
                ConfigHelper::CreateNewConfigFile(newConfigName, copyingConfig ? configToCopyFrom : "");
                configNames = ConfigHelper::GetExistingConfigs();
                int cellIndex = -1;
                for (int i=0; i<configNames.size(); i++) if(configNames[i] == newConfigName) cellIndex = i;
                cellIndex = cellIndex == -1 ? 0 : cellIndex;
                profileSelector->dropdown->SelectCellWithIdx(cellIndex);
                profileSelector->values = CreateConfigNamesList();
                profileSelector->UpdateChoices();
                dropdownAction(newConfigName);
                createModalDropdown->dropdown->SelectCellWithIdx(cellIndex);
                createModalDropdown->values = CreateConfigNamesList();
                createModalDropdown->UpdateChoices();
                createModalInput->____clearSearchButton->get_onClick()->Invoke();
                configToCopyFrom = "";
                createModal->Hide(true, nullptr);
            }
        });

        CreateUIButton(createHoriz->get_transform(), "Cancel", Vector2(0, 0), {20, 0}, [createModal](){
            createModal->Hide(true, nullptr);
        });

        CreateUIButton(deleteHoriz->get_transform(), "No", Vector2(0, 0), {20, 0}, [deleteModal](){
            deleteModal->Hide(true, nullptr);
        });

        CreateUIButton(container->get_transform(), "Create New Profile", [createModal](){
            createModal->Show(true, true, nullptr);
            auto cb = createModal->____blockerGO->get_gameObject()->GetComponent<UnityEngine::Canvas*>();
            auto cm = createModal->get_gameObject()->GetComponent<UnityEngine::Canvas*>();
            cm->set_overrideSorting(true); 
            cm->set_sortingOrder(33);
            cb->set_sortingOrder(32);
        });

        CreateUIButton(container->get_transform(), "Delete Profile", [deleteModal, deleteText, confirmDeleteButton](){
            deleteText->set_text("Are you sure you want to delete the config '" + currentConfigName + "'?");
            confirmDeleteButton->set_interactable(ConfigHelper::toLower(currentConfigName) == "default" ? false : true);
            deleteModal->Show(true, true, nullptr);
        });

        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}