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
    enableSaberTailor->set_isOn(GET_VALUE(isGripModEnabled));
    // enableBaseGameAdjustment->set_isOn(true);
    saberPosIncrement->CurrentValue = GET_VALUE(saberPosIncValue);
    saberRotIncrement->CurrentValue = GET_VALUE(saberRotIncrement);
    incrementUnits->SelectCellWithIdx(GET_VALUE(saberPosIncUnit == "cm" ? 0 : 1));
    std::string s = GET_VALUE(saberPosDisplayUnit);
    displayUnits->SelectCellWithIdx(s == "cm" ? 0 : s == "inches" ? 1 : s == "miles" ? 2 : 3);
    IncrementHelper::SetIncrementText(saberPosIncrement, std::to_string(GET_VALUE(saberPosIncValue)) + " " + GET_VALUE(saberPosIncUnit));
    IncrementHelper::SetIncrementText(saberRotIncrement, std::to_string(GET_VALUE(saberRotIncrement)) + " deg");

    enableSettingsOverride->set_isOn(GET_VALUE(overrideSettingsMethod));
    enableAxisArrows->set_isOn(GET_VALUE(axisEnabled));
    mirrorZRot->set_isOn(GET_VALUE(mirrorZRot));
    axisInReplay->set_isOn(GET_VALUE(axisInReplay));

    enableScaleSettings->set_isOn(GET_VALUE(isSaberScaleModEnabled));
    scaleHitbox->set_isOn(GET_VALUE(saberScaleHitbox));
    length->CurrentValue = GET_VALUE(saberLength);
    width->CurrentValue = GET_VALUE(saberGirth);
    IncrementHelper::SetIncrementText(width, std::to_string(GET_VALUE(saberGirth)) + "%");
    IncrementHelper::SetIncrementText(length, std::to_string(GET_VALUE(saberLength)) + "%");

    enableTrailSettings->set_isOn(GET_VALUE(isTrailModEnabled));
    enableSaberTrails->set_isOn(GET_VALUE(isTrailEnabled));
    duration->CurrentValue = GET_VALUE(trailDuration);
    whitestep->CurrentValue = GET_VALUE(trailWhiteSectionDuration);
    granularity->CurrentValue = GET_VALUE(trailGranularity);
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

        currentConfigName = getMainConfig().currentConfig.GetValue();
        configToCopyFrom = getMainConfig().currentConfig.GetValue();
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

        createModalDropdown = CreateDropdown(createVert->get_transform(), "Select Profile", getMainConfig().currentConfig.GetValue(), configNames, [](StringW thing){
            configToCopyFrom = static_cast<std::string>(thing);
        });

        createModalDropdown->get_transform()->get_gameObject()->SetActive(false);
        createModalDropdown->get_transform()->get_parent()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_alpha(0);

        std::function<void(StringW)> dropdownAction = [](StringW profile){
            currentConfigName = static_cast<std::string>(profile);
            ConfigHelper::LoadConfigFile(profile);
            MirrorConfigValuesToUI();
        };

        auto* profileSelector = CreateDropdown(container->get_transform(), "Select Profile", getMainConfig().currentConfig.GetValue(), configNames, dropdownAction);

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