#include "AnimationJSON.h"

void AnimationJSON::loadJSONC()
{
	for (auto& [atlasName, atlasData] : jsonFile.items())  // Each atlas
	{
		std::string textureID = atlasName;
		if (!atlasData.is_object()) {
			std::cout << "ERROR: Atlas '" << atlasName << "' is not an object\n";
			continue;
		}

		for (auto& [name, animData] : atlasData.items())
		{
			std::unordered_map<int, SDL_FRect> hitboxMap;
			std::unordered_map<std::string, std::vector<SDL_FRect>> varMap;
			std::unordered_map<std::string, std::unordered_map<int, SDL_FRect>> variationHitboxMap;
			std::vector<SDL_FRect> framesT;
			float fixedW = 0, fixedH = 0;
			SDL_FRect srcRect, collisionRect;
			Vector2D anchor;
			Vector2D atlasPos;
			try 
			{
				bool loop = animData["loop"];
				anchor.x = float(animData["defaultAnchor"]["x"]);
				anchor.y = float(animData["defaultAnchor"]["y"]);
				atlasPos.x = float(animData["atlasPosition"]["x"]);
				atlasPos.y = float(animData["atlasPosition"]["y"]);
				bool fixedFrame = animData["fixedFrame"];
				collisionRect.x = animData["collisionBox"]["x"];
				collisionRect.y = animData["collisionBox"]["y"];
				collisionRect.w = animData["collisionBox"]["w"];
				collisionRect.h = animData["collisionBox"]["h"];
				bool canFlip = animData["canFlip"];
				bool hitbox = animData["hasHitbox"];
				if (fixedFrame)
				{
					fixedW = animData["fixedFrameSrc"]["w"];
					fixedH = animData["fixedFrameSrc"]["h"];
				}
				int frameIdx = 0;
				for (const auto& variationObj : animData["frames"])
				{
					if (variationObj.is_object() && variationObj.size() == 1)
					{
						auto [varName, frameArray] = *variationObj.items().begin();
						std::unordered_map<int, SDL_FRect> variationHitboxes;

						int varFrameIdx = 0;
						for (const auto& singleFrame : frameArray)
						{
							auto frame = parseFrameArray(singleFrame, fixedFrame, fixedW, fixedH, variationHitboxes, varFrameIdx);
							framesT.push_back(frame[0]);
							varFrameIdx++;
						}
						varMap[varName] = framesT;
						variationHitboxMap[varName] = variationHitboxes;
						framesT.clear();
					}
					else if (variationObj.is_object())
					{
						auto frame = parseFrameArray(variationObj, fixedFrame, fixedW, fixedH, hitboxMap, frameIdx);
						framesT.push_back(frame[0]);
						frameIdx++;
					}
				}
				if (varMap.empty())
				{
					animations[name] = Animation(textureID, loop, fixedFrame, hitbox, std::move(hitboxMap), collisionRect, canFlip, anchor, atlasPos, std::move(framesT));
				}
				else
				{
					animations[name] = Animation(textureID, loop, fixedFrame, hitbox, std::move(variationHitboxMap), collisionRect, canFlip, anchor, atlasPos, std::move(varMap));
				}
			}
			catch (const json::exception& e) {
				std::cout << "Error parsing " << name << ": " << e.what() << "\n";
			}
		}
	}
}

Animation* AnimationJSON::searchAnimation(std::string name)
{
	if (animations.find(name) == animations.end()) 
	{
		std::cout << "Warning: Animation '" << name << "' not found\n";
		return nullptr;
	}
	return &animations[name];
}

std::vector<SDL_FRect> AnimationJSON::parseFrameArray(const json& variationObj, bool fixedFrame, float fixedWidth, float fixedHeight, std::unordered_map<int, SDL_FRect>& hitboxMap, int frameIndex)
{
	SDL_FRect srcRect, hitRect;
	std::vector<SDL_FRect> frames;
	srcRect.x = variationObj["srcX"];
	srcRect.y = variationObj["srcY"];

	if (fixedFrame)
	{
		srcRect.w = fixedWidth;
		srcRect.h = fixedHeight;
	}
	else
	{
		srcRect.w = variationObj["w"];
		srcRect.h = variationObj["h"];
	}

	if (variationObj.contains("hitbox"))
	{
		hitRect.x = variationObj["hitbox"]["x"];
		hitRect.y = variationObj["hitbox"]["y"];
		hitRect.w = variationObj["hitbox"]["w"];
		hitRect.h = variationObj["hitbox"]["h"];

		hitboxMap[frameIndex] = hitRect;
	}
	frames.push_back(srcRect);
	return frames;
}