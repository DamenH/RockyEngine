#pragma once

#include "Engine/Core/Components/InputComponent.h"
#include "Engine/Core/SystemBase.h"

#include <cstdint>
#include <entt/entt.hpp>
#include <iostream>

class InputSystem : public SystemBase {

    void OnStartup(entt::registry &registry) override {
        auto entity = registry.create();
        registry.emplace<InputComponent>(
                entity,
                InputComponent{
                        false, false, false, false, false, false,
//                        Vector2{0, 0},
//                        Vector2{0, 0},
//                        Vector2{0, 0},
                        0
                });
    }

    void OnUpdate(entt::registry &registry) override {

        // Keys input detection
        // TODO: Input detection is raylib-dependant, it could be moved outside the
        // module

        auto inputView = registry.view<InputComponent>();
        auto inputEntity = inputView.front();
        auto &input = inputView.get<InputComponent>(inputEntity);

        Vector2 previousMousePosition = input.MousePosition;

        input = {
                IsKeyDown('W'),
                IsKeyDown('A'),
                IsKeyDown('S'),
                IsKeyDown('D'),
                IsKeyDown('X'),
                IsKeyDown('Z'),
                GetMousePosition(),
                previousMousePosition,
                {input.MousePosition.x - input.PreviousMousePosition.x,
                 input.MousePosition.y - input.PreviousMousePosition.y},
                GetMouseWheelMove()
        };


//        input.PreviousMousePosition = input.MousePosition;
//
//
//        input.MousePosition = {GetMousePosition()};
//
//
//        input.MousePositionDelta = {input.MousePosition.x - input.PreviousMousePosition.x,
//                                    input.MousePosition.y - input.PreviousMousePosition.y};
//
//        input.MouseWheelMove = GetMouseWheelMove();

    }

};
