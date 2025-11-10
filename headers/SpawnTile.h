#pragma once
#include "Tile.h"
#include "Camera.h"

class SpawnTile : public Tile {
public:
	SpawnTile();

	void Update(float deltaTime) override;
	void Render(SDL_Renderer* renderer, Camera& camera) override;
	bool isActive;
};
