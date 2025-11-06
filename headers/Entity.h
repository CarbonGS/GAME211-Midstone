#pragma once
#include <SDL3/SDL.h>
#include "Camera.h"
#include "Collision.h"

class Entity
{
public:
	Entity(float x, float y, int width, int height)
		: x(x), y(y), angle(0.0f) {
		bounds.w = width;
		bounds.h = height;
	}
	Entity()
		: x(0), y(0), angle(0.0f) {
		bounds = { 0, 0, 32, 32 };
	}
	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer, const Camera& camera) = 0;

	virtual void OnHit(Entity* other) = 0;

	SDL_Rect GetBounds() const { return SDL_Rect{ static_cast<int>(x), static_cast<int>(y), bounds.w, bounds.h }; }
	void setPosition(float x, float y) { this->x = x; this->y = y; }

	float GetAngle() const { return angle; }
	void SetAngle(float a) { angle = a; }

	OBB GetOBB() const {
		return OBB{
			x + bounds.w / 2.0f,
			y + bounds.h / 2.0f,
			bounds.w / 2.0f,
			bounds.h / 2.0f,
			angle
		};
	}

protected:
	float x, y;
	float angle; // in radians
	SDL_Rect bounds;
	SDL_Texture* texture = nullptr;
};

