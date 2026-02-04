#pragma once
#include <map>
#include "TextureManager.h"
#include "AssetManager.h"
#include "Engine.h"
#include "ECS.h"
#include "SDL3/sdl.h"
#include "Animation.h"
#include "AnimationJSON.h"
#include "EngineAPI.h"

class HitBoxComponent;
class TransformComponent;

class SpriteComponent : public Component
{
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_FRect srcRect, destRect;

    bool animated = false;
    int frames = 0;
    int dFrame = -1;
    Animation currentAnimation;
    HitBoxComponent* hitbox;
    const char* animationID;
    int previousAnimIndex = -1;
    float boxStartX, boxStartY, frameTimer;
    std::string name;
    bool manualControl = false;

public:
    int animIndex;
    Uint32 currentTime;
    SDL_FRect* currentFrame = nullptr;
    SDL_FlipMode spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    ENGINE_API SpriteComponent(const char* id);
    ENGINE_API SpriteComponent(const char* id, bool animation);
    ENGINE_API ~SpriteComponent();

    ENGINE_API void setTex(std::string id);
    ENGINE_API void switchAnimation(std::string id, bool isAnimated);
    void init() override;
    void update() override;
    void draw() override;

    bool isAnimated() { return animated; }

    void setFrame(int index) { dFrame = index; }
    void setManualControl(bool b) { manualControl = b; }

    SDL_FRect* getDestRect() { return &destRect; }
    Animation& getCurrentAnimation() { return currentAnimation; }
    std::string* getName() { return &name; }
};