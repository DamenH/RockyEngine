#include "Game/Systems/FreeCameraSystem.h"
#include "Engine/Core/Application.cpp"

int main(int argc, char *argv[])
{
    Application app;
    app.setup();

    FreeCameraSystem freeCameraSystem;
    app.RegisterSystem(&freeCameraSystem);

    app.Run();

    return 0;
}
