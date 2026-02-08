#pragma once
#include <vector>
#include <string>
#include "SDL3/SDL.h"
#include <unordered_map>


struct Animation
{
	int anchorX;
	int anchorY;
	bool Loop;
	bool fixedFrame;
	bool Hitbo;
	bool canFlip;
	std::vector<SDL_FRect> Frames;
	std::unordered_map <std::string, std::vector<SDL_FRect>> FramesVariation;
	std::unordered_map<int, SDL_FRect> HitboxMap;
	std::unordered_map<std::string, std::unordered_map<int, SDL_FRect>> VariationHitboxMap;
	SDL_FRect collisionRect;


	Animation() {}

	Animation(bool loop, bool fixed, bool hitbo,std::unordered_map<int, SDL_FRect> hitboxmap, SDL_FRect collisionrect, bool flip, int achX, int achY, std::vector<SDL_FRect>&& frames)
	{
		Loop = loop;
		fixedFrame = fixed;
		Hitbo = hitbo;
		HitboxMap = hitboxmap;
		collisionRect = collisionrect;
		canFlip = flip;
		anchorX = achX;
		anchorY = achY;
		Frames = std::move(frames);
	}

	Animation(bool loop, bool fixed, bool hitbo, std::unordered_map<std::string, std::unordered_map<int, SDL_FRect>> hitboxmap, SDL_FRect collisionrect, bool flip, int achX, int achY, std::unordered_map <std::string, std::vector<SDL_FRect>> map)
	{
		Loop = loop;
		fixedFrame = fixed;
		Hitbo = hitbo;
		VariationHitboxMap = std::move(hitboxmap);
		collisionRect = collisionrect;
		canFlip = flip;
		anchorX = achX;
		anchorY = achY;
		FramesVariation = std::move(map);
	}

};