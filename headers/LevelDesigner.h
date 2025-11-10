#pragma once
#include "Image.h"
#include "Tile.h"
#include "Camera.h"
#include <vector>
#include <SDL3/SDL.h>

class LevelDesigner {
public:
	LevelDesigner();
	~LevelDesigner();

	void InitTileTextures(SDL_Renderer* renderer); // Initialize tile textures
	void LevelDesignerLoad(Image* levelfile);
	void GenerateLevel(SDL_Renderer* renderer);
	const std::vector<Tile*>& GetWorldTiles() const; // Output the generated tiles

	void RenderWorld(SDL_Renderer* renderer, Camera& camera);
	void UpdateWorldTiles(float deltaTime); // Changed to float

private:
	Image* levelImage;
	Engine::Window* window; // Pointer to the main window for rendering

	std::map<Uint32, Tile::TileType> colorToTileMap; // Map to hold color to tile type mapping
	void InitColorMap(const SDL_PixelFormatDetails* format); // Initialize the color to tile type map

	std::map<Tile::TileType, Image*> tileTextures; // Map to hold tile type to texture mapping

	void placeTile(int x, int y, int tileType); // Place a tile at the given coordinates
	int getTileTypeFromColor(SDL_Color color, SDL_Surface surface); // Get tile type based on color
	std::vector<Tile*> tiles; // Vector to hold all the tiles in the world
};