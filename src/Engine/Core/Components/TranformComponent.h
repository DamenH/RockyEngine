#pragma once

#include <raylib.h>

struct TransformComponent {
    float X;
    float Y;
    float Z;
    Quaternion Rotation;
};
