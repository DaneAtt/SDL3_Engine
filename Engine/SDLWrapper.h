#pragma once
#include "EngineAPI.h"
#include <cstdint>

struct SDL_Texture;

class ENGINE_API Texture
{
public:
	Texture(SDL_Texture* sdlTex);
	~Texture();

	SDL_Texture* getSDLTexture() const { return sdlTexture; }

private:
	SDL_Texture* sdlTexture;

};

struct SDL_Window;
struct SDL_Renderer;

class ENGINE_API Window
{
public:
	Window(SDL_Window* sdlWindow);
	~Window();

	SDL_Window* getSDLWindow() const { return sdlWindow; }

private:
	SDL_Window* sdlWindow;

};

class ENGINE_API Renderer
{
public:
	Renderer(SDL_Renderer* sdlRenderer);
	~Renderer();

	SDL_Renderer* getSDLRenderer() const { return sdlRenderer; }

private:
	SDL_Renderer* sdlRenderer;
};

class ENGINE_API Timer
{
public:
	static uint64_t GetTicks();

};


