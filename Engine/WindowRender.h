#pragma once
#include "EngineAPI.h"
#include "EngineTypes.h"
#include "SDLWrapper.h"

class ENGINE_API WindowRender
{
public:
	WindowRender();
	~WindowRender();

	bool init(const char* title, int w, int h, int xpos, int ypos, Flags flag);
	void clearRenderer();
	void presentRenderer();
	void clean();

	Renderer* getRenderer() const { return renderer; }

	bool isRunning = false;

private:
	Window* window = nullptr;
	Renderer* renderer = nullptr;
};