#pragma once

#include <vector>
#include "AWindow.h"

class UIManager
{
public:
    void addWindow(AWindow& window);
    void render();

private:
    std::vector<AWindow*> windows;
};
