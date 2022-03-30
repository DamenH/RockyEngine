#pragma once

#include <stdint.h>
#include <chrono>
#include <entt/entt.hpp>
#include <map>

//typedef std::chrono::high_resolution_clock::time_point time_point;
typedef std::chrono::high_resolution_clock Clock;
enum timestamp_type {
    start, stop
};

struct TimestampComponent {
    uint16_t identifier_key;
    uint32_t frame;
    std::chrono::high_resolution_clock::time_point timestamp;
    timestamp_type type;
};

class Profiler {

private:

    inline static uint32_t currentFrame = 0;
    inline static std::map<uint16_t, char[16]> identifier_key_map = {};

public:

    static void Begin(char identifier[16], entt::registry &registry) {
        auto timestamp = Clock::now();
        const auto entity = registry.create();
        registry.emplace<TimestampComponent>(entity, 0, currentFrame, timestamp, start);
    }

    static void End(char identifier[16], entt::registry &registry) {
        std::string _identifier(identifier);

        if (_identifier.compare("Frame") == 0) {
            currentFrame++;
        }

        auto timestamp = Clock::now();
        const auto entity = registry.create();
        registry.emplace<TimestampComponent>(entity, 0, currentFrame, timestamp, stop);

    }

    static uint32_t getCurrentFrame() {
        return currentFrame;
    }

};

