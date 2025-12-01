#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <fmod.hpp>
#include "Audio.h"
#include "Engine.h"
#include "LevelDesigner.h"
#include "Player.h"
#include "Camera.h"

class Scene {

private:
	// Main Game Content Here (e.g. game objects, player, enemies, etc.)
	Image* levelImage;
	Image* backgroundImage;
	float backW, backH;

	LevelDesigner levelDesigner;

	Image* playerTexture;
	Player* player;

	Camera camera;
	// Test Audio
	Audio* test = nullptr;
	// Create more audio objects as needed

	// FMOD System Pointer
	FMOD::System* fmodSystem = nullptr;

public:
	// Basic Scene Methods
	Scene(SDL_Renderer* renderer, int w, int h, FMOD::System* fmodSystem); // Initialize any added game content here
	~Scene(); // Cleanup any added game content here

	void Update(float deltaTime); // Update game logic here
	void Render(SDL_Renderer* renderer); // Render game content here
	void EventHandler(const SDL_Event& sdlEvent); // Handle scene-specific events here
	void HandleCollisions(); // Handle collisions between entities here (unused until a need arises)

};