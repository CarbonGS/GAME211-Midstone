#pragma once
#include "Image.h"
#include "Tile.h"
#include <vector>
#include <map>
#include <SDL3/SDL.h>
#include "PlatformTile.h"

class Camera;

class LevelDesigner {
public:
    LevelDesigner();
    ~LevelDesigner();

    void LevelDesignerLoad(Image* levelfile);
    void GenerateLevel(SDL_Renderer* renderer);
    const std::vector<Tile*>& GetWorldTiles() const;
    void RenderWorld(SDL_Renderer* renderer, Camera& camera);
    void UpdateWorldTiles(float deltaTime);

    // Helper for platform tile orientation
    PlatformTileType GetPlatformTileType(int x, int y, const std::vector<std::vector<int>>& map);

private:
    Image* levelImage;
    SDL_Window* window;
    std::map<int, Image*> tileTextures;
    std::map<Uint32, int> colorToTileMap;
    std::vector<Tile*> tiles;

    void InitColorMap(const SDL_PixelFormatDetails* format);
    void InitTileTextures(SDL_Renderer* ren);
    void placeTile(int x, int y, int tileType);
    int getTileTypeFromColor(SDL_Color color, SDL_Surface surface);
};