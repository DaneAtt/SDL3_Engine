#pragma once
#include "ECS.h"

class DamageComponent : public Component
{
public:
	DamageComponent(double damage) : Damage(damage)
	{

	}

	~DamageComponent() {}

	double getDamage() { return Damage; }

private:
	double Damage;
};