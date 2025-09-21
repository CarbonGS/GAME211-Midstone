#pragma once
#include <SDL3\SDL.h>
#include <SDL3_image\SDL_image.h>

class Image {
private:
	SDL_Surface* surface;
	SDL_Texture* texture;
public:
	SDL_Surface* LoadSurface(const char* file);
	SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* file);

	// Getters
	SDL_Surface* GetSurface() const { return surface; }
	SDL_Texture* GetTexture() const { return texture; }
	
};