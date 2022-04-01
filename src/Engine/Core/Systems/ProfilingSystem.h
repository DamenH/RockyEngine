#pragma once

#include <iostream>

#include "Engine/Core/SystemBase.h"
#include "Engine/Utilities/Profiler.h"

class ProfilingSystem : public SystemBase {


    void OnStartup(entt::registry &registry) override {

    }

    void OnUpdate(entt::registry &registry) override {
        Profiler::Stop("Frame", registry); // I hate this.

        // If the frame is not divisible by 128, return;
        if (((Profiler::CurrentFrame() >> 9) << 9) != Profiler::CurrentFrame()) return;

        auto timestampView = registry.view<TimestampComponent>();

        for (auto entity: timestampView) {
            auto &timestamp = timestampView.get<TimestampComponent>(entity);

            if (timestamp.frame != Profiler::CurrentFrame()) continue;
            std::string str_label(timestamp.label.data());

            if (str_label == "Frame") {
                if (timestamp.type == start) { std::cout << "=== Start Frame: "; }
                else { std::cout << "=== End Frame: "; }
                std::cout << timestamp.frame << " ===" << std::endl;
                continue;
            }

        }
    }
};