#include "Engine.h"
#include "ECS.h"
#include "SDL3/sdl.h"
#include "TextureManager.h"
#include "WindowRender.h"
#include "AssetManager.h"
#include "AnimationJSON.h"
#include "Components.h"
#include "SpatialGrid.h"
#include "LoadingManager.h"
#include "PathFinder.h"
#include "EventBus.h"
#include "Vector2D.h"
#include "UIManager.h"
#include <vector>

float Engine::deltaTime = 0.0f;
Uint64 Engine::lastFrameTime = 0;
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
PathFinder* Engine::pathFinder = nullptr;
EventBus* Engine::eventBus = nullptr;
SDL_Rect Engine::camera = { 0, 0, 0, 0 };
Vector2D Engine::mouse = Vector2D(0,0);
UIManager* Engine::uiManager = nullptr;

Engine::Engine()
{
	manager = new Manager;
	windowRender = new WindowRender();
	textureManager = new TextureManager(windowRender);
	assetManager = new AssetManager(manager, textureManager);
	json = new AnimationJSON();
	loadingMgr = new LoadingManager();
	event = new SDL_Event;
	eventBus = new EventBus();
	uiManager = new UIManager();
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

		TTF_Init();

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
	calDeltaTime();
	SDL_GetMouseState(&mouse.x, &mouse.y);
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
			events.push(*event);
			break;

		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP:
			events.push(*event);
			break;

		case SDL_EVENT_MOUSE_WHEEL:
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
	TTF_Quit();
}

void Engine::initCollisionGrid(int worldWidth, int worldHeight) 
{
	collisionGrid = new SpatialGrid(worldWidth, worldHeight);
}

void Engine::calDeltaTime()
{
	Uint64 currentTime = SDL_GetTicks();
	float dt = (currentTime - lastFrameTime) / 1000.0f;
	lastFrameTime = currentTime;
	deltaTime = dt;
}

void Engine::initPathFinder(int mapWidth, int mapHeight, int cellSizeX, int cellSizeY)
{
	pathFinder = new PathFinder(mapWidth, mapHeight, cellSizeX, cellSizeY);

}