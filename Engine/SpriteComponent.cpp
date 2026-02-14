#include "SpriteComponent.h"
#include "HitBoxComponent.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(const char* id)
{
    setTex(id);
    name = id;
}

SpriteComponent::SpriteComponent(const char* id, bool animation)
{
    animated = animation;
    animIndex = 0;
    animationID = id;
    setTex(id);
    name = id;
}

SpriteComponent::~SpriteComponent() {}

void SpriteComponent::setTex(std::string id)
{
    texture = Engine::getAssetManager()->getTexture(id);
}

void SpriteComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();

    srcRect.x = srcRect.y = 0.0f;
    srcRect.w = static_cast<float>(transform->width);
    srcRect.h = static_cast<float>(transform->height);

    if (animated)
    {
        currentAnimation = Engine::getAnimJSON()->searchAnimation(animationID);

        if (!currentAnimation.FramesVariation.empty())
        {
            usingVariation = true;
            currentVariation = currentAnimation.FramesVariation.begin()->first;
            currentFrame = &currentAnimation.FramesVariation[currentVariation][0];
        }
        else
        {
            usingVariation = false;
            currentFrame = &currentAnimation.Frames[0];
        }

        transform->width = static_cast<int>(currentFrame->w);
        transform->height = static_cast<int>(currentFrame->h);

        if (currentAnimation.Hitbo)
        {
            hitbox = &entity->addComponent<HitBoxComponent>();
        }
    }
    else
    {
        destRect.w = static_cast<float>(transform->width);
        destRect.h = static_cast<float>(transform->height);
    }
}

void SpriteComponent::update()
{
    try
    {
        // system setting their own index instead of increment;
        if (dFrame != -1)
        {
            animIndex = dFrame;
            dFrame = -1;
        }

        if (animated)
        {
            if (usingVariation)
            {
                auto& variationFrames = currentAnimation.FramesVariation[currentVariation];
                if (variationFrames.empty()) 
                {
                    std::cout << "ERROR: Variation has no frames!\n";
                    return;
                }

                checkManualControl(manualControl);

                currentFrame = &variationFrames[animIndex];
            }
            else
            {
                if (currentAnimation.Frames.empty()) 
                {
                    std::cout << "ERROR: Animation has no frames!\n";
                    return;
                }

                checkManualControl(manualControl);

                currentFrame = &currentAnimation.Frames[animIndex];
            }

            srcRect = *currentFrame;
            destRectCalculation();
            checkHitBox();
        }
        else
        {
            destRect.x = transform->position.x - Engine::getCamera()->x;
            destRect.y = transform->position.y - Engine::getCamera()->y;
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }
    }
    catch (std::exception)
    {
        std::cout << "SpriteComponent __FUNCTION__ bug" << "\n";
    }
}

void SpriteComponent::draw()
{
    if (destRect.x + destRect.w < 0 || destRect.x > Engine::getCamera()->w ||
        destRect.y + destRect.h < 0 || destRect.y > Engine::getCamera()->h) {
        return;
    }
    SDL_FlipMode actualFlip = (currentAnimation.canFlip) ? spriteFlip : SDL_FLIP_NONE;
    Engine::getTextureManager()->draw(texture, &srcRect, &destRect, actualFlip);
}

void SpriteComponent::switchAnimation(std::string id, bool isAnimated)
{
    animated = isAnimated;
    setTex(id);

    if (animated) {
        currentAnimation = Engine::getAnimJSON()->searchAnimation(id.c_str());
        animIndex = 0;
        frameTimer = 0.0f;


        if (!currentAnimation.FramesVariation.empty())
        {
            usingVariation = true;
            currentVariation = currentAnimation.FramesVariation.begin()->first;
            currentFrame = &currentAnimation.FramesVariation[currentVariation][0];
        }
        else if (!currentAnimation.Frames.empty()) 
        {
            usingVariation = false;
            currentFrame = &currentAnimation.Frames[0];
        }
        else
        {
            std::cout << "ERROR: Animation '" << id << "' has no frames!\n";
            return;
        }

        transform->width = static_cast<int>(currentFrame->w);
        transform->height = static_cast<int>(currentFrame->h);

        // UPDATE COLLISION BOX
        if (entity->hasComponent<CollisionComponent>()) {
            auto& collision = entity->getComponent<CollisionComponent>();
            *collision.getCollision() = currentAnimation.collisionRect;
        }
    }
}

