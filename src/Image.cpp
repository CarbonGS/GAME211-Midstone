#include "Image.h"

SDL_Surface* Image::LoadSurface(const char* file) {
	surface = IMG_Load(file);
	if (!surface) {
		SDL_Log("Failed to load image %s: %s", file, SDL_GetError());
	}
	return surface;
}

SDL_Texture* Image::LoadTexture(SDL_Renderer* renderer, const char* file) {
	texture = IMG_LoadTexture(renderer, file);
	if (!texture) {
		SDL_Log("Failed to load texture %s: %s", file, SDL_GetError());
	}
	return texture;
}