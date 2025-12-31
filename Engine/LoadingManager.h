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

	const char* getCurrentLoadingScreen(int index) 
	{ 
		switch (index)
		{
		case 1:
			curLoad = "D:/Game_cpp/Game/ResourceFiles/loading.png";
			break;
		case 2:
			break;
		case 3:
			break;
		}
		return curLoad; 
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
