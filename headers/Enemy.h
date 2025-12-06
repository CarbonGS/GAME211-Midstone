#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Image.h"
#include <vector>
#include <SDL3/SDL.h>

class Tile;
class Player;

class Enemy : public Entity
{
public:
	Enemy(Image* enemyTexture);
	~Enemy();

	void UpdateAIWithCollision(float deltaTime, float playerX, float playerY, const std::vector<Tile*>& worldTiles);
	void Render(SDL_Renderer* renderer, const Camera& camera) override;
	void OnHit(Entity* other) override;
	void Update(float deltaTime) override {} // No-op for abstract base

	void SetPositionSync(float x, float y) { this->x = x; this->y = y; bounds.x = (int)x; bounds.y = (int)y; }

	SDL_Rect GetBounds() const { return SDL_Rect{ static_cast<int>(x), static_cast<int>(y), bounds.w, bounds.h }; }

	float attackCooldown; // seconds
	float attackInterval; // seconds

private:
	Image* enemyTexture;
	float velX, velY;
	bool onGround;

	// Robust stuck detection
	float stuckRangeTimer = 0.0f;
	float minX = 0.0f, maxX = 0.0f, minY = 0.0f, maxY = 0.0f;
	bool wasStuck = false;

	// Teleport/invisibility logic
	bool isInvisible = false;
	float invisibleTimer = 0.0f;
	float lastCloseToPlayerTime = 0.0f;
};

