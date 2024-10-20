#include "AxisDisplay.hpp"
#include "bsml/shared/Helpers/creation.hpp"
#include "bsml/shared/Helpers/getters.hpp"

using namespace UnityEngine;

DEFINE_TYPE(ControllerSettingsHelper, AxisDisplay);

extern bool isReplay;

namespace ControllerSettingsHelper{

    Sprite* getArrowSprite() {
        static SafePtrUnity<Sprite> arrowSprite;
        if (arrowSprite) return arrowSprite.ptr();
        arrowSprite = BSML::Lite::Base64ToSprite(Sprites::arrow);
        return arrowSprite.ptr();
    }

    AxisDisplay* CreateFor(Color colour, Transform* parent, bool useWorldAngles = false){
        auto* axis = GameObject::New_ctor(isReplay ? "AxisDisplayReplay" : "AxisDisplay")->AddComponent<AxisDisplay*>();
        axis->notReplay = !isReplay;
        axis->parent = parent;
        axis->colour = colour;
        axis->useWorldAngles = useWorldAngles;
        return axis;
    }

    void AxisDisplay::CreateAxes(Transform* parent){
        bool isLeft = static_cast<std::string>(parent->get_name()).find("Left") != std::string::npos;
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
            auto* arrC = arr->AddComponent<Canvas*>();
            auto* arrI = arr->AddComponent<HMUI::ImageView*>();
            auto* labelC = label->AddComponent<Canvas*>();
            auto* tmp = BSML::Helpers::CreateText(labelC->get_transform(), axis[i], {0, 0}, {0, 0});
            arrC->set_sortingOrder(31);
            labelC->set_sortingOrder(31);
            arrI->set_color(colour);
            arrI->set_sprite(getArrowSprite());
            arrI->set_material(BSML::Helpers::GetUINoGlowMat());
            arr->get_transform()->SetParent(get_transform(), useWorldAngles);
            tmp->set_text(axis[i]);
            tmp->set_fontSize(notReplay ? 0.3f : 0.45);
            tmp->set_alignment(TMPro::TextAlignmentOptions::Center);
            tmp->set_color(colour);
            label->get_transform()->SetParent(get_transform(), useWorldAngles);
            auto* r = ((RectTransform*)arr->get_transform().ptr());
            auto* r2 = ((RectTransform*)label->get_transform().ptr());

            r->set_pivot({0.49f, -0.1f});
            r->set_sizeDelta({1, 1});
            r->set_anchoredPosition3D({0, 0, 0});
            r->set_localScale(notReplay ? Vector3(0.2f, 0.2f, 0.0f) : Vector3(0.3f, 0.3f, 0.0f));
            r->set_localEulerAngles(rots[i]);
            r->set_localPosition(Vector3::op_Multiply(r->get_up(), (notReplay ? 0.01f : 0.015f)));

            r2->set_localEulerAngles(Vector3::get_one());
            r2->set_localPosition(Vector3::op_Multiply(r->get_up(), (notReplay ? 0.305f : 0.4575f)));
            r2->set_sizeDelta(Vector2::get_zero());
        }
    }

    void AxisDisplay::LateUpdate(){
        if(parent != nullptr) {
            get_transform()->set_position(parent->get_position());
            if(!useWorldAngles) get_transform()->set_rotation(parent->get_rotation());
            for(auto& text : arrowTexts) text->get_transform()->set_rotation(Quaternion::get_identity());
        }
        else if (!parent || !parent->___m_CachedPtr.m_value) OnDispose();
        if(!GET_VALUE(axisEnabled) && notReplay) OnDispose();
    }

    void AxisDisplay::OnDispose(){
        GameObject::Destroy(get_gameObject());
    }
}