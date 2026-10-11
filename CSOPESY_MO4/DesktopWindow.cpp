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
        ImGui::Text("CSOPESY UEFI Firmware Interface");
        ImGui::Text("CSOPESY Desktop OS Emulator | Version 1.0");
        ImGui::Text("Firmware Build: 2026.10 | Boot Mode: UEFI");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::Text("System Diagnostics:");
        ImGui::Text("System Memory: 16 GB DDR4 ................ OK");
        ImGui::Text("Processor Check: x64 Multi-Core .......... OK");
        ImGui::Text("Graphics Adaptor: OpenGL 3.3 ............. OK");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::Text("Storage Configuration:");
        ImGui::Text("Primary Disk: Virtual SSD 512 GB .......... READY");
        ImGui::Text("Boot Device: CSOPESY Desktop Shell ........ READY");
        ImGui::Text("Secure Boot: Mock Validation ............. ENABLED");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::Text("Initializing desktop compositor .......... DONE");
        ImGui::Dummy(ImVec2(0, 25));
        ImGui::TextWrapped(
            "All startup checks completed successfully. "
            "Launching the CSOPESY desktop environment.");
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

        const float centerY = size.y * 0.32f;
        const float lineGap = 10.0f;
        const char* subtitle = "CSOPESY Operating System Emulator v1.0";
        const char* author = "Created By: CSOPESY Group 8";
        const char* status = "Initializing system...";

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.75f, 0.92f, 1.0f));
        ImGui::SetWindowFontScale(2.3f);
        const ImVec2 titleSize = ImGui::CalcTextSize("C S O P E S Y");
        ImGui::SetCursorPos(ImVec2((size.x - titleSize.x) * 0.5f, centerY));
        ImGui::TextUnformatted("C S O P E S Y");
        ImGui::SetWindowFontScale(1.0f);

        const float normalLineHeight = ImGui::GetTextLineHeight();
        const float subtitleY = centerY + titleSize.y + lineGap;
        const float authorY = subtitleY + normalLineHeight + lineGap;
        const float statusY = authorY + normalLineHeight + lineGap * 2.0f;

        const ImVec2 subtitleSize = ImGui::CalcTextSize(subtitle);
        const ImVec2 authorSize = ImGui::CalcTextSize(author);
        const ImVec2 statusSize = ImGui::CalcTextSize(status);
        ImGui::SetCursorPos(ImVec2((size.x - subtitleSize.x) * 0.5f, subtitleY));
        ImGui::TextUnformatted(subtitle);
        ImGui::SetCursorPos(ImVec2((size.x - authorSize.x) * 0.5f, authorY));
        ImGui::TextUnformatted(author);
        ImGui::SetCursorPos(ImVec2((size.x - statusSize.x) * 0.5f, statusY));
        ImGui::TextUnformatted(status);
        ImGui::PopStyleColor();

        const ImGuiStyle& loadingStyle = ImGui::GetStyle();
        const float loadingContentWidth = ImGui::GetWindowWidth()
            - loadingStyle.WindowPadding.x * 2.0f;
        const float progressWidth = (loadingContentWidth - 24.0f > 280.0f)
            ? 280.0f
            : loadingContentWidth - 24.0f;
        const float progressHeight = ImGui::GetFrameHeight();
        const float requestedProgressY = statusY + normalLineHeight + 28.0f;
        const float lastProgressY = size.y - progressHeight
            - loadingStyle.WindowPadding.y;
        const float progressY = (requestedProgressY < lastProgressY)
            ? requestedProgressY
            : lastProgressY;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - progressWidth) * 0.5f);
        ImGui::SetCursorPosY(progressY);
        const float progress = std::min(elapsed / 3.0f, 1.0f);
        ImGui::ProgressBar(progress, ImVec2(progressWidth, progressHeight));
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
            "Wallpaper could not be loaded. Check assets/wallpaper.png.");
    }

    const float uiScale = ImGui::GetFontSize() / 13.0f;
    const float iconButtonWidth = 120.0f * uiScale;
    const float iconButtonHeight = 42.0f * uiScale;
    const ImGuiStyle& style = ImGui::GetStyle();

    ImGui::SetNextWindowPos(ImVec2(origin.x + 18, origin.y + 25), ImGuiCond_Always);
    ImGui::SetNextWindowSize(
        ImVec2(iconButtonWidth + style.WindowPadding.x * 2.0f,
            iconButtonHeight * 2.5f + style.WindowPadding.y * 2.0f),
        ImGuiCond_Always);
    ImGui::Begin("Desktop Icons", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings);

    ImGui::SetWindowFontScale(1.05f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.18f, 0.18f, 0.82f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f, 0.28f, 0.28f, 0.88f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.36f, 0.36f, 0.36f, 0.92f));
    if (ImGui::Button("Files", ImVec2(iconButtonWidth - 8.0f, iconButtonHeight)))
        state.showFiles = true;
    ImGui::Dummy(ImVec2(0, 6.0f * uiScale));
    if (ImGui::Button("About", ImVec2(iconButtonWidth - 8.0f, iconButtonHeight)))
        state.showAbout = true;
    ImGui::PopStyleColor(3);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();

    std::time_t currentTime = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &currentTime);
    char clockText[80]{};
    std::strftime(clockText, sizeof(clockText),
        "%A, %b %d, %Y | %I:%M:%S %p", &localTime);

    const float buttonHeight = 40.0f * uiScale;
    const float taskbarHeight = buttonHeight + style.WindowPadding.y * 2.0f + 8.0f * uiScale;
    const float taskbarTop = origin.y + size.y - taskbarHeight;
    const ImU32 taskbarColor = IM_COL32(20, 20, 32, 255);
    draw->AddRectFilled(
        ImVec2(origin.x, taskbarTop),
        ImVec2(origin.x + size.x, origin.y + size.y),
        taskbarColor);

    ImGui::SetNextWindowPos(
        ImVec2(origin.x, taskbarTop), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(size.x, taskbarHeight), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.13f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("Taskbar", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings);

    ImGui::SetWindowFontScale(1.05f);
    ImGui::SetCursorPosY((taskbarHeight - buttonHeight) * 0.5f);
    const float clockWidth = ImGui::CalcTextSize(clockText).x + 32.0f * uiScale;
    const float pwrButtonWidth = ImGui::CalcTextSize("PWR").x + 24.0f * uiScale;
    const float filesButtonWidth = ImGui::CalcTextSize("Files").x + 30.0f * uiScale;
    const float settingsButtonWidth = ImGui::CalcTextSize("Settings").x + 30.0f * uiScale;
    const float managerButtonWidth = ImGui::CalcTextSize("Task Manager").x + 32.0f * uiScale;
    const float aboutButtonWidth = ImGui::CalcTextSize("About").x + 30.0f * uiScale;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.12f, 0.15f, 1.0f));
    if (ImGui::Button("PWR", ImVec2(pwrButtonWidth, buttonHeight)))
        state.running = false;
    ImGui::PopStyleColor();

    ImGui::SameLine();
    if (ImGui::Button("Files", ImVec2(filesButtonWidth, buttonHeight)))
        state.showFiles = !state.showFiles;
    ImGui::SameLine();
    if (ImGui::Button("Settings", ImVec2(settingsButtonWidth, buttonHeight)))
        state.showSettings = !state.showSettings;
    ImGui::SameLine();
    if (ImGui::Button("Task Manager", ImVec2(managerButtonWidth, buttonHeight)))
        state.showTaskManager = !state.showTaskManager;
    ImGui::SameLine();
    if (ImGui::Button("About", ImVec2(aboutButtonWidth, buttonHeight)))
        state.showAbout = !state.showAbout;
    ImGui::SameLine();

    const float taskbarContentWidth = ImGui::GetWindowWidth()
        - style.WindowPadding.x * 2.0f;
    const float clockX = taskbarContentWidth - clockWidth;
    ImGui::SetCursorPosX(clockX > ImGui::GetCursorPosX()
        ? clockX
        : ImGui::GetCursorPosX());
    ImGui::SetCursorPosY((taskbarHeight - ImGui::GetTextLineHeight()) * 0.5f);
    ImGui::TextUnformatted(clockText);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    if (state.showFiles)
    {
        ImGui::SetNextWindowSize(ImVec2(440, 320), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(
            ImVec2(origin.x + size.x * 0.5f, origin.y + size.y * 0.5f),
            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
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
        ImGui::SetNextWindowPos(
            ImVec2(origin.x + size.x * 0.5f, origin.y + size.y * 0.5f),
            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
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
                "Wallpaper: Custom image loaded from assets/wallpaper.png.");
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
        ImGui::SetNextWindowPos(
            ImVec2(origin.x + size.x * 0.5f, origin.y + size.y * 0.5f),
            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
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
