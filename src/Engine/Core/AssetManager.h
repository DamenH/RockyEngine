#pragma once

#include <raylib.h>

class AssetManager
{
  public:
    AssetManager();

    static void Load();
    static Mesh GetMesh(int Id);
    static Material GetMaterial(int Id);
    static Music GetMusic(int Id);
    static int LoadMesh(char *path);
    static int LoadMaterial(Material material);
    static int LoadMusic(char *path);
};
