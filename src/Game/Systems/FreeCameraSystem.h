#pragma once

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/InputComponent.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/Components/TranformComponent.h"

#include <raymath.h>
#include <entt/entt.hpp>

class FreeCameraSystem : public SystemBase {

    const float CAMERA_ROT_RATE = 0.025;

    void OnStartup(entt::registry &registry) override {
    }

    void OnUpdate(entt::registry &registry) override {

        // TODO understand why this is called a view and not a filter
        auto inputView = registry.view<InputComponent>();
        auto &input = inputView.get<InputComponent>(inputView[0]);

        auto cameraView = registry.view<CameraComponent, TransformComponent>();

        for (auto cameraEntity: cameraView) {
            auto &cameraTransform = cameraView.get<TransformComponent>(cameraEntity);

//            input.ForwardPressed;
//            input.BackPressed;
//            input.LeftPressed;
//            input.RightPressed;

//            input.MouseDeltaX;
//            input.MouseDeltaY;
/*
            Ogre::Quaternion cameraOrientation = cameraTransform.orientation;
            Ogre::Quaternion yaw = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE),
                                                    Ogre::Vector3(0, CAMERA_ROT_RATE * -input.MouseDeltaX, 0.0));
            cameraOrientation = yaw * cameraOrientation;
            Ogre::Quaternion pitch = Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE),
                                                      Ogre::Vector3(CAMERA_ROT_RATE * -input.MouseDeltaY, 0.0, 0.0));
            cameraOrientation = cameraOrientation * pitch;

            cameraTransform.orientation = cameraOrientation;
*/
//            auto translationLeft = (float) ((input.RightPressed * 0.1) - (input.LeftPressed * 0.1));
//            auto translationForward = (float) ((input.BackPressed * 0.1) - (input.ForwardPressed * 0.1));
//            cameraTransform.Translation.z += .01;
//            cameraTransform.Translation.x += .01;


            Vector3 deltaPosition = {0.0f, 0.0f, 0.0f};
            Quaternion orientation = QuaternionFromEuler(cameraTransform.Rotation.x, cameraTransform.Rotation.y,
                                                         cameraTransform.Rotation.z);
            Quaternion inverse = QuaternionFromEuler(cameraTransform.Rotation.x, cameraTransform.Rotation.y,
                                                     cameraTransform.Rotation.z);

//            Vector3 forward = {2 * (orientation.x * orientation.z + orientation.w * orientation.y),
//                               2 * (orientation.y * orientation.z - orientation.w * orientation.x),
//                               1 - 2 * (orientation.x * orientation.x + orientation.y * orientation.y)};
//
//            Vector3 left = {1 - 2 * (orientation.y * orientation.y + orientation.z * orientation.z),
//                            2 * (orientation.x * orientation.y + orientation.w * orientation.z),
//                            2 * (orientation.x * orientation.z - orientation.w * orientation.y)};

            float currentSpeed = 0.01f;
            float mouseSpeed = 0.01f;

            Vector3 forward = Vector3RotateByQuaternion({0.0f, 0.0f, -1.0f}, QuaternionInvert(orientation));
            Vector3 right = Vector3RotateByQuaternion({1.0f, 0.0f, 0.0f}, QuaternionInvert(orientation));

            Vector3 up = {0.0f, 1.0f, 0.0f};


            if (input.ForwardPressed) {
                deltaPosition = Vector3Add(deltaPosition, forward);
            }
            if (input.BackPressed) {
                deltaPosition = Vector3Subtract(deltaPosition, forward);
            }
            if (input.LeftPressed) {
                deltaPosition = Vector3Subtract(deltaPosition, right);
            }
            if (input.RightPressed) {
                deltaPosition = Vector3Add(deltaPosition, right);
            }

            cameraTransform.Translation = Vector3Add(Vector3Scale(deltaPosition, currentSpeed),
                                                     cameraTransform.Translation);

            // Pitch
            cameraTransform.Rotation = QuaternionToEuler(
                    QuaternionMultiply(
                            QuaternionFromAxisAngle({1.0f, 0.0f, 0.0f}, -input.MouseDelta.y * mouseSpeed),
                            orientation)
            );
            // Yaw
            cameraTransform.Rotation = {cameraTransform.Rotation.x,
                                        cameraTransform.Rotation.y + input.MouseDelta.x * mouseSpeed,
                                        cameraTransform.Rotation.z};


        }
    }
};