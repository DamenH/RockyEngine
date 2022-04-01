#pragma once

#include <cstdint>
#include <chrono>
#include <entt/entt.hpp>

enum timestamp_type {
    start, stop
};

struct TimestampComponent {
    std::array<char, 16> label;
    uint32_t frame;
    std::chrono::high_resolution_clock::time_point timestamp;
    timestamp_type type;
};

class Profiler {

private:

    inline static uint32_t currentFrame = 0;

public:

    static void Start(char const *label, entt::registry &registry) {
        auto timestamp = std::chrono::high_resolution_clock::now();

        if (label == "Frame") currentFrame++;

        std::array<char, 16> _label;
        for (size_t i = 0; i < _label.size(); i++) {
            _label[i] = label[i];
            if (label[i] == '\0') break;
        }

        const auto entity = registry.create();
        TimestampComponent timestampComponent = {_label, currentFrame, timestamp, start};
        registry.emplace<TimestampComponent>(entity, timestampComponent);
    }

    static void Stop(char const *label, entt::registry &registry) {
        auto timestamp = std::chrono::high_resolution_clock::now();

        std::array<char, 16> _label;
        for (size_t i = 0; i < _label.size(); i++) {
            _label[i] = label[i];
            if (label[i] == '\0') break;
        }

        const auto entity = registry.create();
        TimestampComponent timestampComponent = {_label, currentFrame, timestamp, stop};
        registry.emplace<TimestampComponent>(entity, timestampComponent);
    }

    static uint32_t CurrentFrame() {
        return currentFrame;
    }

};

