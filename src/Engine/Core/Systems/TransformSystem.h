#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/NodeComponent.h"
#include "Engine/Core/Components/TranformComponent.h"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

class TransformSystem : public SystemBase {

    void OnStartup(entt::registry &registry) override {
    }

    void OnUpdate(entt::registry &registry) override {

        // TODO understand why this is called a view and not a filter
        auto transformView = registry.view<NodeComponent, TransformComponent>();

        for (auto entity: transformView) {
            auto &node = transformView.get<NodeComponent>(entity);
            auto &transform = transformView.get<TransformComponent>(entity);

            node.node->setOrientation(transform.rotation);
            node.node->setPosition(
                    Ogre::Vector3(transform.X, transform.Y, transform.Z));

        }
    }
};