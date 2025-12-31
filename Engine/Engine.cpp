#include "Engine.h"
#include "SDL3/sdl.h"
#include "TextureManager.h"
#include "WindowRender.h"
#include "AssetManager.h"
#include "AnimationJSON.h"
#include "Components.h"
#include "SpatialGrid.h"
#include "LoadingManager.h"
#include <vector>

SDL_Event* Engine::event = nullptr;
std::queue<SDL_Event> Engine::events;
bool Engine::isRunning = false;
WindowRender* Engine::windowRender = nullptr;
TextureManager* Engine::textureManager = nullptr;
AssetManager* Engine::assetManager = nullptr;
Manager* Engine::manager = nullptr;
AnimationJSON* Engine::json = nullptr;
SpatialGrid* Engine::collisionGrid = nullptr;
LoadingManager* Engine::loadingMgr = nullptr;
SDL_Rect Engine::camera = { 0, 0, 0, 0 };

Engine::Engine()
{
	manager = new Manager;
	windowRender = new WindowRender();
	textureManager = new TextureManager(windowRender);
	assetManager = new AssetManager(manager, textureManager);
	json = new AnimationJSON(1);
    collisionGrid = new SpatialGrid();
	loadingMgr = new LoadingManager();
	event = new SDL_Event;
}

Engine::~Engine()
{
}

bool Engine::init(const char* title, int w, int h, int xpos, int ypos)
{

	if (windowRender->init(title, w, h, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED))
	{
		textureManager->setRenderer(windowRender->getRenderer());

		Size winSize = Engine::getWindowRender()->getWinSize();
		camera.w = winSize.w;
		camera.h = winSize.h;

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
	while (SDL_PollEvent(event))
	{

		if (event->type == SDL_EVENT_QUIT)
		{
			isRunning = false;
		}
		if (event->type == SDL_EVENT_WINDOW_RESIZED)
		{
			Size winSize = windowRender->getWinSize();
			camera.w = winSize.w;
			camera.h = winSize.h;
			std::cout << "Window resized! New camera size: " << camera.w << " x " << camera.h << "\n";
		}

		switch (event->type)
		{
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_KEY_DOWN:
			events.push(*event);
			break;

		case SDL_EVENT_WINDOW_FOCUS_GAINED:
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			// Handle focus events
			break;
		}
	}
}

void Engine::clean()
{
	delete textureManager;
	delete windowRender;
	delete assetManager;
	delete manager;
	delete json;
	delete collisionGrid;
}

void Engine::initCollisionGrid(int worldWidth, int worldHeight) 
{
	collisionGrid = new SpatialGrid(worldWidth, worldHeight);
}