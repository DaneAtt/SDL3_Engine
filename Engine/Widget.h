#pragma once
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
    Button(std::string name, Vector2D position, Vector2D size, std::string text);
    ~Button() {};

    void init() override;
    void renderC() override;
    bool containsPoint(Vector2D& position) override;

    std::string getName() { return name; }

    void setPosition(const Vector2D& position) override { this->pos = position; }
    void setSize(const Vector2D& size) override  { this->size = size; }

protected:
    EventBus* eventBus;
    TTF_Font* font = nullptr;
    TextureManager* texMan;

private:
    std::string name;
    Vector2D pos;
    Vector2D size;
    std::string text;
};

// Label widget
class ENGINE_API Label : public UIElement, public NonCopyable {
public:
    Label(std::string text, Vector2D position, SDL_Color color = { 255, 255, 255, 255 });
    ~Label() {};

    void init() override;
    void renderC() override;

    bool containsPoint(Vector2D& position) override { return false; }
    void handleClick(Vector2D& position) override {}

private:
    std::string text;
    Vector2D pos;
    SDL_Color color;
    TTF_Font* font = nullptr;
    TextureManager* texMan;
};

// Panel widget
class ENGINE_API Panel : public UIElement, public NonCopyable {
public:
    Panel(Vector2D position, Vector2D size, SDL_Color backgroundColor = { 50, 50, 50, 255 });
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

    void setPosition(const Vector2D& position) override { this->pos = position; }
    void setSize(const Vector2D& size) override { this->size = size; }
    void setColor(SDL_Color newColor) { bgColor = newColor; }
    void clearWidgets();

private:
    Vector2D pos;
    Vector2D size;
    SDL_Color bgColor;
    UIContainer<UIElement> container;
    TextureManager* texMan;
};