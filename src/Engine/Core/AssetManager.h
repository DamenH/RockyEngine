#pragma once

#include <raylib.h>

class AssetManager
{
public:
    AssetManager();

    static void Load();
    static Mesh GetMesh(int Id);
    static Material GetMaterial(int Id);
};
