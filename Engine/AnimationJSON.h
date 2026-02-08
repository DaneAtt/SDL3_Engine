#pragma once
#include "EngineAPI.h"
#include "Animation.h"
#include "SDL3/SDL.h"
#include <iostream>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>
#include <unordered_map>

using json = nlohmann::json;

class ENGINE_API AnimationJSON
{
public:
	AnimationJSON();
	~AnimationJSON();
	void loadAnimationJSON(const char* path);
	Animation& searchAnimation(std::string name);

	bool alreadyLoaded(const char* path);
	void readJSONFile(const char* path);
	void parseAnimation();
	std::vector<SDL_FRect> parseFrameArray(const json& variationObj, bool fixedFrame, float fixedWidth, float fixedHeight, std::unordered_map<int, SDL_FRect>& hitboxMap, int frameIndex);

private:
	std::set<std::string> loadedFiles;
	nlohmann::json animationJ;
	std::unordered_map<std::string, Animation> animations;
};