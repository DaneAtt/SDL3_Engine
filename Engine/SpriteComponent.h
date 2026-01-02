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
#include "HitboxComponent.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_FRect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 1;
	Animation currentAnimation;
	HitBoxComponent* hitbox;
	const char* animationID;
	int previousAnimIndex = -1;
	float boxStartX;
	float boxStartY;
	std::string name;
	float frameTimer;

public:

	int animIndex;
	Uint32 currentTime;
	SDL_FRect* currentFrame = nullptr;

	SDL_FlipMode spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const char* id) 
	{
		setTex(id);
		name = id;
	}

	SpriteComponent(const char* id, bool animation)
	{
		animated = animation;
		animIndex = 0;
		animationID = id;
		setTex(id);
		name = id;
		float frameTimer = 0.0f;
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

		if (animated)
		{
			currentAnimation = Engine::getJSON()->searchAnimation(animationID);
			currentFrame = &currentAnimation.Frames[0];
			transform->width = currentFrame->w;
			transform->height = currentFrame->h;

			if (currentAnimation.Hitbo)
			{
				hitbox = &entity->addComponent<HitBoxComponent>();
			}
		}
		else
		{
			destRect.w = transform->width;
			destRect.h = transform->height;
		}

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
				float delta = Engine::getDeltaTime();
				float frameDuration = (currentAnimation.Duration / currentAnimation.Frames.size()) / 5.0f ;

				frameTimer += delta;
				if (frameTimer >= frameDuration)
				{
					frameTimer -= frameDuration;
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

				currentFrame = &currentAnimation.Frames[animIndex];
				srcRect = *currentFrame;

				if (currentAnimation.fixedFrame)
				{
					destRect.x = static_cast<int>(transform->position.x) - Engine::getCamera()->x;
					destRect.y = static_cast<int>(transform->position.y) - Engine::getCamera()->y;
					destRect.w = transform->width * transform->scale;
					destRect.h = transform->height * transform->scale;
				}
				else
				{
					destRect.x = static_cast<int>(transform->position.x) - Engine::getCamera()->x;
					destRect.y = static_cast<int>(transform->position.y) - Engine::getCamera()->y;
					//transform->width = currentFrame->w;
					//transform->height = currentFrame->h ;
					destRect.w = currentFrame->w * transform->scale;
					destRect.h = currentFrame->h * transform->scale;
				}

				if (hitbox)
				{
					if (currentAnimation.Hitbo && currentAnimation.HitboxMap.count(animIndex))
					{
						SDL_FRect* frameHitbox = &currentAnimation.HitboxMap[animIndex];

						if (animIndex != previousAnimIndex)
						{
							if (!hitbox->isON())
							{
								boxStartX = transform->position.x;
								boxStartY = transform->position.y;
							}
							hitbox->updateFromFrame(frameHitbox, boxStartX, boxStartY);
						}

					}
					else if ((!currentAnimation.HitboxMap.count(animIndex)))
					{
						hitbox->deactivate();
					}
				}
				previousAnimIndex = animIndex;
			}
			else
			{
				destRect.x = transform->position.x - Engine::getCamera()->x;
				destRect.y = transform->position.y - Engine::getCamera()->y;
				destRect.w = transform->width * transform->scale;
				destRect.h = transform->height * transform->scale;
			}
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

	SDL_FRect* getDestRect()
	{
		return &destRect;
	}

	Animation& getCurrentAnimation() { return currentAnimation; }

	std::string* getName(){ return &name; }
};