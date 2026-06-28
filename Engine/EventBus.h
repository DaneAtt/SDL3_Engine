#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include "EngineAPI.h"

class ENGINE_API EventBus
{
public:
	template<typename T>
	void subscribe(std::function<void(const T&)> callback)
	{
		// std::function<void(void*)> is the auto
		auto wrapper = [callback = std::move(callback)](const void* data) 
		{
			callback(*static_cast<const T*>(data));
		};
		subscribers[typeid(T)].push_back(wrapper);
	}

	template<typename T>
	void emit(const T& event)
	{
		auto it = subscribers.find(typeid(T));
		if (it != subscribers.end())
		{
			for (auto& callback : it->second)
			{	
				callback(&event);
			}
		}
	}

private:
	std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> subscribers;
};