#include "StaticObjectJSON.h"


void StaticObjectJSON::loadJSONC()
{
	for (auto& [atlasName, atlasObjects] : jsonFile.items())
	{
		textureID = atlasName;

		for (auto& [name, objectInfo] : atlasObjects.items())
		{
			staticObjPos info;
			info.texName = textureID;
			info.pos.x = objectInfo["frame"]["x"];
			info.pos.y = objectInfo["frame"]["y"];
			info.size.w = objectInfo["frame"]["w"];
			info.size.h = objectInfo["frame"]["h"];
			
			sprites[name] = info;
		}
	}
}

staticObjPos* StaticObjectJSON::search(const std::string& name)
{
	auto it = sprites.find(name);
	if (it == sprites.end()) 
	{
		std::cout << "Static sprite '" << name << "' not found\n";
		return nullptr;
	}
	return &it->second;
}