#pragma once
#include "JSONTemplate.h"
#include "Vector2D.h"
#include <vector>
#include "EngineAPI.h"
#include "Size.h"
#include <unordered_map>

struct ENGINE_API UIAnimInfo
{
	std::string textureID;
	bool fixedFrame;
	bool loop;
	Size size;
	Vector2D atlasPosition;
	std::vector<Vector2D> frames;
};

class ENGINE_API UIAnimationJSON : public JSONTemplate
{
public:
	void loadJSONC() override;
	UIAnimInfo* search(const std::string& name);

	const char* getClassName() const override { return "UIAnimation"; }

private:
	std::string texID;
	std::unordered_map<std::string, UIAnimInfo> UIAnimMap;
};