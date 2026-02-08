#pragma once
#include "ECS.h"
#include "SDL3/SDL.h"
#include "EngineAPI.h"

class TransformComponent;

class HitBoxComponent : public Component
{
public:
    HitBoxComponent() : On(false) {}
    ~HitBoxComponent() {};

    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override;

    void updateFromFrame(SDL_FRect* animHitbox, float playerX, float playerY, bool flip, float width, int anchorX, int anchorY);

    void deactivate()
    {
        On = false;         
        lastDebugTarget = { 0, 0, 0, 0 };
        alreadyHit.clear();
    }

    SDL_FRect* getWorldHitBox() { return &worldHitbox; }

    bool isON(){ return On; }

    // getters for debug
    const SDL_FRect& getLastDebugHitbox() const { return lastDebugHitbox; }
    const SDL_FRect& getLastDebugTarget() const { return lastDebugTarget; }

private:
    SDL_FRect worldHitbox;
    TransformComponent* transform;
    bool On;
    SDL_FRect lastDebugHitbox;
    SDL_FRect lastDebugTarget;
    std::vector<Entity*> alreadyHit;
};