#include <iostream>
#include "Engine/Core/Application.cpp"

int main(int argc, char *argv[])
{
	std::cout << "Starting Game!" << std::endl;

	Application app;
	app.initApp();

	double z_position = 15;

	while (true)
	{
		z_position += 0.01;
		app.camNode->setPosition(0, 0, z_position);
		app.getRoot()->renderOneFrame();
	}

	return 0;
}
