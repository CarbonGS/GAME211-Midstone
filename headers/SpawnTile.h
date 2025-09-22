#pragma once
#include "Tile.h"

class SpawnTile : public Tile {
public:
	SpawnTile();
	~SpawnTile() override;

	void Update(Engine::Timer* deltaTime) override;
	bool isActive;
};
