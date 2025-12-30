#pragma once
#include "EngineAPI.h"
#include "SDL3/SDL.h"
#include <vector>

struct Size
{
	int w;
	int h;
};

class ENGINE_API WindowRender
{
public:

	WindowRender();
	~WindowRender();

	bool init(const char* title, int w, int h, int xpos, int ypos);
	void clearRenderer();
	void presentRenderer();
	void clean();

	SDL_Renderer* getRenderer() const { return renderer; }
	Size getWinSize() const {
		Size currentSize;
		SDL_GetWindowSize(window, &currentSize.w, &currentSize.h);
		return currentSize;
	}

	bool isRunning = false;

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Size winSize;
};