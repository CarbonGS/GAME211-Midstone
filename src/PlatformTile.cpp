#include "PlatformTile.h"
#include "Camera.h"

PlatformTile::PlatformTile(Image* platformTexture)
{
	type = TILE_PLATFORM;
	texture = platformTexture;
}

PlatformTile::~PlatformTile()
{
}

void PlatformTile::Update(float deltaTime)
{
	// Platform tile specific update logic can be added here
}

void PlatformTile::Render(SDL_Renderer* renderer, Camera& camera)
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
