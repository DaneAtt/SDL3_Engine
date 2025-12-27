#include "AssetManager.h"
#include "SDL3/sdl.h"


AssetManager::AssetManager(Manager* man, TextureManager* texMgr) : manager(man), textureManager(texMgr){}

AssetManager::~AssetManager() {}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, textureManager->LoadTexture(path));
}


SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}