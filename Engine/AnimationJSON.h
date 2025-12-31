#pragma once
#include "EngineAPI.h"
#include "Animation.h"
#include "SDL3/SDL.h"
#include <iostream>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>
#include <map>

using json = nlohmann::json;

class ENGINE_API AnimationJSON
{
public:
	AnimationJSON(int speed);
	~AnimationJSON();
	void loadAnimationJSON(const char* path);
	Animation& searchAnimation(std::string name);
	void setSpeed(int speed) { Speed = speed; }

private:
	int Speed;
	nlohmann::json animationJ;
	std::map<std::string, Animation> animations;
	std::set<std::string> loadedFiles;

};