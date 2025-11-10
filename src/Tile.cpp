#include "Tile.h"
#include "Camera.h"

Tile::Tile() : type(TILE_EMPTY), texture(nullptr) {
    position = { 0, 0 };
    collisionRect = { 0, 0, TILE_SIZE, TILE_SIZE };
}

Tile::~Tile() {}

void Tile::Update(float deltaTime) {
    // Default tile update does nothing
}

void Tile::Render(SDL_Renderer* renderer, Camera& camera) {
    float zoom = camera.zoom;
    SDL_FRect dst = {
        static_cast<float>(position.x - camera.x) * zoom,
        static_cast<float>(position.y - camera.y) * zoom,
        TILE_SIZE * zoom,
        TILE_SIZE * zoom
    };
    if (texture && texture->GetTexture()) {
        texture->Render(renderer, nullptr, &dst);
    }
}
