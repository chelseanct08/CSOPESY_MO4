#pragma once

#include "AWindow.h"

class DesktopWindow : public AWindow
{
public:
    explicit DesktopWindow(AppState& appState) : AWindow(appState) {}
    void render() override;
};
