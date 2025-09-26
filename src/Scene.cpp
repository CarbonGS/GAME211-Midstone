#include "Scene.h"

/// <summary>
/// Constructs a new Scene object.
/// Initializes any added game content here.
/// </summary>
Scene::Scene(SDL_Renderer* renderer)
{
	levelImage = new Image();
	levelImage->LoadSurface("extern/lvl.png"); // Temp level image path
	levelDesigner.LevelDesignerLoad(levelImage);
	levelDesigner.GenerateLevel(renderer);
}

/// <summary>
/// Destroys the Scene object and releases any associated resources.
/// Cleans up any added game content here.
/// </summary>
Scene::~Scene()
{

}

/// <summary>
/// Updates the scene based on the elapsed time since the last update.
/// Update game logic here.
/// </summary>
/// <param name="deltaTime">The time in seconds since the last update.</param>
void Scene::Update(float deltaTime)
{

}

/// <summary>
/// Renders the scene.
/// Render game content here.
/// </summary>
void Scene::Render(SDL_Renderer* renderer)
{
	const std::vector<Tile*>& worldTiles = levelDesigner.GetWorldTiles();
	for (Tile* tile : worldTiles) {
		if (tile->type != Tile::TILE_EMPTY && tile->type != Tile::TILE_SPAWN) { // Only render textured tiles
			tile->Render(renderer);
		}
	}
}

/// <summary>
/// Handles an SDL event within the Scene.
/// </summary>
/// <param name="sdlEvent">The SDL_Event object representing the event to be handled.</param>
void Scene::EventHandler(const SDL_Event& sdlEvent)
{

}
