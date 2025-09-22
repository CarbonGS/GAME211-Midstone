#pragma once
#include "Tile.h"

class PlatformTile : public Tile {
	public:
	PlatformTile();
	~PlatformTile() override;

	void Update(Engine::Timer* deltaTime) override;
};