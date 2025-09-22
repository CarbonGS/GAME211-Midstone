#include "SpawnTile.h"

SpawnTile::SpawnTile()
{
	type = TILE_SPAWN;
	isActive = false;
}

SpawnTile::~SpawnTile()
{
}

void SpawnTile::Update(Engine::Timer* deltaTime)
{
	// Spawn tile specific update logic can be added here
}
