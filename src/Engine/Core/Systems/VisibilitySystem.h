#pragma once
#include <cstdint>

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/Components/InstanceBinComponent.h"
#include "Engine/Utilities/Profiler.h"
#include "Engine/Utilities/Debug.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include "RLFrustum.h"
#include "imgui.h"

// Responsible for culling, LoD, model transform calculation, and mesh instance binning
class VisibilitySystem : public SystemBase
{
    const float MaxDistance = 2000.0f;

    std::vector<std::tuple<int, int>> modelIndices;   // Unique <Mesh ID, Material ID> permutation
    std::vector<std::vector<Matrix>> transformArrays; // For every model there's a vector of transform matrices

    uint8_t FrameCount = 0;

public:
    inline static float LodBias = 1.0f;
    inline static float AverageFps = 120.0f;
    inline static float TargetFps = 120.0f;
    inline static bool AutomaticBias = true;
    inline static bool UpdateFrustum = true;

    void OnStartup(entt::registry &registry) override
    {

#ifndef NDEBUG
        TargetFps = 60;
        AutomaticBias = false;
#endif
        Debug::RegisterCallback("VisibilitySystem", &DebugGuiCallback);
    }

    void OnUpdate(entt::registry &registry) override
    {
        auto profTimeStamp = Profiler::Start("VisibilitySystem", registry);

        FrameCount++;
        AverageFps = ((3.0f * AverageFps) + GetFPS()) / 4.0f;
        if (AutomaticBias)
        {
            if (AverageFps < TargetFps && LodBias < 10)
            {
                for (int i = 0; i < TargetFps - AverageFps; i++)
                {
                    LodBias *= 1.0001f;
                }
            }
            if (AverageFps > TargetFps + 10 && LodBias > 1)
            {
                for (int i = 0; i < AverageFps - TargetFps; i++)
                {
                    LodBias *= 0.99999f;
                }
            }
        }

        auto renderablesView = registry.view<ModelComponent, TransformComponent, VisibilityComponent>();
        auto camerasView = registry.view<CameraComponent, TransformComponent>();
        int cameraCount = 0;
        for (auto entity : camerasView)
        {
            //std::cout << "Visualizing Cam " << cameraCount++ << "\n";

            CameraComponent cam = camerasView.get<CameraComponent>(entity);
            auto &camTransform = camerasView.get<TransformComponent>(entity);

            for (auto entity : renderablesView) // For every renderable entity
            {
                // Fetch the Transform and Model components of the current entity
                auto &transform = renderablesView.get<TransformComponent>(entity);
                auto &model = renderablesView.get<ModelComponent>(entity);

                // Calculate the entity's Transform matrix

                // Scale
                Matrix transformMatrix = MatrixScale(transform.Scale.x,
                                                     transform.Scale.y,
                                                     transform.Scale.z);

                // Rotation Scale
                transformMatrix = MatrixMultiply(transformMatrix, MatrixRotateXYZ(transform.Rotation));

                // Translation
                transformMatrix = MatrixMultiply(transformMatrix, MatrixTranslate(transform.Translation.x,
                                                                                  transform.Translation.y,
                                                                                  transform.Translation.z));

                // Store the calculated transform for any other systems that need it
                transform.Transform = transformMatrix;

                float distance = abs(camTransform.Translation.x - transform.Translation.x) + abs(camTransform.Translation.y - transform.Translation.y) + abs(camTransform.Translation.z - transform.Translation.z);

                // TODO: View Frustum Culling. Temporary nonsense conditional.
                if (distance > MaxDistance || !InFrustum(cam.Frustum, transform.Translation))
                    continue;

                // If not culled, identify the specific mesh to be used based on LOD
                // TODO: Improve LOD selection to logarithmic distance or something
                float x = LodBias * 2.0f * distance / MaxDistance; // Base function
                x = std::clamp(x, 0.0f, 1.0f);                     // Clamp between 0.0 and 1.0
                x = x * (model.Meshes.size() - 1);                 // Scale to number of LODs
                int meshId = (int)std::floor(x);

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

        Profiler::Stop(profTimeStamp, registry);
    }

    bool InFrustum(RLFrustum* frustum, Vector3 point)
    {
        return frustum->SphereIn(point, 10.0f);
        //return frustum->PointIn(point);
    }

    static void DebugGuiCallback()
    {
        ImGui::Checkbox("Update Frustum", &UpdateFrustum);
        ImGui::Checkbox("Automatic LOD", &AutomaticBias);
        ImGui::InputFloat("LOD Bias", &LodBias);
        ImGui::InputFloat("Target FPS", &TargetFps);
        ImGui::Text("Average FPS: %d", (int)AverageFps);
    }
};