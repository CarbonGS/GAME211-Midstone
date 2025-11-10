#include "LevelDesigner.h"
#include "SpikeTile.h"
#include "SpawnTile.h"
#include "PlatformTile.h"
#include "Camera.h"
#include <algorithm>

// Helper: Find topmost non-transparent pixel in a surface (for spikes)
int getTopNonTransparentPixel(SDL_Surface* surface) {
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
            Uint8 a;
            SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(surface->format), nullptr, nullptr, nullptr, nullptr, &a);
            if (a > 0) return y;
        }
    }
    return 0;
}
// Helper: Find bottommost non-transparent pixel in a surface (for platforms)
int getBottomNonTransparentPixel(SDL_Surface* surface) {
    for (int y = surface->h - 1; y >= 0; --y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
            Uint8 a;
            SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(surface->format), nullptr, nullptr, nullptr, nullptr, &a);
            if (a > 0) return y;
        }
    }
    return surface->h - 1;
}

LevelDesigner::LevelDesigner() : levelImage(nullptr), window(nullptr) {}

LevelDesigner::~LevelDesigner()
{
	levelImage = nullptr;
	for (auto& pair : tileTextures) {
		if (pair.second) {
			delete pair.second;
		}
	}
	tileTextures.clear();
	for (Tile* tile : tiles) {
		if (tile) {
			delete tile;
		}
	}
	tiles.clear();
}

void LevelDesigner::LevelDesignerLoad(Image* levelfile) // Ensure the image is loaded in scene via Image class first
{
	levelImage = levelfile;
}

void LevelDesigner::GenerateLevel(SDL_Renderer* renderer)
{
	SDL_Surface* surface = levelImage->GetSurface();
	const SDL_PixelFormatDetails* formatDetails = SDL_GetPixelFormatDetails(surface->format);
	InitColorMap(formatDetails);
	InitTileTextures(renderer);

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

const std::vector<Tile*>& LevelDesigner::GetWorldTiles() const
{
	return tiles;
}

void LevelDesigner::InitColorMap(const SDL_PixelFormatDetails* format)
{
	// Add more colors and their corresponding tile types as needed
	colorToTileMap[SDL_MapRGB(format, nullptr,255, 255, 255)] = Tile::TILE_EMPTY;     // White - Air
	colorToTileMap[SDL_MapRGB(format, nullptr,0, 0, 0)] = Tile::TILE_PLATFORM; // Black - Platform
	colorToTileMap[SDL_MapRGB(format, nullptr,0, 255, 0)] = Tile::TILE_SPAWN; // Green - Spawn Point
	colorToTileMap[SDL_MapRGB(format, nullptr,255, 0, 0)] = Tile::TILE_SPIKE; // Red - Spike Hazard
}

void LevelDesigner::InitTileTextures(SDL_Renderer* ren)
{
	tileTextures[Tile::TILE_PLATFORM] = new Image();
	tileTextures[Tile::TILE_PLATFORM]->LoadTexture(ren, "assets/platform.png");

	tileTextures[Tile::TILE_SPIKE] = new Image();
	tileTextures[Tile::TILE_SPIKE]->LoadTexture(ren, "assets/spike.png");
}

void LevelDesigner::placeTile(int x, int y, int tileType)
{
    Tile* tile = nullptr;
    int worldX = x * TILE_SIZE;
    int worldY = y * TILE_SIZE;

    switch (tileType) {
        case Tile::TILE_EMPTY:
            tile = new Tile();
            tile->collisionRect = { worldX, worldY, TILE_SIZE, TILE_SIZE };
            break;
        case Tile::TILE_PLATFORM: {
            tile = new PlatformTile(tileTextures[Tile::TILE_PLATFORM]);
            tile->collisionRect = { worldX, worldY + 10, TILE_SIZE, TILE_SIZE - 10 };
            break;
        }
        case Tile::TILE_SPAWN:
            tile = new SpawnTile();
            tile->collisionRect = { worldX, worldY, TILE_SIZE, TILE_SIZE };
            break;
        case Tile::TILE_SPIKE: {
            auto spike = new SpikeTile(tileTextures[Tile::TILE_SPIKE]);
            spike->SetInitialY(worldY);
            spike->collisionRect = { worldX, worldY + 22, TILE_SIZE, 10 };
            tile = spike;
            break;
        }
        default:
            tile = new Tile();
            tile->collisionRect = { worldX, worldY, TILE_SIZE, TILE_SIZE };
            break;
    }
    tile->type = tileType;
    tile->position = { worldX, worldY };
    tiles.push_back(tile);
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

void LevelDesigner::RenderWorld(SDL_Renderer* renderer, Camera& camera)
{
	for (Tile* tile : tiles) {
		if (tile->type != Tile::TILE_EMPTY && tile->type != Tile::TILE_SPAWN) {
			tile->Render(renderer, camera);
		}
	}
}

void LevelDesigner::UpdateWorldTiles(float deltaTime)
{
    for (Tile* tile : tiles) {
        tile->Update(deltaTime);
        if (auto spike = dynamic_cast<SpikeTile*>(tile)) {
            spike->PhysicsUpdate(tiles, deltaTime, 900.0f);
        }
    }
}