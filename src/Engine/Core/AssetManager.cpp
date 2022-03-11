#pragma once

#include "AssetManager.h"
#include <raylib.h>

AssetManager::AssetManager() {

}

void AssetManager::Load() {
    //std::cout << "Loading resources";
    models[0] = LoadModel("media/Asteroid/asteroid_00.obj");
    GenTextureMipmaps(&models[0].materials[0].maps[0].texture);
}