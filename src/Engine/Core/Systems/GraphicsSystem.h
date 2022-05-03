#pragma once

#include "Engine/Core/AssetManager.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/Components/InstanceBinComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Systems/VisibilitySystem.h"
#include "Engine/Utilities/Debug.h"
#include "Engine/Utilities/Profiler.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <entt/entt.hpp>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>
#include <vector>

#define RLIGHTS_IMPLEMENTATION

#include "rlights.h"

#define BUFFER_COUNT 8

class GraphicsSystem : public SystemBase {
    inline static Camera3D camera;
    inline static uint8_t FrameCount = 0;
    inline static int TriangleCount = 0;
    inline static int entityCount = 0;
    inline static int batchCount = 0;
    inline static int cameraCount = 0;

    inline static bool RenderToTexture = true;
    inline static std::vector<RenderTexture2D> renderBuffers;

    inline static bool DebugGuiActive = false;

    inline static Model skybox;

    void OnStartup(entt::registry &registry) override {
        // Camera struct initialization
        // TODO: Eliminate RayLib camera struct
        camera = {0};
        camera.position = (Vector3) {0.0f, 10.0f, 0.0f}; // Camera position
        camera.target = (Vector3) {0.0f, 0.0f, 0.0f};    // Camera looking at point
        camera.up = (Vector3) {0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
        camera.fovy = 60.0f;                            // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;         // Camera mode type
        SetCameraMode(camera, CAMERA_CUSTOM);     // Set a free camera mode

        std::cout << "Generating render buffers\n";
        for (int i = 0; i < BUFFER_COUNT; i++) {
            RenderTexture2D t = LoadRenderTexture(1920, 1080);
            renderBuffers.push_back(t);
        }
        std::cout << "====\n";

#ifndef NDEBUG
        DebugGuiActive = true;
#endif
        Debug::Initialize();
        Debug::RegisterCallback("GraphicsSystem", &DebugGuiCallback);
        Profiler::Initialize(); // ToDo: Move this initialization someplace else

        // ====================================================================
        Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skybox = LoadModelFromMesh(cube);
        skybox.materials[0].shader =
                LoadShader(TextFormat("../media/Shaders/skybox.vs", 330),
                           TextFormat("../media/Shaders/skybox.fs", 330));

        int OnePointer[] = {1};
        int ZeroPointer[] = {0};
        int CubeMapPointer[] = {MATERIAL_MAP_CUBEMAP};

        SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"),
                       CubeMapPointer, SHADER_UNIFORM_INT);
        SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), OnePointer,
                       SHADER_UNIFORM_INT);
        SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"),
                       OnePointer, SHADER_UNIFORM_INT);

        // Load cubemap shader and setup required shader locations
        Shader shdrCubemap =
                LoadShader(TextFormat("../media/Shaders/cubemap.vs", 330),
                           TextFormat("../media/Shaders/cubemap.fs", 330));

        SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), ZeroPointer,
                       SHADER_UNIFORM_INT);
        Texture2D panorama;
        panorama = LoadTexture("../media/Environment/nebula-n0.hdr");
        // Generate cubemap (texture with 6 quads-cube-mapping) from panorama HDR texture
        // NOTE 1: New texture is generated rendering to texture, shader calculates the sphere->cube coordinates mapping
        // NOTE 2: It seems on some Android devices WebGL, fbo does not properly support a FLOAT-based attachment,
        // despite texture can be successfully created.. so using PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 instead of
        // PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
        skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture =
                GenTextureCubemap(shdrCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        // UnloadTexture(panorama);    // Texture not required anymore, cubemap already generated
        // ====================================================================
    }

    void OnUpdate(entt::registry &registry) override {
        auto profTimeStamp = Profiler::Start("GraphicsSystem", registry);
        TriangleCount = 0;
        entityCount = 0;
        batchCount = 0;
        cameraCount = 0;

        auto camerasView = registry.view<CameraComponent, TransformComponent>();
        auto cameraEntity = camerasView.front();
        auto &cam = camerasView.get<CameraComponent>(cameraEntity);
        auto &transform = camerasView.get<TransformComponent>(cameraEntity);

        camera.position = transform.Translation;

        glm::quat rotation = glm::quat(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));

        glm::vec3 target = rotation * glm::vec3(0.0f, 0.0f, 1.0f);

        target = glm::normalize(target);

        camera.target = {
                transform.Translation.x + target.x,
                transform.Translation.y + target.y,
                transform.Translation.z + target.z
        };

        BeginDrawing();

        if (RenderToTexture) {
            BeginTextureMode(renderBuffers[0]);
            ClearBackground(BLANK);
#ifndef NDEBUG
            DrawText("Buffer 0", 100, 200, 32, Color{0x00, 0xFF, 0xFF, 0x8F});
#endif
        } else {
            ClearBackground(BLACK);
        }

        BeginMode3D(camera);
        if (VisibilitySystem::UpdateFrustum) {
            cam.Frustum->Extract();
        }

        // ====================================================================
        // We are inside the cube, we need to disable backface culling!
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        DrawModel(skybox, (Vector3) {0, 0, 0}, 1.0f, WHITE);
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
        // ====================================================================

        auto binsView = registry.view<InstanceBinComponent>();
        for (auto entity: binsView) {
            InstanceBinComponent bin = binsView.get<InstanceBinComponent>(entity);
            const Mesh mesh = AssetManager::GetMesh(bin.Mesh);
            const Material material = AssetManager::GetMaterial(bin.Material);
            const int count = bin.InstanceCount;
            Matrix *array = bin.ModelTransforms.data();

            DrawMeshInstanced(mesh, material, array, count);
            TriangleCount += mesh.triangleCount * count;
            entityCount += count;
            if (count > 0) {
                batchCount++;
            }
        }

        DrawGrid(10, 1.0f);

        EndMode3D();

        if (RenderToTexture) {
            EndTextureMode();
            ClearBackground(BLACK);
            DrawTexturePro(renderBuffers[0].texture, cam.Target.SourceRect, cam.Target.DestRect, cam.Target.Origin,
                           cam.Target.Angle, cam.Target.Tint);
        }


        if (IsKeyReleased(KEY_Q)) {
            DebugGuiActive = !DebugGuiActive;
            if (DebugGuiActive) {
                EnableCursor();
            } else {
                DisableCursor();
            }
        }

        if (DebugGuiActive) {
            Debug::Draw();
        }

        if (!DebugGuiActive || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            UpdateCamera(&camera);
        }

