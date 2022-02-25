#include "Application.h"
#include "Systems/InputSystem.h"
#include "Systems/TransformSystem.h"

#include "Components/CameraComponent.h"
#include "Components/NodeComponent.h"
#include "Components/TranformComponent.h"

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>


Application::Application() : OgreBites::ApplicationContext("Bootstrap Ogre") {
}

void Application::setup() {
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // get a pointer to the already created root
    root = getRoot();
    sceneManager = root->createSceneManager();

    // register our scene with the RTSS
    shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shaderGenerator->addSceneManager(sceneManager);

    // without light we would just get a black screen
    Ogre::Light *light = sceneManager->createLight("MainLight");
    Ogre::SceneNode *lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    camNode = sceneManager->getRootSceneNode()->createChildSceneNode();
//    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera *cam = sceneManager->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    // Create camera entity
    auto entity = registry.create();
    registry.emplace<CameraComponent>(entity);
    registry.emplace<NodeComponent>(entity, camNode);
    registry.emplace<TransformComponent>(entity, 0, 0, 25, camNode->getOrientation());

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);

    // finally something to render
    Ogre::Entity *ent = sceneManager->createEntity("DamagedHelmet.mesh");
    modelNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    modelNode->attachObject(ent);
    modelNode->setScale(Ogre::Vector3(5.0, 5.0, 5.0));

    unsigned short src, dst;
    if (!ent->getMesh()->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dst)) {
        // enforce that we have tangent vectors
        ent->getMesh()->buildTangentVectors(Ogre::VES_TANGENT, src, dst);
    }

    Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("DamagedHelmet");
    // Ogre::GpuProgramParametersSharedPtr mParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    // mParams->setNamedConstant("u_ScaleIBLAmbient", Ogre::Vector4(Ogre::Real(1)));
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

        this->getRoot()->renderOneFrame();
    }
    SDL_Quit();
}
