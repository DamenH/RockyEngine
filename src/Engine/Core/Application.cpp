#include "Application.h"
#include "Systems/InputSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/VisibilitySystem.h"

#include "Components/CameraComponent.h"
#include "Components/TranformComponent.h"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <raylib.h>
#include <iostream>

Application::Application() {
}

void Application::setup() {
    std::cout << "Running Setup\n";

    std::cout << "Creating window and generating OpenGL context\n";
    SetConfigFlags(FLAG_MSAA_4X_HINT |
                   //FLAG_VSYNC_HINT |
                   FLAG_WINDOW_RESIZABLE
                   //FLAG_FULLSCREEN_MODE
    );
    InitWindow(1920, 1080, "Rocky Engine");
    AssetManager::Load();

    // Create camera entity
    auto entity = registry.create();
    registry.emplace<CameraComponent>(entity);
    registry.emplace<TransformComponent>(entity, 0, 0, 25);
}

void Application::RegisterSystem(SystemBase *system) {
    systems[systemsIndex] = system;
    systemsIndex++;
}

void Application::InitializeSystems() {
    std::cout << "Initializing Systems\n";
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

    std::cout << "Running\n";

    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    bool quit = false;

    InputSystem inputSystem;
    TransformSystem transformSystem;
    GraphicsSystem graphicsSystem;
    VisibilitySystem visibilitySystem;

    RegisterSystem(&inputSystem);
    RegisterSystem(&transformSystem);
    RegisterSystem(&graphicsSystem);
    RegisterSystem(&visibilitySystem);

    InitializeSystems();

    std::cout << "Entering Main Loop\n";
    while (!quit) {
        SDL_PumpEvents();

        if (keyStates[SDL_SCANCODE_ESCAPE]) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }

        /*if (SDL_GetRelativeMouseMode())
        {
            UpdateSystems();
        }*/

        UpdateSystems();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        if (!quit) {
            quit = WindowShouldClose();
        }
    }
    SDL_Quit();
}
