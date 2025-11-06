#pragma once
#include "Entity.h"
#include "Image.h"
#include <vector>

class Tile;

class Player : public Entity
{
public:
	Player(Image* playerTexture);
	~Player() {} // Currently no dynamic resources to free

	// Override base class Update for compatibility
	void Update(float deltaTime) override { /* Overloaded version is used not this one */ }
	void Update(float deltaTime, const std::vector<Tile*>& worldTiles);
	void Render(SDL_Renderer* renderer, const Camera& camera) override;

	void OnHit(Entity* other) override;
	void HandleInput(const SDL_Event& sdlEvent);

private:
	Image* playerTexture;

	// Movement (Physics)
	float velX, velY;
	bool onGround = false;
	bool jumpRequested = false;
	float damageCooldown = 0.0f;

	// Player stats
	int health;
	float attackCooldown;
};

