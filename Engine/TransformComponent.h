#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int speed = 5;

	int height = 32;
	int width = 32;
	int scale = 1;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float x, float y, int sc)
	{
		position.x = x;
		position.y = y;
		scale = sc;
	}

	//player position
	TransformComponent(int sc)
	{
		position.x = 50;
		position.y = 50;
		scale = sc;
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
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};