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
	int width = 32;
	int height = 48;
	int scale = 1;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float x, float y, const int& sc, int spd)
	{
		position.x = x;
		position.y = y;
		scale = sc;
		speed = spd * 100;
	}

	TransformComponent(float x, float y, int w, int h, const int& sc, int spd)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
		speed = spd * 100;
	}

	TransformComponent(float x, float y, int w, int h, const int& sc)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		curPosition = position;
	}

	Vector2D getPreviousPos() { return curPosition; }

private:
	Vector2D curPosition;
	float deltaTime;
};