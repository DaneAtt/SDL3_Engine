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

	for (auto& pair : textCache) 
	{
		SDL_DestroyTexture(pair.second.texture);
	}
	textCache.clear();
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

	SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

	textures.push_back(tex);

	return tex;
}

TTF_Font* TextureManager::LoadFont(const char* path, float ptsize)
{
	TTF_Font* font = TTF_OpenFont(path, ptsize);
	if (font == nullptr)
	{
		std::cout << "font load failed: " << std::string(SDL_GetError()) << std::endl;
		return nullptr;
	}

	fonts.push_back(font);
	return font;
}

void TextureManager::drawFont(TTF_Font* font, const char* text, SDL_Color& fg, const Vector2D& pos)
{
	std::string cacheKey = makeTextCacheKey(text, fg);

	// Check cache first
	auto it = textCache.find(cacheKey);
	if (it != textCache.end()) {
		// Use cached texture
		SDL_FRect destRect = { pos.x, pos.y, (float)it->second.width, (float)it->second.height };
		SDL_RenderTexture(renderer, it->second.texture, NULL, &destRect);
		return;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, fg);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);

	if (tex != nullptr)
	{
		// Cache it
		CachedText cached;
		cached.text = text;
		cached.color = fg;
		cached.texture = tex;
		cached.width = surface->w;
		cached.height = surface->h;
		textCache[cacheKey] = cached;

		SDL_FRect destRect = { pos.x, pos.y, (float)surface->w, (float)surface->h };
		SDL_RenderTexture(renderer, tex, NULL, &destRect);
	}

	SDL_DestroySurface(surface);
}

void TextureManager::draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, double angle, SDL_FlipMode flip)
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

void TextureManager::draw(SDL_Texture* tex, const SDL_FRect* srcRect, const SDL_FRect* desRect, SDL_FlipMode flip)
{
	SDL_RenderTextureRotated(
		renderer,
		tex,
		srcRect,
		desRect,
		0.0,
		NULL,
		flip
	);

}

void TextureManager::DrawRectFOutline(const SDL_FRect* rect, const SDL_Color& color)
{
	if (!rect) return;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderRect(renderer, rect);
}

void TextureManager::DrawRectF(const SDL_FRect* rect, const SDL_Color& color)
{
	if (!rect) return;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, rect);
}

void TextureManager::DrawRectFCombined(const SDL_FRect* rect, const SDL_Color& color1 , const SDL_Color& color2)
{
	DrawRectFOutline(rect, color1);
	DrawRectF(rect, color2);
}

void TextureManager::DrawRectFCombinedCam(const SDL_FRect* rect, const SDL_Color& color)
{
	SDL_FRect drawRect = *rect;
	drawRect.x -= Engine::getCamera()->x;
	drawRect.y -= Engine::getCamera()->y;
	DrawRectFOutline(&drawRect, color);
	DrawRectF(&drawRect, color);
}

