#include "UIManager.h"


void UIManager::handleInput(SDL_Event event)
{
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_I)
    {
        visible = !visible;
    }

    if (!visible)
        return;

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        Vector2D mouse = Engine::getMouse();
        for (auto& ui : uiElements)
        {
            if (ui->containsPoint(mouse))
            {
                ui->handleClick(mouse);
                return;
            }
        }
    }
}