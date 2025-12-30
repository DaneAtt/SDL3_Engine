#include "HitboxComponent.h"
#include "SDL3/SDL.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "SpatialGrid.h"
#include "Engine.h"
#include "EngineAPI.h"

void HitBoxComponent::updateFromFrame(SDL_FRect* animHitbox, float playerX, float playerY)
{
    worldHitbox.x = playerX + animHitbox->x * transform->scale;
    worldHitbox.y = playerY + animHitbox->y * transform->scale;
    worldHitbox.w = animHitbox->w * transform->scale;
    worldHitbox.h = animHitbox->h * transform->scale;

    lastDebugHitbox = worldHitbox;  // always update
    drawDebug = true;                // always show hitbox

    On = true;
}

void HitBoxComponent::update()
{
    if (On)
    {
        auto nearby = Engine::getCollisionGrid()->Query(&worldHitbox);
        for (auto* otherCol : nearby)
        {
            Entity* target = otherCol->entity;
            if (target == this->entity) continue;
            if (!target->hasComponent<HealthComponent>()) continue;
            if (SDL_HasRectIntersectionFloat(&worldHitbox, otherCol->getRect()))
            {
                std::cout << "Contact\n";
                otherCol->entity->getComponent<HealthComponent>().TakeDamage(1);
                lastDebugTarget = *otherCol->getRect();
            }
        }
    }
}