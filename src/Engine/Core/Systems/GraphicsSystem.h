#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/AssetManager.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <rlgl.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

class GraphicsSystem : public SystemBase
{
    Camera3D camera;
    uint8_t FrameCount = 0;
    Shader shader;

    std::vector<int> models;
    std::vector<std::vector<Matrix>> transformArrays;

    void OnStartup(entt::registry &registry) override
    {
        camera = {0};
        camera.position = (Vector3){0.0f, 10.0f, 0.0f}; // Camera position
        camera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
        camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
        camera.fovy = 60.0f;                            // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;         // Camera mode type
        SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set a free camera mode

        std::cout << "Generating scene\n";
        float roidCount = 10000.0f;
        float scalar = log(roidCount) * 150.0f;
        for (int i = 0; i < roidCount; i++)
        {
            auto entity = registry.create();
            float x = ((rand() - rand()) / (float)RAND_MAX) * scalar;
            float y = ((rand() - rand()) / (float)RAND_MAX) * scalar * 0.05f;
            float z = ((rand() - rand()) / (float)RAND_MAX) * scalar;

            registry.emplace<TransformComponent>(entity,
                                                 Vector3{x, y, z},
                                                 Vector3{0.0, 0.0, 0.0},
                                                 Vector3{1.0, 1.0, 1.0});
            registry.emplace<ModelComponent>(entity, 0);
            registry.emplace<VisibilityComponent>(entity, 0);
        }

        shader = LoadShader(TextFormat("../media/Shaders/base_lighting_instanced.vs", 330),
                            TextFormat("../media/Shaders/lighting.fs", 330));
        // Get some shader locations
        shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
        shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");

        // Ambient light level
        int ambientLoc = GetShaderLocation(shader, "ambient");
        float ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
        SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    }

    void OnUpdate(entt::registry &registry) override
    {
        int TriangleCount = 0;

        auto transformView = registry.view<TransformComponent, ModelComponent, VisibilityComponent>();

        auto cameraView = registry.view<CameraComponent, TransformComponent>();
        for (auto cameraEntity : cameraView)
        {
            auto &cameraTransform = cameraView.get<TransformComponent>(cameraEntity);
            // TODO Update RayLib camera transform
            UpdateCamera(&camera);
        }

        CreateLight(LIGHT_DIRECTIONAL, (Vector3){50.0f, 50.0f, 0.0f}, Vector3Zero(), WHITE, shader);

        // NOTE: We are assigning the intancing shader to material.shader
        // to be used on mesh drawing with DrawMeshInstanced()
        Material material = AssetManager::GetModel(0)->materials[0];
        material.shader = shader;

        models.clear();
        for (int i = 0; i < transformArrays.size(); i++)
        {
            transformArrays[i].clear();
        }
        //transformArrays.clear();

        int entityCount = 0;
        for (auto entity : transformView)
        {
            auto &visibility = transformView.get<VisibilityComponent>(entity);
            if (visibility.Level >= 0)
            {

                auto &transform = transformView.get<TransformComponent>(entity);
                auto &model = transformView.get<ModelComponent>(entity);

                // Update Transform (Temporary code, Userland concern)
                transform.Rotation.x += 0.00001f * (entityCount >> 1);
                // transform.Rotation.y += 0.01f;
                // transform.Rotation.z += 0.02f;

                // Calculate the view matrix
                Matrix transformMatrix = MatrixMultiply(MatrixRotateXYZ(transform.Rotation),
                                                        MatrixTranslate(transform.Translation.x,
                                                                        transform.Translation.y,
                                                                        transform.Translation.z));

                // Identify the model that should be rendered
                int modelId = AssetManager::GetModelSet(model.ModelSetId)->modelLod2;
                if (visibility.Level == 0)
                {
                    modelId = AssetManager::GetModelSet(model.ModelSetId)->model;
                }
                if (visibility.Level == 1)
                {
                    modelId = AssetManager::GetModelSet(model.ModelSetId)->modelLod1;
                }
                if (visibility.Level == 2)
                {
                    modelId = AssetManager::GetModelSet(model.ModelSetId)->modelLod2;
                }
                // Bin transform based on Model
                std::vector<int>::iterator bin = std::find(models.begin(), models.end(), modelId);
                if (bin != models.end())
                {
                    transformArrays[std::distance(models.begin(), bin)].push_back(transformMatrix);
                }
                else
                {
                    models.push_back(modelId);
                    std::vector<Matrix> newBin;
                    transformArrays.push_back(newBin);
                    newBin.push_back(transformMatrix);
                }

                entityCount++;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        // DrawMeshInstanced(AssetManager::GetModel(0)->meshes[0], material, transforms, entityCount);
        // std::cout << "Drawing "<< models.size() <<" batches\n";
        for (int i = 0; i < models.size(); i++)
        {
            Matrix *array = transformArrays[i].data();
            // std::cout << "Drawing batch " << i << ", model " << models[i] << ", " << transformArrays[i].size() << " entities";
            DrawMeshInstanced(AssetManager::GetModel(models[i])->meshes[0], material, array, transformArrays[i].size());
            // std::cout << ".\n";
        }
        // std::cout << "Done drawing batches\n";

        DrawGrid(10, 1.0f);
        EndMode3D();

        EndDrawing();

        FrameCount++;
        if (FrameCount == 0)
        {
            std::cout << "\n";
            std::cout << "Entities: " << entityCount << "\n";
            std::cout << " Batches: " << models.size() << "\n";
            for (int i = 0; i < models.size(); i++)
            {
                std::cout << "\tID: " << i << ",\t" << transformArrays[i].size() << "\n";
            }
            std::cout << "     FPS: " << GetFPS() << "\n";
            std::cout << "    Tris: " << TriangleCount << "\n";
        }
    }
};