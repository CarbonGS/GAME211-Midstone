#pragma once
#include "Tile.h"
#include "Camera.h"

class SpawnTile : public Tile {
public:
	SpawnTile();

	void Update(Engine::Timer* deltaTime) override;
	void Render(SDL_Renderer* renderer, const Camera& camera) const override;
	bool isActive;
};
