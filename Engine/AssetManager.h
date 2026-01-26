#pragma once
#include "TextureManager.h"
#include "SDL3/SDL.h"
#include <map>

class TextureManager;
class Manager;

class ENGINE_API AssetManager
{
public:
	AssetManager(Manager* man, TextureManager* texMgr);
	~AssetManager();

	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

private:
	Manager* manager;
	TextureManager* textureManager;
	std::map<std::string, SDL_Texture*> textures;
};