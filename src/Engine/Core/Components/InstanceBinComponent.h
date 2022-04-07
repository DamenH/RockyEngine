#pragma once
#include <memory>
#include <raylib.h>
#include <raymath.h>

// ID of a Mesh, Shader, first element in a Matrix array of transforms, and a count of instances
// Everything the renderer needs for instancing
struct InstanceBinComponent {
    int Mesh;
    int Material;
    int InstanceCount;
    std::vector<Matrix> ModelTransforms;
};
