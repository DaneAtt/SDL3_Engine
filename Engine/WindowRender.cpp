#include "WindowRender.h"
#include <iostream>
#include <SDL3/SDL.h>

WindowRender::WindowRender()
{

}

WindowRender::~WindowRender()
{

}

bool WindowRender::init(const char* title, int w, int h, int xpos, int ypos)
{
	winSize.w = w;
	winSize.h = h;
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_CreateWindowAndRenderer(title, w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN,&window, &renderer);
		SDL_SetWindowPosition(window, xpos, ypos);


		//SDL_SetRenderVSync(renderer, 1);

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
	SDL_Quit();
}

void WindowRender::showWindow()
{
	if (window)
	{
		SDL_ShowWindow(window);
	}
}