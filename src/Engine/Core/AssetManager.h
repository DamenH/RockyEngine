#pragma once

#include <raylib.h>

struct ModelSet {
    int model;
    int modelLod1;
    int modelLod2;
    int Billboard;
};

class AssetManager {
public:
    AssetManager();

    static void Load();

    static ModelSet *GetModelSet(int index);

    static Model *GetModel(int index);

    static int
    loadModelSet(char *meshFile, char *meshFileLod1, char *meshFileLod2, char *textureFile, char *billboardFile);

    static int loadModel(char *meshFile, char *textureFile);

    static int loadBillboard(char *billboardFile);

};



