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

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;
template<class T>
using IReadOnlyList = System::Collections::Generic::IReadOnlyList_1<T>;

HMUI::SimpleTextDropdown* createModalDropdown;
HMUI::InputFieldView* createModalInput;
UI::Toggle* copyConfigToggle;
std::string currentConfigName = "";
std::string newConfigName = "";
std::string configToCopyFrom;
std::vector<StringW> configNames;

void MirrorConfigValuesToUI(){
    using namespace SaberTailor::Tabs::MainSettings;
    using namespace SaberTailor::Tabs::Scale;
    using namespace SaberTailor::Tabs::Trail;
    using namespace SaberTailor::Tabs::CSH;
    enableSaberTailor->set_isOn(SaberTailorMain::config.isEnabled);
    enableBaseGameAdjustment->set_isOn(true);
    saberPosIncrement->CurrentValue = SaberTailorMain::config.saberPosIncMultiplier;
    saberRotIncrement->CurrentValue = SaberTailorMain::config.saberRotIncrement;
    incrementUnits->SelectCellWithIdx(SaberTailorMain::config.saberPosIncUnit == "cm" ? 0 : 1);
    std::string s = SaberTailorMain::config.saberPosDisplayValue;
    displayUnits->SelectCellWithIdx(s == "cm" ? 0 : s == "inches" ? 1 : s == "miles" ? 2 : 3);
    IncrementHelper::SetIncrementText(saberPosIncrement, std::to_string(SaberTailorMain::config.saberPosIncMultiplier) + " " + SaberTailorMain::config.saberPosIncUnit);
    IncrementHelper::SetIncrementText(saberRotIncrement, std::to_string(SaberTailorMain::config.saberRotIncrement) + " deg");

    enableSettingsOverride->set_isOn(SaberTailorMain::config.overrideSettingsMethod);
    enableAxisArrows->set_isOn(SaberTailorMain::config.spawnAxisDisplay);
    mirrorZRot->set_isOn(SaberTailorMain::config.mirrorZRot);

    enableScaleSettings->set_isOn(SaberTailorMain::config.enableScaleSettings);
    scaleHitbox->set_isOn(SaberTailorMain::config.scaleHitbox);
    length->CurrentValue = SaberTailorMain::config.saberLength;
    width->CurrentValue = SaberTailorMain::config.saberGirth;
    IncrementHelper::SetIncrementText(width, std::to_string(SaberTailorMain::config.saberGirth) + "%");
    IncrementHelper::SetIncrementText(length, std::to_string(SaberTailorMain::config.saberLength) + "%");

    enableTrailSettings->set_isOn(SaberTailorMain::config.enableTrailSettings);
    enableSaberTrails->set_isOn(SaberTailorMain::config.enableSaberTrails);
    duration->CurrentValue = SaberTailorMain::config.trailDuration;
    whitestep->CurrentValue = SaberTailorMain::config.whitestepDuration;
    granularity->CurrentValue = SaberTailorMain::config.trailGranularity;
    IncrementHelper::SetIncrementText(duration, IncrementHelper::Round((float)SaberTailorMain::config.trailDuration/1000.0f, 1) + " s");
    IncrementHelper::SetIncrementText(whitestep, IncrementHelper::Round((float)SaberTailorMain::config.whitestepDuration/1000.0f, 1) + " s");
    SaberTailor::UI::RightHand::UpdateSliderValues();
    SaberTailor::UI::LeftHand::UpdateSliderValues();
    updatePosIncrements(SaberTailorMain::config.saberPosIncrement);
    updateRotIncrements(SaberTailorMain::config.saberRotIncrement);
    SaberTailorMain::config.currentRightHandPosition = Vector3(SaberTailorMain::config.rightHandPosition);
    SaberTailorMain::config.currentRightHandRotation = Vector3(SaberTailorMain::config.rightHandRotation);
    SaberTailorMain::config.currentLeftHandPosition = Vector3(SaberTailorMain::config.leftHandPosition);
    SaberTailorMain::config.currentLeftHandRotation = Vector3(SaberTailorMain::config.leftHandRotation);
}

IReadOnlyList<StringW>* CreateConfigNameList(std::vector<StringW>* names){
    List<StringW>* list = List<StringW>::New_ctor();
    for (auto& configName : *names) list->Add(configName);
    return reinterpret_cast<IReadOnlyList<StringW>*>(list);
}

