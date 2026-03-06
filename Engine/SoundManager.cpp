#include "SoundManager.h"
#include <iostream>

SoundManager::~SoundManager()
{
	for (auto& [name, sound] : audioMap)
	{
		SDL_free(sound.first);
	}
}

void SoundManager::init()
{
	if(SDL_Init(SDL_INIT_AUDIO))
	{
		devID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		if (!SDL_GetAudioDeviceFormat(devID, &destSpec, NULL))
		{
			std::cout << "SDL_GetAudioDeviceFormat failed" << '\n';
			return;
		}
		srcSpec.freq = 48000;
		srcSpec.channels = 2;
		srcSpec.format = SDL_AUDIO_F32;

		for (int i = 0; i < 8; i++)
		{
			SDL_AudioStream* stream = SDL_CreateAudioStream(&srcSpec, &destSpec);
			AudioStream strm;
			strm.stream = stream;
			strm.busy = false;
			streams.push_back(std::move(strm));
			SDL_BindAudioStream(devID, stream);
		}
	}
}

void SoundManager::update()
{
	for (auto& stream : streams)
	{
		if (stream.busy == true && SDL_GetAudioStreamQueued(stream.stream) <= 28)
		{
			SDL_ClearAudioStream(stream.stream);
			stream.busy = false;
		}
	}
}

void SoundManager::loadAudio(const char* path, const std::string& name)
{
	Uint8* buff = nullptr;
	Uint32 len = 0;
	if (path)
	{
		SDL_LoadWAV(path, &srcSpec, &buff, &len);
		audioMap[name] = std::make_pair(buff, len);
	}
	else
	{
		std::cout << path << " not found" << '\n';
	}
}

void SoundManager::playAudio(const std::string& name)
{
	std::pair<Uint8*, Uint32>* audioInfo = searchAudio(name);
	if (!audioInfo) return;

	for (auto& stream : streams)
	{
		std::cout << SDL_GetAudioStreamQueued(stream.stream) << '\n';
		if (!stream.busy)
		{
			SDL_SetAudioStreamFormat(stream.stream, &srcSpec, &destSpec);
			if (SDL_PutAudioStreamData(stream.stream, audioInfo->first, audioInfo->second))
			{
				stream.busy = true;
			}
			std::cout << "Sound Played" << '\n';
			break;
		}
	}
}