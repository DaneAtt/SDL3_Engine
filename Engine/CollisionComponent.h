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
		collisionWidth = transform->width;
		collisionHeight = transform->height;

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
        float deltaTime = Engine::getDeltaTime();

        Vector2D prevPos = transform->getPreviousPos();
        Vector2D velocity = transform->velocity * transform->speed * deltaTime;

        // Try full movement first
        SDL_FRect predictedRect;
        predictedRect.x = prevPos.x + velocity.x;
        predictedRect.y = prevPos.y + velocity.y;
        predictedRect.w = transform->width * transform->scale;
        predictedRect.h = transform->height * transform->scale;

        auto nearby = Engine::getCollisionGrid()->Query(&predictedRect);
        bool collidedX = false;
        bool collidedY = false;

        for (auto* other : nearby) {
            if (other == this) continue;
            SDL_FRect* otherRect = other->getRect();
            if (!otherRect) continue;

            if (SDL_HasRectIntersectionFloat(&predictedRect, otherRect)) {
                // Try X-only movement
                SDL_FRect testX;
                testX.x = prevPos.x + velocity.x;
                testX.y = prevPos.y;
                testX.w = predictedRect.w;
                testX.h = predictedRect.h;

                if (SDL_HasRectIntersectionFloat(&testX, otherRect)) {
                    collidedX = true;
                }

                // Try Y-only movement
                SDL_FRect testY;
                testY.x = prevPos.x;
                testY.y = prevPos.y + velocity.y;
                testY.w = predictedRect.w;
                testY.h = predictedRect.h;

                if (SDL_HasRectIntersectionFloat(&testY, otherRect)) {
                    collidedY = true;
                }
            }
        }

        // Apply allowed movement (sliding)
        Vector2D newPos = prevPos;
        if (!collidedX) {
            newPos.x += velocity.x;
        }
        else {
            transform->velocity.x = 0;
        }
        if (!collidedY) {
            newPos.y += velocity.y;
        }
        else {
            transform->velocity.y = 0;
        }

        transform->position = newPos;

        // Separation push (for overlapping entities)
        nearby = Engine::getCollisionGrid()->Query(&transformRect);
        for (auto* other : nearby) {
            if (other == this) continue;
            if (other->isStatic) continue; // Don't push static objects
            SDL_FRect* otherRect = other->getRect();
            if (!otherRect) continue;

            if (SDL_HasRectIntersectionFloat(&transformRect, otherRect)) {
                // Calculate push direction
                float overlapX = std::min(transformRect.x + transformRect.w, otherRect->x + otherRect->w)
                    - std::max(transformRect.x, otherRect->x);
                float overlapY = std::min(transformRect.y + transformRect.h, otherRect->y + otherRect->h)
                    - std::max(transformRect.y, otherRect->y);

                // Push along smallest overlap axis
                if (overlapX < overlapY) {
                    float push = (transformRect.x < otherRect->x) ? -overlapX * 0.5f : overlapX * 0.5f;
                    transform->position.x += push;
                }
                else {
                    float push = (transformRect.y < otherRect->y) ? -overlapY * 0.5f : overlapY * 0.5f;
                    transform->position.y += push;
                }
            }
        }

        // Update rect for spatial grid
        transformRect.x = transform->position.x;
        transformRect.y = transform->position.y;
        transformRect.w = collisionWidth * transform->scale;
        transformRect.h = collisionHeight * transform->scale;

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