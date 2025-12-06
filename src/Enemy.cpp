#include "Enemy.h"
#include "Player.h"
#include "Camera.h"
#include "Tile.h"
#include "Collision.h"
#include <algorithm>
#include <cmath>
#include <iostream>

constexpr float ENEMY_GRAVITY = 600.0f;
constexpr float ENEMY_SPEED = 180.0f;
constexpr float ENEMY_JUMP_VELOCITY = -220.0f;
constexpr float ENEMY_UNSTUCK_JUMP_VELOCITY = -500.0f; // boosted jump
constexpr float ENEMY_ATTACK_INTERVAL = 1.0f; // seconds
constexpr int ENEMY_ATTACK_DAMAGE = 10;
constexpr float ENEMY_STUCK_THRESHOLD = 0.5f; // seconds
constexpr float ENEMY_STUCK_X_DIST = 4.0f; // px
constexpr float ENEMY_TP_X_DIST = 600.0f; // px, teleport if farther than this
constexpr float ENEMY_TP_Y_DIST = 200.0f; // px, must be within this Y dist to count as close
constexpr float ENEMY_TP_Y_TIMEOUT = 3.0f; // seconds, if not close on Y for this long, teleport
constexpr float ENEMY_INVISIBLE_TIME = 1.0f; // seconds

static float CenterX(const SDL_Rect& r) { return r.x + r.w / 2.0f; }
static float CenterY(const SDL_Rect& r) { return r.y + r.h / 2.0f; }

Enemy::Enemy(Image* texture)
{
	enemyTexture = texture;
	velX = 0;
	velY = 0;
	onGround = false;
	attackCooldown = 0.0f;
	attackInterval = ENEMY_ATTACK_INTERVAL;
	bounds = { 0, 0, 32, 32 };
	x = static_cast<float>(bounds.x);
	y = static_cast<float>(bounds.y);
	stuckRangeTimer = 0.0f;
	minX = maxX = x;
	wasStuck = false;
	isInvisible = false;
	invisibleTimer = 0.0f;
	lastCloseToPlayerTime = 0.0f;
}

Enemy::~Enemy() {}

void Enemy::UpdateAIWithCollision(float deltaTime, float playerX, float playerY, const std::vector<Tile*>& worldTiles)
{
	static float timeSinceStart = 0.0f;
	timeSinceStart += deltaTime;

	// Invisibility/teleport logic
	if (isInvisible) {
		invisibleTimer -= deltaTime;
		if (invisibleTimer <= 0.0f) {
			isInvisible = false;
			std::cout << "Enemy reappeared!" << std::endl;
		}
		return; // Skip all other logic while invisible
	}

	// Teleport if too far on X
	if (std::abs(x - playerX) > ENEMY_TP_X_DIST) {
		std::cout << "Enemy teleported to player X!" << std::endl;
		x = playerX;
		bounds.x = static_cast<int>(x);
		isInvisible = true;
		invisibleTimer = ENEMY_INVISIBLE_TIME;
		attackCooldown = 0.0f;
		return;
	}

	// Track if close on Y
	if (std::abs(y - playerY) < ENEMY_TP_Y_DIST) {
		lastCloseToPlayerTime = timeSinceStart;
	}
	// Teleport if not close on Y for too long
	if (timeSinceStart - lastCloseToPlayerTime > ENEMY_TP_Y_TIMEOUT) {
		std::cout << "Enemy teleported to player Y!" << std::endl;
		y = playerY;
		bounds.y = static_cast<int>(y);
		isInvisible = true;
		invisibleTimer = ENEMY_INVISIBLE_TIME;
		lastCloseToPlayerTime = timeSinceStart;
		attackCooldown = 0.0f;
		return;
	}

	// Stuck detection using X range only
	stuckRangeTimer += deltaTime;
	if (x < minX) minX = x;
	if (x > maxX) maxX = x;

	bool isStuck = false;
	if (stuckRangeTimer > ENEMY_STUCK_THRESHOLD) {
		float rangeX = maxX - minX;
		isStuck = (rangeX < ENEMY_STUCK_X_DIST);
		if (rangeX >= ENEMY_STUCK_X_DIST) {
			stuckRangeTimer = 0.0f;
			minX = maxX = x;
		}
	}

	if (isStuck && !wasStuck) {
		std::cout << "Enemy stuck!" << std::endl;
	}
	if (!isStuck && wasStuck) {
		std::cout << "Enemy unstuck!" << std::endl;
	}
	wasStuck = isStuck;

	// If stuck for more than threshold, force a boosted jump if on ground
	if (isStuck && onGround) {
		velY = ENEMY_UNSTUCK_JUMP_VELOCITY;
		onGround = false;
		stuckRangeTimer = 0.0f;
		minX = maxX = x;
		attackCooldown = 0.0f; // Reset attack cooldown on unstuck
	}

	// Simple direct pursuit
	float centerX = x + bounds.w / 2.0f;
	if (std::abs(playerX - centerX) > 4.0f) {
		if (playerX < centerX)
			velX = -ENEMY_SPEED;
		else if (playerX > centerX)
			velX = ENEMY_SPEED;
	} else {
		velX = 0.0f;
	}

	// Simple jump if player is above and enemy is on ground (use normal jump velocity)
	if (playerY + 8 < y && onGround) {
		velY = ENEMY_JUMP_VELOCITY;
		onGround = false;
	}

	// Gravity
	velY += ENEMY_GRAVITY * deltaTime;

	// HORIZONTAL movement
	x += velX * deltaTime;
	bounds.x = static_cast<int>(x);
	for (const Tile* tile : worldTiles) {
		if (tile->type == Tile::TILE_PLATFORM) {
			const SDL_Rect& tileRect = tile->collisionRect;
			if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {
				if (velX > 0) {
					x = static_cast<float>(tileRect.x - bounds.w);
					velX = 0;
				} else if (velX < 0) {
					x = static_cast<float>(tileRect.x + tileRect.w);
					velX = 0;
				}
				bounds.x = static_cast<int>(x);
			}
		}
	}

	// VERTICAL movement
	y += velY * deltaTime;
	bounds.y = static_cast<int>(y);
	onGround = false;
	for (const Tile* tile : worldTiles) {
		const SDL_Rect& tileRect = tile->collisionRect;
		if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {
			if (tile->type == Tile::TILE_PLATFORM) {
				if (velY > 0) {
					y = static_cast<float>(tileRect.y - bounds.h);
					velY = 0;
					onGround = true;
				}
				else if (velY < 0) {
					y = static_cast<float>(tileRect.y + tileRect.h);
					velY = 0;
				}
				bounds.y = static_cast<int>(y);
			}
		}
	}
}

void Enemy::Render(SDL_Renderer* renderer, const Camera& camera)
{
	if (isInvisible) return;
	float zoom = camera.zoom;
	SDL_FRect dst = {
		static_cast<float>(GetBounds().x - camera.x) * zoom,
		static_cast<float>(GetBounds().y - camera.y) * zoom,
		static_cast<float>(GetBounds().w) * zoom,
		static_cast<float>(GetBounds().h) * zoom
	};
	if (enemyTexture && enemyTexture->GetTexture()) {
		enemyTexture->Render(renderer, nullptr, &dst);
	}
}

void Enemy::OnHit(Entity* other)
{
	// No-op for now
}
