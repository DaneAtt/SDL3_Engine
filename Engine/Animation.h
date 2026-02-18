#pragma once
#include <vector>
#include <string>
#include "SDL3/SDL.h"
#include <unordered_map>
#include "Vector2D.h"
#include "Size.h"


struct Animation
{
	std::string textureID;
	Vector2D anchor;
	Vector2D atlasOffset;
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

	Animation(std::string tex, const bool& loop, const bool& fixed, const bool& hitbo,std::unordered_map<int, SDL_FRect> hitboxmap, const SDL_FRect& collisionrect, const bool& flip, const Vector2D& ach, const Vector2D& atlasPos,std::vector<SDL_FRect>&& frames)
	{
		textureID = tex;
		Loop = loop;
		fixedFrame = fixed;
		Hitbo = hitbo;
		HitboxMap = hitboxmap;
		collisionRect = collisionrect;
		canFlip = flip;
		anchor.x = ach.x;
		anchor.y = ach.y;
		atlasOffset.x = atlasPos.x;
		atlasOffset.y = atlasPos.y;
		Frames = std::move(frames);
	}

	Animation(std::string tex, bool& loop, const bool& fixed, const bool& hitbo, std::unordered_map<std::string, std::unordered_map<int, SDL_FRect>> hitboxmap, SDL_FRect collisionrect, const bool& flip, const Vector2D& ach, const Vector2D& atlasPos, std::unordered_map<std::string, std::vector<SDL_FRect>> map)
	{
		textureID = tex;
		Loop = loop;
		fixedFrame = fixed;
		Hitbo = hitbo;
		VariationHitboxMap = std::move(hitboxmap);
		collisionRect = collisionrect;
		canFlip = flip;
		anchor.x = ach.x;
		anchor.y = ach.y;
		atlasOffset.x = atlasPos.x;
		atlasOffset.y = atlasPos.y;
		FramesVariation = std::move(map);
	}

};