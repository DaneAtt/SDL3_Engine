#pragma once
#include "TextureManager.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <unordered_map>

class TextureManager;

class ENGINE_API AssetManager
{
public:
	AssetManager(TextureManager* texMgr);
	~AssetManager();

	void addTexture(std::string id, const char* path);
	SDL_Texture* getTexture(const std::string& id);

	void addFont(std::string id, const char* path, float ptsize);
	TTF_Font* getFont(const std::string& id);

private:
	TextureManager* textureManager;
	std::unordered_map<std::string, SDL_Texture*> textures;
	std::unordered_map<std::string, TTF_Font*> fonts;
};