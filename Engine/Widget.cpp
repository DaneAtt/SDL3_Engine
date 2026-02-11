#include "Widget.h"
#include "Engine.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Events.h"
#include "EventBus.h"
#include "UIContainer.h"

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
    forceOpen();
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
    Vector2D world = getWorldPosition();
    // Draw button background (rectangle)
    SDL_FRect rect = { world.x, world.y, size.w, size.h };
    texMan->DrawRectF(&rect, uiColor);

    int textWidth, textHeight;
    TTF_GetStringSize(font, text.c_str(), 0, &textWidth, &textHeight);

    Vector2D textPos = { world.x + (size.w - textWidth) / 2, world.y + (size.h - textHeight) / 2};
    texMan->drawFont(font, text.c_str(), fontColor, textPos);
}

bool Button::containsPoint(Vector2D& position) 
{
    return isMouseOver(position);
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
    forceOpen();
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
    Vector2D world = getWorldPosition();
    texMan->drawFont(font, text.c_str(), color, world);
}

// ===== PANEL =====

Panel::Panel(const Vector2D& position,
    const UISize& size,
    const SDL_Color& bgColor)
    : UIElement(position,size)
    , bgColor(bgColor)
{
    forceOpen();
    texMan = Engine::getTextureManager();
    container.setOwner(this);
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
    Vector2D world = getWorldPosition();
    SDL_FRect rect = { world.x, world.y, size.w, size.h };
    texMan->DrawRectF(&rect, bgColor);
    container.render();
}

bool Panel::containsPoint(Vector2D& position)
{
    return isMouseOver(position);
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
