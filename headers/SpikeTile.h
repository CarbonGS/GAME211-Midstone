#pragma once
#include "Tile.h"

class SpikeTile : public Tile {
private:
	int damage;

public:
	SpikeTile(Image* spikeTexture);

	void Update(Engine::Timer* deltaTime) override;
};