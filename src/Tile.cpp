#include "Tile.h"

Tile::Tile()
{
	type = TILE_EMPTY; // Default to empty tile
	position = { 0, 0 };
	texture = nullptr; // No texture by default
}

Tile::~Tile()
{
	if (texture) {
		delete texture;
		texture = nullptr;
	}
}

void Tile::Update(Engine::Timer* deltaTime)
{
	// Current tile does not have any specific update logic
}

void Tile::Render(SDL_Renderer* renderer) const
{
	if (texture && texture->GetTexture()) {
		const SDL_FRect destRect = { static_cast<float>(position.x), static_cast<float>(position.y), TILE_SIZE, TILE_SIZE};
		texture->Render(renderer, nullptr, &destRect);
	}
}
