#pragma once

#include "AWindow.h"

class TaskManagerWindow : public AWindow
{
public:
    explicit TaskManagerWindow(AppState& appState) : AWindow(appState) {}
    void render() override;
};
