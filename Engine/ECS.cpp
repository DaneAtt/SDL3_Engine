#include "ECS.h"
#include <mutex>
#include <unordered_map>
#include <typeindex>

// single shared counter defined in one translation unit so all TUs/DLLs see same IDs
ENGINE_API ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

// single shared map from std::type_index -> ComponentID
static std::mutex g_componentIDMutex;
static std::unordered_map<std::type_index, ComponentID> g_componentIDMap;

ENGINE_API ComponentID getComponentTypeIDForType(const std::type_index& typeIndex)
{
	std::lock_guard<std::mutex> lock(g_componentIDMutex);
	auto it = g_componentIDMap.find(typeIndex);
	if (it != g_componentIDMap.end()) return it->second;

	ComponentID id = getNewComponentTypeID();
	g_componentIDMap.emplace(typeIndex, id);
	return id;
}

void Entity::addGroup(Group mGroup)
{
	groupBitset[mGroup] = true;
	manager.AddToGroup(this, mGroup);
}
