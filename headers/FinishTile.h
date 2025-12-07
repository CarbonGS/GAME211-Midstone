#pragma once
#include "Tile.h"
#include "Camera.h"

class FinishTile : public Tile {
public:
	FinishTile(Image* finishTexture) {
		type = TILE_FINISH;
		texture = finishTexture;
	}
	void Update(float deltaTime) override {}

	void Render(SDL_Renderer* renderer, Camera& camera) override {
		float zoom = camera.zoom;
		SDL_FRect dst = {
			static_cast<float>(position.x - camera.x) * zoom,
			static_cast<float>(position.y - camera.y) * zoom,
			TILE_SIZE * zoom,
			TILE_SIZE * zoom
		};
		if (texture && texture->GetTexture()) {
			texture->Render(renderer, nullptr, &dst);
		}
	}
};
