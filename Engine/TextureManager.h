#pragma once
#include "EngineAPI.h"
#include "SDL3/sdl.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "WindowRender.h"
#include <iostream>
#include <vector>
#include "Vector2D.h"

class ENGINE_API TextureManager
{
public:
	TextureManager(WindowRender* windowRender);
	~TextureManager();

	SDL_Texture* LoadTexture(const char* path);
	TTF_Font* LoadFont(const char* path, float ptsize);

	void drawFont(TTF_Font* font, const char* text, SDL_Color fg, Vector2D pos);
	void draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, double angle, SDL_FlipMode flip);
	void draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, SDL_FlipMode flip);
	void DrawRectF(const SDL_FRect* rect, SDL_Color color);
	void DrawDebugRectF(const SDL_FRect* rect, SDL_Color color);

	void setRenderer(SDL_Renderer* r) { renderer = r; }

private:
	SDL_Renderer* renderer;
	std::vector<SDL_Texture*> textures;
	std::vector<TTF_Font*> fonts;
};