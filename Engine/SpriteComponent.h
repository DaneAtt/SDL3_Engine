#pragma once
#include <map>
#include "TransformComponent.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Engine.h"
#include "ECS.h"
#include "SDL3/sdl.h"
#include "Animation.h"
#include "AnimationJSON.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_FRect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 1;

	// srcW, srcH;

public:

	int animIndex;
	Uint32 currentTime;

	//std::map<const char*, Animation> animations;

	SDL_FlipMode spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const char* id) 
	{
		setTex(id);
	}

	SpriteComponent(const char* id, bool animation)
	{
		animated = animation;
		animIndex = 0;

		if (animated) {

			currentAnimation = Engine::getJSON()->searchAnimation(id);
		}

		setTex(id);

	}

	~SpriteComponent() {}

	void setTex(std::string id)
	{
		texture = Engine::getAssetManager()->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

		destRect.w = transform->width;
		destRect.h = transform->height;
	}

	void update() override
	{
		try
		{
			if (animated)
			{
				if (currentAnimation.Frames.empty()) {
					std::cout << "ERROR: Animation has no frames!\n";
					return;  // Exit early
				}
				currentTime = SDL_GetTicks();
				Uint32 delta = currentTime - currentAnimation.lastFrameTime;
				if (delta >= currentAnimation.DefaultDuration)
				{
					currentAnimation.lastFrameTime = currentTime;
					animIndex++;
					if (animIndex >= currentAnimation.Frames.size()) {
						if (currentAnimation.Loop) {
							animIndex = 0;  // Loop back to start
						}
						else {
							animIndex = currentAnimation.Frames.size() - 1;  // Stay on last frame
						}
					}
				}

				SDL_FRect& currentFrame = currentAnimation.Frames[animIndex];
				srcRect.x = currentFrame.x;
				srcRect.y = currentFrame.y;
				srcRect.w = currentFrame.w;
				srcRect.h = currentFrame.h;

			}

			destRect.x = static_cast<int>(transform->position.x); //- Game::camera.x;
			destRect.y = static_cast<int>(transform->position.y); //- Game::camera.y;
			destRect.w = transform->width * transform->scale;
			destRect.h = transform->height * transform->scale;
		}
		catch (std::exception)
		{
			std::cout << "SpriteComponent __FUNCTION__ bug" << "\n";
		}
	}

	void draw() override
	{
		Engine::getTextureManager()->Draw(texture, &srcRect, &destRect, spriteFlip);
	}

private:
	Animation currentAnimation;
};