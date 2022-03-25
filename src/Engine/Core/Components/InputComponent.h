#pragma once

#include <raylib.h>

struct InputComponent {
    bool ForwardPressed;
    bool BackPressed;
    bool LeftPressed;
    bool RightPressed;

    Vector2 MouseDelta;
};

