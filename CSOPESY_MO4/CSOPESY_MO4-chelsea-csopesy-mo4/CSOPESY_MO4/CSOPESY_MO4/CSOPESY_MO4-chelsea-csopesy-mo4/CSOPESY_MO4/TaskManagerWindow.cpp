#include "TaskManagerWindow.h"

void TaskManagerWindow::render()
{
    if (!state.showTaskManager || state.screen != Screen::Desktop)
        return;

    ImGui::SetNextWindowSize(ImVec2(620, 390), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Task Manager", &state.showTaskManager))
    {
        ImGui::Text("Processes");
        ImGui::Separator();

        float totalCPU = 0.0f;
        float totalMemory = 0.0f;

        for (const Process& process : state.processes)
        {
            totalCPU += process.cpu;
            totalMemory += process.memory;
        }

        ImGui::Text("CPU: %.1f%%", totalCPU);
        ImGui::SameLine();
        ImGui::Text("Memory: %.1f MB", totalMemory);
        ImGui::Spacing();

        if (ImGui::BeginTable(
            "ProcessesTable", 4,
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Status");
            ImGui::TableSetupColumn("CPU (%)");
            ImGui::TableSetupColumn("Memory (MB)");
            ImGui::TableHeadersRow();

            for (const Process& process : state.processes)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", process.name.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", process.status.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.1f", process.cpu);

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.1f", process.memory);
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::TextDisabled(
            "Demonstration values only; not real system metrics.");
    }
    ImGui::End();
}
