#include "AxisDisplay.hpp"

using namespace UnityEngine;

DEFINE_TYPE(ControllerSettingsHelper, AxisDisplay);

extern bool isReplay;

namespace ControllerSettingsHelper{

    Material* UINoGlow;

    AxisDisplay* CreateFor(Color colour, Transform* parent, bool useWorldAngles = false){
        auto* axis = GameObject::New_ctor("AxisDisplay")->AddComponent<AxisDisplay*>();
        axis->notReplay = static_cast<std::string>(parent->get_name()).find("Controller") != std::string::npos;
        axis->parent = parent;
        axis->colour = colour;
        axis->useWorldAngles = useWorldAngles;
        return axis;
    }

    void AxisDisplay::CreateAxes(Transform* parent){
        bool isLeft = static_cast<std::string>(parent->get_name()).find("Left") != std::string::npos;
        UINoGlow = Resources::FindObjectsOfTypeAll<Material*>().FirstOrDefault([] (Material* x) { return x->get_name() == "UINoGlow"; });
        if (isLeft){
            const Vector3 leftRotations[3] = {Vector3({90, 0, 90}), Vector3::get_zero(), Vector3({90, 0, 0})};
            CreateFor(Color::get_red(), parent)->Init(leftRotations);
            CreateFor(Color::get_gray(), parent, true)->Init(leftRotations);
        }
        else {
            CreateFor(Color::get_red(), parent)->Init();
            CreateFor(Color::get_gray(), parent, true)->Init();
        }
    }

    void AxisDisplay::Init(const Vector3 (&rots)[3]){
        std::string axis[3] = {"X", "Y", "Z"};
        for (int i = 0; i<3; i++){
            std::string arrowstring = axis[i] + " Arrow";
            std::string labelstring = axis[i] + " Label";
            auto* arr = arrows[i] = GameObject::New_ctor(arrowstring);
            auto* label = arrowTexts[i] = GameObject::New_ctor(labelstring);
            arr->AddComponent<Canvas*>();
            arr->AddComponent<HMUI::ImageView*>();
            label->AddComponent<Canvas*>();
            label->AddComponent<TMPro::TextMeshProUGUI*>();
            auto* arrI = arr->GetComponentInChildren<HMUI::ImageView*>();
            arr->GetComponentInChildren<Canvas*>()->set_sortingOrder(31);
            label->GetComponentInChildren<Canvas*>()->set_sortingOrder(31);
            arrI->set_color(colour);
            arrI->set_sprite(QuestUI::BeatSaberUI::Base64ToSprite(Sprites::arrow));
            arrI->set_material(UINoGlow);
            arr->get_transform()->SetParent(get_transform(), useWorldAngles);
            auto* tmp = label->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
            tmp->set_text(axis[i]);
            tmp->set_fontSize(notReplay ? 0.3f : 0.45);
            tmp->set_alignment(TMPro::TextAlignmentOptions::Center);
            tmp->set_color(colour);
            label->get_transform()->SetParent(get_transform(), useWorldAngles);
            auto* r = ((RectTransform*)arr->get_transform());
            auto* r2 = ((RectTransform*)label->get_transform());

            r->set_pivot({0.49f, -0.1f});
            r->set_sizeDelta({1, 1});
            r->set_anchoredPosition3D({0, 0, 0});
            r->set_localScale(notReplay ? Vector3(0.2f, 0.2f, 0.0f) : Vector3(0.3f, 0.3f, 0.0f));
            r->set_localEulerAngles(rots[i]);
            r->set_localPosition(r->get_up() * (notReplay ? 0.01f : 0.015f));

            r2->set_localEulerAngles(Vector3::get_one());
            r2->set_localPosition(r->get_up() * (notReplay ? 0.305f : 0.4575f));
            r2->set_sizeDelta(Vector2::get_zero());
        }
    }

    void AxisDisplay::LateUpdate(){
        if(parent != nullptr) {
            get_transform()->set_position(parent->get_transform()->get_position());
            if(!useWorldAngles) get_transform()->set_rotation(parent->get_transform()->get_rotation());
            for(auto& text : arrowTexts) text->get_transform()->set_rotation(Quaternion::get_identity());
        }
        else if (parent == nullptr || parent->m_CachedPtr.m_value == nullptr) OnDispose();
        if(!SaberTailorMain::config.spawnAxisDisplay && notReplay) OnDispose();
    }

    void AxisDisplay::OnDispose(){
        GameObject::Destroy(get_gameObject());
    }
}