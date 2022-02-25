#pragma once

#include "SystemBase.h"

#include <OgreApplicationContext.h>
#include <entt/entt.hpp>


class Application : public OgreBites::ApplicationContext {
public:
    Application();

    void setup() override;

    void RegisterSystem(SystemBase *system);
    void InitializeSystems();
    void UpdateSystems();

    void Run();

    Ogre::SceneNode *camNode{};
    Ogre::SceneNode *modelNode{};

    entt::registry registry;

private:
    Ogre::Root *root{};
    Ogre::SceneManager *sceneManager{};
    Ogre::RTShader::ShaderGenerator *shaderGenerator{};

    SystemBase *systems[128]{};
    uint8_t systemsIndex = 0;
};

