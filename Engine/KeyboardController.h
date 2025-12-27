#pragma once
#include "EngineAPI.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Engine.h"
#include "SDL3/SDL.h"

class KeyboardController : public Component
{

public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();

	}

	void update() override
	{

		if (Engine::event.type == SDL_EVENT_KEY_DOWN)
		{
			switch (Engine::event.key.key)
			{
			case SDLK_W:
				transform->velocity.y = -1;
				//sprite->Play("Walk");
				break;
			case SDLK_A:
				transform->velocity.x = -1;
				//sprite->Play("Walk");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_S:
				transform->velocity.y = 1;
				//sprite->Play("Walk");
				break;
			case SDLK_D:
				transform->velocity.x = 1;
				//sprite->Play("Walk");
				break;
			default:
				break;
			}
		}

		if (Engine::event.type == SDL_EVENT_KEY_UP)
		{
			switch (Engine::event.key.key)
			{
			case SDLK_W:
				transform->velocity.y = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_A:
				transform->velocity.x = 0;
				//sprite->Play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_S:
				transform->velocity.y = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_D:
				transform->velocity.x = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_ESCAPE:
				Engine::isRunning = false;
			default:
				break;
			}
		}
	}
};