void SpriteComponent::checkManualControl(bool control)
{
    if (!control)
    {
        float delta = Engine::getDeltaTime();
        float frameDuration = 1.0f / 24.0f;

        frameTimer += delta;
        if (frameTimer >= frameDuration)
        {
            frameTimer -= frameDuration;
            animIndex++;

            size_t maxFrames = 0;
            if (usingVariation) 
            {
                maxFrames = currentAnimation.FramesVariation[currentVariation].size();
            }
            else 
            {
                maxFrames = currentAnimation.Frames.size();
            }

            if (animIndex >= maxFrames) 
            {
                if (currentAnimation.Loop) 
                {
                    animIndex = 0;
                }
                else {
                    animIndex = static_cast<int>(maxFrames) - 1;
                }
            }
        }
    }
}

void SpriteComponent::destRectCalculation()
{
    if (currentAnimation.fixedFrame)
    {
        float anchorOffsetX = static_cast<float>(currentAnimation.anchorX) * transform->scale;

        destRect.w = static_cast<float>(transform->width) * transform->scale;
        destRect.h = static_cast<float>(transform->height) * transform->scale;
        destRect.y = transform->position.y - (currentAnimation.anchorY * transform->scale) - Engine::getCamera()->y;

        if (spriteFlip == SDL_FLIP_HORIZONTAL && currentAnimation.canFlip) {
            destRect.x = transform->position.x - (destRect.w - anchorOffsetX) - Engine::getCamera()->x;
        }
        else {
            destRect.x = transform->position.x - anchorOffsetX - Engine::getCamera()->x;
        }
    }
    else
    {
        float anchorOffsetX = static_cast<float>(currentAnimation.anchorX) * transform->scale;

        destRect.w = currentFrame->w * transform->scale;
        destRect.h = currentFrame->h * transform->scale;
        destRect.x = transform->position.x - anchorOffsetX - Engine::getCamera()->x;
        destRect.y = transform->position.y - (currentAnimation.anchorY * transform->scale) - Engine::getCamera()->y;

        if (spriteFlip == SDL_FLIP_HORIZONTAL && currentAnimation.canFlip) {
            destRect.x = transform->position.x - (destRect.w - anchorOffsetX) - Engine::getCamera()->x;
        }
    }
}

void SpriteComponent::checkHitBox()
{
    if (hitbox)
    {
        SDL_FRect* frameHitbox = nullptr;
        bool hasHitbox = false;

        if (usingVariation)
        {
            // Check variation hitbox map
            auto& varHitboxMap = currentAnimation.VariationHitboxMap[currentVariation];
            if (currentAnimation.Hitbo && varHitboxMap.count(animIndex))
            {
                frameHitbox = &varHitboxMap[animIndex];
                hasHitbox = true;
            }
        }
        else
        {
            if (currentAnimation.Hitbo && currentAnimation.HitboxMap.count(animIndex))
            {
                frameHitbox = &currentAnimation.HitboxMap[animIndex];
                hasHitbox = true;
            }
        }

        if (hasHitbox)
        {
            if (animIndex != previousAnimIndex)
            {
                if (!hitbox->isON())
                {
                    boxStartX = transform->position.x;
                    boxStartY = transform->position.y;
                }
                bool flip = (spriteFlip == SDL_FLIP_HORIZONTAL && currentAnimation.canFlip);
                hitbox->updateFromFrame(frameHitbox, boxStartX, boxStartY, flip, currentFrame->w, currentAnimation.anchorX, currentAnimation.anchorY);
            }
        }
        else
        {
            hitbox->deactivate();
        }
    }
    previousAnimIndex = animIndex;
}

void SpriteComponent::setVariation(const std::string& variationName) 
{
    if (usingVariation) 
    {
        currentVariation = variationName;
        animIndex = 0;
    }
}