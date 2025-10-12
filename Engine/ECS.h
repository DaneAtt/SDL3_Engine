#pragma once
#include <iostream>
#include <bitset>
#include <array>
#include <memory>
#include <vector>

using ComponentID = std::size_t;
using Group = std::size_t;

class Component;
class Entity;
class Manager;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;


class Component
{
public:
	Entity* entity;
	virtual void init();
	virtual void update();
	virtual void draw();

	virtual ~Component();

};

class Entity
{
public:
	Entity(Manager& mManager) : manager(mManager) {};

	void update()
	{
		for (auto& c : components)
		{
			c->update();
		}
	}

	void draw()
	{
		for (auto& c : components)
		{
			c->draw();
		}
	}

	bool const isActive() { return active; };
	void destroy() { active = false; };

	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()]
	}

	template <typename T, typename... TArgs> 
	T& addComponent(TArgs&&... mArgs)
	{
		T* c = new T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));
		componentArray[getComponentTypeID<T>] = c;
		componentBitset[getComponentTypeID<T>] = true;

		c->init(); //initialise inside the T's init
		return *c;
	}

	template <typename T> T& getComponent() const
	{
		auto ptr = componentArray[getComponentTypeID<T>()]; //ptr is getting Array's index, which is Component's pointer
		return *static_cast<T*>(ptr); //static cast to its T because its Component* right now
	}

private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitSet groupBitset;
};

class Manager
{
public:
	void update()
	{
		for (auto& e : entities)
		{
			e->update();
		}
	}

	void draw()
	{
		for (auto& e : entities)
		{
			e->draw();
		}
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v = groupedEntities[i];
			v.erase(std::remove_if(std::begin(v), std::end(v),
				[i](Entity* mEntity)
				{
					return !mEntity->isActive() || !mEntity->hasGroup(i);
				}),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity> &mEntity) //reference cause unique ptr can't be copied
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

};