#include "UI/SettingsFlowCoordinator.hpp"
#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SaberTailorRightHand.hpp"
#include "SaberTailorHelper.hpp"

DEFINE_TYPE(SaberTailor, SettingsFlowCoordinator);

void SaberTailor::SettingsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    using namespace HMUI;
    
    if (firstActivation) {
        SetTitle(ID, ViewController::AnimationType::Out);
    }
    if (SaberTailorMain::saberTailorMainSettingsPage == nullptr) SaberTailorMain::saberTailorMainSettingsPage = QuestUI::BeatSaberUI::CreateViewController<SaberTailor::Views::SettingsViewController*>();
    if (SaberTailorMain::saberTailorLeftHand == nullptr) SaberTailorMain::saberTailorLeftHand = QuestUI::BeatSaberUI::CreateViewController<SaberTailor::Views::SaberTailorLeftHand*>();
    if (SaberTailorMain::saberTailorRightHand == nullptr) SaberTailorMain::saberTailorRightHand = QuestUI::BeatSaberUI::CreateViewController<SaberTailor::Views::SaberTailorRightHand*>();
    settingsViewController = SaberTailorMain::saberTailorMainSettingsPage;
    leftHand = SaberTailorMain::saberTailorLeftHand;
    rightHand = SaberTailorMain::saberTailorRightHand;
    
    showBackButton = true;

    ProvideInitialViewControllers(settingsViewController, leftHand, rightHand, nullptr, nullptr);
}

void SaberTailor::SettingsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController){
    parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    
}