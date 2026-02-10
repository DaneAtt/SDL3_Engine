#include "Widget.h"
#include "Engine.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Events.h"
#include "EventBus.h"

// ===== BUTTON =====

Button::Button(const std::string& name,
    const Vector2D& position,
    const UISize& size,
    const SDL_Color& uiColor,
    const std::string& text,
    const SDL_Color& fontColor,
    const char* textSize)
    : UIElement(position, size), 
    name(name),
    text(text),
    uiColor(uiColor),
    fontColor(fontColor),
    textSize(textSize)
{
    this->toggleVisibility();
    if (textSize != nullptr)
    {
        font = Engine::getAssetManager()->getFont(textSize);
    }
    else
    {
        std::cout << "Font size is nullptr" << '\n';
        return;
    }
    texMan = Engine::getTextureManager();
    eventBus = Engine::getEventBus();
}

void Button::init() 
{

}

void Button::renderC() 
{
    // Draw button background (rectangle)
    SDL_FRect rect = { pos.x, pos.y, size.w, size.h };
    texMan->DrawRectF(&rect, uiColor);

    int textWidth, textHeight;
    TTF_GetStringSize(font, text.c_str(), 0, &textWidth, &textHeight);

    Vector2D textPos = { pos.x + (size.w - textWidth) / 2, pos.y + (size.h - textHeight) / 2};
    texMan->drawFont(font, text.c_str(), fontColor, textPos);
}

bool Button::containsPoint(Vector2D& position) 
{
    return position.x >= pos.x && position.x <= pos.x + size.w &&
        position.y >= pos.y && position.y <= pos.y + size.h;
}

// ===== LABEL =====

Label::Label(const std::string& text,
    const Vector2D& position,
    const SDL_Color& color,
    const char* textSize)
    : text(text), 
    UIElement(position), 
    color(color), 
    textSize(textSize)
{
    this->toggleVisibility();
    if (textSize != nullptr)
    {
        font = Engine::getAssetManager()->getFont(textSize);
    }
    else
    {
        std::cout << "Font size is nullptr" << '\n';
        return;
    }
    texMan = Engine::getTextureManager();
}

void Label::init() 
{
    // Optional: Load any resources needed
}

void Label::renderC() 
{
    texMan->drawFont(font, text.c_str(), color, pos);
}

// ===== PANEL =====

Panel::Panel(const Vector2D& position,
    const UISize& size,
    const SDL_Color& bgColor)
    : UIElement(position,size)
    , bgColor(bgColor)
{
    this->toggleVisibility();
    texMan = Engine::getTextureManager();
}

void Panel::init()
{
    container.init();
}

void Panel::updateC()
{
    container.update();
}

void Panel::renderC()
{
    SDL_FRect rect = { pos.x, pos.y, size.w, size.h };
    texMan->DrawRectF(&rect, bgColor);
    container.render();
}

bool Panel::containsPoint(Vector2D& position)
{
    return position.x >= pos.x && position.x <= pos.x + size.w &&
        position.y >= pos.y && position.y <= pos.y + size.h;
}

void Panel::handleClick(Vector2D& position) 
{
    if (container.containsPoint(position))
    {
        container.handleClick(position);
    }
}

void Panel::clearWidgets()
{
    container.clear();
}
