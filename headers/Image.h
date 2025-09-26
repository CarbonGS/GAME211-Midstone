#pragma once
#include <SDL3\SDL.h>
#include <SDL3_image\SDL_image.h>

class Image {
private:
	SDL_Surface* surface;
	SDL_Texture* texture;

public:
	Image() : surface(nullptr), texture(nullptr) {}
	~Image() {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
		if (surface) {
			SDL_DestroySurface(surface);
		}
	}

	SDL_Surface* LoadSurface(const char* file);
	SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* file);
	void Render(SDL_Renderer* renderer, const SDL_FRect* srcRect, const SDL_FRect* destRect) const;

	// Getters
	SDL_Surface* GetSurface() const { return surface; }
	SDL_Texture* GetTexture() const { return texture; }
	
};