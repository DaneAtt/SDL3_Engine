#include "AnimationJSON.h"


AnimationJSON::AnimationJSON() {}

AnimationJSON::~AnimationJSON() {}

void AnimationJSON::loadJSON(std::string path)
{
	if (!j.empty()) return;
	std::ifstream file(path);
	if (!file.is_open()) { std::cout << "JSON File not found" << "\n"; return; }
	file >> j;
	file.close();

	for (auto& [name, animData] : j.items()) {
		std::vector<SDL_FRect> frames;
		SDL_FRect srcRect, destRect;
        try {
            bool loop = animData["loop"];
            double defaultDuration = animData["defaultDuration"];
            int anchorX = animData["defaultAnchor"]["x"];
            int anchorY = animData["defaultAnchor"]["y"];
            srcRect.w = animData["frameSize"]["w"];
            srcRect.h = animData["frameSize"]["h"];
		
			for (const auto& frame : animData["frames"]) {
				srcRect.x = frame["srcX"];
				srcRect.y = frame["srcY"];
				frames.push_back(srcRect);
			}

			animations[name] = Animation(name, loop, 1, defaultDuration, anchorX, anchorY, frames);

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
