#pragma once

#include <sstream>
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Canvas.hpp"
#include "HMUI/ImageView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Material.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "SaberTailorConfig.hpp"
#include "main.hpp"
#include <iostream>  
#include <typeinfo>
#include "UnityEngine/Space.hpp"
#include "UnityEngine/Camera.hpp"

DECLARE_CLASS_CODEGEN(ControllerSettingsHelper, AxisDisplay, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, LateUpdate);

    public:
        UnityEngine::Color colour;
        bool useWorldAngles;
        UnityEngine::GameObject* arrows[2][3];
        UnityEngine::GameObject* arrowTexts[2][3];
        UnityEngine::Vector3 therots[3];
        int lefty;
        void Init(UnityEngine::Vector3 rots[], int num, int lefty);
);

namespace ControllerSettingsHelper::Sprites{
    static std::string arrow = "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";
}