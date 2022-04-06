#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/Components/InstanceBinComponent.h"
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/Systems/VisibilitySystem.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <rlgl.h>

struct AsteroidComponent
{
};

class AsteroidSystem : public SystemBase
{
    Camera3D camera;
    uint8_t FrameCount = 0;

#ifdef RENDER_TO_TEXTURE
    bool RenderToTexture = true;
    RenderTexture2D renderTex;
    Rectangle sourceRect;
    Rectangle destRect;
    Vector2 Origin;
#endif

    bool DebugGuiActive = false;

    void OnStartup(entt::registry &registry) override
    {
        std::cout << "Generating scene\n";
        float roidCount = 10000.0f;             // Number of asteroids
        float scalar = log(roidCount) * 150.0f; // Scale range of locations for even density
        for (int i = 0; i < roidCount; i++)
        {
            auto entity = registry.create();
            // Attach a Transform
            registry.emplace<TransformComponent>(entity, TransformComponent{
                                                             Vector3{
                                                                 ((rand() - rand()) / (float)RAND_MAX) * scalar,
                                                                 ((rand() - rand()) / (float)RAND_MAX) * scalar * 0.05f,
                                                                 ((rand() - rand()) / (float)RAND_MAX) * scalar},
                                                             Vector3{
                                                                 ((rand() - rand()) / (float)RAND_MAX) * 2 * PI,
                                                                 ((rand() - rand()) / (float)RAND_MAX) * 2 * PI,
                                                                 ((rand() - rand()) / (float)RAND_MAX) * 2 * PI},
                                                             Vector3{
                                                                 0.5f + (1.0f * rand() / (float)RAND_MAX),
                                                                 0.5f + (1.0f * rand() / (float)RAND_MAX),
                                                                 0.5f + (1.0f * rand() / (float)RAND_MAX)
                                                                 },
                                                             MatrixIdentity()});
            // Attach a Model
            std::vector<int> meshIds = {0, 1, 2, 3};
            int materialId = 4;
            registry.emplace<ModelComponent>(entity, ModelComponent{
                                                         meshIds,
                                                         materialId});

            // Attach a Visibility
            registry.emplace<VisibilityComponent>(entity, VisibilityComponent{});
            registry.emplace<AsteroidComponent>(entity, AsteroidComponent{});
        }
    }

    void OnUpdate(entt::registry &registry) override
    {
        auto view = registry.view<TransformComponent, AsteroidComponent>();

        for (auto entity : view)
        {
            auto &transform = view.get<TransformComponent>(entity);

            transform.Rotation.x += 0.01f;
            transform.Rotation.y += 0.03f;
            transform.Rotation.z += 0.001f;
        }
    }
};