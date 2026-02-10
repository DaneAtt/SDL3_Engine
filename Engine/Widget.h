#pragma once
#include "EngineAPI.h"
#include "UIElement.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include "UIContainer.h"
#include "NonCopyable.h"
#include <string>

// Button widget
class ENGINE_API Button : public UIElement, public NonCopyable {
public:
    Button(const std::string& name,
        const Vector2D& position,
        const UISize& size,
        const SDL_Color& uiColor,
        const std::string& text,
        const SDL_Color& fontColor,
        const char* textSize);
    ~Button() {};

    void init() override;
    void renderC() override;
    bool containsPoint(Vector2D& position) override;

    std::string getName() { return name; }

protected:
    std::string name;
    const char* textSize;
    std::string text;
    SDL_Color uiColor;
    SDL_Color fontColor;

    EventBus* eventBus;
    TTF_Font* font = nullptr;
    TextureManager* texMan;
};

// Label widget
class ENGINE_API Label : public UIElement, public NonCopyable {
public:
    Label(const std::string& text,
        const Vector2D& position,
        const SDL_Color& color,
        const char* textSize);
    ~Label() {};

    void init() override;
    void renderC() override;

    bool containsPoint(Vector2D& position) override { return false; }
    void handleClick(Vector2D& position) override {}

protected:
    std::string text;
    SDL_Color color;
    TTF_Font* font = nullptr;
    const char* textSize;
    TextureManager* texMan;
};

// Panel widget
class ENGINE_API Panel : public UIElement, public NonCopyable {
public:
    Panel(const Vector2D& position,
        const UISize& size,
        const SDL_Color& bgColor);
    ~Panel() {};

    void init() override;
    void updateC() override;
    void renderC() override;
    bool containsPoint(Vector2D& position) override;
    void handleClick(Vector2D& position) override;
    template<typename T, typename... TArgs>
    T& addWidget(TArgs&&... mArgs) 
    {
        return container.add<T>(std::forward<TArgs>(mArgs)...);
    }

    void setColor(const SDL_Color& newColor) { bgColor = newColor; }
    void clearWidgets();

protected:
    SDL_Color bgColor;
    UIContainer<UIElement> container;
    TextureManager* texMan;
};