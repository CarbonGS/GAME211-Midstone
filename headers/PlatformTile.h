#pragma once
#include "Tile.h"
#include "Camera.h"

class PlatformTile : public Tile {
private:

public:
	PlatformTile(Image* platformTexture);
	~PlatformTile() override;

	void Update(float deltaTime) override;
	void Render(SDL_Renderer* renderer, Camera& camera) override;
};