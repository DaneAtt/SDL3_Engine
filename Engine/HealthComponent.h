#pragma once
#include "ECS.h"
#include "Engine.h"


class HealthComponent : public Component
{
public:
	HealthComponent() { health = 10000; }
	HealthComponent(int health) 
	{
		this->health = health;
	}
	~HealthComponent() {}

	void init() override
	{
		manager = Engine::getManager();
		name = *entity->getComponent<SpriteComponent>().getName();
	}

	void update() override
	{
		if (health <= 0)
		{
			std::cout << name << " died" << "\n";


			entity->destroy();
		}
	}
	
	void TakeDamage(double damage)
	{
		health -= damage;
	}

	int getHealth() { return health; }

private:
	int health;
	Manager* manager;
	std::string name;
};