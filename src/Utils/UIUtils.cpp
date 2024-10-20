#include "Utils/UIUtils.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

#include "VRUIControls/VRGraphicRaycaster.hpp"

#include "HMUI/TitleViewController.hpp"

using namespace VRUIControls;
using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace BSML::Lite;
using namespace BSML;
using namespace TMPro;

namespace UIUtils
{
    TextMeshProUGUI* AddHeader(Transform* parent, std::string title, Color color)
    {
        Color otherColor = color;
        otherColor.a = 0.0f;
        return AddHeader(parent, title, color, otherColor);
    }

    TextMeshProUGUI* AddHeader(Transform* parent, std::string title, Color leftColor, Color rightColor)
    {

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
        background->ApplyBackground("title-gradient");
        background->ApplyAlpha(1.0f);

        ImageView* imageView = background->get_gameObject()->GetComponentInChildren<ImageView*>();
        imageView->____gradient = true;
        imageView->____gradientDirection = 0;
        imageView->set_color(Color::get_white());
        imageView->set_color0(leftColor);
        imageView->set_color1(rightColor);
        imageView->____curvedCanvasSettingsHelper->Reset();
        return text;
    }
}