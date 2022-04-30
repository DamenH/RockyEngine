#pragma once

#include "Engine/Core/Components/MusicComponent.h"
#include "Engine/Core/SystemBase.h"

#include <entt/entt.hpp>
#include <raylib.h>

class AudioSystem : public SystemBase
{

    void OnStartup(entt::registry &registry) override
    {
        }

    void OnUpdate(entt::registry &registry) override
    {

        // TODO understand why this is called a view and not a filter
        auto musicView = registry.view<MusicComponent>();

        for (auto entity : musicView)
        {
            auto &musicComponent = musicView.get<MusicComponent>(entity);
            Music music = AssetManager::GetMusic(musicComponent.Music);

            if (IsMusicStreamPlaying(music) && !musicComponent.Playing)
            {
                StopMusicStream(music);
                std::cout << "Stopping music";
            }

            if (!IsMusicStreamPlaying(music) && musicComponent.Playing)
            {
                PlayMusicStream(music);
                std::cout << "Starting music";
            }

            UpdateMusicStream(music);
        }
    }
};