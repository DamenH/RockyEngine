#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <stdlib.h>
#include <rlgl.h>


class GraphicsSystem : public SystemBase {
    Camera3D camera;
    float LodScalar = 100.0f;
    Color lodColors[5];
    int TargetFps = 60;
    uint8_t FrameCount = 0;
    float fps = 60;
    Quaternion tempQuat;
    void OnStartup(entt::registry &registry) override
    {
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
    }

    void OnUpdate(entt::registry &registry) override
    {
        int TriangleCount = 0;

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
        int entityCount = 0;
        for (auto entity : transformView)
        {
            auto &transform = transformView.get<TransformComponent>(entity);
            auto &model = transformView.get<ModelComponent>(entity);
            Vector3 pos = (Vector3) {transform.X, transform.Y, transform.Z};
            float distance = Vector3Distance(pos, camera.position);

            ModelSet* modelSet = AssetManager::GetModelSet(model.ModelSetId);
            Vector3 axis;
            float angle;
            QuaternionToAxisAngle(transform.Rotation, &axis, &angle);
            if(entityCount == 0)
            {
                //std::cout << "Angle: " << angle << "\n";
            }
            //rlPushMatrix();
            //Matrix mat = QuaternionToMatrix(transform.Rotation);
            //rlMultMatrixf((float*)&mat);
            if (distance < 0.05f * LodScalar)
            {
                //DrawModel(modelSet->model, pos, 1.0f, lodColors[0]);
                DrawModelEx(modelSet->model, pos, axis, angle, Vector3{1.0f, 1.0f, 1.0f}, lodColors[0]);
                TriangleCount += modelSet->model.meshes[0].triangleCount;
                //rlPopMatrix();
            }
            else if (distance < 0.25f * LodScalar)
            {
                //DrawModel(modelSet->modelLod1, pos, 1.0f, lodColors[1]);
                DrawModelEx(modelSet->modelLod1, pos, axis, angle, Vector3{1.0f, 1.0f, 1.0f}, lodColors[1]);
                TriangleCount += modelSet->modelLod1.meshes[0].triangleCount;
                //rlPopMatrix();
            }
            else if (distance < 2.0f * LodScalar)
            {
                //DrawModel(modelSet->modelLod2, pos, 1.0f, lodColors[2]);
                DrawModelEx(modelSet->modelLod2, pos, axis, angle, Vector3{1.0f, 1.0f, 1.0f}, lodColors[2]);
                TriangleCount += modelSet->modelLod2.meshes[0].triangleCount;
                //rlPopMatrix();
            }
            else
            {
                //rlPopMatrix();
                DrawBillboard(camera, modelSet->Billboard, pos, 7.0f, lodColors[3]);
                TriangleCount += 2;
            }

            entityCount++;
        }

        DrawGrid(10, 1.0f);
        EndMode3D();

        EndDrawing();

        fps = ((3.0f * fps) + GetFPS()) / 4.0f;

        FrameCount++;
        if(FrameCount == 0)
        {
            std::cout << "\n";
            std::cout << "     FPS: " << fps << "\n";
            std::cout << "    Tris: " << TriangleCount << "\n";
            std::cout << "LOD Bias: " << LodScalar << "\n";
        }
        if(FrameCount % 2 == 0)
        {
            if (fps < TargetFps - 2)
            {
                LodScalar *= 0.999f;
            }
            if (fps > TargetFps)
            {
                LodScalar *= 1.01f;
            }
        }
    }
};