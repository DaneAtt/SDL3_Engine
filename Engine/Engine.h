#pragma once
#include "EngineAPI.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <queue>

class WindowRender;
class TextureManager;
class AssetManager;
class AnimationJSON;
class SpatialGrid;
class CollisionComponent;
class HitboxComponent;
class LoadingManager;
class PathFinder;
class EventBus;
class Vector2D;
class UIManager;
class Manager;
struct Size;


class ENGINE_API Engine {
public:
    Engine();
    ~Engine();

    bool init(const char* title, int w, int h, int xpos, int ypos);
    void update();
    void handleEvents();
    void clean();


    static TextureManager* getTextureManager() { return textureManager; }
    static WindowRender* getWindowRender() { return windowRender; }
    static AssetManager* getAssetManager() { return assetManager; }
    static Manager* getManager() { return manager; }
    static AnimationJSON* getAnimJSON() { return animJson; }
    static void initCollisionGrid(int worldWidth, int worldHeight);
    static SpatialGrid* getCollisionGrid() { return collisionGrid; }
    static SDL_Rect* getCamera() { return &camera; }
    static LoadingManager* getLoadingMgr() { return loadingMgr; }
    static SDL_Event* getEvent() { return event; }
    static std::queue<SDL_Event>& getEventQueue() { return events; }
    static float& getDeltaTime() { return deltaTime; }
    static void calDeltaTime();
    static void initPathFinder(int mapWidth, int mapHeight, int cellSizeX, int cellSizeY);
    static PathFinder* getPathFinder() { return pathFinder; }
    static EventBus* getEventBus() { return eventBus; }
    static Vector2D& getMouse() { return mouse; }
    static UIManager* getUIManager() { return uiManager; }
    static bool isRunning;

private:
    static WindowRender* windowRender;
    static TextureManager* textureManager;
    static AssetManager* assetManager;
    static Manager* manager;
    static AnimationJSON* animJson;
    static SpatialGrid* collisionGrid;
    static SDL_Rect camera;
    static LoadingManager* loadingMgr;
    static SDL_Event* event;
    static std::queue<SDL_Event> events;
    static float deltaTime;
    static Uint64 lastFrameTime;
    static PathFinder* pathFinder;
    static EventBus* eventBus;
    static Vector2D mouse;
    static UIManager* uiManager;
};