#pragma once

struct InputComponent {
    bool ForwardPressed;
    bool BackPressed;
    bool LeftPressed;
    bool RightPressed;
    bool UpPressed;
    bool DownPressed;

    Vector2 MousePosition;
    Vector2 PreviousMousePosition;
    Vector2 MousePositionDelta;

    float MouseWheelMove = 0;
};

