#include "UI/Tabs/ScaleSettings.hpp"
#include "SaberTailorHelper.hpp"
#include "main.hpp"

using namespace BSML::Lite;
using namespace UnityEngine;

namespace SaberTailor::Tabs::Scale{

    BSML::IncrementSetting* length;
    BSML::IncrementSetting* width;
    BSML::ToggleSetting* enableScaleSettings;
    BSML::ToggleSetting* scaleHitbox;

    GameObject* CreateScaleSettings(Transform* parent){
        auto* container = CreateScrollableSettingsContainer(parent);

        auto* text = CreateText(container->get_transform(), "Scale", {0, 0}, {0, 7});
        text->set_fontSize(6.0f);
        text->set_alignment(TMPro::TextAlignmentOptions::Center);

        enableScaleSettings = CreateToggle(container->get_transform(), "Enable Scale Settings", GET_VALUE(isSaberScaleModEnabled), [](bool value){
            SET_VALUE(isSaberScaleModEnabled, value);
        });
        AddHoverHint(enableScaleSettings->get_gameObject(), "Allow saber tailor to control the scale of the sabers");

        length = CreateIncrementSetting(container->get_transform(), "Length (Default 100%)", 0, 5, GET_VALUE(saberLength), 5, 500, [](float value){
            SET_VALUE(saberLength, (int)value);
            IncrementHelper::SetIncrementText(length, std::to_string((int)value) + "%");
        });
        AddHoverHint(length->get_gameObject(), "Makes saber longer. (the trail will look funny if you shrink the saber without scaling the hitbox)");

        width = CreateIncrementSetting(container->get_transform(), "Width (Default 100%)", 0, 5, GET_VALUE(saberGirth), 5, 500, [](float value){
            SET_VALUE(saberGirth, (int)value);
            IncrementHelper::SetIncrementText(width, std::to_string((int)value) + "%");
        });
        AddHoverHint(width->get_gameObject(), "Makes saber chonkier, or less chonky if you would like");

        scaleHitbox = CreateToggle(container->get_transform(), "Scale Hitbox", GET_VALUE(saberScaleHitbox), [](bool value){
            SET_VALUE(saberScaleHitbox, value);
        });
        AddHoverHint(scaleHitbox->get_gameObject(), "Makes it so the actual saber hitbox matches the size of your scaled saber");

        auto* hitboxOhNo = CreateText(container->get_transform(), "Enabling hitbox scaling will disable score submission.", {0, 0}, {0, 5});
        hitboxOhNo->set_fontSize(4.5f);
        hitboxOhNo->set_alignment(TMPro::TextAlignmentOptions::Center);
        hitboxOhNo->set_color(UnityEngine::Color::get_red());

        IncrementHelper::SetIncrementText(width, std::to_string(GET_VALUE(saberGirth)) + "%");
        IncrementHelper::SetIncrementText(length, std::to_string(GET_VALUE(saberLength)) + "%");

        return TabHelper::AdjustedScrollContainerObject(container, false);
    }
}