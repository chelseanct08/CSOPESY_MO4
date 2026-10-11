#include "UIManager.h"

void UIManager::addWindow(AWindow& window)
{
    windows.push_back(&window);
}

void UIManager::render()
{
    for (AWindow* window : windows)
    {
        if (window != nullptr)
            window->render();
    }
}
