#pragma once
#include <cstdint>

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/Components/InstanceBinComponent.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include "RLFrustum.h"

// Responsible for culling, LoD, model transform calculation, and mesh instance binning
class VisibilitySystem : public SystemBase
{
    const float MaxDistance = 2000.0f;

    std::vector<std::tuple<int, int>> modelIndices;   // Unique <Mesh ID, Material ID> permutation
    std::vector<std::vector<Matrix>> transformArrays; // For every model there's a vector of transform matrices
public:
    inline static float LodBias = 100.0f;
    inline static float AverageFps = 120.0f;
    inline static float TargetFps = 120.0f;
    inline static bool AutomaticBias = false;
    inline static Camera CullingCamera;

    void OnStartup(entt::registry &registry) override
    {
        // TODO: Eliminate RayLib camera struct
        CullingCamera = {0};
        CullingCamera.position = (Vector3){0.0f, 10.0f, 0.0f}; // Camera position
        CullingCamera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
        CullingCamera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
        CullingCamera.fovy = 60.0f;                            // Camera field-of-view Y
        CullingCamera.projection = CAMERA_PERSPECTIVE;         // Camera mode type
    }

    void OnUpdate(entt::registry &registry) override
    {
        AverageFps = ((3.0f * AverageFps) + GetFPS()) / 4.0f;
        if (AutomaticBias)
        {
            if (AverageFps < TargetFps)
            {
                LodBias *= 1.01f;
            }
            if (AverageFps > TargetFps + 10)
            {
                LodBias *= 0.999f;
            }
        }

        auto renderablesView = registry.view<ModelComponent, TransformComponent, VisibilityComponent>();
        auto cameraView = registry.view<CameraComponent, TransformComponent>();

        Vector3 cameraLocation = {0, 0, 0}; // Temporary until Camera component is implemented

        for (auto entity : renderablesView) // For every renderable entity
        {
            // Fetch the Transform and Model components of the current entity
            auto &transform = renderablesView.get<TransformComponent>(entity);
            auto &model = renderablesView.get<ModelComponent>(entity);

            // Calculate the entity's Transform matrix
            // Translation
            Matrix transformMatrix = MatrixTranslate(transform.Translation.x,
                                                     transform.Translation.y,
                                                     transform.Translation.z);
            // Rotation
            transformMatrix = MatrixMultiply(transformMatrix, MatrixRotateXYZ(transform.Rotation));
            // Scale
            transformMatrix = MatrixMultiply(transformMatrix, MatrixScale(transform.Scale.x,
                                                                          transform.Scale.y,
                                                                          transform.Scale.z));

            // Store the calculated transform for any other systems that need it
            transform.Transform = transformMatrix;

            float distance = Vector3Distance(cameraLocation, transform.Translation);

            // TODO: View Frustum Culling. Temporary nonsense conditional.
            if (distance > MaxDistance || !InFrustum(CullingCamera, transform.Translation))
            {
                continue;
            }

            // If not culled, identify the specific mesh to be used based on LOD
            // TODO: Improve LOD selection to logarithmic distance or something
            float x = LodBias * 2.0f * distance / MaxDistance; // Scale relative to 50% of view distance
            x = std::clamp(x, 0.0f, 1.0f);                     // Clamp between 0.0 and 1.0
            x = x * (model.Meshes.size() - 1);                 // Scale to number of LODs
            int meshId = (int)std::floor(x);
            if (meshId >= 4)
            {
                std::cout << "Invalid meshId: " << meshId << "\n";
            }

            // Create tuple representing this specific combination of Mesh and Material
            std::tuple<int, int> modelId = std::make_tuple(meshId, model.Material);
            // Find the location of this tuple in the models vector, if it is there
            std::vector<std::tuple<int, int>>::iterator modelIndex = std::find(modelIndices.begin(), modelIndices.end(), modelId);
            if (modelIndex != modelIndices.end())
            {
                // If the combination is already present, add the matrix to the
                transformArrays[std::distance(modelIndices.begin(), modelIndex)].push_back(transformMatrix);
            }
            else
            {
                // The combination isn't already present, so push it onto the models vector
                modelIndices.push_back(modelId);

                // ...and check if the transformArrays vector is big enough
                if (transformArrays.size() < modelIndices.size())
                {
                    std::cout << "Creating new vector<Matrix>\n";
                    // If not, create a new vector and add it
                    std::vector<Matrix> newBin = std::vector<Matrix>();
                    transformArrays.push_back(newBin);
                }
                // Push the entity's calculated transform matrix onto the vector
                modelIndex = std::find(modelIndices.begin(), modelIndices.end(), modelId);
                int i = std::distance(modelIndices.begin(), modelIndex);
                transformArrays[i].push_back(transformMatrix);
            }
        }

        // Get all the current bin entities
        auto binsView = registry.view<InstanceBinComponent>();
        int binEntityCount = 0; // A counter to track how many bin entities currently exist
        for (auto entity : binsView)
        {
            auto &bin = binsView.get<InstanceBinComponent>(entity);

            if (binEntityCount > modelIndices.size()) // This bin entity isn't needed this frame
            {
                bin.InstanceCount = 0; // Mark this bin as empty
            }
            else
            {
                // We are using this bin this frame
                bin.Mesh = get<0>(modelIndices[binEntityCount]);
                bin.Material = get<1>(modelIndices[binEntityCount]);
                bin.InstanceCount = transformArrays[binEntityCount].size();
                bin.ModelTransforms = transformArrays[binEntityCount];
            }
            binEntityCount++;
        }
        // If the scene has reached a new peak complexity, new bin entities may be required
        while (binEntityCount < modelIndices.size())
        {
            // Create entity and attach a new bin component with the appropriate fields
            auto binEntity = registry.create();
            registry.emplace<InstanceBinComponent>(binEntity, InstanceBinComponent{
                                                                  get<0>(modelIndices[binEntityCount]),
                                                                  get<1>(modelIndices[binEntityCount]),
                                                                  (int)transformArrays[binEntityCount].size(),
                                                                  transformArrays[binEntityCount]});
            binEntityCount++;
        }

        // Flush all the vectors for the next frame, but do not free them
        for (int i = 0; i < modelIndices.size(); i++)
        {
            transformArrays[i].clear();
        }
        modelIndices.clear();
    }

    bool InFrustum(Camera camera, Vector3 point)
    {
        RLFrustum frustum;
        frustum.Extract();
        //return frustum.PointIn(point);

        return point.x >= 0 && point.z >= 0;
    }

};