#pragma once
#include "SDL3/SDL.h"
#include <optional>

namespace InputHelper
{
    static bool isMouseLButtonDown(SDL_Event& event)
    {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) return true;
        return false;
    }

    static bool isMouseLButtonUp(SDL_Event& event)
    {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) return true;
        return false;
    }

    static bool isMouseRButtonDown(SDL_Event& event)
    {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT) return true;
        return false;
    }

    static bool isMouseRButtonUp(SDL_Event& event)
    {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_RIGHT) return true;
        return false;
    }

    static bool isKeyDown(SDL_Event& event, std::optional<SDL_Keycode> key = std::nullopt)
    {
        if (event.type != SDL_EVENT_KEY_DOWN)
            return false;

        if (!key.has_value())   // if keycode is null
            return true;

        return event.key.key == key.value();
    }

    static bool isScrolling(SDL_Event& event)
    {
        return event.type == SDL_EVENT_MOUSE_WHEEL;
    }
};