#pragma once
#include "ECS.h"

class DamageComponent : public Component
{
public:
	DamageComponent(double damage) : Damage(damage)
	{

	}

	~DamageComponent() {}

	int getDamage() { return Damage; }

private:
	int Damage;
};