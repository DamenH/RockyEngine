#pragma once

// Pointer to a mesh or array of LOD meshes, shader, and other 3D rendering parameters
struct ModelComponent {
    std::shared_ptr<Mesh> Meshes;
    std::shared_ptr<Shader> Shader;
    int LodCount;
    float LodBias;
};
