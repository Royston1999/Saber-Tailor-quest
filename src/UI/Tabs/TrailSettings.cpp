#include "UI/Tabs/TrailSettings.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;

namespace SaberTailor::Tabs::Trail{

    QuestUI::IncrementSetting* duration;
    QuestUI::IncrementSetting* whitestep;
    UnityEngine::UI::Toggle* enableTrailSettings;
    UnityEngine::UI::Toggle* enableSaberTrails;
    QuestUI::IncrementSetting* granularity;

    GameObject* CreateTrailSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Trails");
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);

        enableTrailSettings = CreateToggle(container->get_transform(), "Enable Trail Settings", GET_VALUE(isTrailModEnabled), [](bool value){
            SET_VALUE(isTrailModEnabled, value);
        });
        AddHoverHint(enableTrailSettings->get_gameObject(), "Allow saber tailor to control the saber trail");

        enableSaberTrails = CreateToggle(container->get_transform(), "Enable Saber Trails", GET_VALUE(isTrailEnabled), [](bool value){
            SET_VALUE(isTrailEnabled, value);
        });
        AddHoverHint(enableSaberTrails->get_gameObject(), "Toggles the trail on/off");

        duration = CreateIncrementSetting(container->get_transform(), "Trail Duration", 0, 100, GET_VALUE(trailDuration), 100, 5000, [](float value){
            SET_VALUE(trailDuration, (int)value);
            IncrementHelper::SetIncrementText(duration, IncrementHelper::Round(value/1000.0f, 1) + " s");
        });
        AddHoverHint(duration->get_gameObject(), "Controls the length of the trail");

        granularity = CreateIncrementSetting(container->get_transform(), "Trail Granularity", 0, 1, GET_VALUE(trailGranularity), 10, 200, [](float value){
            SET_VALUE(trailGranularity, (int)value);
        });
        AddHoverHint(granularity->get_gameObject(), "Tweak how granular the trail is");

        whitestep = CreateIncrementSetting(container->get_transform(), "Trail Whitestep Duration", 0, 100, GET_VALUE(trailWhiteSectionDuration), 0, 2000, [](float value){
            SET_VALUE(trailWhiteSectionDuration, (int)value);
            IncrementHelper::SetIncrementText(whitestep, IncrementHelper::Round(value/1000.0f, 1) + " s");
        });
        AddHoverHint(whitestep->get_gameObject(), "Tweak how much of the white section you can see in the saber trail");

        auto* qosmeticsPoo = CreateText(container->get_transform(), "use Qosmetics instead if installed");
        qosmeticsPoo->set_fontSize(4.5f);
        qosmeticsPoo->set_alignment(TMPro::TextAlignmentOptions::Center);
        qosmeticsPoo->set_color(UnityEngine::Color::get_red());

        IncrementHelper::SetIncrementText(duration, IncrementHelper::Round((float)GET_VALUE(trailDuration)/1000.0f, 1) + " s");
        IncrementHelper::SetIncrementText(whitestep, IncrementHelper::Round((float)GET_VALUE(trailWhiteSectionDuration)/1000.0f, 1) + " s");

        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}