#pragma once
#include "EngineAPI.h"
#include "SDL3/sdl.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "WindowRender.h"
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <unordered_map>
#include <string>

class ENGINE_API TextureManager
{
public:
	TextureManager(WindowRender* windowRender);
	~TextureManager();

	SDL_Texture* LoadTexture(const char* path);
	TTF_Font* LoadFont(const char* path, float ptsize);

	void drawFont(TTF_Font* font, const char* text, SDL_Color& fg, const Vector2D& pos);
	void draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, double angle, SDL_FlipMode flip);
	void draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, SDL_FlipMode flip);
	void DrawRectFOutline(const SDL_FRect* rect, const SDL_Color& color);
	void DrawRectF(const SDL_FRect* rect, const SDL_Color& color);
	void DrawRectFCombined(const SDL_FRect* rect, const SDL_Color& color1, const SDL_Color& color2);
	void DrawRectFCombinedCam(const SDL_FRect* rect, const SDL_Color& color);

	void setRenderer(SDL_Renderer* r) { renderer = r; }

private:
	SDL_Renderer* renderer;
	std::vector<SDL_Texture*> textures;
	std::vector<TTF_Font*> fonts;

	struct TextKey 
	{
		std::string text;
		SDL_Color color;

		// Must define equality operator for hash collisions
		// Required for resolving hash collisions in the map's buckets
		bool operator==(const TextKey& other) const {
			return text == other.text && color.r == other.color.r &&
				color.g == other.color.g && color.b == other.color.b;
		}
	};

	// Custom hash function
	struct TextKeyHasher {
		std::size_t operator()(const TextKey& k) const {
			std::size_t h1 = std::hash<std::string>{}(k.text);
			// Pack RGB into a single 32-bit integer via bit-shifting
			std::size_t h2 = (k.color.r << 16) | (k.color.g << 8) | k.color.b;
			// XOR combined with a 1-bit shift to prevent mathematical symmetry/collisions
			return h1 ^ (h2 << 1);
		}
	};

	struct CachedText {
		std::string text;
		SDL_Color color;
		SDL_Texture* texture;
		int width, height;
	};
	std::unordered_map<TextKey, CachedText, TextKeyHasher> textCache;
};