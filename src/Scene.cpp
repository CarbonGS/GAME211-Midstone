#include "Scene.h"

Scene::Scene(SDL_Renderer* renderer, int width, int height)
	: camera(0, 0, width, height, 2.5f) // camera zoom is the last parameter
{
	levelImage = new Image();
	levelImage->LoadSurface("assets/lvl.png");

	backgroundImage = new Image();
	backgroundImage->LoadTexture(renderer, "assets/background.png");
	SDL_Texture* bgTexture = backgroundImage->GetTexture();
	SDL_GetTextureSize(bgTexture,&backH, &backW);

	levelDesigner.LevelDesignerLoad(levelImage);
	levelDesigner.GenerateLevel(renderer);

	playerTexture = new Image();
	playerTexture->LoadTexture(renderer, "assets/player.png");
	player = new Player(playerTexture);
	camera.zoom = 2.0f;

	for (Tile* tile : levelDesigner.GetWorldTiles()) { // Set Player Spawn Position
		if (tile->type == Tile::TILE_SPAWN) {
			player->setPosition(static_cast<float>(tile->position.x), static_cast<float>(tile->position.y));
			break;
		}
	}
}

Scene::~Scene()
{
	delete levelImage;
	delete backgroundImage;
	delete playerTexture;
	delete player;
}

void Scene::Update(float deltaTime)
{
	// Update world tiles (physics for spikes, etc.)
	levelDesigner.UpdateWorldTiles(deltaTime);

	// Update entities
	if (player) {
		player->Update(deltaTime, levelDesigner.GetWorldTiles());
		// Center camera on player
		camera.CenterOn(player->GetBounds().x + player->GetBounds().w / 2,
			player->GetBounds().y + player->GetBounds().h / 2);
	}

	// Check Collisions
	HandleCollisions();

	// Other scene updates
}

void Scene::Render(SDL_Renderer* renderer)
{
	// Set Background
	float zoom = 1.5f;
	// Render the background (Currently only fixed background)
	SDL_FRect bgDestRect = { 0, 0, static_cast<float>(camera.width), static_cast<float>(camera.height) };
	backgroundImage->Render(renderer, nullptr, &bgDestRect);

	// Render World Tiles
	const std::vector<Tile*>& worldTiles = levelDesigner.GetWorldTiles();
	for (Tile* tile : worldTiles) {
		if (tile->type != Tile::TILE_EMPTY && tile->type != Tile::TILE_SPAWN) {
			tile->Render(renderer, camera);
		}
	}

	// Render Player
	if (player) {
		player->Render(renderer, camera);
	}

	// Debugging: Render collision boxes
	// Set color for tile collision boxes (e.g., red)
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
	for (const Tile* tile : worldTiles) {
		if (tile->type != Tile::TILE_EMPTY) {
			SDL_FRect rect = {
				(float)tile->collisionRect.x - camera.x,
				(float)tile->collisionRect.y - camera.y,
				(float)tile->collisionRect.w,
				(float)tile->collisionRect.h
			};
			// Apply camera zoom if needed
			rect.x *= camera.zoom;
			rect.y *= camera.zoom;
			rect.w *= camera.zoom;
			rect.h *= camera.zoom;
			SDL_RenderRect(renderer, &rect);
		}
	}
	// Set color for player collision box
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
	SDL_Rect playerRect = player->GetBounds();
	SDL_FRect playerFRect = {
		(float)playerRect.x - camera.x,
		(float)playerRect.y - camera.y,
		(float)playerRect.w,
		(float)playerRect.h
	};
	playerFRect.x *= camera.zoom;
	playerFRect.y *= camera.zoom;
	playerFRect.w *= camera.zoom;
	playerFRect.h *= camera.zoom;
	SDL_RenderRect(renderer, &playerFRect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Scene::EventHandler(const SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_EVENT_WINDOW_RESIZED) {
		camera.width = sdlEvent.window.data1; // new width
		camera.height = sdlEvent.window.data2; // new height
		// Re-center camera on player
		if (player) {
			camera.CenterOn(player->GetBounds().x + player->GetBounds().w / 2,
				player->GetBounds().y + player->GetBounds().h / 2);
		}
	}

	if (player) {
		player->HandleInput(sdlEvent);
	}
}

void Scene::HandleCollisions()
{
	// Currently unused until a need arises
}
