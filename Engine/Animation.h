#pragma once
#include <vector>
#include <string>
#include "SDL3/SDL.h"


struct Animation
{
	int anchorX;
	int anchorY;
	bool Loop;
	double Speed;
	double DefaultDuration;
	std::string Name;
	std::vector<SDL_FRect> Frames;
	Uint32 lastFrameTime;


	Animation() {}

	Animation(std::string name, bool loop, int speed, double defaultduration, int achX, int achY, const std::vector<SDL_FRect>& frames)
	{
		lastFrameTime = SDL_GetTicks();
		Name = name;
		Loop = loop;
		Speed = speed;
		DefaultDuration = defaultduration;
		anchorX = achX;
		anchorY = achY;
		Frames = frames;
	}

};