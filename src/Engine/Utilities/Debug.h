#pragma once

#include "rlImGui.h"
#include "imgui.h"
#include <vector>

typedef void (*DebugCallback)(void);

class Debug
{
    // Store the callback function pointers with friendly labels
    inline static std::vector<std::tuple<char *, DebugCallback>> DebugGuiCallbacks;

public:
    // Registers a callback with label
    inline static void RegisterCallback(char *label, DebugCallback callback)
    {
        DebugGuiCallbacks.push_back(std::make_tuple(label, callback));
        std::cout << "Registered debug callback for " << label << "\n";
    }

    // Initialize the debug system.
    // MUST be called before Debug::Draw() and after the OpenGL context is created.
    inline static void Initialize()
    {
        rlImGuiSetup(true);
    }

    // Starts an imGUI session and invokes all the callbacks in turn
    inline static void Draw()
    {
        rlImGuiBegin();

            for (auto &callback : DebugGuiCallbacks)
            {
                ImGui::Begin(get<0>(callback));
                get<1>(callback)();
                ImGui::End();
            }

            rlImGuiEnd();
    }
};