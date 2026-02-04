#pragma once
#include "EngineAPI.h"
#include "Vector2D.h"

class Entity;
class Items;

struct ENGINE_API DamageEvent
{
	Entity* attacker;
	Entity* target;
};

struct ENGINE_API DeathEvent 
{
	Entity* entity;
};

struct ENGINE_API ButtonClickedEvent 
{
	std::string whatPURPOSE;
	int slotIndex;
	Items* item;
};

struct ENGINE_API InventoryRowLeftClickedEvent
{
	int slotIndex;
	Items* item;
};

struct ENGINE_API InventoryRowRightClickedEvent
{
	int slotIndex;
	Items* item;
	Vector2D mousePosition;
};

struct ENGINE_API ChestClickedEvent
{
	
};