namespace SaberTailor::Tabs::Profile{
    GameObject* CreateProfileSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Profiles");
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);

        currentConfigName = SaberTailorMain::config.currentConfigName;
        configToCopyFrom = SaberTailorMain::config.currentConfigName;
        configNames = ConfigHelper::GetExistingConfigs();

        auto* createModal = CreateModal(container->get_transform(), UnityEngine::Vector2(60, 40), [](HMUI::ModalView *modal) {}, true);
        auto* deleteModal = CreateModal(container->get_transform(), UnityEngine::Vector2(60, 40), [](HMUI::ModalView *modal) {}, true);

        auto* deleteVert = CreateVerticalLayoutGroup(deleteModal->get_transform());
        deleteVert->get_gameObject()->GetComponentInChildren<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(45.0f);
        deleteVert->set_childControlWidth(true);
        auto* deleteText = CreateText(deleteVert->get_transform(), "Are you sure you want to delete Deez Nuts?");
        deleteText->set_enableWordWrapping(true);
        deleteText->set_alignment(TMPro::TextAlignmentOptions::Center);
        auto* deleteHoriz = CreateHorizontalLayoutGroup(deleteVert->get_transform());
        deleteHoriz->set_spacing(4.0f);

        auto* createVert = CreateVerticalLayoutGroup(createModal->get_transform());
        createVert->get_gameObject()->GetComponentInChildren<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(45.0f);

        createModalInput = CreateStringSetting(createVert->get_transform(), "Profile Name", "", [](std::string name){
            newConfigName = name;
        });

        copyConfigToggle = CreateToggle(createVert->get_transform(), "Copy from existing Config?", false, [](bool value){
            createModalDropdown->get_transform()->get_gameObject()->SetActive(value);
            createModalDropdown->get_transform()->get_parent()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_alpha((int)value);
        });

        createModalDropdown = CreateDropdown(createVert->get_transform(), "Select Profile", SaberTailorMain::config.currentConfigName, configNames, [](StringW thing){
            configToCopyFrom = static_cast<std::string>(thing);
        });

        createModalDropdown->get_transform()->get_gameObject()->SetActive(false);
        createModalDropdown->get_transform()->get_parent()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_alpha(0);

        std::function<void(StringW)> dropdownAction = [](StringW profile){
            currentConfigName = static_cast<std::string>(profile);
            ConfigHelper::LoadConfigFile(profile);
            MirrorConfigValuesToUI();
        };

        auto* profileSelector = CreateDropdown(container->get_transform(), "Select Profile", SaberTailorMain::config.currentConfigName, configNames, dropdownAction);

        auto* confirmDeleteButton = CreateUIButton(deleteHoriz->get_transform(), "Yes", [deleteModal, profileSelector, dropdownAction](){
            ConfigHelper::DeleteFile(currentConfigName);
            configNames = ConfigHelper::GetExistingConfigs();
            auto* readOnlyList = CreateConfigNameList(&configNames);
            profileSelector->SelectCellWithIdx(0);
            profileSelector->SetTexts(readOnlyList);
            dropdownAction("Default");
            createModalDropdown->SelectCellWithIdx(0);
            createModalDropdown->SetTexts(readOnlyList);
            deleteModal->Hide(true, nullptr);
        });

        auto* createHoriz = CreateHorizontalLayoutGroup(createVert->get_transform());
        createHoriz->set_spacing(4.0f);

        CreateUIButton(createHoriz->get_transform(), "Create", [createModal, profileSelector, dropdownAction](){
            std::replace(newConfigName.begin(), newConfigName.end(), ' ', '_');
            if (newConfigName != "" && !ConfigHelper::HasConfigWithName(newConfigName)){
                bool copyingConfig = copyConfigToggle->get_isOn();
                ConfigHelper::CreateNewConfigFile(newConfigName, copyingConfig ? configToCopyFrom : "");
                configNames = ConfigHelper::GetExistingConfigs();
                auto* readOnlyList = CreateConfigNameList(&configNames);
                int cellIndex = -1;
                for (int i=0; i<configNames.size(); i++) if((configNames)[i] == newConfigName) cellIndex = i;
                cellIndex = cellIndex == -1 ? 0 : cellIndex;
                profileSelector->SelectCellWithIdx(cellIndex);
                profileSelector->SetTexts(readOnlyList);
                dropdownAction(newConfigName);
                createModalDropdown->SelectCellWithIdx(cellIndex);
                createModalDropdown->SetTexts(readOnlyList);
                createModalInput->clearSearchButton->get_onClick()->Invoke();
                configToCopyFrom = "";
                createModal->Hide(true, nullptr);
            }
        });

        CreateUIButton(createHoriz->get_transform(), "Cancel", [createModal](){
            createModal->Hide(true, nullptr);
        });

        CreateUIButton(deleteHoriz->get_transform(), "No", [deleteModal](){
            deleteModal->Hide(true, nullptr);
        });

        CreateUIButton(container->get_transform(), "Create New Profile", [createModal](){
            createModal->Show(true, true, nullptr);
            auto cb = createModal->blockerGO->get_gameObject()->GetComponent<UnityEngine::Canvas*>();
            auto cm = createModal->get_gameObject()->GetComponent<UnityEngine::Canvas*>();
            cm->set_overrideSorting(true); 
            cm->set_sortingOrder(33);
            cb->set_sortingOrder(32);
        });

        CreateUIButton(container->get_transform(), "Delete Profile", [deleteModal, deleteText, confirmDeleteButton](){
            deleteText->SetText("Are you sure you want to delete the config '" + currentConfigName + "'?");
            confirmDeleteButton->set_interactable(currentConfigName == "Default" ? false : true);
            deleteModal->Show(true, true, nullptr);
        });

        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}