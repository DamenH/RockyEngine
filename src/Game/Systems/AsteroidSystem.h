#pragma once

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>

#include <entt/entt.hpp>
#include <iostream>
#include <vector>

#include "Engine/Core/AssetManager.h"
#include "Engine/Core/Components/CameraComponent.h"
#include "Engine/Core/Components/InstanceBinComponent.h"
#include "Engine/Core/Components/ModelComponent.h"
#include "Engine/Core/Components/MusicComponent.h"
#include "Engine/Core/Components/TranformComponent.h"
#include "Engine/Core/Components/VisibilityComponent.h"
#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Systems/GraphicsSystem.h"
#include "Engine/Core/Systems/VisibilitySystem.h"
#include "Engine/Utilities/Profiler.h"
#include "RLFrustum.h"

struct Planet {
    Vector3 Transform{0.0f, -100.0f, 0.0f};
    float Mass = 1e12;
} planet;

struct AsteroidComponent {
    Vector3 Rotation;
    Vector3 Velocity;
    float Mass = 10;
};

double G = 6.673e-11;

class AsteroidSystem : public SystemBase {
    Camera3D camera;
    RLFrustum cameraFrustum;

    void OnStartup(entt::registry &registry) override {
        std::cout << "Generating scene\n";
        uint32_t asteroidCount = 4000; // Number of asteroids
#ifndef NDEBUG
        asteroidCount = 500.0f; // Number of asteroids
#endif
        float scalar = log((float) asteroidCount) * 150.0f; // Scale range of locations for even density
        for (int i = 0; i < asteroidCount; i++) {
            auto asteroidEntity = registry.create();

            // Location within a ring
            float radius = ((((rand() - rand()) / (float) RAND_MAX) * (scalar / 4.0f)) + 500.0f);
            float theta = ((rand() / (float) RAND_MAX) * 2.0f * PI);

            auto transform = TransformComponent{
                    Vector3{radius * cos(theta), (((rand() - rand()) / (float) RAND_MAX) * scalar * 0.01f) - 100.0f,
                            radius * sin(theta)},
                    Vector3{((rand() - rand()) / (float) RAND_MAX) * 2.0f * PI,
                            ((rand() - rand()) / (float) RAND_MAX) * 2.0f * PI,
                            ((rand() - rand()) / (float) RAND_MAX) * 2.0f * PI},
                    Vector3{0.5f + (1.0f * rand() / (float) RAND_MAX), 0.5f + (1.0f * rand() / (float) RAND_MAX),
                            0.5f + (1.0f * rand() / (float) RAND_MAX)},
                    MatrixIdentity()};

            float rsqr_radius = (float) (sqrt((G * (double) planet.Mass) / (double) (radius * 7)));

            /*if (i % 2 == 0)
            {
                rsqr_radius = -1.0f * rsqr_radius;
            }*/

            Vector3 startingVelocity =
                    Vector3Scale(Vector3Perpendicular(Vector3Normalize(Vector3{
                                         transform.Translation.x, transform.Translation.y + 10.0f, transform.Translation.z})),
                                 rsqr_radius);

            if (startingVelocity.y > 0.01f || startingVelocity.y < -0.01f)
                continue;

            registry.emplace<TransformComponent>(asteroidEntity, transform);
            // Attach a Model
            std::vector<int> meshIds = {0, 1, 2, 3};
            int materialId = 4;
            registry.emplace<ModelComponent>(asteroidEntity, ModelComponent{meshIds, materialId});

            // Attach a Visibility
            registry.emplace<VisibilityComponent>(asteroidEntity, VisibilityComponent{});

            registry.emplace<AsteroidComponent>(
                    asteroidEntity, AsteroidComponent{Vector3{((rand() - rand()) / (float) RAND_MAX) * 0.01f,
                                                              ((rand() - rand()) / (float) RAND_MAX) * 0.01f,
                                                              ((rand() - rand()) / (float) RAND_MAX) * 0.01f},
                                                      startingVelocity});
        }

//        auto cameraEntity = registry.create();
//        registry.emplace<TransformComponent>(
//            cameraEntity,
//            TransformComponent{Vector3{0.0f, 10.0f, 0.0f}, Vector3{0, 0, 0}, Vector3{1.0, 1.0, 1.0}, MatrixIdentity()});
//
//        registry.emplace<CameraComponent>(
//            cameraEntity, CameraComponent{60.0f, Matrix(),
//                                          RenderTarget{Rectangle{0, 0, 1920, -1080}, Rectangle{0, 0, 1920, 1080},
//                                                       Vector2{0, 0}, 0.0f, WHITE},
//                                          &cameraFrustum});
    }

