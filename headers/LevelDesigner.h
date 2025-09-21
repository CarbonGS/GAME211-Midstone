#pragma once
#include "Image.h"
#include "Tile.h"
#include <vector>
#include <SDL3/SDL.h>

class LevelDesigner {
public:
	LevelDesigner();
	~LevelDesigner();

	void LevelDesignerLoad(Image* levelfile);
	void GenerateLevel();

private:
	Image* levelImage;
	const int TILE_SIZE = 32; // Tile size in world units

	std::map<Uint32, Tile::TileType> colorToTileMap; // Map to hold color to tile type mapping
	void InitColorMap(SDL_PixelFormatDetails* format); // Initialize the color to tile type map

	void placeTile(int x, int y, int tileType); // Place a tile at the given coordinates
	int getTileTypeFromColor(SDL_Color color, SDL_Surface surface); // Get tile type based on color

	std::vector<Tile> tiles; // Vector to hold all the tiles in the world
};