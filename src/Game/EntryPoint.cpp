#include "Engine/Core/Application.cpp"
#include "Systems/FreeCameraSystem.h"

int main(int argc, char *argv[])
{
	Application app;
	app.initApp();

	app.Run();

	return 0;
}
