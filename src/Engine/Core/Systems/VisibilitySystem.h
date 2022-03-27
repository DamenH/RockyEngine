#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/Components/CameraComponent.h"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>

class VisibilitySystem : public SystemBase {
    float fps = 60;
    int TargetFps = 120;
    float LodBias = 100.0f;

    bool InFrustum(TransformComponent transform)
    {
        return transform.Translation.x > 0 && transform.Translation.z > 0;
    }

    void OnStartup(entt::registry &registry) override {
    }

    void OnUpdate(entt::registry &registry) override {
        auto transformView = registry.view<ModelComponent, TransformComponent, VisibilityComponent>();
        auto cameraView = registry.view<CameraComponent, TransformComponent>();

        for (auto entity: transformView) {
            auto &transform = transformView.get<TransformComponent>(entity);
            auto &model = transformView.get<ModelComponent>(entity);
            auto &visibility = transformView.get<VisibilityComponent>(entity);

            visibility.Level = 2;

            if(!InFrustum(transform))
            {
                visibility.Level = -1;
                continue;
            }

            if(Vector3Distance(transform.Translation, (Vector3){0,0,0}) < (1.0f * LodBias))
            {
                visibility.Level = 0;
                continue;
            }

            if(Vector3Distance(transform.Translation, (Vector3){0,0,0}) < (2.0f * LodBias))
            {
                visibility.Level = 1;
                continue;
            }

            if(Vector3Distance(transform.Translation, (Vector3){0,0,0}) < (5.0f * LodBias))
            {
                visibility.Level = 2;
                continue;
            }

            visibility.Level = 3;
        }

        fps = ((3.0f * fps) + GetFPS()) / 4.0f;
        if (fps < TargetFps)
        {
            LodBias *= 0.999f;
            //std::cout << "FPS:" << fps <<"/"<<TargetFps << "\t" << "v" << LodBias << "\n";
        }
        if (fps > TargetFps + 10)
        {
            LodBias *= 1.001f;
            //std::cout << "FPS:" << fps <<"/"<<TargetFps << "\t" << "^" << LodBias << "\n";
        }
    }
};