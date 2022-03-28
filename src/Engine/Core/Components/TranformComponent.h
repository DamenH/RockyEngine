#pragma once

#include <raylib.h>

struct TransformComponent {
    Vector3 Translation;
    Vector3 Rotation;
    Vector3 Scale;
    Matrix Transform;
};
