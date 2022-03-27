#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/InputComponent.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/Components/TranformComponent.h"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

class FreeCameraSystem : public SystemBase {

    const float CAMERA_ROT_RATE = 0.025;

    void OnStartup(entt::registry &registry) override {
    }

    void OnUpdate(entt::registry &registry) override {
/*
        // TODO understand why this is called a view and not a filter
        auto inputView = registry.view<InputComponent>();
        auto &input = inputView.get<InputComponent>(inputView[0]);

        auto cameraView = registry.view<CameraComponent, TransformComponent>();

        for (auto cameraEntity: cameraView) {
            auto &cameraTransform = cameraView.get<TransformComponent>(cameraEntity);

            input.ForwardPressed;
            input.BackPressed;
            input.LeftPressed;
            input.RightPressed;

            input.MouseDeltaX;
            input.MouseDeltaY;
/*
            Ogre::Quaternion cameraOrientation = cameraTransform.rotation;
            Ogre::Quaternion yaw = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE),
                                                    Ogre::Vector3(0, CAMERA_ROT_RATE * -input.MouseDeltaX, 0.0));
            cameraOrientation = yaw * cameraOrientation;
            Ogre::Quaternion pitch = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE),
                                                      Ogre::Vector3(CAMERA_ROT_RATE * -input.MouseDeltaY, 0.0, 0.0));
            cameraOrientation = cameraOrientation * pitch;

            cameraTransform.rotation = cameraOrientation;
*//*
            auto translationLeft = (float) ((input.RightPressed * 0.1) - (input.LeftPressed * 0.1));
            auto translationForward = (float) ((input.BackPressed * 0.1) - (input.ForwardPressed * 0.1));
            cameraTransform.Translation.z += translationForward;
            cameraTransform.Translation.x += translationLeft;
        }*/
    }
};