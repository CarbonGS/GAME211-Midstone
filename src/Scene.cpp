#include "Scene.h"

/// <summary>
/// Constructs a new Scene object.
/// Initializes any added game content here.
/// </summary>
Scene::Scene(SDL_Renderer* renderer, int width, int height)
	: camera(0, 0, width, height, 2.0f) // Example: 2x zoom
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

	for (Tile* tile : levelDesigner.GetWorldTiles()) { // Set Player Spawn Position
		if (tile->type == Tile::TILE_SPAWN) {
			player->setPosition(static_cast<float>(tile->position.x), static_cast<float>(tile->position.y));
			break;
		}
	}
}

/// <summary>
/// Destroys the Scene object and releases any associated resources.
/// Cleans up any added game content here.
/// </summary>
Scene::~Scene()
{
	delete levelImage;
	delete backgroundImage;
	delete playerTexture;
	delete player;
}

/// <summary>
/// Updates the scene based on the elapsed time since the last update.
/// Update game logic here.
/// </summary>
/// <param name="deltaTime">The time in seconds since the last update.</param>
void Scene::Update(float deltaTime)
{
	// Update entities
	if (player) {
		player->Update(deltaTime);
		// Center camera on player
		camera.CenterOn(player->GetBounds().x + player->GetBounds().w / 2,
			player->GetBounds().y + player->GetBounds().h / 2);
	}

	// Check Collisions
	HandleCollisions();

	// Other scene updates
}

/// <summary>
/// Renders the scene.
/// Render game content here.
/// </summary>
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

/// <summary>
/// Handles an SDL event within the Scene.
/// </summary>
/// <param name="sdlEvent">The SDL_Event object representing the event to be handled.</param>
void Scene::EventHandler(const SDL_Event& sdlEvent)
{
	if (player) {
		player->HandleInput(sdlEvent);
	}
}

void Scene::HandleCollisions()
{

}
