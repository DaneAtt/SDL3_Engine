#pragma once
#include "EngineAPI.h"

struct ENGINE_API Rect
{
	float x, y, w, h;
};

enum class ENGINE_API FlipMode
{
	NONE,
	HORIZONTAL,
	VERTICAL
};

enum class ENGINE_API Flags
{
	FULLSCREEN,
	BORDERLESS,
	RESIZABLE
};

constexpr int CENTERED = -1;

enum class ENGINE_API Key
{
	UP,
	DOWN
};
