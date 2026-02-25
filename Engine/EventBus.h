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
	void subscribe(const std::function<void(T&)>& callback)
	{
		auto wrapper = [callback = std::move(callback)](void* data) {
			callback(*static_cast<T*>(data));
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
				callback(const_cast<T*>(&event));
			}
		}
	}

private:
	std::unordered_map<std::type_index, std::vector<std::function<void(void*)>>> subscribers;
};