#pragma once
#include "EngineAPI.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3/SDL.h"

class WindowRender;
class TextureManager;

enum class WindowMode
{
    FULLSCREEN,
    RESIZABLE,
    BORDERLESS
};

class ENGINE_API Engine {
public:
    Engine();
    ~Engine();

    // ========== CORE ==========
    bool init(const char* title, int w, int h, 
        int xpos = SDL_WINDOWPOS_CENTERED,
        int ypos = SDL_WINDOWPOS_CENTERED,
        WindowMode mode = WindowMode::RESIZABLE);
    void update();
    void handleEvents();
    void clean();

    // ========== WRAPPERS (80% use cases) ==========
    SDL_Texture* loadTexture(const char* path);
    void drawTexture(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, SDL_FlipMode flip);
    void clearRender();
    void presentRender();



    // ========== SUBSYSTEM ACCESS (Advanced use) ==========
    TextureManager* getTextureManager() const { return textureManager; };
    WindowRender* getWindowRender() const { return windowRender; };

    bool isRunning = false;

private:
    WindowRender* windowRender;
    TextureManager* textureManager;
};