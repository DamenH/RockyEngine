#pragma once

#include <cstdint>

struct InputComponent {
    bool ForwardPressed;
    bool BackPressed;
    bool LeftPressed;
    bool RightPressed;

    int32_t MouseDeltaX;
    int32_t MouseDeltaY;
};

