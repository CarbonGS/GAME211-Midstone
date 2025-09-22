#pragma once
#include "Tile.h"

Tile::Tile()
{
	type = TILE_EMPTY; // Default to empty tile
	position = { 0, 0 };
}

Tile::~Tile()
{
}

void Tile::Update(Engine::Timer* deltaTime)
{
}
