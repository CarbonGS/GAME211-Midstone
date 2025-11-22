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

	// Movement
	float velX, velY;
	bool onGround = false;
	bool jumpRequested = false;
	float damageCooldown = 0.0f;
	bool leftHeld = false;
	bool rightHeld = false;
	bool canDoubleJump = false;
	bool canDash = true;
	float dashCooldown = 0.0f;
	float dashDuration = 0.15f; // seconds
	float dashTimer = 0.0f;
	float dashSpeed = 500.0f;
	int dashDir = 0; // -1 for left, 1 for right, 0 for none
	float lastLeftTap = -1.0f;
	float lastRightTap = -1.0f;
	float doubleTapThreshold = 0.25f; // seconds
	float timeSinceStart = 0.0f; // incremented in Update

	// Player stats
	int health;
	float attackCooldown;
};

