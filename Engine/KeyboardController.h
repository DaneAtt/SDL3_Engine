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
        const bool* keystates = SDL_GetKeyboardState(NULL);
        transform->velocity.x = 0;
        transform->velocity.y = 0;

        if (keystates[SDL_SCANCODE_W]) transform->velocity.y = -1;
        if (keystates[SDL_SCANCODE_S]) transform->velocity.y = 1;
        if (keystates[SDL_SCANCODE_A])
        {
            transform->velocity.x = -1;
            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        }
        if (keystates[SDL_SCANCODE_D])
        {
            transform->velocity.x = 1;
            sprite->spriteFlip = SDL_FLIP_NONE;
        }

        if (keystates[SDL_SCANCODE_W] == 0 && keystates[SDL_SCANCODE_S] == 0) transform->velocity.y = 0;
        if (keystates[SDL_SCANCODE_A] == 0 && keystates[SDL_SCANCODE_D] == 0) transform->velocity.x = 0;

        if (keystates[SDL_SCANCODE_ESCAPE]) Engine::isRunning = false;
    }

};