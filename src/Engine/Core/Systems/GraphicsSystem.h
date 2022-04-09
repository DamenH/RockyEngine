#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/Components/InstanceBinComponent.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/Systems/VisibilitySystem.h"
#include "Engine/Utilities/Profiler.h"
#include "Engine/Utilities/Debug.h"

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
    inline static Camera3D camera;
    inline static uint8_t FrameCount = 0;
    inline static int TriangleCount = 0;
    inline static int entityCount = 0;
    inline static int batchCount = 0;

    inline static bool RenderToTexture = true;
    inline static RenderTexture2D renderTex;

    inline static bool DebugGuiActive = false;


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

        std::cout << "Generating render buffer\n";
        renderTex = LoadRenderTexture(1920, 1080);
        std::cout << "====\n";

#ifndef NDEBUG
        DebugGuiActive = true;
#endif
        Debug::Initialize();
        Debug::RegisterCallback("GraphicsSystem", &DebugGuiCallback);
        Profiler::Initialize(); //ToDo: Move this initialization someplace else
    }

    void OnUpdate(entt::registry &registry) override
    {
        auto profTimeStamp = Profiler::Start("GraphicsSystem", registry);
        TriangleCount = 0;
        entityCount = 0;
        batchCount = 0;

        auto camerasView = registry.view<CameraComponent, TransformComponent>();
        RenderTarget target;
        for (auto entity : camerasView)
        {
            CameraComponent cam = camerasView.get<CameraComponent>(entity);
            auto &transform = camerasView.get<TransformComponent>(entity);
            camera.position = transform.Translation;
            target = cam.Target;
        }

        BeginDrawing();

        if (RenderToTexture)
        {
            BeginTextureMode(renderTex);
            ClearBackground(BLANK);
#ifndef NDEBUG
            DrawText("Buffer 0", 100, 200, 32, Color{0x00, 0xFF, 0xFF, 0x8F});
#endif
        }
        else
        {
            ClearBackground(BLACK);
        }

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

        if (RenderToTexture)
        {
            EndTextureMode();
            ClearBackground(BLACK);
            DrawTexturePro(renderTex.texture, target.SourceRect, target.DestRect, target.Origin, target.Angle, target.Tint);
        }

        if (IsKeyReleased(KEY_Q))
        {
            DebugGuiActive = !DebugGuiActive;
            if (DebugGuiActive)
            {
                EnableCursor();
            }
            else
            {
                DisableCursor();
            }
        }

        if (DebugGuiActive)
        {
            Debug::Draw();            
        }

        if (!DebugGuiActive || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            UpdateCamera(&camera);
        }

#ifndef NDEBUG
        DrawText("DEBUG", 1000, 100, 32, Color{0xFF, 0x00, 0x00, 0x8F});
#endif

        EndDrawing();

        FrameCount++;

        Profiler::Stop(profTimeStamp, registry);

        /*if(FrameCount == 0)
        {
            std::cout << profTimeStamp->label.data() << "\t" << std::chrono::duration_cast<std::chrono::microseconds>(profTimeStamp->stopTimestamp - profTimeStamp->startTimestamp).count() << "us\n";
        }*/
    }

    static void DebugGuiCallback()
    {
        ImGui::Checkbox("Use Render Buffers", &RenderToTexture);

        ImGui::Text("Drawn Entities: %d", entityCount);
        ImGui::Text("Triangles: %d", TriangleCount);
        ImGui::Text("FPS: %d", GetFPS());
        ImGui::Text("Draw Batches: %d", batchCount);
    }
};