#include "Player.h"
#include "Camera.h"

Player::Player(Image* texture)
{
	playerTexture = texture;
	velX = 0;
	velY = 0;
	health = 100;
	attackCooldown = 0;
}

void Player::Update(float deltaTime)
{
	// Update player position based on velocity
	x += velX * deltaTime;
	y += velY * deltaTime;
	// Update bounds
	bounds.x = static_cast<int>(x);
	bounds.y = static_cast<int>(y);
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
			velY = -200; // Move up
			break;
		case SDLK_S:
			velY = 200; // Move down
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
		case SDLK_W:
		case SDLK_S:
			velY = 0; // Stop vertical movement
			break;
		default:
			break;
		}
	}
}
