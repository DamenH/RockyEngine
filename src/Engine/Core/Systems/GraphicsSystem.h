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
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "rlImGui.h"
#include "imgui.h"

#define RENDER_TO_TEXTURE

class GraphicsSystem : public SystemBase
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
        // Camera struct initialization
        // TODO: Eliminate RayLib camera struct
        camera = {0};
        camera.position = (Vector3){0.0f, 10.0f, 0.0f}; // Camera position
        camera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
        camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
        camera.fovy = 60.0f;                            // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;         // Camera mode type
        SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set a free camera mode

#ifdef RENDER_TO_TEXTURE
        destRect.width = 1920;
        destRect.height = 1080;
        destRect.x = 0;
        destRect.y = 0;
        renderTex = LoadRenderTexture(destRect.width, destRect.height);
        sourceRect.width = renderTex.texture.width;
        sourceRect.height = -renderTex.texture.height;
        sourceRect.x = 0;
        sourceRect.y = 0;
#endif
        rlImGuiSetup(true); // sets up ImGui with ether a dark or light default theme
    }

    void OnUpdate(entt::registry &registry) override
    {
        int TriangleCount = 0;
        int entityCount = 0;
        int batchCount = 0;

        BeginDrawing();

#ifdef RENDER_TO_TEXTURE
        if (RenderToTexture)
        {
            BeginTextureMode(renderTex);
            ClearBackground(BLANK);
        }
        else
        {
#endif

            ClearBackground(BLACK);
#ifdef RENDER_TO_TEXTURE
        }
#endif

        BeginMode3D(camera);
        if (VisibilitySystem::UpdateFrustum)
        {
            VisibilitySystem::frustum.Extract();
            VisibilitySystem::CullingCamera = camera;
        }

        auto binsView = registry.view<InstanceBinComponent>();
        for (auto entity : binsView)
        {
            InstanceBinComponent bin = binsView.get<InstanceBinComponent>(entity);
            const Mesh mesh = AssetManager::GetMesh(bin.Mesh);
            const Material material = AssetManager::GetMaterial(bin.Material);
            const int count = bin.InstanceCount;
            Matrix *array = bin.ModelTransforms.data();

            DrawMeshInstanced(mesh, material, array, count);
            TriangleCount += mesh.triangleCount * count;
            entityCount += count;
            if (count > 0)
            {
                batchCount++;
            }
        }

        DrawGrid(10, 1.0f);

        EndMode3D();
#ifdef RENDER_TO_TEXTURE
        if (RenderToTexture)
        {
            EndTextureMode();
            // BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(renderTex.texture, sourceRect, destRect, Origin, 0.0f, WHITE);
        }
#endif

        if (IsKeyReleased(KEY_Q))
        {
            DebugGuiActive = !DebugGuiActive;
        }

        if (DebugGuiActive)
        {
            rlImGuiBegin();
            // bool open = false;
            // ImGui::ShowDemoWindow(&open);

            ImGui::Begin("GraphicsSystem");
#ifdef RENDER_TO_TEXTURE
            ImGui::Checkbox("Use Render Buffers", &RenderToTexture);
#endif
            ImGui::Text("Drawn Entities: %d", entityCount);
            ImGui::Text("Triangles: %d", TriangleCount);
            ImGui::Text("FPS: %d", GetFPS());
            ImGui::Text("Draw Batches: %d", batchCount);
            ImGui::End();

            ImGui::Begin("VisibilitySystem");
            ImGui::Checkbox("Update Frustum", &VisibilitySystem::UpdateFrustum);
            ImGui::Checkbox("Automatic LOD", &VisibilitySystem::AutomaticBias);
            ImGui::InputFloat("LOD Bias", &VisibilitySystem::LodBias);
            ImGui::InputFloat("Target FPS", &VisibilitySystem::TargetFps);
            ImGui::Text("Average FPS: %f", VisibilitySystem::AverageFps);
            ImGui::End();
            rlImGuiEnd();
        }
        
        if(!DebugGuiActive || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            UpdateCamera(&camera);
        }

        EndDrawing();

        FrameCount++;
    }
};