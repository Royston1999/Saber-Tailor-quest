#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "BeatSaber/GameSettings/ControllerProfilesSettingsViewController.hpp"
#include "UnityEngine/CanvasGroup.hpp"
#include "Utils/TaskCoroutine.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor::UI, ImportExportModal, System::Object) {
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, modal);
    DECLARE_INSTANCE_FIELD(BSML::DropdownListSetting*, dropdown);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, warningText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, syncProgress);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, confirmButtonsLayout);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, closeButtonLayout);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, confirmButton);
    DECLARE_INSTANCE_FIELD_PRIVATE(TMPro::TextMeshProUGUI*, confirmButtonText);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::CanvasGroup*, dropdownCanvas);
    DECLARE_INSTANCE_FIELD_PRIVATE(BeatSaber::GameSettings::ControllerProfilesSettingsViewController*, baseGameProfilesController);

    DECLARE_INSTANCE_METHOD(void, ConfirmModal);
    DECLARE_INSTANCE_METHOD(void, Cancel);
    DECLARE_INSTANCE_METHOD(void, OnConfigSelected, int index);

    DECLARE_INSTANCE_METHOD(void, PostParse);

    DECLARE_INSTANCE_METHOD(void, Show, bool isImport);
    DECLARE_INSTANCE_METHOD(void, SetSelectionUI);
    DECLARE_INSTANCE_METHOD(void, SetSyncingUI);
    DECLARE_INSTANCE_METHOD(void, SetCompletedUI);

    DECLARE_CTOR(ctor, UnityEngine::Transform* parent);
    DECLARE_SIMPLE_DTOR();

    public:
    bool isImport;
    int selectedIndex;
    task_coroutine<void> ImportFromBaseGame();
    task_coroutine<void> ExportToBaseGame();
};