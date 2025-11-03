#pragma once
#include <SDL3/SDL.h>
#include "Camera.h"

class Entity
{
public:
	Entity(float x, float y, int width, int height)
		: x(x), y(y) {
		bounds.w = width;
		bounds.h = height;
	}
	Entity()
		: x(0), y(0) {
		bounds = { 0, 0, 32, 32 };
	}
	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer, const Camera& camera) = 0;

	virtual void OnHit(Entity* other) = 0;

	SDL_Rect GetBounds() const { return SDL_Rect{ static_cast<int>(x), static_cast<int>(y), bounds.w, bounds.h }; }
	void setPosition(float x, float y) { this->x = x; this->y = y; }

protected:
	float x, y;
	SDL_Rect bounds;
	SDL_Texture* texture = nullptr;
};

