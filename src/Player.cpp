#include "Player.h"
#include "Camera.h"
#include "Physics.h"
#include "SpikeTile.h"
#include "Collision.h"

constexpr float GRAVITY = 900.0f; // pixels/sec^2
constexpr float JUMP_VELOCITY = -400.0f; // Upward jump velocity
constexpr float DAMAGE_COOLDOWN_TIME = 1.0f; // seconds

Player::Player(Image* texture)
{
	playerTexture = texture;
	velX = 0;
	velY = 0;
	health = 100;
	attackCooldown = 0;
	onGround = false;
	jumpRequested = false;
	damageCooldown = 0.0f;
}

#include <iostream>
void Player::Update(float deltaTime, const std::vector<Tile*>& worldTiles)
{
	// Update damage cooldown
	if (damageCooldown > 0.0f) {
		damageCooldown -= deltaTime;
		if (damageCooldown < 0.0f) damageCooldown = 0.0f;
	}

	// Apply gravity
	Physics::ApplyGravity(velY, GRAVITY, deltaTime);

	// Integrate position
	Physics::Integrate(x, y, velX, velY, deltaTime);

	// Update bounds
	bounds.x = static_cast<int>(x);
	bounds.y = static_cast<int>(y);

	// Check collision with solid tiles
	for (const Tile* tile : worldTiles) {
		if (tile->type == Tile::TILE_PLATFORM) {
			SDL_Rect tileRect = { tile->position.x, tile->position.y, TILE_SIZE, TILE_SIZE };
			if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {
				float oldVelY = velY;
				SDL_Rect playerRect = this->GetBounds();
				Collision::ResolveAABBCollision(playerRect, tileRect, velX, velY);
				x = static_cast<float>(playerRect.x);
				y = static_cast<float>(playerRect.y);
				if (velY <= 0 && bounds.y + bounds.h <= tileRect.y + tileRect.h) {
					onGround = true;
				}
				bounds.x = playerRect.x;
				bounds.y = playerRect.y;
			}
		}
		if (tile->type == Tile::TILE_SPIKE) {
			SDL_Rect tileRect = { tile->position.x, tile->position.y, TILE_SIZE, TILE_SIZE };
			if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {
				if (damageCooldown <= 0.0f) {
					auto spike = dynamic_cast<const SpikeTile*>(tile);
					if (spike)
					{
						health -= spike->getDamage();
						damageCooldown = DAMAGE_COOLDOWN_TIME;
						std::cout << "Player hit spike! Health: " << health << std::endl;
					}
				}
			}
		}
	}

	// Handle jump if requested
	if (jumpRequested && onGround) {
		velY = JUMP_VELOCITY;
		jumpRequested = false;
		onGround = false;
	}
	else {
		jumpRequested = false;
	}

	// Update attack cooldown
	if (attackCooldown > 0)
	{
		attackCooldown -= deltaTime;
		if (attackCooldown < 0)
			attackCooldown = 0;
	}
}

void Player::Render(SDL_Renderer* renderer, const Camera& camera)
{
	float zoom = camera.zoom;
	SDL_FRect dst = {
		static_cast<float>(GetBounds().x - camera.x) * zoom,
		static_cast<float>(GetBounds().y - camera.y) * zoom,
		static_cast<float>(GetBounds().w) * zoom,
		static_cast<float>(GetBounds().h) * zoom
	};
	if (playerTexture && playerTexture->GetTexture()) {
		playerTexture->Render(renderer, nullptr, &dst);
	}
}

void Player::OnHit(Entity* other)
{
	if(dynamic_cast<Player*>(other))
	{
		// Player takes damage (implement damage logic here)
		return;
	}
}

void Player::HandleInput(const SDL_Event& sdlEvent)
{
	// Handle keyboard input for movement
	if (sdlEvent.type == SDL_EVENT_KEY_DOWN)
	{
		switch (sdlEvent.key.key)
		{
		case SDLK_A:
			velX = -200; // Move left
			break;
		case SDLK_D:
			velX = 200; // Move right
			break;
		case SDLK_W:
		case SDLK_SPACE:
			jumpRequested = true;
			break;
		default:
			break;
		}
	}
	else if (sdlEvent.type == SDL_EVENT_KEY_UP)
	{
		switch (sdlEvent.key.key)
		{
		case SDLK_A:
		case SDLK_D:
			velX = 0; // Stop horizontal movement
			break;
		default:
			break;
		}
	}
}