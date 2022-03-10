#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/MeshComponent.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <iostream>
#include <stdlib.h>

class GraphicsSystem : public SystemBase
{
    Camera3D camera;
    Model model;
    Texture2D texture;

    void OnStartup(entt::registry &registry) override
    {
        InitWindow(800, 600, "Rocky Engine - Raylib Edition");
        SetTargetFPS(60);

        camera = {0};
        camera.position = (Vector3){0.0f, 10.0f, 0.0f}; // Camera position
        camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at point
        camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
        camera.fovy = 75.0f;                              // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;           // Camera mode type
        SetCameraMode(camera, CAMERA_FIRST_PERSON);       // Set a free camera mode
        
        std::cout << "Generating scene";
        float roidCount = 1000.0f;
        for (int i = 0; i < roidCount; i++)
        {
            auto entity = registry.create();
            registry.emplace<MeshComponent>(entity);
            float x = ((rand() - rand()) / (float)RAND_MAX ) * roidCount;
            float y = ((rand() - rand()) / (float)RAND_MAX ) * roidCount * 0.1f;
            float z = ((rand() - rand()) / (float)RAND_MAX ) * roidCount;
            
            registry.emplace<TransformComponent>(entity, x, y, z);
        }

        std::cout << "Loading resources";
        model = LoadModel("media/Asteroid/asteroid_03.obj");
        texture = LoadTexture("media/Asteroid/asteroid_4k.jpg");
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;


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
            DrawModel(model, pos, 1.0f, WHITE);
        }

        DrawGrid(10, 1.0f);

        EndMode3D();

        EndDrawing();
    }
};