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
    float FoVy;             // Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
    RenderTarget Target;
    RLFrustum* Frustum;
};


