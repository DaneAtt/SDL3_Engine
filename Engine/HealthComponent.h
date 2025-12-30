#pragma once
#include "ECS.h"
#include "Engine.h"


class HealthComponent : public Component
{
public:
	HealthComponent() {}
	~HealthComponent() {}

	void init() override
	{
		health = 100;
		manager = Engine::getManager();

	}

	void update() override
	{
		if (health < 0)
		{
			std::cout << &entity << " died" << "\n";
		}
	}
	
	void TakeDamage(double damage)
	{
		health -= damage;
	}


private:
	int health;
	Manager* manager;

};