#include "SpawnTile.h"
#include "Camera.h"

SpawnTile::SpawnTile()
{
	type = TILE_SPAWN;
	isActive = false;
}

void SpawnTile::Update(Engine::Timer* deltaTime)
{
	// Spawn tile specific update logic can be added here
}

void SpawnTile::Render(SDL_Renderer* renderer, const Camera& camera) const
{
	// Optionally render spawn marker, or leave empty
}
