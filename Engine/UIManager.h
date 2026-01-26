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
	virtual void render() = 0;
	virtual bool containsPoint(Vector2D position) = 0;
	virtual void handleClick(Vector2D position) = 0;

	virtual ~UIElement() = default;
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

	void handleInput(SDL_Event event);

	void render()
	{
		for (auto& ui : uiElements)
		{
			ui->render();
		}
	}

	bool isVisible() { return visible; }

private:
	std::vector<std::unique_ptr<UIElement>> uiElements;
	bool visible = false;
};