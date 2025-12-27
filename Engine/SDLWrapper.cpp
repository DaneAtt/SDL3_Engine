#include "SDLWrapper.h"
#include "SDL3/SDL.h"


Texture::Texture(SDL_Texture* sdlTex) : sdlTexture(sdlTex) {}

Texture::~Texture()
{
	if (sdlTexture)
	{
		SDL_DestroyTexture(sdlTexture);
	}
}

Window::Window(SDL_Window* sdlWindow) : sdlWindow(sdlWindow) {}

Window::~Window()
{
    if (sdlWindow)
    {
        SDL_DestroyWindow(sdlWindow);
        sdlWindow = nullptr;
    }
}


Renderer::Renderer(SDL_Renderer* sdlRenderer) : sdlRenderer(sdlRenderer) {}

Renderer::~Renderer()
{
    if (sdlRenderer)
    {
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = nullptr;
    }
}

uint64_t Timer::GetTicks()
{
    return SDL_GetTicks();
}

