#include "Engine.h"
#include <vector>
#include "ECS.h"
#include "SDL3/sdl.h"
#include "TextureManager.h"
#include "WindowRender.h"
#include "AssetManager.h"
#include "AnimationJSON.h"
#include "UIAnimationJSON.h"
#include "StaticObjectJSON.h"
#include "Components.h"
#include "SpatialGrid.h"
#include "LoadingManager.h"
#include "PathFinder.h"
#include "EventBus.h"
#include "Vector2D.h"
#include "UIManager.h"
#include "SoundManager.h"

float Engine::deltaTime = 0.0f;
Uint64 Engine::lastFrameTime = 0;
SDL_Event* Engine::event = nullptr;
std::queue<SDL_Event> Engine::events;
bool Engine::isRunning = false;
WindowRender* Engine::windowRender = nullptr;
TextureManager* Engine::textureManager = nullptr;
AssetManager* Engine::assetManager = nullptr;
Manager* Engine::manager = nullptr;
AnimationJSON* Engine::animJson = nullptr;
StaticObjectJSON* Engine::objJson = nullptr;
UIAnimationJSON* Engine::uiAnimJson = nullptr;
SpatialGrid* Engine::collisionGrid = nullptr;
LoadingManager* Engine::loadingMgr = nullptr;
PathFinder* Engine::pathFinder = nullptr;
EventBus* Engine::eventBus = nullptr;
SDL_Rect Engine::camera = { 0, 0, 0, 0 };
Vector2D Engine::mouse = Vector2D(0,0);
UIManager* Engine::uiManager = nullptr;
SoundManager* Engine::soundManager = nullptr;

Engine::Engine()
{
	windowRender = new WindowRender();
	loadingMgr = new LoadingManager();
	textureManager = new TextureManager(windowRender);
}

Engine::~Engine()
{
	clean();
}

bool Engine::init(const char* title, int w, int h, int xpos, int ypos)
{
	if (windowRender->init(title, w, h, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED))
	{
		textureManager->setRenderer(windowRender->getRenderer());

		Size winSize = Engine::getWindowRender()->getWinSize();
		camera.w = winSize.w;
		camera.h = winSize.h;

		manager = new Manager;
		assetManager = new AssetManager(textureManager);
		animJson = new AnimationJSON();
		objJson = new StaticObjectJSON();
		event = new SDL_Event;
		eventBus = new EventBus();
		uiManager = new UIManager();
		uiAnimJson = new UIAnimationJSON();
		soundManager = new SoundManager();
		if (!TTF_Init()) return false;
		if (!soundManager->init()) return false;

		isRunning = true;
		lastFrameTime = SDL_GetTicks();
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
	soundManager->update();
}

void Engine::handleEvents()
{
	while (SDL_PollEvent(event))
	{
		switch (event->type)
		{
		case SDL_EVENT_QUIT:
			isRunning = false;
			break;

		case SDL_EVENT_WINDOW_RESIZED:
		{
			Size winSize = windowRender->getWinSize();
			camera.w = winSize.w;
			camera.h = winSize.h;
			std::cout << "Window resized! New camera size: " << camera.w << " x " << camera.h << "\n";
			break;
		}

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
	delete manager;
	delete windowRender;
	delete textureManager;
	delete assetManager;
	delete animJson;
	delete objJson;
	delete loadingMgr;
	delete eventBus;
	delete uiManager;
	delete uiAnimJson;
	delete soundManager;
	delete collisionGrid;
	delete pathFinder;
	delete event;
	TTF_Quit();
}

SpatialGrid* Engine::initCollisionGrid(int worldWidth, int worldHeight) 
{
	return collisionGrid = new SpatialGrid(worldWidth, worldHeight);
}

void Engine::calDeltaTime()
{
	Uint64 currentTime = SDL_GetTicks();
	float dt = (currentTime - lastFrameTime) / 1000.0f;
	lastFrameTime = currentTime;
	deltaTime = dt;
}

PathFinder* Engine::initPathFinder(int mapWidth, int mapHeight, int cellSizeX, int cellSizeY)
{
	return pathFinder = new PathFinder(mapWidth, mapHeight, cellSizeX, cellSizeY);

}