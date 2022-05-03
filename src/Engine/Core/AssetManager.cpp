#include "AssetManager.h"
#include "Systems/rlights.h"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <unistd.h>
#include <unordered_map>

static int NextId = 0;

std::unordered_map<int, Mesh> Meshes;
std::unordered_map<int, Material> Materials;
std::unordered_map<int, Music> Musics;

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

Music AssetManager::GetMusic(int Id)
{
    return Musics[Id];
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

    Material material = LoadModel("../media/Asteroid/asteroid_04.obj").materials[0];
    Texture2D texture = LoadTexture("../media/Asteroid/asteroid_4k.png");
    material.shader = shader;
    material.maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    LoadMesh("../media/Asteroid/asteroid_00.obj");
    LoadMesh("../media/Asteroid/asteroid_01.obj");
    LoadMesh("../media/Asteroid/asteroid_02.obj");
    LoadMesh("../media/Asteroid/asteroid_03.obj");

    LoadMaterial(material);

    LoadMusic("../media/Audio/ambient-space.ogg");

    std::cout << "Done" << std::endl;
}

int AssetManager::LoadMesh(char *path)
{
    Model model = LoadModel(path);
    Meshes[NextId] = model.meshes[0];
    NextId++;
    return NextId - 1;
}

int AssetManager::LoadMaterial(Material material)
{
    Materials[NextId] = material;
    NextId++;
    return NextId - 1;
}

int AssetManager::LoadMusic(char *path)
{
    Music music = LoadMusicStream(path);
    Musics[NextId] = music;
    NextId++;
    return NextId - 1;
}