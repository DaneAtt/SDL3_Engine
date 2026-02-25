#pragma once
#include "JSONTemplate.h"
#include "Vector2D.h"
#include "Size.h"
#include <unordered_map>

struct ENGINE_API staticObjPos
{
	std::string texName;
	Vector2D pos;
	Size size;
};

class ENGINE_API StaticObjectJSON : public JSONTemplate
{
public:
	void loadJSONC() override;
	staticObjPos* search(const std::string& name);

	const char* getClassName() const override { return "Static Object"; }

private:
	std::string textureID;
	std::unordered_map<std::string, staticObjPos> sprites;
};