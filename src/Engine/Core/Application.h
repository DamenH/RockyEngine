#pragma once

#include "SystemBase.h"

#include <entt/entt.hpp>

class Application{
public:
    Application();

    void setup();

    void RegisterSystem(SystemBase *system);
    void InitializeSystems();
    void UpdateSystems();

    void Run();

    entt::registry registry;

private:
    

    SystemBase *systems[128]{};
    uint8_t systemsIndex = 0;
};

