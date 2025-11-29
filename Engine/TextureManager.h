#pragma once
#include "EngineAPI.h"
#include "EngineTypes.h"
#include "SDLWrapper.h"
#include "WindowRender.h"
#include <iostream>
#include <vector>

struct SDL_FRect;
enum SDL_FlipMode;

class ENGINE_API TextureManager
{
public:
	TextureManager(WindowRender* windowRender);
	~TextureManager();

	Texture* LoadTexture(const char* path);
	void Draw(Texture* tex, const Rect* srcRect, const Rect* desRect, double angle, FlipMode flip);
	void Draw(Texture* tex, const Rect* srcRect, const Rect* desRect, FlipMode flip);

private:
	WindowRender* windowRender;
	std::vector<Texture*> textures;

	SDL_FRect* ConvertRect(const Rect* rect, SDL_FRect& out);
	SDL_FlipMode ConvertFlip(FlipMode flip);
};