#include "UIElement.h"
#include "Widget.h"
#include "UIManager.h"

bool UIElement::screenChangeUpdate(Panel* mainPanel)
{
	if (screenWidth != lastScreenWidth || screenHeight != lastScreenHeight)
	{
		// Recalculation position
		pos.x = (screenWidth - size.w) / 2;
		pos.y = (screenHeight - size.h) / 2;
		bounds = { pos.x, pos.y, size.w, size.h };

		// Update panel position
		mainPanel->setPosition({ pos.x, pos.y });
		mainPanel->setSize({ size.w, size.h });

		lastScreenWidth = screenWidth;
		lastScreenHeight = screenHeight;
		return true;
	}
	return false;
}

void UIElement::setVisibility(bool visible)
{
	UIManager* mgr = Engine::getUIManager();

	if (visible)
	{
		mgr->openUI(this);
	}
	else
	{
		mgr->closeUI(this);
	}

	this->visible = visible;
}

const SDL_FRect& UIElement::calculateScreenPosition(const float& Img_Width, const float& Img_Height)
{
	size.w = Img_Width * inventoryScale;
	size.h = Img_Height * inventoryScale;
	pos.x = (screenWidth - size.w) / 2;
	pos.y = (screenHeight - size.h) / 2;
	return SDL_FRect{ pos.x , pos.y, size.w, size.h };
}

bool UIElement::isMouseOver(Vector2D& mouse) const 
{
	Vector2D world = getWorldPosition();
	return mouse.x >= world.x && mouse.x <= world.x + size.w &&
		mouse.y >= world.y && mouse.y <= world.y + size.h;
}
