#pragma once
#include <memory>
#include <raylib.h>
#include <raymath.h>

// Pointer to a Mesh, Shader, first element in a Matrix array of transforms, and a count of instances
// Everything the renderer needs for instancing
struct InstanceBinComponent {
    std::shared_ptr<Mesh> Mesh;
    std::shared_ptr<Shader> Shader;
    int InstanceCount;
    std::shared_ptr<Matrix> ModelTransform;
};
