#pragma once
#include <memory>;
#include "Widget.h";
#include "Engine.h"

class LayoutHelperBase
{
protected:
	std::unique_ptr<Panel> createMainPanel(SDL_FRect& bounds)
	{
		return std::make_unique<Panel>(
			Vector2D(bounds.x, bounds.y),
			Size(bounds.w, bounds.h),
			SDL_Color{ 0, 0, 0, 0 });
	}

	virtual float getPanelStartX() const = 0;
	virtual float getPanelStartY() const = 0;
	virtual float getScreenUIX() const = 0;
	virtual float getScreenUIY() const = 0;
	virtual int getInventoryScale() const = 0;

	TextureManager* texManager = Engine::getTextureManager();
	std::unique_ptr<Panel> mainPanel;
};