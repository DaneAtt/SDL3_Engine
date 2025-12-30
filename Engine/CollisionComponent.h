#pragma once
#include "SDL3/SDL.h"
#include "ECS.h"
#include <vector>
#include "TransformComponent.h"
#include "Engine.h"
#include "SpatialGrid.h"
#include "TextureManager.h"
#include "SpriteComponent.h"

class CollisionComponent : public Component
{
public:

	CollisionComponent() : isStatic(false) {}
	CollisionComponent(bool staticCollider) : isStatic(staticCollider) {}
	~CollisionComponent() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		// If entity has SpriteComponent with animation, get collision from that
		if (entity->hasComponent<SpriteComponent>()) {
			sprite = &entity->getComponent<SpriteComponent>();

			Animation& anim = sprite->getCurrentAnimation();

			if (!anim.Frames.empty()) 
			{
				collisionWidth = anim.collisionRect.w;
				collisionHeight = anim.collisionRect.h;
			}
		}
		else 
		{
			// No sprite, use transform size
			collisionWidth = transform->width;
			collisionHeight = transform->height;
		}

		transformRect.x = transform->position.x;
		transformRect.y = transform->position.y;
		transformRect.w = collisionWidth * transform->scale;
		transformRect.h = collisionHeight * transform->scale;
		lastRect = transformRect;

		Engine::getCollisionGrid()->Insert(this, &transformRect);
	}

	void update() override
	{
		if (isStatic) return;
		transform = &entity->getComponent<TransformComponent>();

		// Calculate predicted position
		Vector2D predictedPos;
		predictedPos.x = transform->position.x + (transform->velocity.x * transform->speed);
		predictedPos.y = transform->position.y + (transform->velocity.y * transform->speed);

		// Create predicted rect
		SDL_FRect predictedRect;
		predictedRect.x = predictedPos.x;
		predictedRect.y = predictedPos.y;
		predictedRect.w = transform->width * transform->scale;
		predictedRect.h = transform->height * transform->scale;


		// Check collision using PREDICTED rect, not current position
		auto nearby = Engine::getCollisionGrid()->Query(&predictedRect);
		bool willCollide = false;

		for (auto* other : nearby) {
			if (other == this) continue;
			SDL_FRect* otherRect = other->getRect();
			if (!otherRect) continue;

			if (SDL_HasRectIntersectionFloat(&predictedRect, otherRect)) {
				std::cout << "Collision at " << transform->position.x << ", " << transform->position.y << " " << "Wall at:" << otherRect->x << ", " << otherRect->y << '\n';
				willCollide = true;
				break;
			}
		}

		// If collision would happen, stop movement
		if (willCollide) {
			transform->velocity.Zero();
		}

		// Update current transformRect for spatial grid
		transformRect.x = transform->position.x;
		transformRect.y = transform->position.y;
		transformRect.w = transform->width * transform->scale;
		transformRect.h = transform->height * transform->scale;

		// Only update grid if position changed
		if (!areRectsEqual(&transformRect, &lastRect)) {
			Engine::getCollisionGrid()->Update(this, &transformRect);
			lastRect = transformRect;
		}
	}

	SDL_FRect* getRect()
	{
			return &transformRect;

	}

	bool areRectsEqual(const SDL_FRect* a, const SDL_FRect* b) const 
	{
		// Only check position, ignore size changes from animation
		return a->x == b->x && a->y == b->y;
	}

private:
	bool isStatic;
	SDL_FRect* destRect = nullptr;
	SDL_FRect transformRect;
	SDL_FRect lastRect{};
	SpriteComponent* sprite = nullptr;
	TransformComponent* transform = nullptr;
	int collisionWidth, collisionHeight;  
	Animation currentAnimation;
};