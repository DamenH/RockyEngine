#pragma once

#include <raylib.h>


class AssetManager{
public:
    AssetManager();
    void Load();
private:
    Model models[128];
    Texture2D textures[128];
};