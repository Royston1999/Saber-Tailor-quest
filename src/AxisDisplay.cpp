#include "AxisDisplay.hpp"

using namespace UnityEngine;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

DEFINE_TYPE(ControllerSettingsHelper, AxisDisplay);
namespace ControllerSettingsHelper{

    void AxisDisplay::Awake(){
        if (get_transform()->get_gameObject()->get_name() == "ControllerLeft"){
            Vector3 leftrots[3] = {Vector3({90, 0, 90}), Vector3({0,0,0}), Vector3({90, 0, 0})};
            colour = Color::get_red(); useWorldAngles = false; Init(leftrots, 0, -1);
            colour = Color::get_gray(); useWorldAngles = true; Init(leftrots, 1, -1);
            
        }
        if (get_transform()->get_gameObject()->get_name() == "ControllerRight"){
            Vector3 rightrots[3] = {Vector3({90, 0, -90}), Vector3({0,0,0}), Vector3({90, 0, 0})};
            colour = Color::get_red(); useWorldAngles = false; Init(rightrots, 0, 1);
            colour = Color::get_gray(); useWorldAngles = true; Init(rightrots, 1, 1);
        }
    }

    void AxisDisplay::Init(Vector3 rots[], int num, int left){
        std::string axis[3] = {"X", "Y", "Z"};
        lefty = left;
        therots[0] = rots[0];
        therots[1] = rots[1];
        therots[2] = rots[2];
        for (int i = 0; i<3; i++){
            std::string arrowstring = axis[i] + " Arrow";
            std::string labelstring = axis[i] + " Label";
            auto* arr = arrows[num][i] = GameObject::New_ctor(arrowstring);
            auto* label = arrowTexts[num][i] = GameObject::New_ctor(labelstring);
            arr->AddComponent<Canvas*>();
            arr->AddComponent<HMUI::ImageView*>();
            label->AddComponent<Canvas*>();
            label->AddComponent<TMPro::TextMeshProUGUI*>();
            auto* arrI = arr->GetComponentInChildren<HMUI::ImageView*>();
            arr->GetComponentInChildren<Canvas*>()->set_sortingOrder(31);
            label->GetComponentInChildren<Canvas*>()->set_sortingOrder(31);
            arrI->set_color(colour);
            arrI->set_sprite(Base64ToSprite(Sprites::arrow));
            arrI->set_material(ArrayUtil::First(Resources::FindObjectsOfTypeAll<Material*>(), [] (Material* x) { return x->get_name() == "UINoGlow"; }));
            arr->get_transform()->SetParent(get_transform(), useWorldAngles);
            auto* tmp = label->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
            tmp->set_text(axis[i]);
            tmp->set_fontSize(0.3f);
            tmp->set_alignment(TMPro::TextAlignmentOptions::Center);
            tmp->set_color(colour);
            label->get_transform()->SetParent(get_transform(), useWorldAngles);
            auto* r = ((RectTransform*)arr->get_transform());
            auto* r2 = ((RectTransform*)label->get_transform());

            r->set_pivot({0.49f, -0.1f});
            r->set_sizeDelta({1, 1});
            r->set_anchoredPosition3D({0, 0, 0});
            r->set_localScale({0.2f, 0.2f, 0.0f});
            r->set_localEulerAngles(rots[i]);
            r->set_localPosition({0, 0, 0});

            r2->set_sizeDelta({0.0f, 0.0f});
            if (i == 0) r2->set_localPosition({0.3f * lefty, 0.0f, 0.0f});
            if (i == 1) r2->set_localPosition({0.0f, 0.3f, 0.0f});
            if (i == 2) r2->set_localPosition({0.0f, 0.0f, 0.3f});
        }
    }

    void AxisDisplay::Update(){
        if (SaberTailorMain::config.spawnAxisDisplay){
            for (int i = 0; i<3; i++){
                auto* greyarrows = arrows[1][i]->get_transform();
                auto* greytext = arrowTexts[1][i]->get_transform();
                auto* redtext = arrowTexts[0][i]->get_transform();
                greyarrows->set_rotation(Quaternion::get_identity());
                greyarrows->Rotate(therots[i]);
                greytext->set_rotation(Quaternion::get_identity());
                redtext->set_rotation(Quaternion::get_identity());
                if (i == 0) greytext->set_position(get_transform()->get_position() + Vector3(0.3f * lefty, 0.0f, 0.0f));
                if (i == 1) greytext->set_position(get_transform()->get_position() + Vector3(0.0f, 0.3f, 0.0f));
                if (i == 2) greytext->set_position(get_transform()->get_position() + Vector3(0.0f, 0.0f, 0.3f));
            }
        }
    }
    void AxisDisplay::LateUpdate(){
        if(!SaberTailorMain::config.spawnAxisDisplay){
            for (int i = 0; i<3; i++){
                if (arrows[0][i] != nullptr) GameObject::Destroy(arrows[0][i]);
                if (arrows[1][i] != nullptr) GameObject::Destroy(arrows[1][i]);
                if (arrowTexts[0][i] != nullptr) GameObject::Destroy(arrowTexts[0][i]);
                if (arrowTexts[1][i] != nullptr) GameObject::Destroy(arrowTexts[1][i]);
            }
            GameObject::Destroy(GetComponent<AxisDisplay*>());
        }
    }
}