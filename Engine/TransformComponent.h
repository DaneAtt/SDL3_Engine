#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include "Engine.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int speed = 0;
	int height = 48;
	int width = 32;
	int scale = 1;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float x, float y, int sc, int spd)
	{
		position.x = x;
		position.y = y;
		scale = sc;
		speed = spd * 100;
	}

	TransformComponent(float x, float y, int h, int w, int sc, int spd)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
		speed = spd * 100;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		deltaTime = Engine::getDeltaTime();
		curPosition = position;
		position.x += velocity.x * speed * deltaTime;
		position.y += velocity.y * speed * deltaTime;
	}

	Vector2D getPreviousPos() { return curPosition; }

private:
	Vector2D curPosition;
	float deltaTime;
};