#ifndef NDEBUG
        DrawText("DEBUG", 1000, 100, 32, Color{0xFF, 0x00, 0x00, 0x8F});
#endif

        EndDrawing();

        FrameCount++;

        Profiler::Stop(profTimeStamp, registry);
    }

    static void DebugGuiCallback() {
        ImGui::Checkbox("Use Render Buffers", &RenderToTexture);

        ImGui::Text("Drawn Entities: %d", entityCount);
        ImGui::Text("Triangles: %d", TriangleCount);
        ImGui::Text("FPS: %d", GetFPS());
        ImGui::Text("Draw Batches: %d", batchCount);
    }

    // Generate cubemap texture from HDR texture
    static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format) {
        TextureCubemap cubemap = {0};

        rlDisableBackfaceCulling(); // Disable backface culling to render inside the cube

        // STEP 1: Setup framebuffer
        //------------------------------------------------------------------------------------------
        unsigned int rbo = rlLoadTextureDepth(size, size, true);
        cubemap.id = rlLoadTextureCubemap(0, size, format);

        unsigned int fbo = rlLoadFramebuffer(size, size);
        rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

        // Check if framebuffer is complete with attachments (valid)
        if (rlFramebufferComplete(fbo))
            TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
        //------------------------------------------------------------------------------------------

        // STEP 2: Draw to framebuffer
        //------------------------------------------------------------------------------------------
        // NOTE: Shader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
        rlEnableShader(shader.id);

        // Define projection matrix and send it to shader
        Matrix matFboProjection = MatrixPerspective(90.0 * DEG2RAD, 1.0, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

        // Define view matrix for every side of the cubemap
        Matrix fboViews[6] = {
                MatrixLookAt((Vector3) {0.0f, 0.0f, 0.0f}, (Vector3) {1.0f, 0.0f, 0.0f}, (Vector3) {0.0f, -1.0f, 0.0f}),
                MatrixLookAt((Vector3) {0.0f, 0.0f, 0.0f}, (Vector3) {-1.0f, 0.0f, 0.0f},
                             (Vector3) {0.0f, -1.0f, 0.0f}),
                MatrixLookAt((Vector3) {0.0f, 0.0f, 0.0f}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector3) {0.0f, 0.0f, 1.0f}),
                MatrixLookAt((Vector3) {0.0f, 0.0f, 0.0f}, (Vector3) {0.0f, -1.0f, 0.0f},
                             (Vector3) {0.0f, 0.0f, -1.0f}),
                MatrixLookAt((Vector3) {0.0f, 0.0f, 0.0f}, (Vector3) {0.0f, 0.0f, 1.0f}, (Vector3) {0.0f, -1.0f, 0.0f}),
                MatrixLookAt((Vector3) {0.0f, 0.0f, 0.0f}, (Vector3) {0.0f, 0.0f, -1.0f},
                             (Vector3) {0.0f, -1.0f, 0.0f})};

        rlViewport(0, 0, size, size); // Set viewport to current fbo dimensions

        // Activate and enable texture for drawing to cubemap faces
        rlActiveTextureSlot(0);
        rlEnableTexture(panorama.id);

        for (int i = 0; i < 6; i++) {
            // Set the view matrix for the current cube face
            rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);

            // Select the current cubemap face attachment for the fbo
            // WARNING: This function by default enables->attach->disables fbo!!!
            rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
            rlEnableFramebuffer(fbo);

            // Load and draw a cube, it uses the current enabled texture
            rlClearScreenBuffers();
            rlLoadDrawCube();

            // ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
            // for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
            // TODO: Investigate this issue...
            // rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using
            // internal batch system! rlClearScreenBuffers(); DrawCubeV(Vector3Zero(), Vector3One(), WHITE);
            // rlDrawRenderBatchActive();
        }
        //------------------------------------------------------------------------------------------

        // STEP 3: Unload framebuffer and reset state
        //------------------------------------------------------------------------------------------
        rlDisableShader();        // Unbind shader
        rlDisableTexture();       // Unbind texture
        rlDisableFramebuffer();   // Unbind framebuffer
        rlUnloadFramebuffer(fbo); // Unload framebuffer (and automatically attached depth texture/renderbuffer)

        // Reset viewport dimensions to default
        rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
        rlEnableBackfaceCulling();
        //------------------------------------------------------------------------------------------

        cubemap.width = size;
        cubemap.height = size;
        cubemap.mipmaps = 1;
        cubemap.format = format;

        return cubemap;
    }
};