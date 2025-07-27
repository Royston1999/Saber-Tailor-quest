#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "SaberTailorSuperSlider.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor::Views, SaberTailorRightHand, HMUI::ViewController) {
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    public:
        custom_types::Helpers::Coroutine CreateSliders();
};

namespace SaberTailor::UI::RightHand{
    extern SaberTailor::SaberTailorSuperSlider* posX;
    extern SaberTailor::SaberTailorSuperSlider* posY;
    extern SaberTailor::SaberTailorSuperSlider* posZ;
    extern SaberTailor::SaberTailorSuperSlider* rotX;
    extern SaberTailor::SaberTailorSuperSlider* rotY;
    extern SaberTailor::SaberTailorSuperSlider* rotZ;
    void UpdateSliderValues();
}
