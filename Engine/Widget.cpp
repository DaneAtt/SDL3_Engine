#include "Widget.h"
#include "Engine.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Events.h"
#include "EventBus.h"

// ===== BUTTON =====

Button::Button(std::string name, Vector2D position, Vector2D size, std::string text)
    : name(name), pos(position), size(size), text(text) {
    this->setVisible();
    font = Engine::getAssetManager()->getFont("Arial");
}

void Button::init() {

}

void Button::update() {
    // Optional: Handle hover states, animations, etc.
}

void Button::renderC() {
    auto* textureManager = Engine::getTextureManager();

    // Draw button background (rectangle)
    SDL_FRect rect = { pos.x, pos.y, size.x, size.y };
    SDL_Color buttonColor = { 100, 100, 100, 255 }; // Gray background
    textureManager->DrawRectF(&rect, buttonColor);

    // Draw button text
    SDL_Color textColor = { 0, 255, 255, 255 };
    Vector2D textPos = { pos.x + 10, pos.y + 10 }; // Offset from button corner
    textureManager->drawFont(font, text.c_str(), textColor, textPos);
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
*/


// ===== LABEL =====

Label::Label(std::string text, Vector2D position, SDL_Color color)
    : text(text), pos(position), color(color) {
}

void Label::init() {
    // Optional: Load any resources needed
}

void Label::update() {
    // Optional: Handle text animations, etc.
}

void Label::renderC() {
    auto* textureManager = Engine::getTextureManager();
    TTF_Font* font = Engine::getAssetManager()->getFont("PLACEHOLDER");
    textureManager->drawFont(font, text.c_str(), color, pos);
}

bool Label::containsPoint(Vector2D& position) {
    // Labels typically don't need click detection
    // Return false so clicks pass through
    return false;
}

// ===== PANEL =====

Panel::Panel(Vector2D position, Vector2D size, SDL_Color backgroundColor)
    : pos(position), size(size), bgColor(backgroundColor) {
}

void Panel::init() {
    // Optional: Load any resources needed
}

void Panel::update() {
    // Optional: Handle panel animations, etc.
}

void Panel::renderC() {
    auto* textureManager = Engine::getTextureManager();
    SDL_FRect rect = { pos.x, pos.y, size.x, size.y };
    textureManager->DrawDebugRectF(&rect, bgColor);
}

bool Panel::containsPoint(Vector2D& position) {
    return position.x >= pos.x && position.x <= pos.x + size.x &&
        position.y >= pos.y && position.y <= pos.y + size.y;
}