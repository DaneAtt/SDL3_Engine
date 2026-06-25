#include "AssetManager.h"
#include "SDL3/sdl.h"


AssetManager::AssetManager(TextureManager* texMgr) : textureManager(texMgr){}

AssetManager::~AssetManager() {}

void AssetManager::addTexture(std::string id, const char* path)
{
	textures.emplace(std::move(id), textureManager->LoadTexture(path));
}

void AssetManager::addFont(std::string id, const char* path, float ptsize)
{
	fonts.emplace(std::move(id), textureManager->LoadFont(path, ptsize));
}

SDL_Texture* AssetManager::getTexture(const std::string& id)
{
    auto it = textures.find(id);
    if (it == textures.end())
    {
        std::cout << "Texture not found: " << id << '\n';
        return nullptr;
    }
    return it->second;
}

TTF_Font* AssetManager::getFont(const std::string& id)
{
	auto it = fonts.find(id);
	if (it == fonts.end())
	{
		std::cout << "Font not found: " << id << '\n';
		return nullptr;
	}
	return it->second;
}