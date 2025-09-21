#include "LevelDesigner.h"

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
			SDL_GetRGB(pixel, SDL_GetPixelFormatDetails(surface->format), nullptr, &color.r, &color.g, &color.b);
			int tileType = getTileTypeFromColor(color, *surface); // Determine tile type based on color
			placeTile(x, y, tileType); // Place the tile in the game world
		}
	}
	if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}

void LevelDesigner::InitColorMap(SDL_PixelFormatDetails* format)
{
	// Add more colors and their corresponding tile types as needed
	colorToTileMap[SDL_MapRGB(format, nullptr,255, 255, 255)]	= Tile::TILE_EMPTY;     // White - Air
	colorToTileMap[SDL_MapRGB(format, nullptr,0, 0, 0)]			= Tile::TILE_PLATFORM;	// Black - Platform
	colorToTileMap[SDL_MapRGB(format, nullptr,128, 128, 128)]	= Tile::TILE_WALL;		// Gray - Wall
	colorToTileMap[SDL_MapRGB(format, nullptr,0, 255, 0)]		= Tile::TILE_SPAWN;		// Green - Spawn Point
	colorToTileMap[SDL_MapRGB(format, nullptr,255, 0, 0)]		= Tile::TILE_SPIKE;		// Red - Spike Hazard
}

void LevelDesigner::placeTile(int x, int y, int tileType)
{
	////// TODO: Create tile child classes for each type and handle them accordingly 
	////// then use switch case here to instantiate the correct type

	// Scale pixel coordinates to world coordinates
	int worldX = x * TILE_SIZE;
	int worldY = y * TILE_SIZE;

	Tile tile; // Create a new tile
	tile.type = tileType; // Set the tile type
	tiles.push_back(tile); // Add the tile to the list (World Tiles)
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
