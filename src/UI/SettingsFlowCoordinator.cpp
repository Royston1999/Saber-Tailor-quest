#include "UI/SettingsFlowCoordinator.hpp"
#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SaberTailorRightHand.hpp"
#include "bsml/shared/Helpers/creation.hpp"

DEFINE_TYPE(SaberTailor, SettingsFlowCoordinator);

void SaberTailor::SettingsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    
    if (!firstActivation) return;
    
    SetTitle("Saber Tailor", HMUI::ViewController::AnimationType::Out);
    
    auto main = BSML::Helpers::CreateViewController<SaberTailor::Views::SettingsViewController*>();
    auto leftHand = BSML::Helpers::CreateViewController<SaberTailor::Views::SaberTailorLeftHand*>();
    auto rightHand = BSML::Helpers::CreateViewController<SaberTailor::Views::SaberTailorRightHand*>();

    ____showBackButton = true;

    ProvideInitialViewControllers(main, leftHand, rightHand, nullptr, nullptr);
}

void SaberTailor::SettingsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController){
    ____parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    
}