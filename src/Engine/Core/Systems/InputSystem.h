#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/InputComponent.h"

#include <entt/entt.hpp>
#include <cstdint>
#include <iostream>

class InputSystem : public SystemBase {

    void OnStartup(entt::registry &registry) override {
        auto entity = registry.create();
        registry.emplace<InputComponent>(entity, false, false, false, false, 0, 0);
    }

    void OnUpdate(entt::registry &registry) override {

        auto inputView = registry.view<InputComponent>();

        auto &input = inputView.get<InputComponent>(inputView[0]);

        input.ForwardPressed = IsKeyDown('W');
        input.BackPressed = IsKeyDown('S');
        input.LeftPressed = IsKeyDown('A');
        input.RightPressed = IsKeyDown('D');

        input.MouseDelta = GetMouseDelta();

    }
};


