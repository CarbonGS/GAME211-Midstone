#pragma once
#include "Image.h"
#include "Tile.h"
#include "Camera.h"
#include <SDL3/SDL.h>

#define TILE_SIZE 32

// Enum for all 9 tile states plus Middle Up (MU)
enum PlatformTileType {
    TILE_TL, // Top Left
    TILE_TM, // Top Middle
    TILE_TR, // Top Right
    TILE_ML, // Middle Left
    TILE_MM, // Middle Middle (center/filled)
    TILE_MR, // Middle Right        
    TILE_BL, // Bottom Left
    TILE_BM, // Bottom Middle
    TILE_BR, // Bottom Right
    TILE_MU  // Middle Up
};

class PlatformTile : public Tile {
public:
    PlatformTileType platformType;
    Image* image;
    SDL_Point position;

    PlatformTile(PlatformTileType type, SDL_Renderer* renderer);
    ~PlatformTile();

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer, Camera& camera) override;
};