#pragma once
#include "Tile.h"
#include "Camera.h"

class SpikeTile : public Tile {
private:
	int damage;

public:
	SpikeTile(Image* spikeTexture);

	void Update(Engine::Timer* deltaTime) override;
	void Render(SDL_Renderer* renderer, const Camera& camera) const override;
};