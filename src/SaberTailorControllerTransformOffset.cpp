#include "SaberTailorControllerTransformOffset.hpp"
#include "SaberTailorConfig.hpp"
#include "main.hpp"

DEFINE_TYPE(SaberTailor, SaberTailorControllerTransformOffset);

using namespace UnityEngine;

namespace SaberTailor {
    static UnityEngine::Vector3 operator/(Vector3 a, float_t b) { return UnityEngine::Vector3::op_Division(a,b); }

    bool SaberTailorControllerTransformOffset::get_alternativeHandling() {
        return GET_VALUE(offsetApplicationMethod) == SaberTailor::ApplicationMethod::PreUnityUpdate;
    }
    UnityEngine::Vector3 SaberTailorControllerTransformOffset::get_leftPositionOffset() {
        return UnityEngine::Vector3(GET_VALUE(leftHandPosition)) / 1000;
    }
    UnityEngine::Vector3 SaberTailorControllerTransformOffset::get_leftRotationOffset() {
        return GET_VALUE(leftHandRotation);
    }
    UnityEngine::Vector3 SaberTailorControllerTransformOffset::get_positionOffset() {
        return UnityEngine::Vector3::get_zero();
    }
    UnityEngine::Vector3 SaberTailorControllerTransformOffset::get_rightPositionOffset() {
        return UnityEngine::Vector3(GET_VALUE(rightHandPosition)) / 1000;
    }
    UnityEngine::Vector3 SaberTailorControllerTransformOffset::get_rightRotationOffset() {
        return GET_VALUE(rightHandRotation);
    }
    UnityEngine::Vector3 SaberTailorControllerTransformOffset::get_rotationOffset() {
        return UnityEngine::Vector3::get_zero();
    }
}