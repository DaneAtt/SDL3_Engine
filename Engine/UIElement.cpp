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

const SDL_FRect& UIElement::calculateScreenPosition(float Img_Width, float Img_Height)
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

void UIElement::uiAnimUpdate()
{
	if (animated && currentAnimation)
	{
		frameTimer += Engine::getDeltaTime();
		if (frameTimer >= frameDuration)
		{
			frameTimer = 0;
			animIndex++;

			// Check if animation finished
			if (animIndex >= currentAnimation->frames.size())
			{
				if (currentAnimation->loop) 
				{
					animIndex = 0;
				}
				else
				{
					animIndex = currentAnimation->frames.size() - 1;

					// Hide when complete if auto-hide enabled
					if (autoHideOnComplete)
					{
						setVisibility(false);
						animated = false;
					}
				}
			}
		}

		uiSrcRect.x = currentAnimation->frames[animIndex].x;
		uiSrcRect.y = currentAnimation->frames[animIndex].y;
		uiSrcRect.x += currentAnimation->atlasPosition.x;
		uiSrcRect.y += currentAnimation->atlasPosition.y;
	}
	destRectCalculation();
}

void UIElement::destRectCalculation()
{
	Vector2D worldPos = getWorldPosition();

	uiDestRect.x = worldPos.x;
	uiDestRect.y = worldPos.y;
	uiDestRect.w = size.w;
	uiDestRect.h = size.h;
}

void UIElement::setAnimation(const char* animName, bool autoHide)
{
	currentAnimation = Engine::getUIAnimJSON()->search(animName);
	texture = Engine::getAssetManager()->getTexture(currentAnimation->textureID);
	uiSrcRect.x = currentAnimation->frames[0].x + currentAnimation->atlasPosition.x;
	uiSrcRect.y = currentAnimation->frames[0].y + currentAnimation->atlasPosition.y;
	uiSrcRect.w = currentAnimation->size.w;
	uiSrcRect.h = currentAnimation->size.h;
	animated = true;
	animIndex = 0;
	frameTimer = 0.0f;
	autoHideOnComplete = autoHide;

	destRectCalculation();
}

void UIElement::setTexture(const char* texID)
{
	staticObjPos* spriteInfo = Engine::getObjJSON()->search(texID);

	if (spriteInfo)
	{
		texture = Engine::getAssetManager()->getTexture(spriteInfo->texName);
		uiSrcRect.x = spriteInfo->pos.x;
		uiSrcRect.y = spriteInfo->pos.y;
		uiSrcRect.w = spriteInfo->size.w;
		uiSrcRect.h = spriteInfo->size.h;

		destRectCalculation();
	}
	else
	{
		std::cout << "Texture not in atlas" << '\n';
		return;
	}
}