#pragma once
#include "SDL3/SDL.h"
#include "ECS.h"
#include <vector>
#include "TransformComponent.h"
#include "Engine.h"
#include "SpatialGrid.h"
#include "TextureManager.h"
#include "SpriteComponent.h"
#include "AIComponent.h"

class CollisionComponent : public Component
{
public:

	CollisionComponent() : isStatic(false) {}
	CollisionComponent(bool staticCollider) : isStatic(staticCollider) {}
	~CollisionComponent() { Engine::getCollisionGrid()->Remove(this); }

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		collision.x = collision.y = 0;
		collision.w = static_cast<float>(transform->width);
		collision.h = static_cast<float>(transform->height);

		// If entity has SpriteComponent with animation, get collision from that
		if (entity->hasComponent<SpriteComponent>()) {
			sprite = &entity->getComponent<SpriteComponent>();
			Animation& anim = sprite->getCurrentAnimation();
			if (!anim.Frames.empty())
			{
				collision = anim.collisionRect;
			}
			transformRect.x = transform->position.x - (anim.anchorX * transform->scale) + (collision.x * transform->scale);
			transformRect.y = transform->position.y - (anim.anchorY * transform->scale) + (collision.y * transform->scale);
		}
		else 
		{
			transformRect.x = transform->position.x + (collision.x * transform->scale);
			transformRect.y = transform->position.y + (collision.y * transform->scale);
		}
		transformRect.w = collision.w * transform->scale;
		transformRect.h = collision.h * transform->scale;

		Engine::getCollisionGrid()->Insert(this, &transformRect);
	}

	SDL_FRect* getRect() { return &transformRect; }
	bool getIsStatic() const { return isStatic; }
	SDL_FRect* getCollision() { return &collision; }
	SDL_FRect* getLastRect() { return &lastRect; }

private:
	bool isStatic;
	SDL_FRect* destRect = nullptr;
	SDL_FRect transformRect;
	SDL_FRect lastRect{};
	SpriteComponent* sprite = nullptr;
	TransformComponent* transform = nullptr;
	SDL_FRect collision;
	Animation currentAnimation;
};