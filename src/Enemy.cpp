#include "Enemy.h"
#include "Player.h"
#include "Camera.h"

void Enemy::Update(float deltaTime)
{
	// Implement enemy movement logic here
}

void Enemy::Render(SDL_Renderer* renderer, const Camera& camera)
{
	float zoom = camera.zoom;
	SDL_FRect dst = {
		static_cast<float>(GetBounds().x - camera.x) * zoom,
		static_cast<float>(GetBounds().y - camera.y) * zoom,
		static_cast<float>(GetBounds().w) * zoom,
		static_cast<float>(GetBounds().h) * zoom
	};
	if (texture) {
		SDL_RenderTexture(renderer, texture, nullptr, &dst);
	}
}

void Enemy::OnHit(Entity* other)
{
	if (dynamic_cast<Player*>(other))
	{
		// Enemy reacts to player hit (implement logic here)
		return;
	}
}
