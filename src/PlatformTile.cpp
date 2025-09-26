#include "PlatformTile.h"


PlatformTile::PlatformTile(Image* platformTexture)
{
	type = TILE_PLATFORM;
	texture = platformTexture;
}

PlatformTile::~PlatformTile()
{
}

void PlatformTile::Update(Engine::Timer* deltaTime)
{
	// Platform tile specific update logic can be added here
}
