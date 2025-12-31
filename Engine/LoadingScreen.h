#pragma once
#include "EngineAPI.h"
#include "SDL3/SDL.h"
#include <string>
#include "Animation.h"

class AnimationJSON;
class TextureManager;
class WindowRender;
class AssetManager;

class ENGINE_API LoadingScreen
{
public:
    LoadingScreen();
    ~LoadingScreen();

    // Initialize loading screen with texture and animation
    void init(const char* texturePath, const char* animJsonPath, const char* animName);

    // Update animation frame based on time
    void update(float deltaTime);

    // Render the current frame stretched to fill screen
    void render();

    // Clean up resources
    void cleanup();

    bool isInitialized() const { return initialized; }

private:
    TextureManager* textureManager;
    WindowRender* windowRender;
    AssetManager* assetManager;
    AnimationJSON* json;

    SDL_Texture* texture;
    Animation* animation;
    Animation animationCopy;
    bool hasAnimation;
    bool useAnimationCopy;

    int currentFrame;
    float frameTimer;
    bool initialized;

    std::string textureKeyStr;
    std::string animationNameStr;
};