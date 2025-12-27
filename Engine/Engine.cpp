#include "Engine.h"
#include "SDL3/sdl.h"
#include "TextureManager.h"
#include "WindowRender.h"
#include "AssetManager.h"
#include "AnimationJSON.h"

SDL_Event Engine::event;
bool Engine::isRunning = false;
WindowRender* Engine::windowRender = nullptr;
TextureManager* Engine::textureManager = nullptr;
AssetManager* Engine::assetManager = nullptr;
Manager* Engine::manager = nullptr;
AnimationJSON* Engine::json = nullptr;

Engine::Engine()
{
	manager = new Manager;
	windowRender = new WindowRender();
	textureManager = new TextureManager(windowRender);
	assetManager = new AssetManager(manager, textureManager);
	json = new AnimationJSON();
}

Engine::~Engine()
{
}

bool Engine::init(const char* title, int w, int h, int xpos, int ypos)
{

	if (windowRender->init(title, w, h, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED))
	{
		textureManager->setRenderer(windowRender->getRenderer());
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
	SDL_PollEvent(&Engine::event);

	if (event.type == SDL_EVENT_QUIT)
	{
			isRunning = false;
	}
}

void Engine::clean()
{
	delete textureManager;
	delete windowRender;
	delete assetManager;
	delete manager;
	delete json;
}


