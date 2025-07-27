#include "UI/ImportExportModal.hpp"
#include "bsml/shared/BSML.hpp"
#include "assets.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/Helpers/getters.hpp"
#include "BeatSaber/GameSettings/ControllerProfilesModel.hpp"
#include "BeatSaber/GameSettings/ControllerProfile.hpp"
#include "main.hpp"


DEFINE_TYPE(SaberTailor::UI, ImportExportModal);

using namespace UnityEngine;
using namespace BeatSaber::GameSettings;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

void MirrorConfigValuesToUI();

namespace SaberTailor::UI {
    void ImportExportModal::ctor(Transform* parent) {
        INVOKE_CTOR();
        baseGameProfilesController = BSML::Helpers::GetDiContainer()->Resolve<ControllerProfilesSettingsViewController*>();
        selectedIndex = 1;
        BSML::parse_and_construct(IncludedAssets::import_export_modal_bsml, parent, this);
    }

    void ImportExportModal::PostParse() {
        confirmButtonText = confirmButton->get_transform()->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
        dropdownCanvas = dropdown->dropdown->get_transform()->get_parent()->get_gameObject()->AddComponent<UnityEngine::CanvasGroup*>();
        ListW<StringW> profileNames = ListW<StringW>::New();
        for (auto* profile : ListW<ControllerProfile*>(baseGameProfilesController->____profileModel->____profiles)) {
            profileNames.push_back(ControllerProfilesSettingsViewController::GetControllerProfileDisplayName(profile));
        }
        profileNames->RemoveAt(0);
        dropdown->dropdown->SetTexts(*profileNames);
        dropdown->dropdown->add_didSelectCellWithIdxEvent(custom_types::MakeDelegate<System::Action_2<UnityW<HMUI::DropdownWithTableView>, int>*>(std::function([this](UnityW<HMUI::DropdownWithTableView>, int index) {
            this->OnConfigSelected(index);
        })));
    }

    void ImportExportModal::Show(bool isImport) {
        this->isImport = isImport;
        SetSelectionUI();
        confirmButtonText->set_text(isImport ? "Import" : "Export");
        modal->Show(true, true, nullptr);
    }

    void ImportExportModal::SetSelectionUI() {
        syncProgress->set_active(false);
        warningText->get_gameObject()->set_active(true);
        warningText->set_text(fmt::format("Please choose a config to {}", isImport ? "import from" : "export to"));
        confirmButtonsLayout->get_gameObject()->set_active(true);
        closeButtonLayout->get_gameObject()->set_active(false);
        dropdown->dropdown->get_gameObject()->get_transform()->get_parent()->get_gameObject()->set_active(true);
        dropdownCanvas->set_alpha(1.0f);
    }

    void ImportExportModal::SetSyncingUI() {
        syncProgress->set_active(true);
        warningText->get_gameObject()->set_active(false);
        confirmButtonsLayout->get_gameObject()->set_active(false);
        closeButtonLayout->get_gameObject()->set_active(false);
        dropdown->dropdown->get_gameObject()->get_transform()->get_parent()->get_gameObject()->set_active(false);
        dropdownCanvas->set_alpha(0.0f);
    }

    void ImportExportModal::SetCompletedUI() {
        syncProgress->set_active(false);
        warningText->get_gameObject()->set_active(true);
        auto s1 = isImport ? "imported" : "exported";
        auto s2 = isImport ? "from the base game" : "to the base game";
        warningText->set_text(fmt::format("Successfully {} config {}", s1, s2));
        confirmButtonsLayout->get_gameObject()->set_active(false);
        closeButtonLayout->get_gameObject()->set_active(true);
        dropdown->dropdown->get_gameObject()->get_transform()->get_parent()->get_gameObject()->set_active(true);
        dropdownCanvas->set_alpha(0.0f);
    }

    void ImportExportModal::ConfirmModal() {
        SetSyncingUI();
        isImport ? ImportFromBaseGame() : ExportToBaseGame();
    }

    void ImportExportModal::Cancel() {
        modal->Hide(true, nullptr);
    }

    void ImportExportModal::OnConfigSelected(int index) {
        selectedIndex = index+1;
    }

    static UnityEngine::Vector3 operator*(UnityEngine::Vector3 a, float_t b) { return UnityEngine::Vector3::op_Multiply(a,b); }
    static UnityEngine::Vector3 operator/(UnityEngine::Vector3 a, float_t b) { return UnityEngine::Vector3::op_Division(a,b); }

    task_coroutine<void> ImportExportModal::ImportFromBaseGame() {
        co_await baseGameProfilesController->____profileModel->LoadAsync();
        auto profile = ListW<ControllerProfile*>(baseGameProfilesController->____profileModel->____profiles)[selectedIndex];
        SET_VALUE(leftHandPosition, profile->get_leftController().position * 1000);
        SET_VALUE(leftHandRotation, profile->get_leftController().rotation);
        SET_VALUE(rightHandPosition, profile->get_rightController().position * 1000);
        SET_VALUE(rightHandRotation, profile->get_rightController().rotation);
        SET_VALUE(offsetApplicationMethod, profile->get_alternativeHandling() ? ApplicationMethod::PreUnityUpdate : ApplicationMethod::Default);
        co_await Task::Delay(400); // deliberate delay lol
        co_await YieldMainThread();
        baseGameProfilesController->____profileModel->____vrPlatformHelper->RefreshControllersReference();
        MirrorConfigValuesToUI();
        SetCompletedUI();
    }

    task_coroutine<void> ImportExportModal::ExportToBaseGame() {
        co_await baseGameProfilesController->____profileModel->LoadAsync();
        auto profile = ListW<ControllerProfile*>(baseGameProfilesController->____profileModel->____profiles)[selectedIndex];
        auto leftController = profile->get_leftController();
        auto rightController = profile->get_rightController();
        leftController.position = GET_VALUE(leftHandPosition) / 1000;
        leftController.rotation = GET_VALUE(leftHandRotation);
        rightController.position = GET_VALUE(rightHandPosition) / 1000;
        rightController.rotation = GET_VALUE(rightHandRotation);
        profile->set_leftController(leftController);
        profile->set_rightController(rightController);
        profile->set_alternativeHandling(GET_VALUE(offsetApplicationMethod) == ApplicationMethod::PreUnityUpdate);
        co_await baseGameProfilesController->____profileModel->SaveAsync();
        co_await Task::Delay(400); // deliberate delay lol
        co_await YieldMainThread();
        baseGameProfilesController->____profileModel->____vrPlatformHelper->RefreshControllersReference();
        SetCompletedUI();
    }
}