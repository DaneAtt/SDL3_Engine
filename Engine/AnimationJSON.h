#pragma once
#include "EngineAPI.h"
#include "Animation.h"
#include "SDL3/SDL.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <map>

using json = nlohmann::json;

class ENGINE_API AnimationJSON
{
public:
	AnimationJSON();
	~AnimationJSON();
	void loadJSON(std::string path);
	Animation& searchAnimation(std::string name);
	void setSpeed(int speed) { Speed = speed; }

	json j;
	std::map<std::string, Animation> animations;

	
private:
	int Speed;

};