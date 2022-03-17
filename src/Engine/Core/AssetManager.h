#pragma once

#include <raylib.h>

struct ModelSet
{
    Model model;
    Model modelLod1;
    Model modelLod2;
    Texture2D Billboard;
};

class AssetManager {
public:
    //static ModelSet modelSets[128];
    //static int modelSetCount;
    AssetManager();
    static void Load();
    static ModelSet* GetModelSet(int index);

    static void loadModelSet(char* meshFile, char* meshFileLod1, char* meshFileLod2, char* textureFile, char* billboardFile);

};



