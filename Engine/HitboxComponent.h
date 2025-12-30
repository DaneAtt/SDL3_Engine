#pragma once
#include "ECS.h"
#include "SDL3/SDL.h"
#include "EngineAPI.h"

class TransformComponent;

class HitBoxComponent : public Component
{
public:
    HitBoxComponent() { On = false; }
    ~HitBoxComponent() {};

    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        if (On)
        {

        }
    }

    ENGINE_API void updateFromFrame(SDL_FRect animHitbox, float playerX, float playerY);

    void deactivate(){ On = false; }

    // getters for debug
    const SDL_FRect& getLastDebugHitbox() const { return lastDebugHitbox; }
    const SDL_FRect& getLastDebugTarget() const { return lastDebugTarget; }

    bool getDrawDebug() const { return drawDebug; }

private:
    SDL_FRect worldHitbox;
    TransformComponent* transform;
    bool On;
    SDL_FRect lastDebugHitbox{};
    SDL_FRect lastDebugTarget{};
    bool drawDebug = false;
};