#include "WindowRender.h"
#include <iostream>

WindowRender::WindowRender()
{

}

WindowRender::~WindowRender()
{

}

bool WindowRender::init(const char* title, int w, int h, int xpos, int ypos, SDL_WindowFlags flag)
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_CreateWindowAndRenderer(title, w, h, flag, &window, &renderer);
		SDL_SetWindowPosition(window, xpos, ypos);

		std::cout << (window ? "Window initialized" : "Window Failed: " + std::string(SDL_GetError())) << "\n"
			<< (renderer ? "Renderer initialized" : "Renderer Failed: " + std::string(SDL_GetError())) << std::endl;

		return true;
	}
	else 
	{
		return false;
	}
}

void WindowRender::clearRenderer()
{
	SDL_RenderClear(renderer);
}

void WindowRender::presentRenderer()
{
	SDL_RenderPresent(renderer);
}

void WindowRender::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}