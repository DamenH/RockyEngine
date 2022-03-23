#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>

class TransformSystem : public SystemBase {

    void OnStartup(entt::registry &registry) override {
    }

    void OnUpdate(entt::registry &registry) override {

        // TODO understand why this is called a view and not a filter
        auto transformView = registry.view<ModelComponent, TransformComponent>();

        for (auto entity: transformView) {
            auto &transform = transformView.get<TransformComponent>(entity);
        }
    }
};