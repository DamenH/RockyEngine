#include "AssetManager.h"
#include <raylib.h>
#include <iostream>

ModelSet modelSets[128];
int modelSetCount = 0;

AssetManager::AssetManager() {
}

ModelSet* AssetManager::GetModelSet(int index)
{
    return &modelSets[index];
}

void AssetManager::Load() {
    std::cout << "Loading resources";
    //TODO: Change to dynamic search and load of assets
    loadModelSet("../media/Asteroid/asteroid_00.obj",
                 "../media/Asteroid/asteroid_01.obj",
                 "../media/Asteroid/asteroid_03.obj",
                 "../media/Asteroid/asteroid_4k.jpg",
                 "../media/Asteroid/asteroid_billboard.png"
                 );
    std::cout << "Done";
}

void AssetManager::loadModelSet(char* meshFile, char* meshFileLod1, char* meshFileLod2, char* textureFile, char* billboardFile)
{
    Model model = LoadModel(meshFile);
    Model model1 = LoadModel(meshFileLod1);
    Model model2 = LoadModel(meshFileLod2);
    Texture2D texture = LoadTexture(textureFile);
    GenTextureMipmaps(&texture);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    model1.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Texture2D billboard = LoadTexture(billboardFile);
    GenTextureMipmaps(&billboard);

    ModelSet set;
    set.model = model;
    set.modelLod1 = model1;
    set.modelLod2 = model2;
    set.Billboard = billboard;

    modelSets[modelSetCount] = set;

    modelSetCount++;
}

