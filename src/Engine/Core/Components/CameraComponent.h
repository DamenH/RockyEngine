#pragma once
#include <raylib.h>

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
};


