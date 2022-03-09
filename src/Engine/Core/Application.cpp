#include "Application.h"
#include "Systems/InputSystem.h"
#include "Systems/TransformSystem.h"

#include "Components/CameraComponent.h"
#include "Components/NodeComponent.h"
#include "Components/TranformComponent.h"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <boost/math/quaternion.hpp>
#include <raylib.h>


Application::Application(){
}

void Application::setup() {
    InitWindow(800, 600, "Rocky Engine - Raylib Edition");
    SetTargetFPS(60);

    // Create camera entity
    auto entity = registry.create();
    registry.emplace<CameraComponent>(entity);
    registry.emplace<TransformComponent>(entity, 0, 0, 25, new boost::math::quaternion<float>());
}

void Application::RegisterSystem(SystemBase *system) {
    systems[systemsIndex] = system;
    systemsIndex++;
}

void Application::InitializeSystems() {
    for (uint8_t i = 0; i < systemsIndex; i++) {
        systems[i]->OnStartup(registry);
    }
}

void Application::UpdateSystems() {
    for (uint8_t i = 0; i < systemsIndex; i++) {
        systems[i]->OnUpdate(registry);
    }
}

void Application::Run() {

    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    bool quit = false;

    InputSystem inputSystem;
    TransformSystem transformSystem;
    RegisterSystem(&inputSystem);
    RegisterSystem(&transformSystem);
    InitializeSystems();

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode


    while (!quit) {
        SDL_PumpEvents();

        if (keyStates[SDL_SCANCODE_ESCAPE]) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }

        if (SDL_GetRelativeMouseMode()) {
            UpdateSystems();
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawCube({0.0f,0.0f,0.0f}, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires({0.0f,0.0f,0.0f}, 2.0f, 2.0f, 2.0f, MAROON);

                DrawGrid(10, 1.0f);

            EndMode3D();

        EndDrawing();
    }
    SDL_Quit();
}
