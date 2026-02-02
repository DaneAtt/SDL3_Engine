#include "Widget.h"
#include "Engine.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Events.h"
#include "EventBus.h"

// ===== BUTTON =====

Button::Button(std::string name, Vector2D position, Vector2D size, std::string text)
    : name(name), pos(position), size(size), text(text) 
{
    this->setVisible();
    font = Engine::getAssetManager()->getFont("Arial");
    texMan = Engine::getTextureManager();
}

void Button::init() 
{

}

void Button::update() 
{
    // Optional: Handle hover states, animations, etc.
}

void Button::renderC() 
{
    // Draw button background (rectangle)
    SDL_FRect rect = { pos.x, pos.y, size.x, size.y };
    SDL_Color buttonColor = { 25, 60, 180, 255 }; // Gray background
    texMan->DrawRectF(&rect, buttonColor);

    // Draw button text
    SDL_Color textColor = { 0, 255, 255, 255 };
    Vector2D textPos = { pos.x + 10, pos.y + 10 }; // Offset from button corner
    texMan->drawFont(font, text.c_str(), textColor, textPos);
}

bool Button::containsPoint(Vector2D& position) {
    return position.x >= pos.x && position.x <= pos.x + size.x &&
        position.y >= pos.y && position.y <= pos.y + size.y;
}

void Button::handleClick(Vector2D& position) {
    // Emit button clicked event through event bus
    Engine::getEventBus()->emit(ButtonClickedEvent{ name + " Clicked" });
}

/*

    // Create grid buttons
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            float imgX = GRID_START_X + (CELL_STRIDE * j);
            float imgY = GRID_START_Y + (CELL_STRIDE * i);
            float screenX = bounds.x + (imgX * inventoryScale);
            float screenY = bounds.y + (imgY * inventoryScale);
            float screenW = CELL_SIZE * inventoryScale;
            float screenH = CELL_SIZE * inventoryScale;
            Button* button = new Button(
                "Button_" + std::to_string(i) + "_" + std::to_string(j),
                { screenX, screenY },
                { screenW, screenH },
                "Button_" + std::to_string(i) + "_" + std::to_string(j)
            );
            buttons.push_back(button);
        }
    }

    int btnIndex = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            float imgX = GRID_START_X + (CELL_STRIDE * j);
            float imgY = GRID_START_Y + (CELL_STRIDE * i);
            float screenX = bounds.x + (imgX * inventoryScale);
            float screenY = bounds.y + (imgY * inventoryScale);
            float screenW = CELL_SIZE * inventoryScale;
            float screenH = CELL_SIZE * inventoryScale;

            // Update existing button position and size
            buttons[btnIndex]->setPosition({ screenX, screenY });
            buttons[btnIndex]->setSize({ screenW, screenH });
            btnIndex++;
        }
    }

    // Grid measurements
    const float GRID_START_X = 8.0f;
    const float GRID_START_Y = 8.0f;
    const float CELL_SIZE = 32.0f;
    const float CELL_GAP = 8.0f;
    const float CELL_STRIDE = CELL_SIZE + CELL_GAP;
*/


// ===== LABEL =====

Label::Label(std::string text, Vector2D position, SDL_Color color)
    : text(text), pos(position), color(color) 
{
    this->setVisible();
    font = Engine::getAssetManager()->getFont("Arial");
    texMan = Engine::getTextureManager();
}

void Label::init() 
{
    // Optional: Load any resources needed
}

void Label::update() 
{
    // Optional: Handle text animations, etc.
}

void Label::renderC() 
{
    texMan->drawFont(font, text.c_str(), color, pos);
}

// ===== PANEL =====

Panel::Panel(Vector2D position, Vector2D size, SDL_Color backgroundColor)
    : pos(position), size(size), bgColor(backgroundColor) 
{
    this->setVisible();
    texMan = Engine::getTextureManager();
}

void Panel::init()
{
    container.init();
}

void Panel::update()
{
    container.update();
}

void Panel::renderC()
{
    SDL_FRect rect = { pos.x, pos.y, size.x, size.y };
    texMan->DrawRectF(&rect, bgColor);
    container.render();
}

bool Panel::containsPoint(Vector2D& position)
{
    return position.x >= pos.x && position.x <= pos.x + size.x &&
        position.y >= pos.y && position.y <= pos.y + size.y;
}

void Panel::handleClick(Vector2D& position) 
{
    container.handleClick(position);
}

void Panel::clearWidgets()
{
    container.clear();
}
