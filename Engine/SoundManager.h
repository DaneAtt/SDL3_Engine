#pragma once
#include "SDL3/SDL.h"
#include <unordered_map>
#include <string>
#include "EngineAPI.h"
#include <functional>
#include <iostream>

class ENGINE_API SoundManager
{
public:
	SoundManager() {};
	~SoundManager();
	bool init();
	void update();
	void loadAudio(const char* path, const std::string& name);
	void playAudio(const std::string& name);

private:
	SDL_AudioDeviceID devID;
	SDL_AudioSpec srcSpec;
	SDL_AudioSpec destSpec;
	std::unordered_map<size_t, std::pair<Uint8*, Uint32>> audioMap;
	const int MAX_STREAMS = 8;

	struct AudioStream
	{
		SDL_AudioStream* stream;
		bool busy;
	};
	std::vector<AudioStream> streams;

	std::pair<Uint8*, Uint32>* searchAudio(const std::string& name) 
	{ 
		size_t id = std::hash<std::string>{} (name);
		auto it = audioMap.find(id);
		if (it != audioMap.end())
		{
			return &it->second;
		}
		std::cout << "Audio not found" << '\n';
		return nullptr;
	}

};
