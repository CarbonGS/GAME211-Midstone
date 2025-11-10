#pragma once
#include <vector>
#include "Tile.h"
#include "Camera.h"

class SpikeTile : public Tile {
private:
	int damage;
	float velY = 0.0f;
	float mass = 10000.0f; // Very heavy
	bool atRest = false;
	float posY = 0.0f; // Use float for smooth vertical position
public:
	SpikeTile(Image* spikeTexture);

	void Update(float deltaTime) override;
	void Render(SDL_Renderer* renderer, Camera& camera) override;
	int getDamage() const { return damage; }
	void PhysicsUpdate(const std::vector<Tile*>& worldTiles, float deltaTime, float gravity);
	void SetInitialY(int y) { posY = static_cast<float>(y); position.y = y; }
};