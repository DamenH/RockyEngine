#pragma once
#include <raylib.h>
#include "RLFrustum.h"

struct RenderTarget
{
    Rectangle SourceRect;
    Rectangle DestRect;
    Vector2 Origin;
    float Angle;
    Color Tint;
};

struct CameraComponent {
    float FoV;
    Matrix ViewProjectionTransform;
    RenderTarget Target;
    RLFrustum* Frustum;
};


