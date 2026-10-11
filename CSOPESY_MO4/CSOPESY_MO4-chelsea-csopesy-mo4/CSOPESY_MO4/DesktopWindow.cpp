#include "DesktopWindow.h"

#include <algorithm>
#include <ctime>
#include <cstdio>

void DesktopWindow::render()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    const ImVec2 origin = viewport->Pos;
    const ImVec2 size = viewport->Size;
    state.viewportOrigin = origin;
    state.viewportSize = size;

    const auto now = std::chrono::steady_clock::now();
    const float elapsed =
        std::chrono::duration<float>(now - state.screenStart).count();

    if (state.screen == Screen::Boot && elapsed >= 4.0f)
    {
        state.screen = Screen::Loading;
        state.screenStart = now;
    }
    else if (state.screen == Screen::Loading && elapsed >= 3.0f)
    {
        state.screen = Screen::Desktop;
        state.screenStart = now;
    }

    if (state.screen == Screen::Boot)
    {
        draw->AddRectFilled(
            origin, ImVec2(origin.x + size.x, origin.y + size.y),
            IM_COL32(0, 0, 0, 255));

        ImGui::SetNextWindowPos(origin);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Boot Screen", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground);

        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.5f, 1.0f));
        ImGui::Text("CSOPESY Megatrends");
        ImGui::Text("Released: 12/01/94");
        ImGui::Text("DLSU GWE Lab (C)1994 CSOPESY Megatrends Inc.");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::Text("Memory Test:");
        ImGui::Text("Checking RAM : 64000K OK");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::Text("CPU Type: Pentium III");
        ImGui::Text("BIOS Version: BCN SIT 1989-1994 Special CSOPESY");
        ImGui::Text("Main Processor: Pentium III");
        ImGui::Text("Numeric Processor: Built-in");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::Text("Primary Master: 420 MB WDCAC2420H");
        ImGui::Text("Primary Slave: None");
        ImGui::Text("Secondary Master: CD-ROM LTN-305A");
        ImGui::Text("Secondary Slave: None");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::TextWrapped(
            "Fun Fact: The CPU specs are a tribute to the early "
            "gaming era. This screen simulates a classic BIOS boot.");
        ImGui::PopStyleColor();
        ImGui::End();
        return;
    }

    if (state.screen == Screen::Loading)
    {
        draw->AddRectFilled(
            origin, ImVec2(origin.x + size.x, origin.y + size.y),
            IM_COL32(0, 0, 0, 255));

        ImGui::SetNextWindowPos(origin);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Loading Screen", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground);

        const float centerX = size.x * 0.5f;
        const float centerY = size.y * 0.40f;

        ImGui::SetCursorPos(ImVec2(centerX - 190, centerY));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.75f, 0.92f, 1.0f));
        ImGui::SetWindowFontScale(2.3f);
        ImGui::Text("C S O P E S Y");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::SetCursorPos(ImVec2(centerX - 155, centerY + 55));
        ImGui::Text("CSOPESY Operating System Emulator v1.0");
        ImGui::SetCursorPos(ImVec2(centerX - 125, centerY + 80));
        ImGui::Text("Created By: CSOPESY Group 8");
        ImGui::SetCursorPos(ImVec2(centerX - 65, centerY + 120));
        ImGui::Text("Initializing system...");
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2(centerX - 120, centerY + 160));
        const float progress = std::min(elapsed / 3.0f, 1.0f);
        ImGui::ProgressBar(progress, ImVec2(240, 12));
        ImGui::End();
        return;
    }

    if (state.wallpaperTexture != 0)
    {
        draw->AddImage(
            (ImTextureID)(intptr_t)state.wallpaperTexture,
            origin, ImVec2(origin.x + size.x, origin.y + size.y),
            ImVec2(0, 0), ImVec2(1, 1));
    }
    else
    {
        draw->AddRectFilled(
            origin, ImVec2(origin.x + size.x, origin.y + size.y),
            IM_COL32(25, 55, 90, 255));
        draw->AddText(
            ImVec2(origin.x + 20, origin.y + 20),
            IM_COL32(255, 255, 255, 255),
            "Wallpaper could not be loaded. Check assets/wallpaper.jpg.");
    }

    draw->AddText(
        ImVec2(origin.x + 20, origin.y + size.y - 75),
        IM_COL32(0, 255, 120, 255),
        "CSOPESY OS v1.0 - Desktop Shell");

    ImGui::SetNextWindowPos(ImVec2(origin.x + 18, origin.y + 25), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(95, 85), ImGuiCond_Always);
    ImGui::Begin("Desktop Icons", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings);

    if (ImGui::Button("FILES", ImVec2(75, 35)))
        state.showFiles = true;
    ImGui::Dummy(ImVec2(0, 5));
    if (ImGui::Button("ABOUT", ImVec2(75, 35)))
        state.showAbout = true;
    ImGui::End();

    std::time_t currentTime = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &currentTime);
    char clockText[80]{};
    std::strftime(clockText, sizeof(clockText),
        "%A, %b %d, %Y | %I:%M:%S %p", &localTime);

    ImGui::SetNextWindowPos(
        ImVec2(origin.x + size.x - 290, origin.y + 8), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280, 32), ImGuiCond_Always);
    ImGui::Begin("Clock", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("%s", clockText);
    ImGui::End();

    ImGui::SetNextWindowPos(
        ImVec2(origin.x, origin.y + size.y - 62), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(size.x, 62), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.13f, 0.98f));
    ImGui::Begin("Taskbar", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings);

    if (ImGui::Button("FILES", ImVec2(70, 40)))
        state.showFiles = !state.showFiles;
    ImGui::SameLine();
    if (ImGui::Button("SETTINGS", ImVec2(85, 40)))
        state.showSettings = !state.showSettings;
    ImGui::SameLine();
    if (ImGui::Button("TASK MANAGER", ImVec2(125, 40)))
        state.showTaskManager = !state.showTaskManager;
    ImGui::SameLine();
    if (ImGui::Button("ABOUT", ImVec2(70, 40)))
        state.showAbout = !state.showAbout;
    ImGui::SameLine();
    ImGui::SetCursorPosX(std::max(ImGui::GetCursorPosX(), size.x - 100.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.12f, 0.15f, 1.0f));
    if (ImGui::Button("PWR", ImVec2(65, 40)))
        state.running = false;
    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::PopStyleColor();

    if (state.showFiles)
    {
        ImGui::SetNextWindowSize(ImVec2(440, 320), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("File Explorer", &state.showFiles))
        {
            ImGui::Text("This PC");
            ImGui::Separator();
            ImGui::BulletText("Local Disk (C:)");
            ImGui::BulletText("Documents");
            ImGui::BulletText("Downloads");
            ImGui::BulletText("Pictures");
            ImGui::BulletText("Desktop");
            ImGui::Spacing();
            ImGui::TextDisabled("This is a simulated file explorer.");
        }
        ImGui::End();
    }

    if (state.showSettings)
    {
        ImGui::SetNextWindowSize(ImVec2(430, 310), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Settings", &state.showSettings))
        {
            ImGui::Text("System Settings");
            ImGui::Separator();
            ImGui::Text("Display");
            ImGui::Text("Resolution: %d x %d",
                static_cast<int>(size.x), static_cast<int>(size.y));
            ImGui::Spacing();
            ImGui::Text("Personalization");
            ImGui::TextWrapped(
                "Wallpaper: Custom image loaded from assets/wallpaper.jpg.");
            ImGui::Spacing();
            ImGui::Text("System");
            ImGui::Text("Operating System: CSOPESY OS v1.0");
            ImGui::Text("Interface: Dear ImGui");
            ImGui::Text("Renderer: OpenGL");
        }
        ImGui::End();
    }

    if (state.showAbout)
    {
        ImGui::SetNextWindowSize(ImVec2(400, 240), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("About CSOPESY", &state.showAbout))
        {
            ImGui::Text("CSOPESY Desktop OS Emulator");
            ImGui::Separator();
            ImGui::Text("Version: 1.0");
            ImGui::Text("Project: Semi-Major Output 2");
            ImGui::Text("Interface: Dear ImGui");
            ImGui::Text("Graphics: GLFW + OpenGL");
            ImGui::Spacing();
            ImGui::TextWrapped(
                "A desktop-style operating system mock-up "
                "created for the CSOPESY project.");
        }
        ImGui::End();
    }
}
