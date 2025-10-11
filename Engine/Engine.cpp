#include "Engine.h"
#include "WindowRender.h"
#include "TextureManager.h"

Engine::Engine()
{
	windowRender = new WindowRender();
	textureManager = new TextureManager(windowRender);
}

Engine::~Engine()
{
	delete windowRender;
	delete textureManager;
}

bool Engine::init(const char* title, int w, int h, int xpos, int ypos, WindowMode flag)
{
	SDL_WindowFlags sdlFlag;

	switch(flag)
	{
		case WindowMode::FULLSCREEN:
		{
			sdlFlag = SDL_WINDOW_FULLSCREEN;
			break;
		}
		case WindowMode::BORDERLESS:
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

	if (windowRender->init(title, w, h, xpos, ypos, sdlFlag))
	{
		isRunning = true;
		return true;
	}
	else
	{
		isRunning = false;
		return false;
	}
}

void Engine::update()
{

}

void Engine::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			isRunning = false;
		}
	}
}

void Engine::clean()
{
	windowRender->clean();
}



