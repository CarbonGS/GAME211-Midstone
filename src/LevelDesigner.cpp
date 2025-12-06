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

    Uint32* pixels = (Uint32*)surface->pixels;
    int width = surface->w;
    int height = surface->h;

    // Build a 2D map of tile types for neighbor checks
    std::vector<std::vector<int>> tileTypeMap(height, std::vector<int>(width, Tile::TILE_EMPTY));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Uint32 pixel = pixels[y * width + x];
            SDL_Color color;
            SDL_GetRGB(pixel, formatDetails, nullptr, &color.r, &color.g, &color.b);
            int tileType = getTileTypeFromColor(color, *surface);
            tileTypeMap[y][x] = tileType;
        }
    }

    // Place tiles using the map and orientation logic
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileType = tileTypeMap[y][x];
            int worldX = x * TILE_SIZE;
            int worldY = y * TILE_SIZE;
            if (tileType == Tile::TILE_PLATFORM) {
                PlatformTileType type = GetPlatformTileType(x, y, tileTypeMap);
                PlatformTile* tile = new PlatformTile(type, renderer);
                tile->position = { worldX, worldY };
                tile->type = tileType;
                if (type == TILE_MM) {
                    tile->collisionRect = { worldX, worldY, TILE_SIZE, TILE_SIZE };
                } else {
                    tile->collisionRect = { worldX + 8, worldY + 8, TILE_SIZE - 16, TILE_SIZE - 16 };
                }
                tiles.push_back(tile);
            } else {
                placeTile(x, y, tileType); // Use your existing logic for other tile types
            }
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
	tileTextures[Tile::TILE_PLATFORM]->LoadTexture(ren, "assets/tiles/tile_BM.png");

	tileTextures[Tile::TILE_SPIKE] = new Image();
	tileTextures[Tile::TILE_SPIKE]->LoadTexture(ren, "assets/tiles/spike.png");
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

PlatformTileType LevelDesigner::GetPlatformTileType(int x, int y, const std::vector<std::vector<int>>& map) {
    auto isEmpty = [&](int nx, int ny) {
        if (ny < 0 || ny >= (int)map.size() || nx < 0 || nx >= (int)map[0].size()) return true;
        return map[ny][nx] != Tile::TILE_PLATFORM;
    };
    bool emptyUp    = isEmpty(x, y-1);
    bool emptyDown  = isEmpty(x, y+1);
    bool emptyLeft  = isEmpty(x-1, y);
    bool emptyRight = isEmpty(x+1, y);

    // Corners: grass faces empty
    if (emptyUp && emptyLeft) return TILE_TL; // Top left corner
    if (emptyUp && emptyRight) return TILE_TR; // Top right corner
    if (emptyDown && emptyLeft) return TILE_BL; // Bottom left corner
    if (emptyDown && emptyRight) return TILE_BR; // Bottom right corner

    // Edges: grass faces empty
    if (emptyUp) return TILE_BM; // Bottom edge
    if (emptyDown) return TILE_TM; // Top edge
    if (emptyLeft) return TILE_ML; // Left edge
    if (emptyRight) return TILE_MR; // Right edge

	// Extra case: default to MU if surrounded on three sides
    if (emptyUp && !emptyDown && !emptyLeft && !emptyRight) return TILE_MU;

    // Center (surrounded by platforms)
    return TILE_MM;
}