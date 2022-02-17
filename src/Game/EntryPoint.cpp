#include <iostream>
#include "Engine/Core/Application.cpp"

int main(int argc, char *argv[])
{
	std::cout << "Starting Game!" << std::endl;

	Application app;
	app.initApp();

	app.modelNode->setScale(Ogre::Vector3(5.0, 5.0, 5.0));

	while (true)
	{
		Ogre::Quaternion orientation = app.modelNode->getOrientation();
		orientation = orientation * Ogre::Quaternion(Ogre::Radian(0.01), Ogre::Vector3(0.0, 0.0, 1.0));
		app.modelNode->setOrientation(orientation);

		app.getRoot()->renderOneFrame();
	}

	return 0;
}
