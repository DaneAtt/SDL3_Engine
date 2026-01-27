#pragma once
#include "TextureManager.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <map>

class TextureManager;
class Manager;

class ENGINE_API AssetManager
{
public:
	AssetManager(Manager* man, TextureManager* texMgr);
	~AssetManager();

	void addTexture(std::string id, const char* path);
	SDL_Texture* getTexture(std::string id);

	void addFont(std::string id, const char* path, float ptsize);
	TTF_Font* getFont(std::string id);

private:
	Manager* manager;
	TextureManager* textureManager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};