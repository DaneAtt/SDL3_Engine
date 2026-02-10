#pragma once
#include "EngineAPI.h"
#include "Vector2D.h"

class Entity;
class Items;
class ChestComponent;

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
	ChestComponent* chest;
};

struct ENGINE_API ContextMenuDeleteEvent
{
	int slotIndex;
	Items* item;
};

struct ENGINE_API ChestUIExitEvent
{
	ChestComponent* chest;
};

struct ENGINE_API SkillbarUIRightClickEvent
{
	int row;
	int col;
	Vector2D mousePosition;
};