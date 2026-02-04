#pragma once
#include <vector>
#include <string>
#include "SDL3/SDL.h"
#include <map>


struct Animation
{
	int anchorX;
	int anchorY;
	bool Loop;
	bool fixedFrame;
	bool Hitbo;
	bool canFlip;
	std::vector<SDL_FRect> Frames;
	std::map<int, SDL_FRect> HitboxMap;
	SDL_FRect collisionRect;


	Animation() {}

	Animation(bool loop, bool fixed, bool hitbo,std::map<int, SDL_FRect> hitboxmap, SDL_FRect collisionrect, bool flip, int achX, int achY, const std::vector<SDL_FRect>& frames)
	{
		Loop = loop;
		fixedFrame = fixed;
		Hitbo = hitbo;
		HitboxMap = hitboxmap;
		collisionRect = collisionrect;
		canFlip = flip;
		anchorX = achX;
		anchorY = achY;
		Frames = frames;
	}

};