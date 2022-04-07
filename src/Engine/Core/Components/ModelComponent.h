#pragma once

// ID of a mesh or array of LOD meshes, material, and other 3D rendering parameters
struct ModelComponent {
    std::vector<int> Meshes;
    int Material;
};
