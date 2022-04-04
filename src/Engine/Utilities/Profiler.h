#pragma once

#include <cstdint>
#include <chrono>
#include <entt/entt.hpp>

struct TimestampComponent {
    std::array<char, 16> label;
    std::chrono::high_resolution_clock::time_point startTimestamp;
    std::chrono::high_resolution_clock::time_point stopTimestamp;
};

class Profiler {

private:
    static TimestampComponent *currentFrameTimestamp;

public:
    static TimestampComponent *Start(char const *label, entt::registry &registry) {
        auto startTimestamp = std::chrono::high_resolution_clock::now();

        std::array<char, 16> _label;
        for (size_t i = 0; i < _label.size(); i++) {
            _label[i] = label[i];
            if (label[i] == '\0')
                break;
        }

        const auto entity = registry.create();

        auto timestamp = &registry.emplace<TimestampComponent>(entity, _label, startTimestamp, startTimestamp);

        if (label == "Frame")
            currentFrameTimestamp = timestamp;

        return timestamp;
    }

    static void Stop(char const *label, TimestampComponent *timestampComponent) {
        auto stopTimestamp = std::chrono::high_resolution_clock::now();

        timestampComponent->stopTimestamp = stopTimestamp;
    }

    static TimestampComponent *CurrentFrameTimestamp() {
        return currentFrameTimestamp;
    }
};