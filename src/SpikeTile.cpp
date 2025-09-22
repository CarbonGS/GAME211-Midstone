#include "SpikeTile.h"

SpikeTile::SpikeTile()
{
	type = TILE_SPIKE;
	damage = 10;
}

SpikeTile::~SpikeTile()
{
}

void SpikeTile::Update(Engine::Timer* deltaTime)
{
	// Spike tile specific update logic can be added here
}
