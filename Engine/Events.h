#pragma once
#include "EngineAPI.h"
#include "Vector2D.h"

class Entity;

struct ENGINE_API DamageEvent
{
	Entity* attacker;
	Entity* target;
};

struct ENGINE_API DeathEvent {
	Entity* entity;
};