#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/InputComponent.h"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <cstdint>
#include <iostream>

class InputSystem : public SystemBase {

    void OnStartup(entt::registry &registry) override {
        auto entity = registry.create();
        registry.emplace<InputComponent>(entity, false, false, false, false, 0, 0);
    }

    void OnUpdate(entt::registry &registry) override {
        // Get key states
        const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);

        // Get relative mouse deltas
        int32_t MouseDeltaX, MouseDeltaY;
        SDL_GetRelativeMouseState(&MouseDeltaX, &MouseDeltaY);

        // TODO understand why this is called a view and not a filter
        auto inputView = registry.view<InputComponent>();

        auto &input = inputView.get<InputComponent>(inputView[0]);

        input.ForwardPressed = !!keyStates[SDL_SCANCODE_W];
        input.BackPressed = !!keyStates[SDL_SCANCODE_S];
        input.LeftPressed = !!keyStates[SDL_SCANCODE_A];
        input.RightPressed = !!keyStates[SDL_SCANCODE_D];

        input.MouseDeltaX = MouseDeltaX;
        input.MouseDeltaY = MouseDeltaY;

    }
};


