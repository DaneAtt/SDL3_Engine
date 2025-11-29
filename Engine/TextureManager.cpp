#include "TextureManager.h"
#include "SDL3_image/SDL_image.h"

TextureManager::TextureManager(WindowRender* windowRender) : windowRender(windowRender){}

TextureManager::~TextureManager()
{
	for (auto tex : textures)
	{
		if(tex)
		{
			delete(tex);
		}
	}
	textures.clear();
}

Texture* TextureManager::LoadTexture(const char* path)
{
	SDL_Surface* tempSurface = IMG_Load(path);
	if (tempSurface == nullptr)
	{
		std::cout << "IMG_Load failed: " << std::string(SDL_GetError()) << std::endl;
		return nullptr;
	}


	SDL_Texture* sdlTex = SDL_CreateTextureFromSurface(windowRender->getRenderer()->getSDLRenderer(), tempSurface);
	SDL_DestroySurface(tempSurface);

	if (sdlTex == nullptr)
	{
		std::cout << "Create texture failed: " << std::string(SDL_GetError()) << std::endl;
	}

	Texture* tex = new Texture(sdlTex);
	textures.push_back(tex);

	return tex;
}

void TextureManager::Draw(Texture* tex, const Rect* srcRect, const Rect* desRect, double angle, FlipMode flip)
{
	SDL_FRect sdlSrc, sdlDest;
	SDL_RenderTextureRotated(
		windowRender->getRenderer()->getSDLRenderer(),
		tex->getSDLTexture(),
		ConvertRect(srcRect, sdlSrc),
		ConvertRect(desRect, sdlDest),
		angle,
		NULL,
		ConvertFlip(flip)
		);
}

void TextureManager::Draw(Texture* tex, const Rect* srcRect, const Rect* desRect, FlipMode flip)
{
	SDL_FRect sdlSrc, sdlDest;

	SDL_RenderTexture(
		windowRender->getRenderer()->getSDLRenderer(),
		tex->getSDLTexture(),
		ConvertRect(srcRect, sdlSrc),
		ConvertRect(desRect, sdlDest)
	);

}

SDL_FRect* TextureManager::ConvertRect(const Rect* rect, SDL_FRect& out)
{
	if (!rect) return nullptr;
	out = { rect->x, rect->y, rect->w, rect->h };
	return &out;
}

SDL_FlipMode TextureManager::ConvertFlip(FlipMode flip)
{
	switch (flip) {
	case FlipMode::HORIZONTAL: return SDL_FLIP_HORIZONTAL;
	case FlipMode::VERTICAL: return SDL_FLIP_VERTICAL;
	default: return SDL_FLIP_NONE;
	}
}
