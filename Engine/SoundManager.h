#pragma once
#include "SDL3/SDL.h"
#include <unordered_map>
#include <string>
#include "EngineAPI.h"

class ENGINE_API SoundManager
{
public:
	SoundManager() {};
	~SoundManager();
	void init();
	void update();
	void loadAudio(const char* path, const std::string& name);
	void playAudio(const std::string& name);

private:
	SDL_AudioDeviceID devID;
	SDL_AudioSpec srcSpec;
	SDL_AudioSpec destSpec;
	std::unordered_map<std::string, std::pair<Uint8*, Uint32>> audioMap;

	struct AudioStream
	{
		SDL_AudioStream* stream;
		bool busy;
	};
	std::vector<AudioStream> streams;

	std::pair<Uint8*, Uint32>* searchAudio(const std::string& name) { return &audioMap[name]; }

};
