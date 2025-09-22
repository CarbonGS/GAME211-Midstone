#include "PlatformTile.h"

PlatformTile::PlatformTile()
{
	type = TILE_PLATFORM;
}

PlatformTile::~PlatformTile()
{
}

void PlatformTile::Update(Engine::Timer* deltaTime)
{
	// Platform tile specific update logic can be added here
}
