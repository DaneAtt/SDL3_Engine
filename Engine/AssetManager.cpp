#include "AssetManager.h"
#include "SDL3/sdl.h"


AssetManager::AssetManager(Manager* man, TextureManager* texMgr) : manager(man), textureManager(texMgr){}

AssetManager::~AssetManager() {}

void AssetManager::addTexture(std::string id, const char* path)
{
	textures.emplace(id, textureManager->LoadTexture(path));
}

void AssetManager::addFont(std::string id, const char* path, float ptsize)
{
	fonts.emplace(id, textureManager->LoadFont(path, ptsize));
}

SDL_Texture* AssetManager::getTexture(std::string id)
{
	return textures[id];
}

TTF_Font* AssetManager::getFont(std::string id)
{
	return fonts[id];
}