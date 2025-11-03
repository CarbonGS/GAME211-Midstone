#pragma once
#include "Entity.h"
#include "Image.h"

class Camera; // Forward declaration

class Player : public Entity
{
public:
	Player(Image* playerTexture);
	~Player() {} // Currently no dynamic resources to free

	void Update(float deltaTime) override;
	void Render(SDL_Renderer* renderer, const Camera& camera) override;

	void OnHit(Entity* other) override;
	void HandleInput(const SDL_Event& sdlEvent);

private:
	Image* playerTexture;

	// Movement (Physics)
	float velX, velY;

	// Player stats
	int health;
	float attackCooldown;
};

