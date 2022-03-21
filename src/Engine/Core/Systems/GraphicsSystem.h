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
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"


class GraphicsSystem : public SystemBase {
    Camera3D camera;
    float LodBias = 100.0f;
    int TargetFps = 60;
    uint8_t FrameCount = 0;
    float fps = 60;
    Shader shader;

    void OnStartup(entt::registry &registry) override
    {
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

            registry.emplace<TransformComponent>(entity,
                                                 Vector3{x,y,z},
                                                 Vector3{0.0,0.0,0.0},
                                                 Vector3{1.0,1.0,1.0});
            registry.emplace<ModelComponent>(entity, 0);
        }

        shader = LoadShader(TextFormat("../media/Shaders/base_lighting_instanced.vs", 330),
                                   TextFormat("../media/Shaders/lighting.fs", 330));
        // Get some shader locations
        shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
        shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");

        // Ambient light level
        int ambientLoc = GetShaderLocation(shader, "ambient");
        float ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
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

        int MAX_INSTANCES = transformView.size_hint();
        Matrix *transforms = (Matrix*)malloc(MAX_INSTANCES*sizeof(Matrix));   // Pre-multiplied transformations passed to rlgl




        CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50.0f, 50.0f, 0.0f }, Vector3Zero(), WHITE, shader);

        // NOTE: We are assigning the intancing shader to material.shader
        // to be used on mesh drawing with DrawMeshInstanced()
        Material material = AssetManager::GetModelSet(0)->model.materials[0];
        material.shader = shader;

        int entityCount = 0;
        for (auto entity : transformView)
        {
            auto &transform = transformView.get<TransformComponent>(entity);

            transform.Rotation.x += 0.00001f * (entityCount>>1);
            //transform.Rotation.y += 0.01f;
            //transform.Rotation.z += 0.02f;


            transforms[entityCount] = MatrixMultiply(MatrixRotateXYZ(transform.Rotation),
                                                     MatrixTranslate(transform.Translation.x,
                                                                     transform.Translation.y,
                                                                     transform.Translation.z));

            entityCount++;
        }



        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawMeshInstanced(AssetManager::GetModelSet(0)->modelLod2.meshes[0], material, transforms, MAX_INSTANCES);

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
            std::cout << "LOD Bias: " << LodBias << "\n";
        }
        if(FrameCount % 2 == 0)
        {
            if (fps < TargetFps - 2)
            {
                LodBias *= 0.999f;
            }
            if (fps > TargetFps)
            {
                LodBias *= 1.01f;
            }
        }
    }
};