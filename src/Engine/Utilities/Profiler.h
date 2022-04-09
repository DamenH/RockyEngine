#pragma once

#include "Debug.h"
#include "imgui.h"

#include <cstdint>
#include <chrono>
#include <entt/entt.hpp>

#define LABEL_LENGTH 32

struct TimestampComponent
{
    std::array<char, LABEL_LENGTH> label;
    std::chrono::high_resolution_clock::time_point startTimestamp;
    std::chrono::high_resolution_clock::time_point stopTimestamp;
};

class Profiler
{

private:
    inline static TimestampComponent *currentFrameTimestamp;
    inline static std::vector<std::tuple<char *, DebugCallback>> DebugGuiCallbacks;
    inline static std::unordered_map<unsigned int, std::tuple<char *, float, float>> Measurements;

public:
    static TimestampComponent *Start(char const *label, entt::registry &registry)
    {
        auto startTimestamp = std::chrono::high_resolution_clock::now();

        std::array<char, LABEL_LENGTH> _label;
        for (size_t i = 0; i < LABEL_LENGTH; i++)
        {
            _label[i] = label[i];
            if (label[i] == '\0')
                break;
        }
        _label[LABEL_LENGTH - 1] = '\0';

        const auto entity = registry.create();

        auto timestamp = &registry.emplace<TimestampComponent>(entity, _label, startTimestamp, startTimestamp);

        if (strcmp(_label.data(), "Frame") == 0)
        {
            currentFrameTimestamp = timestamp;
        }

        return timestamp;
    }

    static void Stop(TimestampComponent *timestampComponent, entt::registry &registry)
    {
        auto stopTimestamp = std::chrono::high_resolution_clock::now();

        timestampComponent->stopTimestamp = stopTimestamp;
        if (strcmp(timestampComponent->label.data(), "Frame") == 0)
        {
            Analyze(registry);
        }
    }

    static TimestampComponent *CurrentFrameTimestamp()
    {
        return currentFrameTimestamp;
    }

    static void Analyze(entt::registry &registry)
    {
        auto timestampView = registry.view<TimestampComponent>();

        for (auto entity : timestampView)
        {
            auto &timestamp = timestampView.get<TimestampComponent>(entity);
            auto delta = std::chrono::duration_cast<std::chrono::microseconds>(timestamp.stopTimestamp - timestamp.startTimestamp).count();
            float average = get<2>(Measurements[entt::hashed_string::value(timestamp.label.data())]);
            average = ((119.0f * average) + delta) / 120.0f;
            Measurements[entt::hashed_string::value(timestamp.label.data())] = std::make_tuple(timestamp.label.data(), delta, average);
            registry.erase<TimestampComponent>(entity);
        }
    }

    static void Initialize()
    {
        Debug::RegisterCallback((char*)"Profiler", &DebugGuiCallback);
    }

    static void DebugGuiCallback()
    {
        ImGui::Text("Measurements: %d", (int)Measurements.size());
        auto FrameMeasurement = Measurements[entt::hashed_string::value("Frame")];
        for (const auto &[key, value] : Measurements)
        {
            ImGui::Text("%*d%% %s = %dus, avg %dus", 3,(int)(100.0f * get<2>(value) / get<2>(FrameMeasurement)), get<0>(value), (int)get<1>(value), (int)get<2>(value));
        }
    }
};