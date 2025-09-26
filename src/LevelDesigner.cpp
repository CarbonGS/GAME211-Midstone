#include "LevelDesigner.h"

// Tiles
#include "SpikeTile.h"
#include "SpawnTile.h"
#include "PlatformTile.h"

LevelDesigner::LevelDesigner() : levelImage(nullptr) {}

LevelDesigner::~LevelDesigner()
{
	levelImage = nullptr;
}

void LevelDesigner::LevelDesignerLoad(Image* levelfile) // Ensure the image is loaded in scene via Image class first
{
	levelImage = levelfile;
}

void LevelDesigner::GenerateLevel()
{
	SDL_Surface* surface = levelImage->GetSurface();
	const SDL_PixelFormatDetails* formatDetails = SDL_GetPixelFormatDetails(surface->format);
	InitColorMap(formatDetails);

	if (!surface) {
		SDL_Log("Level image surface is null.");
		return;
	}

	if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

	Uint32* pixels = (Uint32*)surface->pixels; // Gets pixels from the surface
	int width = surface->w;
	int height = surface->h;

	// Iterate over each pixel in the image
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Uint32 pixel = pixels[y * width + x];
			SDL_Color color;
			SDL_GetRGB(pixel, formatDetails, nullptr, &color.r, &color.g, &color.b);
			int tileType = getTileTypeFromColor(color, *surface); // Determine tile type based on color
			placeTile(x, y, tileType); // Place the tile in the game world
		}
	}
	if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}

void LevelDesigner::InitColorMap(const SDL_PixelFormatDetails* format)
{
	// Add more colors and their corresponding tile types as needed
	colorToTileMap[SDL_MapRGB(format, nullptr,255, 255, 255)]	= Tile::TILE_EMPTY;     // White - Air
	colorToTileMap[SDL_MapRGB(format, nullptr,0, 0, 0)]			= Tile::TILE_PLATFORM;	// Black - Platform
	colorToTileMap[SDL_MapRGB(format, nullptr,0, 255, 0)]		= Tile::TILE_SPAWN;		// Green - Spawn Point
	colorToTileMap[SDL_MapRGB(format, nullptr,255, 0, 0)]		= Tile::TILE_SPIKE;		// Red - Spike Hazard
}
#include <iostream>

void LevelDesigner::placeTile(int x, int y, int tileType)
{

	Tile* tile = nullptr;
	switch (tileType) {
		case Tile::TILE_EMPTY: tile = new Tile(); break;
		case Tile::TILE_PLATFORM: tile = new PlatformTile(); break;
		case Tile::TILE_SPAWN: tile = new SpawnTile(); break;
		case Tile::TILE_SPIKE: tile = new SpikeTile(); break;
		default: tile = new Tile(); break; // Default to empty tile
	}

	// Scale pixel coordinates to world coordinates
	int worldX = x * TILE_SIZE;
	int worldY = y * TILE_SIZE;

	tile->type = tileType;
	if (tileType != Tile::TILE_EMPTY) {
		std::cout << "Placed tile type " << tileType << " at (" << x << ", " << y << ")" << std::endl;
	}
	tile->position = { worldX, worldY };
	tiles.push_back(*tile);
}

int LevelDesigner::getTileTypeFromColor(SDL_Color color, SDL_Surface surface)
{
	Uint32 pixel = SDL_MapRGB(SDL_GetPixelFormatDetails(surface.format), nullptr, color.r, color.g, color.b);
	auto it = colorToTileMap.find(pixel);
	if (it != colorToTileMap.end()) {
		return it->second;
	}
	return Tile::TILE_EMPTY; // Default to empty if color not found
}