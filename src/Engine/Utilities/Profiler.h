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

struct Measurement
{
    std::array<char, LABEL_LENGTH> label;
    std::vector<float> samples;
    float average;
};

class Profiler
{

private:
    inline static TimestampComponent *currentFrameTimestamp;
    inline static std::vector<std::tuple<char *, DebugCallback>> DebugGuiCallbacks;
    inline static std::unordered_map<unsigned int, Measurement> Measurements;

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
            float delta = std::chrono::duration_cast<std::chrono::microseconds>(timestamp.stopTimestamp - timestamp.startTimestamp).count();

            Measurement m = Measurements[entt::hashed_string::value(timestamp.label.data())];
            m.label = timestamp.label;
            m.samples.push_back(delta);
            if (m.samples.size() > 240)
            {
                std::cout << "Truncating " << m.label.data() << "\n";
                while (m.samples.size() > 120)
                {
                    m.samples.erase(m.samples.begin());
                }
            }
            m.average = ((99.0f * m.average) + delta) / 100.0f;
            Measurements[entt::hashed_string::value(timestamp.label.data())] = m;

            registry.erase<TimestampComponent>(entity);
        }
    }

    static void Initialize()
    {
        Debug::RegisterCallback((char *)"Profiler", &DebugGuiCallback);
    }

    static void DebugGuiCallback()
    {
        ImGui::Text("Measurements: %d", (int)Measurements.size());
        auto FrameMeasurement = Measurements[entt::hashed_string::value("Frame")];
        for (const auto &[key, value] : Measurements)
        {
            ImGui::Text("%*d%% %s = %dus, avg %dus", 3, (int)(100.0f * value.average / FrameMeasurement.average), value.label.data(), (int)value.samples.back(), (int)value.average);
        }
    }
};