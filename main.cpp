#include "Game/Systems/FreeCameraSystem.h"
#include "Engine/Core/Application.cpp"

int main(int argc, char *argv[])
{
    Application app;
<<<<<<< HEAD
    app.setup();
=======
//    app.initApp();
>>>>>>> 2bd03941a668bfd9d7f1740928eca15ada6dd96e

    FreeCameraSystem freeCameraSystem;
    app.RegisterSystem(&freeCameraSystem);

//    app.Run();

    return 0;
}
