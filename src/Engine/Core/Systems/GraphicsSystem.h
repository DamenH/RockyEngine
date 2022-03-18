#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <stdlib.h>


class GraphicsSystem : public SystemBase {
    Camera3D camera;
    float LodScalar;
    Color lodColors[5];

    void OnStartup(entt::registry &registry) override {
        //SetTargetFPS(144);

        camera = {0};
        camera.position = (Vector3) {0.0f, 10.0f, 0.0f}; // Camera position
        camera.target = (Vector3) {0.0f, 0.0f, 0.0f};    // Camera looking at point
        camera.up = (Vector3) {0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
        camera.fovy = 60.0f;                            // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;         // Camera mode type
        SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set a free camera mode

        std::cout << "Generating scene\n";
        float roidCount = 10000.0f;
        float scalar = log(roidCount) * 150.0f;
        for (int i = 0; i < roidCount; i++) {
            auto entity = registry.create();
            float x = ((rand() - rand()) / (float) RAND_MAX) * scalar;
            float y = ((rand() - rand()) / (float) RAND_MAX) * scalar * 0.05f;
            float z = ((rand() - rand()) / (float) RAND_MAX) * scalar;

            registry.emplace<TransformComponent>(entity, x, y, z);
            registry.emplace<ModelComponent>(entity, 0);
        }

        LodScalar = 100.0f;
    }

    void OnUpdate(entt::registry &registry) override {
        auto transformView = registry.view<TransformComponent, ModelComponent>();

        auto cameraView = registry.view<CameraComponent, TransformComponent>();
        for (auto cameraEntity: cameraView) {
            auto &cameraTransform = cameraView.get<TransformComponent>(cameraEntity);
            // TODO Update RayLib camera transform
            UpdateCamera(&camera);
        }


        BeginDrawing();

        ClearBackground(GREEN);

        BeginMode3D(camera);

        if (IsKeyDown(KEY_SPACE)) {
            lodColors[0] = (Color) {255, 255, 255, 255}; //WHITE
            lodColors[1] = (Color) {0, 255, 64, 255}; //GREEN
            lodColors[2] = (Color) {255, 255, 0, 255}; //YELLOW
            lodColors[3] = (Color) {255, 64, 0, 255}; // RED
        } else {
            lodColors[0] = (Color) {255, 255, 255, 255};
            lodColors[1] = (Color) {255, 255, 255, 255};
            lodColors[2] = (Color) {255, 255, 255, 255};
            lodColors[3] = (Color) {255, 255, 255, 255};
        }

        for (auto entity: transformView) {
            auto &transform = transformView.get<TransformComponent>(entity);
            auto &model = transformView.get<ModelComponent>(entity);
            Vector3 pos = (Vector3) {transform.X, transform.Y, transform.Z};
            float distance = Vector3Distance(pos, camera.position);

            ModelSet *modelSet = AssetManager::GetModelSet(model.ModelSetId);

            if (distance < 0.05f * LodScalar) {
                DrawModel(modelSet->model, pos, 1.0f, lodColors[0]);
            } else if (distance < 0.25f * LodScalar) {
                DrawModel(modelSet->modelLod1, pos, 1.0f, lodColors[1]);
            } else if (distance < 2.0f * LodScalar) {
                DrawModel(modelSet->modelLod2, pos, 1.0f, lodColors[2]);
            } else {
                DrawBillboard(camera, modelSet->Billboard, pos, 5.0f, lodColors[3]);
            }
        }

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();

        float fps = GetFPS();
        if (fps < 60) {
            LodScalar *= 0.999f;
        }
        if (fps > 60) {
            LodScalar *= 1.001f;
        }
    }
};