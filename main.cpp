#include "Engine/Core/Application.cpp"

#include <boost/math/quaternion.hpp>

int main(int argc, char *argv[]) {
    boost::math::quaternion MyQuat = boost::math::quaternion<float>();

    Application app;
    app.setup();

    app.Run();

    return 0;
}
