#pragma once
#include <SDL3/SDL.h>
#include "Engine.h"
#include "LevelDesigner.h"
#include "Player.h"
#include "Camera.h"
#include "Audio.h"

class Scene {

private:
	// Main Game Content Here (e.g. game objects, player, enemies, etc.)
	Image* levelImage;
	Image* backgroundImage;
	float backW, backH;

	LevelDesigner levelDesigner;

	// Player Objects
	Image* idleTexture;
	Image* runTexture;
	Player* player;
	Audio* test;

	Camera camera; // Add camera as a member

public:
	// Basic Scene Methods
	Scene(SDL_Renderer* renderer, int w, int h); // Initialize any added game content here
	~Scene(); // Cleanup any added game content here

	void Update(float deltaTime); // Update game logic here
	void Render(SDL_Renderer* renderer); // Render game content here
	void EventHandler(const SDL_Event& sdlEvent); // Handle scene-specific events here
	void HandleCollisions(); // Handle collisions between entities here (unused until a need arises)
};