#pragma once
#include "Engine.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include "EngineAPI.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Size.h"
#include "UIAnimationJSON.h"
#include "StaticObjectJSON.h"

class Panel;

class ENGINE_API UIElement
{
public:
	UIElement()
		: pos(0, 0), size() {}

	UIElement(const Vector2D& position)
		: pos(position), size() {}

	UIElement(const Vector2D& position, const Size& size)
		: pos(position), size(size) {}

	virtual void init() {};

	void update()
	{
		if (!visible) return;
		uiAnimUpdate();
		updateC();
	};

	void render()
	{
		if (!visible) return;
		drawAnimation();
		renderC();
	}

	virtual bool containsPoint(Vector2D& position) = 0;
	virtual void handleClick(Vector2D& position) {};
	void handleInput(SDL_Event& event, Vector2D& position)
	{
		handleCustomInput(event, position);

		if (!isVisible()) return;

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
			event.button.button == SDL_BUTTON_LEFT)
		{
			handleClick(position);
		}
	}

	void setParent(UIElement* p) { parent = p; }

	Vector2D getWorldPosition() const
	{
		if (parent) return parent->getWorldPosition() + pos;
		return pos;
	}

	// Animation
	void uiAnimUpdate();

	void drawAnimation()
	{
		if (texture != nullptr)
		{
			std::cout << "drawing texture: " << texture << " dest: " << uiDestRect.x << "," << uiDestRect.y << "\n";

			Engine::getTextureManager()->draw(texture, &uiSrcRect, &uiDestRect, SDL_FLIP_NONE);
		}
	}

	void destRectCalculation();

	// Getters
	bool isVisible() { return visible; }
	bool isMouseOver(Vector2D& mouse) const;
	const char* getFontSize() { return fontSize; }
	Vector2D& getPosition() { return pos; }
	int getScale() { return inventoryScale; }
	SDL_FRect& getSrcRect() { return uiSrcRect; }
	SDL_FRect& getDestRect() { return uiDestRect; }
	Size& getSize() { return size; }
	SDL_Texture* getTexture(){ return texture; }
	bool isAnimated() { return animated; }

	// Setters
	virtual void setVisibility(bool vis);
	void setPosition(const Vector2D& position) { this->pos = position; }
	virtual void setSize(const Size& size) { this->size = size; }
	void setScale(const int& s) { this->inventoryScale = s; }
	void setFontSize(const char* size) { this->fontSize = size; }
	void forceOpen() { this->visible = true; }
	virtual void forceClose() { this->visible = false; }
	void setTexture(const char* texID);
	void setAnimation(const char* animName, bool autoHide);

	// Function
	bool screenChangeUpdate(Panel* mainPanel);
	const SDL_FRect& calculateScreenPosition(const float& Img_Width, const float& Img_Height);

	virtual ~UIElement() = default;

protected:
	virtual void renderC() {};
	virtual void updateC() {};
	virtual void handleCustomInput(SDL_Event& event, Vector2D& position) {}
	bool visible = false;

	SDL_FRect bounds{ 0, 0, 0, 0 };
	EventBus* eventBus = Engine::getEventBus();
	TextureManager* texManager = Engine::getTextureManager();
	AssetManager* assets = Engine::getAssetManager();
	int inventoryScale = 3;
	float screenWidth = Engine::getCamera()->w;
	float screenHeight = Engine::getCamera()->h;
	UIElement* parent = nullptr;
	
	const char* fontSize = "font16";
	Vector2D pos;
	Size size;

	SDL_Texture* texture = nullptr;
	UIAnimInfo* currentAnimation = nullptr;
	bool animated = false;
	int animIndex = 0;
	SDL_FRect uiSrcRect,uiDestRect;
	float frameTimer;
	float frameDuration = 1.0f / 12.0f;
	bool autoHideOnComplete = false;

	// Screen
	float lastScreenWidth = Engine::getCamera()->w;
	float lastScreenHeight = Engine::getCamera()->h;
};