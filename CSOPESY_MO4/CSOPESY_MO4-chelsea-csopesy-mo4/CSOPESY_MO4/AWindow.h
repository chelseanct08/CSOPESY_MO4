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
        { "System",          "Running", 1.2f, 120.0f },
        { "Desktop Shell",   "Running", 3.8f,  85.5f },
        { "File Explorer",   "Running", 0.7f,  42.0f },
        { "Settings",        "Running", 0.4f,  35.2f },
        { "Task Manager",    "Running", 0.9f,  28.4f },
        { "OpenGL Renderer",  "Running", 2.1f,  64.8f }
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
