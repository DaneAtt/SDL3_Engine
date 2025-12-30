#pragma once
#include "EngineAPI.h"
#include "ECS.h"
#include "SDL3/SDL.h"

class WindowRender;
class TextureManager;
class AssetManager;
class AnimationJSON;
class SpatialGrid;
class CollisionComponent;
class HitboxComponent;
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
    static AnimationJSON* getJSON() { return json; }
    static SpatialGrid* getCollisionGrid() { return collisionGrid; }
    static SDL_Rect* getCamera() { return &camera; }
    static void initCollisionGrid(int worldWidth, int worldHeight);

    static bool isRunning;
    static SDL_Event event;

private:
    static WindowRender* windowRender;
    static TextureManager* textureManager;
    static AssetManager* assetManager;
    static Manager* manager;
    static AnimationJSON* json;
    static SpatialGrid* collisionGrid;
    static SDL_Rect camera;
};