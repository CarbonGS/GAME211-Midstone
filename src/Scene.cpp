#include "Scene.h"

Scene::Scene(SDL_Renderer* renderer, int width, int height)
	: camera(0, 0, width, height, 2.5f) // camera zoom is the last parameter
{
	levelImage = new Image();
	levelImage->LoadSurface("assets/lvl.png");

	backgroundImage = new Image();
	backgroundImage->LoadTexture(renderer, "assets/background.png");

	levelDesigner.LevelDesignerLoad(levelImage);
	levelDesigner.GenerateLevel(renderer);

	playerTexture = new Image();
	playerTexture->LoadTexture(renderer, "assets/player.png");
	player = new Player(playerTexture);
	camera.zoom = 4.0f;

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
	float zoom = camera.zoom;
	// Set Background
	SDL_FRect bgDestRect = { 0, 0, static_cast<float>(camera.width) * zoom, static_cast<float>(camera.height) * zoom };
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
