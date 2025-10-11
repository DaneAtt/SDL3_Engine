#pragma once
#include "EngineAPI.h"
#include "SDL3_image/SDL_image.h"
#include "WindowRender.h"
#include <iostream>
#include <vector>

class ENGINE_API TextureManager
{
public:
	TextureManager(WindowRender* windowRender);
	~TextureManager();

	SDL_Texture* LoadTexture(const char* path);
	void Draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, double angle, SDL_FlipMode flip);
	void Draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, SDL_FlipMode flip);

private:
	WindowRender* windowRender;
	std::vector<SDL_Texture*>textures;
};