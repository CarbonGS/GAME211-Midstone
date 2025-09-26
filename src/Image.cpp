#include "Image.h"

SDL_Surface* Image::LoadSurface(const char* file) {
	surface = IMG_Load(file);
	if (!surface) {
		SDL_Log("Failed to load image %s: %s", file, SDL_GetError());
		return nullptr;
	}
	return surface;
}

SDL_Texture* Image::LoadTexture(SDL_Renderer* renderer, const char* file) {
	texture = IMG_LoadTexture(renderer, file);
	if (!texture) {
		SDL_Log("Failed to load texture %s: %s", file, SDL_GetError());
		return nullptr;
	}
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return texture;
}

void Image::Render(SDL_Renderer* renderer, const SDL_FRect* srcRect, const SDL_FRect* destRect) const
{
	if (texture) {
		SDL_RenderTexture(renderer, texture, srcRect, destRect);
	}
}
