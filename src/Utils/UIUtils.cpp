#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace TMPro;

namespace UIUtils{
    TextMeshProUGUI* AddHeader(Transform* parent, std::string title, Color color) {
        Color otherColor = color;
        otherColor.a = 0.0f;
        return AddHeader(parent, title, color, otherColor);
    }

    TextMeshProUGUI* AddHeader(Transform* parent, std::string title, Color leftColor, Color rightColor) {
        VerticalLayoutGroup* vertical = CreateVerticalLayoutGroup(parent);
        vertical->get_rectTransform()->set_anchoredPosition({0.0f, 45.0f});
        HorizontalLayoutGroup* horizontal = CreateHorizontalLayoutGroup(vertical->get_transform());

        TextMeshProUGUI* text = CreateText(horizontal->get_transform(), title);
        text->set_fontSize(text->get_fontSize() * 2.0f);
        text->set_alignment(TextAlignmentOptions::Center);

        LayoutElement* layoutelem = text->get_gameObject()->AddComponent<LayoutElement*>();
        layoutelem->set_preferredHeight(10.0f);
        layoutelem->set_preferredWidth(90.0f);

        Backgroundable* background = horizontal->get_gameObject()->AddComponent<Backgroundable*>();
        background->ApplyBackgroundWithAlpha("title-gradient", 1.0f);

        ImageView* imageView = background->get_gameObject()->GetComponentInChildren<ImageView*>();
        imageView->gradient = true;
        imageView->gradientDirection = 0;
        imageView->set_color(Color::get_white());
        imageView->set_color0(leftColor);
        imageView->set_color1(rightColor);
        imageView->curvedCanvasSettingsHelper->Reset();
        return text;
    }
}