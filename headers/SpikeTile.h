#pragma once
#include "Tile.h"

class SpikeTile : public Tile {
private:
	int damage;

public:
	SpikeTile(Image* spikeTexture);
	~SpikeTile() override;

	void Update(Engine::Timer* deltaTime) override;
};