#include "Application.h"
#include "Systems/InputSystem.h"
#include "Systems/TransformSystem.h"

#include "Components/CameraComponent.h"
#include "Components/NodeComponent.h"
#include "Components/TranformComponent.h"


#include <SDL2/SDL.h>


Application::Application() {
}

void Application::setup() {

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
//
//    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
//    SDL_SetRelativeMouseMode(SDL_TRUE);
//
//    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
//    bool quit = false;
//
//    InputSystem inputSystem;
//    TransformSystem transformSystem;
//    RegisterSystem(&inputSystem);
//    RegisterSystem(&transformSystem);
//    InitializeSystems();
//
//
//    while (!quit) {
//        SDL_PumpEvents();
//
//        if (keyStates[SDL_SCANCODE_ESCAPE]) {
//            SDL_SetRelativeMouseMode(SDL_FALSE);
//        }
//
//        if (SDL_GetRelativeMouseMode()) {
//            UpdateSystems();
//        }
//
//        SDL_Event event;
//        while (SDL_PollEvent(&event)) {
//            switch (event.type) {
//                case SDL_QUIT:
//                    quit = true;
//                    break;
//            }
//        }
//
//        this->getRoot()->renderOneFrame();
//    }
//    SDL_Quit();
}
