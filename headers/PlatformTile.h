#pragma once
#include "Tile.h"

class PlatformTile : public Tile {
private:

public:
	PlatformTile(Image* platformTexture);
	~PlatformTile() override;

	void Update(Engine::Timer* deltaTime) override;
};