#include "Tile.h"
#include "Camera.h"

Tile::Tile()
{
	type = TILE_EMPTY; // Default to empty tile
	position = { 0, 0 };
	texture = nullptr; // No texture by default
}

Tile::~Tile()
{
	// Level Designer handles texture cleanup
}

void Tile::Update(Engine::Timer* deltaTime)
{
	// Current tile does not have any specific update logic
}

void Tile::Render(SDL_Renderer* renderer, const Camera& camera) const
{
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
