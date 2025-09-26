#pragma once
#include <SDL3/SDL.h>
#include "Engine.h"

#include "LevelDesigner.h"
class Scene {

private:
	// Main Game Content Here (e.g. game objects, player, enemies, etc.)
	Image* levelImage;
	Image* backgroundImage;
	LevelDesigner levelDesigner;

public:
	// Basic Scene Methods
	Scene(SDL_Renderer* renderer); // Initialize any added game content here
	~Scene(); // Cleanup any added game content here

	void Update(float deltaTime); // Update game logic here
	void Render(SDL_Renderer* renderer, int w, int h); // Render game content here
	void EventHandler(const SDL_Event& sdlEvent); // Handle scene-specific events here
};