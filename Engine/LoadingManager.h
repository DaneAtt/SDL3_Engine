#pragma once
#include "Engine.h"
#include "EngineAPI.h"
#include <queue>

class ENGINE_API LoadingManager
{
public:
	LoadingManager() {};
	~LoadingManager() {};

	void queueTask(std::function<void()> task)
	{
		tasks.push(task);
	}
	void update()
	{
		if (!tasks.empty())
		{
			tasks.front()();
			tasks.pop();
		}
	}

	bool isDone() const
	{
		return tasks.empty();
	}

	/*
	Loading Template
	loadingManager->queueTask([this]()
	{

	});
	*/

private:
	std::queue<std::function<void()>> tasks;
	const char* curLoad;
};
