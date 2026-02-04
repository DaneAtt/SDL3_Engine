#include "AnimationJSON.h"


AnimationJSON::AnimationJSON(int speed) : Speed(speed){}

AnimationJSON::~AnimationJSON() {}

void AnimationJSON::loadAnimationJSON(const char* path)
{
	std::set<std::string> loadedFiles;  // Track loaded file paths

	if (loadedFiles.find(path) != loadedFiles.end()) {
		return;  //Only blocks THIS specific file if already loaded
	}
	loadedFiles.insert(path);

	std::ifstream file(path);
	if (!file.is_open()) { std::cout << "Animation JSON File not found" << "\n"; return; }
	file >> animationJ;
	file.close();

	for (auto& [name, animData] : animationJ.items()) {
		std::vector<SDL_FRect> frames;
		std::map<int, SDL_FRect> hitboxMap;
		SDL_FRect srcRect, hitRect, collisionRect;
        try {
            bool loop = animData["loop"];
            int anchorX = animData["defaultAnchor"]["x"];
            int anchorY = animData["defaultAnchor"]["y"];
			bool fixedFrame = animData["fixedFrame"];
			collisionRect.x = animData["collisionBox"]["x"];
			collisionRect.y = animData["collisionBox"]["y"];
			collisionRect.w = animData["collisionBox"]["w"];
			collisionRect.h = animData["collisionBox"]["h"];
			bool canFlip = animData["canFlip"];
			bool hitbox = animData["hasHitbox"];
			if (fixedFrame)
			{
				srcRect.w = animData["fixedFrameSrc"]["w"];
				srcRect.h = animData["fixedFrameSrc"]["h"];
			}

			for (const auto& frame : animData["frames"])
			{
				srcRect.x = frame["srcX"];
				srcRect.y = frame["srcY"];

				if (!fixedFrame)
				{
					srcRect.w = frame["w"];
					srcRect.h = frame["h"];
				}

				if (frame.contains("hitbox"))
				{
					hitRect.x = frame["hitbox"]["x"];
					hitRect.y = frame["hitbox"]["y"];
					hitRect.w = frame["hitbox"]["w"];
					hitRect.h = frame["hitbox"]["h"];

					hitboxMap[frames.size()] = hitRect;
				}

				frames.push_back(srcRect);
				
			}
			animations[name] = Animation(loop, fixedFrame, hitbox, hitboxMap, collisionRect, canFlip, anchorX, anchorY, frames);
		}
		catch (const json::exception& e) {
			std::cout << "Error parsing " << name << ": " << e.what() << "\n";
		}

	}
}

Animation& AnimationJSON::searchAnimation(std::string name)
{
	if (animations.find(name) == animations.end()) {
		std::cout << "Warning: Animation '" << name << "' not found\n";
		static Animation empty;  // Return empty animation as fallback
		return empty;
	}
	return animations[name];
}
