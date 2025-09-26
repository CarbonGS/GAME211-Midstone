#include "SpikeTile.h"

SpikeTile::SpikeTile(Image* spikeTexture)
{
	type = TILE_SPIKE;
	texture = spikeTexture;
	damage = 10; // Default damage value, can be adjusted as needed
}

SpikeTile::~SpikeTile()
{
}

void SpikeTile::Update(Engine::Timer* deltaTime)
{
	// Spike tile specific update logic can be added here
}
