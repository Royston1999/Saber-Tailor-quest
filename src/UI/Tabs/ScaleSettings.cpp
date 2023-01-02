#include "UI/Tabs/ScaleSettings.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;

namespace SaberTailor::Tabs::Scale{

    QuestUI::IncrementSetting* length;
    QuestUI::IncrementSetting* width;
    UnityEngine::UI::Toggle* enableScaleSettings;
    UnityEngine::UI::Toggle* scaleHitbox;

    GameObject* CreateScaleSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Scale");
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);

        enableScaleSettings = CreateToggle(container->get_transform(), "Enable Scale Settings", SaberTailorMain::config.enableScaleSettings, [](bool value){
            setBool(SaberTailorMain::config.currentlyLoadedConfig, "IsSaberScaleModEnabled", value, false);
        });
        AddHoverHint(enableScaleSettings->get_gameObject(), "Allow saber tailor to control the scale of the sabers");

        length = CreateIncrementSetting(container->get_transform(), "Length (Default 100%)", 0, 5, SaberTailorMain::config.saberLength, 5, 500, [](float value){
            setInt(SaberTailorMain::config.currentlyLoadedConfig, "SaberLength", (int)value, false);
            IncrementHelper::SetIncrementText(length, std::to_string((int)value) + "%");
        });
        AddHoverHint(length->get_gameObject(), "Makes saber longer. (the trail will look funny if you shrink the saber without scaling the hitbox)");

        width = CreateIncrementSetting(container->get_transform(), "Width (Default 100%)", 0, 5, SaberTailorMain::config.saberGirth, 5, 500, [](float value){
            setInt(SaberTailorMain::config.currentlyLoadedConfig, "SaberGirth", (int)value, false);
            IncrementHelper::SetIncrementText(width, std::to_string((int)value) + "%");
        });
        AddHoverHint(width->get_gameObject(), "Makes saber chonkier, or less chonky if you would like");

        scaleHitbox = CreateToggle(container->get_transform(), "Scale Hitbox", SaberTailorMain::config.scaleHitbox, [](bool value){
            setBool(SaberTailorMain::config.currentlyLoadedConfig, "SaberScaleHitbox", value, false);
        });
        AddHoverHint(scaleHitbox->get_gameObject(), "Makes it so the actual saber hitbox matches the size of your scaled saber");

        auto* hitboxOhNo = CreateText(container->get_transform(), "Enabling hitbox scaling will disable score submission.");
        hitboxOhNo->set_fontSize(4.5f);
        hitboxOhNo->set_alignment(TMPro::TextAlignmentOptions::Center);
        hitboxOhNo->set_color(UnityEngine::Color::get_red());

        IncrementHelper::SetIncrementText(width, std::to_string(SaberTailorMain::config.saberGirth) + "%");
        IncrementHelper::SetIncrementText(length, std::to_string(SaberTailorMain::config.saberLength) + "%");

        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}