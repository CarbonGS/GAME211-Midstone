#include "SpikeTile.h"
#include "Camera.h"

SpikeTile::SpikeTile(Image* spikeTexture)
{
	type = TILE_SPIKE;
	texture = spikeTexture;
	damage = 10;
}

void SpikeTile::Update(Engine::Timer* deltaTime)
{
	// Spike tile specific update logic can be added here
}

void SpikeTile::Render(SDL_Renderer* renderer, const Camera& camera) const
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
