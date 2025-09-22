#pragma once
#include <map>
#include "Engine.h"

class Tile {	

public:
	Tile();
	virtual ~Tile();

	/// <summary>
	/// RGB VALUES
	/// Air: 255,255, 255 (White)
	/// 
	/// Platform: 0,0,0 (Black)
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

		// Spawn Tiles
		TILE_SPAWN = 2, // Player Spawn Point

		// Hazard Tiles
		TILE_SPIKE = 3 // Spike Hazard
		// Add more tile types as needed

	};	

	int type;
	SDL_Point position; // Tile position in the grid
	virtual void Update(Engine::Timer* deltaTime);
};