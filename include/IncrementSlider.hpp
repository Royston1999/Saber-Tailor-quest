#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "UnityEngine/MonoBehaviour.hpp"


DECLARE_CLASS_CODEGEN(SaberTailor, IncrementSlider, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(QuestUI::SliderSetting*, sliderComponent);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, leftButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, rightButton);
    DECLARE_INSTANCE_FIELD(float, increment);

    public:
        static SaberTailor::IncrementSlider* CreateIncrementSlider(UnityEngine::UI::VerticalLayoutGroup* vertical, std::string_view text, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange = nullptr); 
)