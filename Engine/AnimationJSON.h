#pragma once
#include "EngineAPI.h"
#include "Animation.h"
#include "SDL3/SDL.h"
#include <iostream>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include "JSONTemplate.h"

using json = nlohmann::json;

class ENGINE_API AnimationJSON : public JSONTemplate
{
public:
	AnimationJSON();
	~AnimationJSON();
	void loadJSONC() override;
	Animation& searchAnimation(std::string name);
	std::vector<SDL_FRect> parseFrameArray(const json& variationObj, bool fixedFrame, float fixedWidth, float fixedHeight, std::unordered_map<int, SDL_FRect>& hitboxMap, int frameIndex);

private:
	std::unordered_map<std::string, Animation> animations;
};