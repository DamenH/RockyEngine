#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/Components/InstanceBinComponent.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/Systems/VisibilitySystem.h"
#include "Engine/Core/Systems/GraphicsSystem.h"
#include "Engine/Utilities/Profiler.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <rlgl.h>

struct AsteroidComponent
{
    Vector3 Rotation;
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
        float roidCount = 50000.0f;             // Number of asteroids
        #ifndef NDEBUG
        roidCount = 5000.0f;             // Number of asteroids
        #endif
        float scalar = log(roidCount) * 150.0f; // Scale range of locations for even density
        for (int i = 0; i < roidCount; i++)
        {
            auto asteroidEntity = registry.create();
            // Attach a Transform
            registry.emplace<TransformComponent>(asteroidEntity, TransformComponent{
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
                                                                         0.5f + (1.0f * rand() / (float)RAND_MAX)},
                                                                     MatrixIdentity()});
            // Attach a Model
            std::vector<int> meshIds = {0, 1, 2, 3};
            int materialId = 4;
            registry.emplace<ModelComponent>(asteroidEntity, ModelComponent{
                                                                 meshIds,
                                                                 materialId});

            // Attach a Visibility
            registry.emplace<VisibilityComponent>(asteroidEntity, VisibilityComponent{});
            registry.emplace<AsteroidComponent>(asteroidEntity, AsteroidComponent{
                                                                    Vector3{
                                                                        ((rand() - rand()) / (float)RAND_MAX) * 0.01f,
                                                                        ((rand() - rand()) / (float)RAND_MAX) * 0.01f,
                                                                        ((rand() - rand()) / (float)RAND_MAX) * 0.01f}});
        }

        auto cameraEntity = registry.create();
        registry.emplace<TransformComponent>(cameraEntity, TransformComponent{
                                                                     Vector3{0.0f, 10.0f, 0.0f},
                                                                     Vector3{0,0,0},
                                                                     Vector3{1.0, 1.0, 1.0},
                                                                     MatrixIdentity()});
        registry.emplace<CameraComponent>(cameraEntity, CameraComponent{
                                                            60.0f,
                                                            Matrix(),
                                                            RenderTarget{
                                                                Rectangle{0, 0, 1920, -1080},
                                                                Rectangle{0, 0, 1920, 1080},
                                                                Vector2{0, 0},
                                                                0.0f,
                                                                WHITE}});
    }

    void OnUpdate(entt::registry &registry) override
    {
        auto profTimeStamp = Profiler::Start("AsteroidSystem", registry);
        auto view = registry.view<TransformComponent, AsteroidComponent>();

        for (auto entity : view)
        {
            auto &transform = view.get<TransformComponent>(entity);
            auto &asteroid = view.get<AsteroidComponent>(entity);

            transform.Rotation.x += asteroid.Rotation.x;
            transform.Rotation.y += asteroid.Rotation.y;
            transform.Rotation.z += asteroid.Rotation.z;
        }

        Profiler::Stop(profTimeStamp, registry);
    }
};