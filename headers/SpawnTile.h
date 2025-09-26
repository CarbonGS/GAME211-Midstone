#pragma once
#include "Tile.h"

class SpawnTile : public Tile {
public:
	SpawnTile();

	void Update(Engine::Timer* deltaTime) override;
	bool isActive;
};
