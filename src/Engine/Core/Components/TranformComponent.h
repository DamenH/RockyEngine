#pragma once

#include <Ogre.h>

//struct Translation {
//    float X;
//    float Y;
//    float Z;
//};

struct TransformComponent {
//    Translation translation;
    float X;
    float Y;
    float Z;
    Ogre::Quaternion rotation;
};
