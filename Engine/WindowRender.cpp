#include "WindowRender.h"
#include <iostream>
#include <SDL3/SDL.h>

WindowRender::WindowRender()
{

}

WindowRender::~WindowRender()
{
	clean();
}

bool WindowRender::init(const char* title, int w, int h, int xpos, int ypos, Flags flag)
{
	SDL_WindowFlags sdlFlag;
	switch (flag)
	{
		case Flags::FULLSCREEN:
		{
			sdlFlag = SDL_WINDOW_FULLSCREEN;
			break;
		}
		case Flags::BORDERLESS:
		{
			sdlFlag = SDL_WINDOW_BORDERLESS;
			break;
		}
		default:
		{
			sdlFlag = SDL_WINDOW_RESIZABLE;
			break;
		}
	}

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Window* sdlWindow;
		SDL_Renderer* sdlRenderer;
		SDL_CreateWindowAndRenderer(title, w, h, sdlFlag, &sdlWindow, &sdlRenderer);
		SDL_SetWindowPosition(sdlWindow, xpos, ypos);

		std::cout << (window ? "Window initialized" : "Window Failed: " + std::string(SDL_GetError())) << "\n"
			<< (renderer ? "Renderer initialized" : "Renderer Failed: " + std::string(SDL_GetError())) << std::endl;

		window = new Window(sdlWindow);
		renderer = new Renderer(sdlRenderer);

		return true;
	}
	else 
	{
		return false;
	}
}

void WindowRender::clearRenderer()
{
	SDL_RenderClear(renderer->getSDLRenderer());
}

void WindowRender::presentRenderer()
{
	SDL_RenderPresent(renderer->getSDLRenderer());
}

void WindowRender::clean()
{
	SDL_DestroyWindow(window->getSDLWindow());
	SDL_DestroyRenderer(renderer->getSDLRenderer());
	SDL_Quit();
}
