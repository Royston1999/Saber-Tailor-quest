#pragma once

#include "custom-types/shared/macros.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"

DECLARE_CLASS_CODEGEN(SaberTailor, SaberTailorControllerTransformOffset, GlobalNamespace::VRControllerTransformOffset) {
    DECLARE_OVERRIDE_METHOD_MATCH(bool, get_alternativeHandling, &GlobalNamespace::VRControllerTransformOffset::get_alternativeHandling);
    DECLARE_OVERRIDE_METHOD_MATCH(UnityEngine::Vector3, get_leftPositionOffset, &GlobalNamespace::VRControllerTransformOffset::get_leftPositionOffset);
    DECLARE_OVERRIDE_METHOD_MATCH(UnityEngine::Vector3, get_leftRotationOffset, &GlobalNamespace::VRControllerTransformOffset::get_leftRotationOffset);
    DECLARE_OVERRIDE_METHOD_MATCH(UnityEngine::Vector3, get_positionOffset, &GlobalNamespace::VRControllerTransformOffset::get_positionOffset);
    DECLARE_OVERRIDE_METHOD_MATCH(UnityEngine::Vector3, get_rightPositionOffset, &GlobalNamespace::VRControllerTransformOffset::get_rightPositionOffset);
    DECLARE_OVERRIDE_METHOD_MATCH(UnityEngine::Vector3, get_rightRotationOffset, &GlobalNamespace::VRControllerTransformOffset::get_rightRotationOffset);
    DECLARE_OVERRIDE_METHOD_MATCH(UnityEngine::Vector3, get_rotationOffset, &GlobalNamespace::VRControllerTransformOffset::get_rotationOffset);
};