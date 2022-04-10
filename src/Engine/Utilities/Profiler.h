#pragma once

#include "Debug.h"
#include "imgui.h"
#include "implot.h"

#include <cstdint>
#include <chrono>
#include <entt/entt.hpp>

#define LABEL_LENGTH 32
#define SAMPLE_LENGTH 1024

struct TimestampComponent
{
    std::array<char, LABEL_LENGTH> label;
    std::chrono::high_resolution_clock::time_point startTimestamp;
    std::chrono::high_resolution_clock::time_point stopTimestamp;
};

struct Measurement
{
    std::array<char, LABEL_LENGTH> label;
    std::vector<int> samples;
    std::vector<int> sampleIndices;
    int sampleCount;
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
            m.samples.push_back((int)delta);
            if (m.sampleIndices.size() < SAMPLE_LENGTH)
            {
                m.sampleIndices.push_back(m.sampleCount++);
            }
            while (m.samples.size() > SAMPLE_LENGTH)
            {
                m.samples.erase(m.samples.begin());
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
        auto FrameMeasurement = Measurements[entt::hashed_string::value("Frame")];
        if (ImPlot::BeginPlot("Execution Time (us)"))
        {
            for (const auto &[key, value] : Measurements)
            {
                if (value.samples.size() > 0)
                {
                    /*ImGui::Text("%*d%% %s = %dus, avg %dus",
                                3, (int)(100.0f * value.average / FrameMeasurement.average),
                                value.label.data(),
                                (int)value.samples.back(),
                                (int)value.average);*/

                    ImPlot::PlotLine(value.label.data(), value.sampleIndices.data(), value.samples.data(), value.sampleIndices.size());
                }
            }
            ImPlot::EndPlot();
        }
    }
};
