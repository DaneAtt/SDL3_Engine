#pragma once
#include "EngineAPI.h"
#include "EngineTypes.h"

class WindowRender;
class TextureManager;
class Texture;
struct Rect;
enum class FlipMode;

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
    bool init(const char* title, int w, int h, int xpos, int ypos, Flags flag = Flags::RESIZABLE);
    void update();
    void handleEvents();
    void clean();

    // ========== WRAPPERS (80% use cases) ==========
    Texture* loadTexture(const char* path);
    void drawTexture(Texture* tex, const Rect* srcRect, const Rect* desRect, FlipMode flip);
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