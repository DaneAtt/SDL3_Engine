#pragma once
#include "Engine.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include "EngineAPI.h"
#include "TextureManager.h"
#include "AssetManager.h"

class Panel;

class ENGINE_API UIElement
{
public:
	virtual void init() {};

	void update()
	{
		if (!visible) return;
		updateC();
	};

	void render()
	{
		if (!visible) return;
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

	void toggleVisibility() { visible = !visible; }
	void setVisibility(bool vis);
	bool isVisible() { return visible; }
	void forceClose()
	{
		this->visible = false;
	}

	virtual void setPosition(const Vector2D& newPos) {};
	virtual void setSize(const Vector2D& newSize) {};
	bool screenChangeUpdate(Panel* mainPanel);

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
	const int inventoryScale = 3;
	float Screen_UI_Width, Screen_UI_Height, Screen_UI_X, Screen_UI_Y;
	float screenWidth = Engine::getCamera()->w;
	float screenHeight = Engine::getCamera()->h;

	// Screen
	float lastScreenWidth = 0;
	float lastScreenHeight = 0;
};