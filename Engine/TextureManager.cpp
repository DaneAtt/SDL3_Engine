#include "TextureManager.h"
#include "SDL3_image/SDL_image.h"
#include "Engine.h"


TextureManager::TextureManager(WindowRender* windowRender)
{
	renderer = windowRender->getRenderer();
}

TextureManager::~TextureManager()
{
	for (auto tex : textures)
	{
		if(tex)
		{
			SDL_DestroyTexture(tex);
		}
	}
	textures.clear();
}

SDL_Texture* TextureManager::LoadTexture(const char* path)
{
	SDL_Surface* tempSurface = IMG_Load(path);
	if (tempSurface == nullptr)
	{
		std::cout << "IMG_Load failed: " << std::string(SDL_GetError()) << std::endl;
		return nullptr;
	}


	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_DestroySurface(tempSurface);

	if (tex == nullptr)
	{
		std::cout << "Create texture failed: " << std::string(SDL_GetError()) << std::endl;
	}

	textures.push_back(tex);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, double angle, SDL_FlipMode flip)
{
	SDL_RenderTextureRotated(
		renderer,
		tex,
		srcRect,
		desRect,
		angle,
		NULL,
		flip
		);
}

void TextureManager::Draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, SDL_FlipMode flip)
{
	SDL_RenderTexture(
		renderer,
		tex,
		srcRect,
		desRect
	);

}

