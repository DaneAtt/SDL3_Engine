#pragma once
#include "EngineAPI.h"
#include "Vector2D.h"
#include <vector>
#include "SDL3/SDL.h"
#include "Engine.h"
#include "UIContainer.h";
#include "NonCopyable.h"
#include "UIElement.h"

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