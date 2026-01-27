#pragma once
#pragma once
#include "EngineAPI.h"
#include "UIManager.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include <string>

// Button widget
class ENGINE_API Button : public UIElement {
public:
    Button(std::string name, Vector2D position, Vector2D size, std::string text);

    void init() override;
    void update() override;
    void renderC() override;
    bool containsPoint(Vector2D& position) override;
    void handleClick(Vector2D& position) override;

private:
    std::string name;
    Vector2D pos;
    Vector2D size;
    std::string text;
    TTF_Font* font = nullptr;
};

// Label widget
class ENGINE_API Label : public UIElement {
public:
    Label(std::string text, Vector2D position, SDL_Color color = { 255, 255, 255, 255 });

    void init() override;
    void update() override;
    void renderC() override;
    bool containsPoint(Vector2D& position) override;
    void handleClick(Vector2D& position) override {} // Empty - labels don't handle clicks

private:
    std::string text;
    Vector2D pos;
    SDL_Color color;
};

// Panel widget
class ENGINE_API Panel : public UIElement {
public:
    Panel(Vector2D position, Vector2D size, SDL_Color backgroundColor = { 50, 50, 50, 255 });

    void init() override;
    void update() override;
    void renderC() override;
    bool containsPoint(Vector2D& position) override;
    void handleClick(Vector2D& position) override {} // Empty - panels don't handle clicks

private:
    Vector2D pos;
    Vector2D size;
    SDL_Color bgColor;
};