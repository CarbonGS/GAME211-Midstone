#pragma once
#include <map>

class Tile {	

public:
	Tile();
	~Tile();

	/// <summary>
	/// RGB VALUES
	/// Air: 255,255, 255 (White)
	/// 
	/// Platform: 0,0,0 (Black)
	/// Wall: 128,128,128 (Gray)
	/// 
	/// Spawn: 0,255,0 (Green)
	/// 
	/// Spike : 255,0,0 (Red)
	/// 
	/// </summary>
	enum TileType { 
		// Add more tile types as needed 
		// Dont forget to add them to color map in LevelDesigner::InitColorMap(SDL_PixelFormatDetails* format)
		
		// Air
		TILE_EMPTY = 0,

		//Solid Tiles
		TILE_PLATFORM = 1, // Platform/Floor
		TILE_WALL = 2, // Wall (Might not be used)

		// Spawn Tiles
		TILE_SPAWN = 3, // Player Spawn Point

		// Hazard Tiles
		TILE_SPIKE = 4 // Spike Hazard
		// Add more tile types as needed

	};	

	int type;

};