#include "AssetManager.h"
#include <raylib.h>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <raymath.h>
#include "Systems/rlights.h"

int NextId = 0;

std::unordered_map<int, Mesh> Meshes;
std::unordered_map<int, Material> Materials;

AssetManager::AssetManager()
{
}

Mesh AssetManager::GetMesh(int Id)
{
    return Meshes[Id];
}

Material AssetManager::GetMaterial(int Id)
{
    return Materials[Id];
}

void AssetManager::Load()
{
    char cwd[256];
    getcwd(cwd, 256);
    std::cout << "Current working directory: " << cwd << std::endl;
    std::cout << "Loading resources" << std::endl;

    Shader shader = LoadShader(TextFormat("../media/Shaders/base_lighting_instanced.vs", 330),
                               TextFormat("../media/Shaders/lighting.fs", 330));
    // Get some shader locations
    shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");
    // Ambient light level
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    CreateLight(LIGHT_DIRECTIONAL, (Vector3){50.0f, 50.0f, 0.0f}, Vector3Zero(), WHITE, shader);

    Model model = LoadModel("../media/Asteroid/asteroid_01.obj");
    Texture2D texture = LoadTexture("../media/Asteroid/asteroid_4k.jpg");
    model.materials[0].shader = shader;
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    Meshes[NextId] = model.meshes[0]; //0
    NextId++;

    Materials[NextId] = model.materials[0]; //1
    NextId++;

    std::cout << "Done" << std::endl;
}