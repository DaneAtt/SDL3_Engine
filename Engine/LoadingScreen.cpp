#include "LoadingScreen.h"
#include "Engine.h"
#include "TextureManager.h"
#include "WindowRender.h"
#include "AssetManager.h"
#include "AnimationJSON.h"
#include "Animation.h"
#include <iostream>

LoadingScreen::LoadingScreen()
    : texture(nullptr)
    , animation(nullptr)
    , hasAnimation(false)
    , useAnimationCopy(false)
    , currentFrame(0)
    , frameTimer(0.0f)
    , initialized(false)
{
    textureManager = Engine::getTextureManager();
    windowRender = Engine::getWindowRender();
    assetManager = Engine::getAssetManager();
    json = Engine::getJSON();
}

LoadingScreen::~LoadingScreen()
{
    cleanup();
}

void LoadingScreen::init(const char* texturePath, const char* animJsonPath, const char* animName)
{
    if (initialized) {
        return;
    }

    textureKeyStr = "loading_screen_texture";
    if (animName) {
        animationNameStr = animName;
    }

    // Load texture
    assetManager->AddTexture(textureKeyStr.c_str(), texturePath);
    texture = assetManager->GetTexture(textureKeyStr.c_str());

    // Load animation if provided
    if (animJsonPath && animName) {
        json->loadAnimationJSON(animJsonPath);
        animation = &json->searchAnimation(animName);

        // Check if animation was found and valid
        if (animation && !animation->Frames.empty()) {

            // CRITICAL: Copy the animation data locally to protect from map reallocation
            try {
                animationCopy = *animation;  // Deep copy the Animation object

                useAnimationCopy = true;
                hasAnimation = true;
            }
            catch (const std::exception& e) {
                std::cout << "[LoadingScreen::init] ERROR copying animation: " << e.what() << "\n";
                hasAnimation = false;
                useAnimationCopy = false;
            }
        }
        else {
            std::cout << "[LoadingScreen::init] WARNING: Animation not found or has no frames\n";
            animation = nullptr;
            hasAnimation = false;
            useAnimationCopy = false;
        }
    }
    else {
        hasAnimation = false;
        useAnimationCopy = false;
        std::cout << "[LoadingScreen::init] No animation specified, using static mode\n";
    }

    currentFrame = 0;
    frameTimer = 0.0f;
    initialized = true;

}

void LoadingScreen::update(float deltaTime)
{
    if (!initialized || !hasAnimation) {
        return;
    }

    // Use our local copy (safe from external map changes)
    Animation& currentAnim = animationCopy;

    if (currentAnim.Frames.empty()) {
        hasAnimation = false;
        return;
    }

    frameTimer += deltaTime;
    float frameDuration = (currentAnim.Duration / currentAnim.Frames.size()) / 5.0f;

    if (frameTimer >= frameDuration)
    {
        frameTimer = 0.0f;
        currentFrame++;

        if (currentFrame >= currentAnim.Frames.size())
        {
            if (currentAnim.Loop) {
                currentFrame = 0;
            }
            else {
                currentFrame = currentAnim.Frames.size() - 1;
            }
        }
    }
}

void LoadingScreen::render()
{
    if (!initialized || !texture) {
        return;
    }

    SDL_Renderer* renderer = windowRender->getRenderer();
    Size winSize = windowRender->getWinSize();

    SDL_FRect destRect = {
        0.0f,
        0.0f,
        (float)winSize.w,
        (float)winSize.h
    };

    if (hasAnimation) {
        // Use our local copy (safe from external changes)
        Animation& currentAnim = animationCopy;

        if (!currentAnim.Frames.empty() && currentFrame >= 0 && currentFrame < currentAnim.Frames.size()) {
            SDL_FRect srcRect = currentAnim.Frames[currentFrame];
            SDL_RenderTexture(renderer, texture, &srcRect, &destRect);
        }
    }
    else {
        // Static mode
        SDL_RenderTexture(renderer, texture, nullptr, &destRect);
    }
}

void LoadingScreen::cleanup()
{
    if (!initialized) {
        return;
    }

    texture = nullptr;
    animation = nullptr;
    hasAnimation = false;
    useAnimationCopy = false;
    initialized = false;
    currentFrame = 0;
    frameTimer = 0.0f;
}