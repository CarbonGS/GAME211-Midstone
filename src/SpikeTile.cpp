#include <vector>
#include "SpikeTile.h"
#include "Camera.h"
#include "Collision.h"

SpikeTile::SpikeTile(Image* spikeTexture)
{
	type = TILE_SPIKE;
	texture = spikeTexture;
	damage = 10;
	posY = 0.0f;
}

void SpikeTile::Update(float deltaTime)
{
	if (atRest) return;
	float gravity = 900.0f;
	velY += gravity * deltaTime;
	posY += velY * deltaTime;
	position.y = static_cast<int>(posY);
	collisionRect.y = position.y + 22;
}

void SpikeTile::PhysicsUpdate(const std::vector<Tile*>& worldTiles, float deltaTime, float gravity)
{
	if (atRest) return;
	for (const Tile* tile : worldTiles) {
		if (tile == this) continue;
		if (tile->type == Tile::TILE_PLATFORM) {
			if (Collision::CheckAABBCollision(this->collisionRect, tile->collisionRect)) {
				posY = static_cast<float>(tile->collisionRect.y - TILE_SIZE);
				position.y = static_cast<int>(posY);
				collisionRect.y = position.y + 22;
				velY = 0;
				atRest = true;
				break;
			}
		}
	}
}

void SpikeTile::Render(SDL_Renderer* renderer, Camera& camera)
{
	float zoom = camera.zoom;
	SDL_FRect dst = {
		static_cast<float>(position.x - camera.x) * zoom,
		static_cast<float>(position.y - camera.y) * zoom,
		TILE_SIZE * zoom,
		TILE_SIZE * zoom
	};
	if (texture && texture->GetTexture()) {
		texture->Render(renderer, nullptr, &dst);
	}
}
