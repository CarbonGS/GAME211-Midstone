#pragma once
#include "Tile.h"

class SpikeTile : public Tile {
public:
	SpikeTile();
	~SpikeTile() override;

	int damage;
	void Update(Engine::Timer* deltaTime) override;
};