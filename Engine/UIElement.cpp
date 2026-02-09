#include "UIElement.h"
#include "Widget.h"
#include "UIManager.h"

bool UIElement::screenChangeUpdate(Panel* mainPanel)
{
	if (screenWidth != lastScreenWidth || screenHeight != lastScreenHeight)
	{
		// Recalculation position
		Screen_UI_X = (screenWidth - Screen_UI_Width) / 2;
		Screen_UI_Y = (screenHeight - Screen_UI_Height) / 2;
		bounds = { Screen_UI_X, Screen_UI_Y, Screen_UI_Width, Screen_UI_Height };

		// Update panel position
		mainPanel->setPosition({ Screen_UI_X, Screen_UI_Y });
		mainPanel->setSize({ Screen_UI_Width, Screen_UI_Height });

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