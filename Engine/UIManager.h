#pragma once
#include "EngineAPI.h"
#include "Vector2D.h"
#include <vector>
#include "SDL3/SDL.h"
#include "Engine.h"
#include "UIContainer.h";

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
			if (containsPoint(position))
			{
				handleClick(position);
			}
		}
	}

	void toggleVisibility() { visible = !visible; }
	bool isVisible() { return visible; }
	virtual void setPosition(const Vector2D& newPos) {};
	virtual void setSize(const Vector2D& newSize) {};

	virtual ~UIElement() = default;

protected:
	virtual void renderC() {};
	virtual void updateC() {};
	virtual void handleCustomInput(SDL_Event& event, Vector2D& position) {}
	bool visible = false;
};

class ENGINE_API UIManager : public NonCopyable
{
public:
	UIManager() {};
	~UIManager() {};

	template<typename T, typename... TArgs>
	T& addUI(TArgs&&... mArgs)
	{
		return container.add<T>(std::forward<TArgs>(mArgs)...);
	}

	void handleAllInput(SDL_Event& event)
	{
		Vector2D& mouse = Engine::getMouse();
		container.handleInput(event, mouse);
	};

	void render()
	{
		container.render();
	}

	void update()
	{
		container.update();
	}

private:
	UIContainer<UIElement> container;

};