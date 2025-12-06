#include "PlatformTile.h"
#include "Camera.h"
#include <string>

PlatformTile::PlatformTile(PlatformTileType type, SDL_Renderer* renderer)
	: platformType(type)
{
	std::string filename = "assets/tiles/tile_";
	switch (type) {
		case TILE_TL: filename += "TL.png"; break;
		case TILE_TM: filename += "TM.png"; break;
		case TILE_TR: filename += "TR.png"; break;
		case TILE_ML: filename += "ML.png"; break;
		case TILE_MM: filename += "MM.png"; break;
		case TILE_MR: filename += "MR.png"; break;
		case TILE_BL: filename += "BL.png"; break;
		case TILE_BM: filename += "BM.png"; break;
		case TILE_BR: filename += "BR.png"; break;
		case TILE_MU: filename += "MU.png"; break;
	}
	image = new Image();
	image->LoadTexture(renderer, filename.c_str());
}

PlatformTile::~PlatformTile()
{
	delete image;
}

void PlatformTile::Update(float deltaTime)
{
	// Platform tile specific update logic can be added here
}

void PlatformTile::Render(SDL_Renderer* renderer, Camera& camera)
{
	float zoom = camera.zoom;
	SDL_FRect dst = {
		static_cast<float>(position.x - camera.x) * zoom,
		static_cast<float>(position.y - camera.y) * zoom,
		TILE_SIZE * zoom,
		TILE_SIZE * zoom
	};
	image->Render(renderer, nullptr, &dst);
}
