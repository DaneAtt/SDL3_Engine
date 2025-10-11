#include "TextureManager.h"

TextureManager::TextureManager(WindowRender* windowRender) : windowRender(windowRender){}

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

	SDL_Texture* tex = SDL_CreateTextureFromSurface(windowRender->getRenderer(), tempSurface);
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
	SDL_RenderTextureRotated(windowRender->getRenderer(), tex, srcRect, desRect, angle, NULL, flip);
}

void TextureManager::Draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, SDL_FlipMode flip)
{
	SDL_RenderTextureRotated(windowRender->getRenderer(), tex, srcRect, desRect, 0, NULL, flip);

}
