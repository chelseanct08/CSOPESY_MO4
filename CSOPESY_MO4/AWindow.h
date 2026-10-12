#pragma once
#pragma once

#include <chrono>
#include <string>
#include <vector>
#include "imgui.h"

enum class Screen
{
    Boot,
    Loading,
    Desktop
};

struct Process
{
    std::string name;
    std::string status;
    float cpu;
    float memory;
};

struct AppState
{
    Screen screen = Screen::Boot;
    std::chrono::steady_clock::time_point screenStart =
        std::chrono::steady_clock::now();

    bool showFiles = false;
    bool showSettings = false;
    bool showTaskManager = false;
    bool showAbout = false;
    bool running = true;

    unsigned int wallpaperTexture = 0;
    ImVec2 viewportOrigin = ImVec2(0.0f, 0.0f);
    ImVec2 viewportSize = ImVec2(1280.0f, 800.0f);

    std::vector<Process> processes =
    {
        { "DOSBox-X", "Active", 0.7f, 12.4f },
        { "Visual Studio 2026", "Active", 1.3f, 18.6f },
        { "Google Chrome", "Idle",   0.2f,  8.1f },
        { "Minecraft", "Active", 0.9f, 15.2f },
        { "File Explorer", "Idle",   0.3f,  9.7f },
        { "Spotify", "Active", 1.1f, 21.8f }
    };
};

class AWindow
{
public:
    explicit AWindow(AppState& appState) : state(appState) {}
    virtual ~AWindow() = default;

    virtual void render() = 0;

protected:
    AppState& state;
};