    inline static float ManhattanDistance(Vector3 a, Vector3 b) {
        return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
    }

    uint8_t entityCount = 0;

    void OnUpdate(entt::registry &registry) override {
        return;
        auto profTimeStamp = Profiler::Start("AsteroidSystem", registry);
        auto view = registry.view<TransformComponent, AsteroidComponent>();
        auto collisionView = registry.view<TransformComponent, AsteroidComponent>();

        //        const float g = -1.0f; // gravitational acceleration

        const float deltaTime = 1.0f / 60.0f;

        for (auto entity: view) {
            auto &transform = view.get<TransformComponent>(entity);
            auto &asteroid = view.get<AsteroidComponent>(entity);

            transform.Rotation.x += asteroid.Rotation.x;
            transform.Rotation.y += asteroid.Rotation.y;
            transform.Rotation.z += asteroid.Rotation.z;

            float r = sqrtf((transform.Translation.x * transform.Translation.x) +
                            (transform.Translation.y * transform.Translation.y) +
                            (transform.Translation.z * transform.Translation.z));
            if (r < 10.0f) {
                r = 10.0f;
            }
            float F_g = (G * planet.Mass * asteroid.Mass) / (r * r); // gravitational acceleration

            Vector3 deltaV =
                    Vector3Scale(Vector3Normalize(Vector3{transform.Translation.x, transform.Translation.y + 100.0f,
                                                          transform.Translation.z}),
                                 F_g * deltaTime * -1);

            // calculate new velocity
            asteroid.Velocity.x += deltaV.x;
            asteroid.Velocity.y += deltaV.y;
            asteroid.Velocity.z += deltaV.z;

            // calculate new position
            transform.Translation.x += asteroid.Velocity.x;
            transform.Translation.y += asteroid.Velocity.y;
            transform.Translation.z += asteroid.Velocity.z;

            // All-to-All collision checking
            if (entityCount % 7 == 0) {
                for (auto otherEntity: collisionView) {
                    auto &otherTransform = collisionView.get<TransformComponent>(otherEntity);
                    auto &otherAsteroid = collisionView.get<AsteroidComponent>(otherEntity);
                    float d = ManhattanDistance(transform.Translation, otherTransform.Translation);

                    if (d < 10.0f && d != 0.0f) {
                        // Manhatten translates to AABB collisions.
                        // Do a second check of Euclidean distance to simulate
                        // spherical collisions.
                        d = Vector3Distance(transform.Translation, otherTransform.Translation);

                        // Impulse force pushes the body away from whatever it is
                        // colliding with. The magnitude of the impulse is directly
                        // proportional to the relative velocity of the other body.
                        Vector3 impulse = Vector3Subtract(transform.Translation, otherTransform.Translation);
                        impulse = Vector3Normalize(impulse);
                        impulse = Vector3Scale(
                                impulse,
                                Vector3Length(Vector3Subtract(asteroid.Velocity, otherAsteroid.Velocity)) * 0.5f / d);

                        asteroid.Velocity = Vector3Add(asteroid.Velocity,
                                                       impulse); // Velocity is gained from one body
                        otherAsteroid.Velocity = Vector3Subtract(otherAsteroid.Velocity,
                                                                 impulse); // ...and lost from the other
                    }
                }
            }
            entityCount++;
        }
        Profiler::Stop(profTimeStamp, registry);
    }
};