#include "Engine.h"
#include "SDL3/sdl.h"
#include "SDLWrapper.h"
#include "TextureManager.h"
#include "WindowRender.h"

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

bool Engine::init(const char* title, int w, int h, int xpos, int ypos, Flags flag)
{

	int sdlXPos = (xpos == CENTERED) ? SDL_WINDOWPOS_CENTERED : xpos;
	int sdlYPos = (ypos == CENTERED) ? SDL_WINDOWPOS_CENTERED : ypos;

	if (windowRender->init(title, w, h, sdlXPos, sdlYPos, flag))
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


