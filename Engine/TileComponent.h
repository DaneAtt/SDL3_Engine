#include "ECS.h"
#include "SDL3/SDL.h"
#include "Engine.h"
#include "AssetManager.h"
#include "TextureManager.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

class TileComponent : public RenderComponent
{
public:
    TileComponent(int srcX, int srcY, int scale, std::string texID, SDL_FlipMode flip)
    {
        ID = texID;
        texture = Engine::getAssetManager()->getTexture(ID);

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = 32;
        srcRect.h = 48;

        destRect.w = srcRect.w * scale;
        destRect.h = srcRect.h * scale;
        this->flip = flip;
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void draw() override {

        destRect.x = transform->position.x - Engine::getCamera()->x;
        destRect.y = transform->position.y - Engine::getCamera()->y;

        const SDL_Rect* cam = Engine::getCamera();
        if (destRect.x + destRect.w < 0 || destRect.x > cam->w ||
            destRect.y + destRect.h < 0 || destRect.y > cam->h) {
            return;
        }

        Engine::getTextureManager()->draw(texture, &srcRect, &destRect, 0.0,flip);
    }

    float getY() const { return destRect.y; }

private:
    SDL_FRect srcRect, destRect;
    SDL_FlipMode flip;
    std::string ID;
    TransformComponent* transform;
    SDL_Texture* texture;
};