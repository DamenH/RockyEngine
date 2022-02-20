#include <iostream>
#include "Engine/Core/Application.cpp"
#include <SDL2/SDL.h>

#include "Systems/FreeCameraSystem.h"

int main(int argc, char *argv[])
{
	Application app;
	app.initApp();

	// FreeCameraSystem freeCameraSystem;
	// app.RegisterSystem(&freeCameraSystem);

	// app.Run();



	// app.modelNode->setScale(Ogre::Vector3(5.0, 5.0, 5.0));

	// SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
	// SDL_SetRelativeMouseMode(SDL_TRUE);

	// const double CAMERA_ROT_RATE = 0.025;

	// const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

	// bool quit = false;

	// while (!quit)
	// {
	// 	SDL_PumpEvents();

	// 	if (keyStates[SDL_SCANCODE_ESCAPE])
	// 	{
	// 		SDL_SetRelativeMouseMode(SDL_FALSE);
	// 	}

	// 	if (SDL_GetRelativeMouseMode())
	// 	{
	// 		int32_t MouseDeltaX, MouseDeltaY;
	// 		SDL_GetRelativeMouseState(&MouseDeltaX, &MouseDeltaY);

	// 		Ogre::Quaternion cameraOrientation = app.camNode->getOrientation();
	// 		Ogre::Quaternion yaw = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE), Ogre::Vector3(0, CAMERA_ROT_RATE * -MouseDeltaX, 0.0));
	// 		cameraOrientation = yaw * cameraOrientation;
	// 		Ogre::Quaternion pitch = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE), Ogre::Vector3(CAMERA_ROT_RATE * -MouseDeltaY, 0.0, 0.0));
	// 		cameraOrientation = cameraOrientation * pitch;
	// 		app.camNode->setOrientation(cameraOrientation);

	// 		Ogre::Vector3 cameraPosition = app.camNode->getPosition();
	// 		double translationForward = (!!keyStates[SDL_SCANCODE_D] * 0.1) - (!!keyStates[SDL_SCANCODE_A] * 0.1);
	// 		double translationLeft = (!!keyStates[SDL_SCANCODE_S] * 0.1) - (!!keyStates[SDL_SCANCODE_W] * 0.1);
	// 		app.camNode->setPosition(Ogre::Vector3(cameraPosition.x + translationForward, 0, cameraPosition.z + translationLeft));
	// 	}

	// 	SDL_Event event;
	// 	while (SDL_PollEvent(&event))
	// 	{
	// 		switch (event.type)
	// 		{
	// 		case SDL_QUIT:
	// 			quit = true;
	// 			break;
	// 		}
	// 	}

	// 	app.getRoot()->renderOneFrame();
	// }
	// SDL_Quit();

	return 0;
}
