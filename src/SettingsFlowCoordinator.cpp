#include "SettingsFlowCoordinator.hpp"
#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "SaberTailorLeftHand.hpp"
#include "SaberTailorRightHand.hpp"
#include "SaberTailorHelper.hpp"

DEFINE_TYPE(SaberTailor, SettingsFlowCoordinator);

void SaberTailor::SettingsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    using namespace HMUI;
    
    if (firstActivation) {
        SetTitle(ID, ViewController::AnimationType::Out);
    }
    if (SaberTailorMain::saberTailorMainSettingsPage == nullptr) SaberTailorMain::saberTailorMainSettingsPage = QuestUI::BeatSaberUI::CreateViewController<SaberTailor::Views::SettingsViewController*>();
    settingsViewController = SaberTailorMain::saberTailorMainSettingsPage;
    isInUse = true;
    showBackButton = true;
    settingsViewController->flowCoordinator = this;
    currentViewController = nullptr;
    ProvideInitialViewControllers(settingsViewController, nullptr, nullptr, nullptr, nullptr);
}

void SaberTailor::SettingsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController){
    isInUse = false;
    parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    
}

void SaberTailor::SettingsFlowCoordinator::Update(){
    if (SaberTailorMain::config.okButtonPressed && isInUse){
        SaberTailorMain::config.okButtonPressed = false;
        using namespace HMUI;

        if (currentViewController){
            SetTitle(ID, ViewController::AnimationType::In);
            ReplaceTopViewController(settingsViewController, this, this, nullptr, ViewController::AnimationType::Out, ViewController::AnimationDirection::Horizontal);
            currentViewController = nullptr;
        } 
    }
}