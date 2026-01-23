#pragma once
#include "ECS.h"
#include "Engine.h"
#include "Events.h"
#include "EventBus.h"


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
	}
	
	void TakeDamage(double damage)
	{
		health -= damage;
		if (health <= 0) {
			health = 0;

			// Emit death event
			Engine::getEventBus()->emit(DeathEvent{ entity });
		}
	}

	int getHealth() { return health; }

private:
	int health;
	Manager* manager;
	std::string name;
};