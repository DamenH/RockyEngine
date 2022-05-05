#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/Components/InputComponent.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/SystemBase.h"

#include "imgui.h"
#include <entt/entt.hpp>

#define PLAYER_MOVEMENT_SENSITIVITY 20.0f

#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.003f

#define CAMERA_FIRST_PERSON_MIN_CLAMP 89.0f
#define CAMERA_FIRST_PERSON_MAX_CLAMP -89.0f

class FreeCameraSystem : public SystemBase
{

    const float CAMERA_ROT_RATE = 0.025;

    void OnStartup(entt::registry &registry) override
    {

        RLFrustum *cameraFrustum = new RLFrustum;

        auto cameraEntity = registry.create();
        registry.emplace<TransformComponent>(
            cameraEntity,
            TransformComponent{Vector3{0.0f, 10.0f, 0.0f}, Vector3{0, 0, 0}, Vector3{1.0, 1.0, 1.0}, MatrixIdentity()});

        glm::mat4 m4(1.0f);

        registry.emplace<CameraComponent>(
            cameraEntity, CameraComponent{60.0f,
                                          RenderTarget{Rectangle{0, 0, 1920, -1080}, Rectangle{0, 0, 1920, 1080},
                                                       Vector2{0, 0}, 0.0f, WHITE},
                                          cameraFrustum});

        Debug::RegisterCallback("FreeCameraSystem", &DebugGuiCallback);
    }

    void OnUpdate(entt::registry &registry) override
    {

        auto cameraView = registry.view<CameraComponent, TransformComponent>();
        auto cameraEntity = cameraView.front();
        auto &camera = cameraView.get<CameraComponent>(cameraEntity);
        auto &transform = cameraView.get<TransformComponent>(cameraEntity);

        auto inputView = registry.view<InputComponent>();
        auto inputEntity = inputView.front();
        auto &input = inputView.get<InputComponent>(inputEntity);

        //        camera.angle.x += (input.MousePositionDelta.x * -CAMERA_MOUSE_MOVE_SENSITIVITY);
        //        camera.angle.y += (input.MousePositionDelta.y * -CAMERA_MOUSE_MOVE_SENSITIVITY);
        //
        //        // Angle clamp
        //        if (camera.angle.y > CAMERA_FIRST_PERSON_MIN_CLAMP * DEG2RAD)
        //            camera.angle.y = CAMERA_FIRST_PERSON_MIN_CLAMP * DEG2RAD;
        //        else if (camera.angle.y < CAMERA_FIRST_PERSON_MAX_CLAMP * DEG2RAD)
        //            camera.angle.y = CAMERA_FIRST_PERSON_MAX_CLAMP * DEG2RAD;

        //        transform.Translation.x += (sinf(0) * (float) input.BackPressed -
        //                                    sinf(0) * (float) input.ForwardPressed -
        //                                    cosf(0) * (float) input.LeftPressed +
        //                                    cosf(0) * (float) input.RightPressed) / PLAYER_MOVEMENT_SENSITIVITY;
        //
        //        transform.Translation.y += (sinf(0) * (float) input.ForwardPressed -
        //                                    sinf(0) * (float) input.BackPressed +
        //                                    1.0f * (float) input.UpPressed - 1.0f * (float) input.DownPressed) /
        //                                   PLAYER_MOVEMENT_SENSITIVITY;
        //
        //        transform.Translation.z += (cosf(0) * (float) input.BackPressed -
        //                                    cosf(0) * (float) input.ForwardPressed +
        //                                    sinf(0) * (float) input.LeftPressed -
        //                                    sinf(0) * (float) input.RightPressed) / PLAYER_MOVEMENT_SENSITIVITY;

        glm::quat rotation = glm::quat(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));
        //        auto rotation = glm::eulerAngleXYX(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z);

        auto const mouseSensitivity = 0.001f;

        rotation =
            rotation * glm::angleAxis(input.MousePositionDelta.y * mouseSensitivity, glm::vec3(1.0f, 0.0f, 0.0f));

        auto euler = eulerAngles(rotation);

        rotation =
            glm::quat(glm::vec3(euler.x, euler.y + (input.MousePositionDelta.x * mouseSensitivity * -1), euler.z));

        euler = eulerAngles(rotation);

        transform.Rotation = {euler.x, euler.y, euler.z};

        //        camera.target = {
        //                transform.Translation.x + 1,
        //                transform.Translation.y,
        //                transform.Translation.z
        //        };

        //      float Translate = 0.1f;
        //      glm::vec2 Rotate = {0.1f, 0.1f};
        //
        //      glm::mat4 Projection =
        //          glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f,
        //          100.f);
        //      glm::mat4 View =
        //          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f,
        //          -Translate));
        //      View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
        //      View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
        //      glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        //      glm::mat4 MVP = Projection * View * Model;
        //    }

        //                    input.ForwardPressed;
        //                    input.BackPressed;
        //                    input.LeftPressed;
        //                    input.RightPressed;
        //
        //                    input.MouseDeltaX;
        //                    input.MouseDeltaY;
        /*
                    Ogre::Quaternion cameraOrientation = cameraTransform.rotation;
                    Ogre::Quaternion yaw =
        Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE), Ogre::Vector3(0,
        CAMERA_ROT_RATE * -input.MouseDeltaX, 0.0)); cameraOrientation = yaw *
        cameraOrientation; Ogre::Quaternion pitch =
        Ogre::Quaternion(Ogre::Radian(CAMERA_ROT_RATE),
                                                              Ogre::Vector3(CAMERA_ROT_RATE
        * -input.MouseDeltaY, 0.0, 0.0)); cameraOrientation = cameraOrientation *
        pitch;

                    cameraTransform.rotation = cameraOrientation;
        *//*
            auto translationLeft = (float) ((input.RightPressed * 0.1) - (input.LeftPressed * 0.1));
            auto translationForward = (float) ((input.BackPressed * 0.1) - (input.ForwardPressed * 0.1));
            cameraTransform.Translation.z += translationForward;
            cameraTransform.Translation.x += translationLeft;
        }*/
    }

    static void DebugGuiCallback()
    {
        ImGui::Text("Value: %d", 0);
    }
};