#include "HitboxComponent.h"
#include "SDL3/SDL.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "SpatialGrid.h"
#include "Engine.h"
#include "EngineAPI.h"
#include "DamageComponent.h"
#include "SpriteComponent.h"

void HitBoxComponent::updateFromFrame(SDL_FRect* animHitbox, float playerX, float playerY, bool isFlipped, float width, int anchorX, int anchorY)
{
    // Flip anchor X if sprite is flipped
    float adjustedAnchorX = anchorX;
    if (isFlipped) {
        adjustedAnchorX = width - anchorX;
    }

    // Convert anchor position to top-left position
    float topLeftX = playerX - adjustedAnchorX;
    float topLeftY = playerY - anchorY;

    if (isFlipped)
    {
        worldHitbox.x = topLeftX + (width * transform->scale) - (animHitbox->x * transform->scale) - (animHitbox->w * transform->scale);
    }
    else
    {
        worldHitbox.x = topLeftX + animHitbox->x * transform->scale;
    }
    worldHitbox.y = topLeftY + animHitbox->y * transform->scale;
    worldHitbox.w = animHitbox->w * transform->scale;
    worldHitbox.h = animHitbox->h * transform->scale;
    lastDebugHitbox = worldHitbox;
    drawDebug = true;
    On = true;
    alreadyHit.clear();
}

void HitBoxComponent::update()
{
    if (On)
    {
        auto nearby = Engine::getCollisionGrid()->Query(&worldHitbox);
        auto& entDam = entity->getComponent<DamageComponent>();
        double damage = entDam.getDamage();

        for (auto* otherCol : nearby)
        {
            Entity* target = otherCol->entity;
            if (target == this->entity) continue;
            if (!target->isActive()) continue;
            if (!target->hasComponent<HealthComponent>()) continue;

            if (std::find(alreadyHit.begin(), alreadyHit.end(), target) != alreadyHit.end()) continue;

            if (SDL_HasRectIntersectionFloat(&worldHitbox, otherCol->getRect()))
            {
                std::cout << "Contact\n";
                otherCol->entity->getComponent<HealthComponent>().TakeDamage(damage);
                alreadyHit.push_back(target);
                lastDebugTarget = *otherCol->getRect();
            }
        }
    }
    else
    {
        drawDebug = false;
    }
}