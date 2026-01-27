#pragma once
#include "EngineAPI.h"
#include "Vector2D.h"
#include <vector>
#include "SDL3/SDL.h"
#include "Engine.h"

class ENGINE_API UIElement
{
public:
	virtual void init() = 0;
	virtual void update() = 0;

	void render()
	{
		if (!visible) return;
		renderC();
	}

	virtual bool containsPoint(Vector2D& position) = 0;
	virtual void handleClick(Vector2D& position) = 0;
	void handleInput(SDL_Event& event, Vector2D& position)
	{

		handleCustomInput(event, position);

		if (!isVisible()) return;

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			Vector2D mouse = Engine::getMouse();
			if (containsPoint(position))
			{
				handleClick(position);
			}
		}
	}

	void setVisible() { visible = !visible; }
	bool isVisible() { return visible; }

	virtual ~UIElement() = default;

protected:
	virtual void renderC() = 0;
	virtual void handleCustomInput(SDL_Event& event, Vector2D& position) {}
	bool visible = false;
};

class ENGINE_API UIManager
{
public:
	UIManager() {};
	~UIManager() {};

	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;

	template<typename T, typename... TArgs>
	T& addUI(TArgs&&... mArgs)
	{
		T* ui = new T(std::forward<TArgs>(mArgs)...);
		std::unique_ptr<UIElement> uPtr{ ui };
		uiElements.emplace_back(std::move(uPtr));

		ui->init();
		return *ui;
	}

	std::vector<std::unique_ptr<UIElement>>& getUI(){ return uiElements; }

	void handleAllInput(SDL_Event& event)
	{
		Vector2D& mouse = Engine::getMouse();
		SDL_Rect* camera = Engine::getCamera();

		for (auto& ui : uiElements)
		{
			ui->handleInput(event, mouse);
		}
	};

	void render()
	{
		for (auto& ui : uiElements)
		{
			ui->render();
		}
	}

private:
	std::vector<std::unique_ptr<UIElement>> uiElements;
};