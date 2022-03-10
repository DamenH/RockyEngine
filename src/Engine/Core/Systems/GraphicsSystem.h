#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/MeshComponent.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <iostream>

class GraphicsSystem : public SystemBase
{
    Camera3D camera;

    void OnStartup(entt::registry &registry) override
    {
        InitWindow(800, 600, "Rocky Engine - Raylib Edition");
        SetTargetFPS(60);

        camera = {0};
        camera.position = (Vector3){10.0f, 10.0f, 10.0f}; // Camera position
        camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at point
        camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
        camera.fovy = 45.0f;                              // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;           // Camera mode type
        SetCameraMode(camera, CAMERA_FIRST_PERSON);       // Set a free camera mode
        
        std::srand(0);
        int roidCount = 100;
        for (int i = 0; i < roidCount; i++)
        {
            auto entity = registry.create();
            registry.emplace<MeshComponent>(entity);
            float x = ((rand() - rand()) / RAND_MAX) * roidCount;
            float y = ((rand() - rand()) / RAND_MAX) * roidCount;
            float z = ((rand() - rand()) / RAND_MAX) * roidCount * 0.1f;
            std::cout << x;
            registry.emplace<TransformComponent>(entity, x, y, z);
        }
    }

    void OnUpdate(entt::registry &registry) override
    {
        auto transformView = registry.view<TransformComponent, MeshComponent>();

        UpdateCamera(&camera);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        for (auto entity : transformView)
        {
            auto &transform = transformView.get<TransformComponent>(entity);
            Vector3 pos = (Vector3){transform.X, transform.Y, transform.Z};

            DrawCube(pos, 2.0f, 2.0f, 2.0f, RED);
        }

        DrawGrid(100, 10.0f);

        EndMode3D();

        EndDrawing();
    }
};