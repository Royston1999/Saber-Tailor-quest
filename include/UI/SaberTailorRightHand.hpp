#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "IncrementSlider.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor::Views, SaberTailorRightHand, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    public:
        custom_types::Helpers::Coroutine CreateSliders();
)

namespace SaberTailor::UI::RightHand{
    extern SaberTailor::IncrementSlider* posX;
    extern SaberTailor::IncrementSlider* posY;
    extern SaberTailor::IncrementSlider* posZ;
    extern SaberTailor::IncrementSlider* rotX;
    extern SaberTailor::IncrementSlider* rotY;
    extern SaberTailor::IncrementSlider* rotZ;
    void UpdateSliderValues();
}
