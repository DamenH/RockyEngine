// #include "Ogre.h"
// #include "OgreApplicationContext.h"
// #include <SDL2/SDL.h>
// #include <entt/entt.hpp>

// #include "SystemBase.h"

#include "Application.h"
// class Application : public OgreBites::ApplicationContext
// {
// public:
// 	Application();
// 	void Setup();
// 	void Run();
// 	void RegisterSystem(SystemBase *system);

// 	Ogre::SceneNode *camNode;
// 	Ogre::SceneNode *modelNode;
// 	entt::registry *registry;
// };

Application::Application() : OgreBites::ApplicationContext("Bootstrap Ogre")
{
}

void Application::Setup()
{
	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();

	// get a pointer to the already created root
	Ogre::Root *root = getRoot();
	Ogre::SceneManager *scnMgr = root->createSceneManager();

	// register our scene with the RTSS
	Ogre::RTShader::ShaderGenerator *shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	// without light we would just get a black screen
	Ogre::Light *light = scnMgr->createLight("MainLight");
	Ogre::SceneNode *lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 10, 15);
	lightNode->attachObject(light);

	// also need to tell where we are
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->setPosition(0, 0, 15);
	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	// create the camera
	Ogre::Camera *cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);

	// and tell it to render into the main window
	getRenderWindow()->addViewport(cam);

	// finally something to render
	Ogre::Entity *ent = scnMgr->createEntity("DamagedHelmet.mesh");
	modelNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	modelNode->attachObject(ent);

	unsigned short src, dst;
	if (!ent->getMesh()->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dst))
	{
		// enforce that we have tangent vectors
		ent->getMesh()->buildTangentVectors(Ogre::VES_TANGENT, src, dst);
	}

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("DamagedHelmet");
	// Ogre::GpuProgramParametersSharedPtr mParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	// mParams->setNamedConstant("u_ScaleIBLAmbient", Ogre::Vector4(Ogre::Real(1)));
}

void Application::RegisterSystem(SystemBase *system)
{
	// Add system to systems array
}

void Application::Run()
{
	SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// const double CAMERA_ROT_RATE = 0.025;

	const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

	bool quit = false;

	while (!quit)
	{
		SDL_PumpEvents();

		if (keyStates[SDL_SCANCODE_ESCAPE])
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		if (SDL_GetRelativeMouseMode())
		{
			// int32_t MouseDeltaX, MouseDeltaY;
			// SDL_GetRelativeMouseState(&MouseDeltaX, &MouseDeltaY);

			// Ogre::Quaternion cameraOrientation = this->camNode->getOrientation();
			// Ogre::Quaternion yaw = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE), Ogre::Vector3(0, CAMERA_ROT_RATE * -MouseDeltaX, 0.0));
			// cameraOrientation = yaw * cameraOrientation;
			// Ogre::Quaternion pitch = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE), Ogre::Vector3(CAMERA_ROT_RATE * -MouseDeltaY, 0.0, 0.0));
			// cameraOrientation = cameraOrientation * pitch;
			// this->camNode->setOrientation(cameraOrientation);

			// Ogre::Vector3 cameraPosition = this->camNode->getPosition();
			// double translationForward = (!!keyStates[SDL_SCANCODE_D] * 0.1) - (!!keyStates[SDL_SCANCODE_A] * 0.1);
			// double translationLeft = (!!keyStates[SDL_SCANCODE_S] * 0.1) - (!!keyStates[SDL_SCANCODE_W] * 0.1);
			// this->camNode->setPosition(Ogre::Vector3(cameraPosition.x + translationForward, 0, cameraPosition.z + translationLeft));
		}

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		this->getRoot()->renderOneFrame();
	}
	SDL_Quit();
}
