#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/InputComponent.h"

#include <entt/entt.hpp>
#include <cstdint>
#include <iostream>

class InputSystem : public SystemBase {

    void OnStartup(entt::registry &registry) override {
        auto entity = registry.create();
        registry.emplace<InputComponent>(entity, InputComponent{false, false, false, false, 0, 0});
    }

    void OnUpdate(entt::registry &registry) override {


    }
};


