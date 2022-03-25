#include "AssetManager.h"
#include <raylib.h>
#include <iostream>
#include <unistd.h>

ModelSet modelSets[128];
Model models[128];
Texture2D billboards[128];

int modelSetCount = 0;
int modelCount = 0;
int billboardCount = 0;

AssetManager::AssetManager() {
}

ModelSet *AssetManager::GetModelSet(int index) {
    return &modelSets[index];
}

Model *AssetManager::GetModel(int index) {
    return &models[index];
}

void AssetManager::Load() {
    char cwd[256];
    getcwd(cwd, 256);
    std::cout << "Current working directory: " << cwd << std::endl;
    std::cout << "Loading resources" << std::endl;
    //TODO: Change to dynamic search and load of assets
    loadModelSet("../media/Asteroid/asteroid_00.obj",
                 "../media/Asteroid/asteroid_01.obj",
                 "../media/Asteroid/asteroid_03.obj",
                 "../media/Asteroid/asteroid_4k.jpg",
                 "../media/Asteroid/asteroid_billboard.png"
    );
    std::cout << "Done" << std::endl;
}



int AssetManager::loadModelSet(char *meshFile, char *meshFileLod1, char *meshFileLod2, char *textureFile,
                                char *billboardFile) {
    ModelSet set;
    set.model = loadModel(meshFile, textureFile);;
    set.modelLod1 = loadModel(meshFileLod1, textureFile);
    set.modelLod2 = loadModel(meshFileLod2, textureFile);
    set.Billboard = loadBillboard(billboardFile);

    modelSets[modelSetCount] = set;
    int id = modelSetCount;
    modelSetCount++;
    return id;
}

int AssetManager::loadModel(char *meshFile, char *textureFile){
    Model model = LoadModel(meshFile);
    Texture2D texture = LoadTexture(textureFile);
    GenTextureMipmaps(&texture);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    models[modelCount] = model;
    int id = modelCount;
    modelCount++;
    return id;
}

int AssetManager::loadBillboard(char *billboardFile){
    Texture2D billboard = LoadTexture(billboardFile);
    GenTextureMipmaps(&billboard);
    billboards[billboardCount] = billboard;
    int id = billboardCount;
    billboardCount++;
    return id;
}

