#pragma once
#include "EngineAPI.h"
#include "SDL3/SDL.h"
#include <vector>
#include "Size.h"

class ENGINE_API WindowRender
{
public:

	WindowRender() {}
	~WindowRender() {}

	bool init(const char* title, int w, int h, int xpos, int ypos);
	void clearRenderer();
	void presentRenderer();
	void clean();

	SDL_Renderer* getRenderer() const { return renderer; }
	SDL_Window* getWindow() const { return window; }
	Size getWinSize() const {
		Size currentSize;
		int size_width, size_height;
		SDL_GetWindowSize(window, &size_width, &size_height);
		currentSize.w = static_cast<float>(size_width);
		currentSize.h = static_cast<float>(size_height);
		return currentSize;
	}

	void showWindow();


private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Size winSize;
};