#pragma once
#include "EngineAPI.h"
#include "SDL3/SDL.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include "JSONTemplate.h"

using json = nlohmann::json;

class ENGINE_API SkillJSON : public JSONTemplate
{
	SkillJSON() {}
	~SkillJSON() {}
};