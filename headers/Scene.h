#pragma once
#include <SDL3/SDL.h>

class Scene {

private:
	// Main Game Content Here (e.g. game objects, player, enemies, etc.)

public:
	// Basic Scene Methods
	Scene();
	~Scene();

	void Update(float deltaTime);
	void Render();
	void EventHandler(const SDL_Event& sdlEvent);
};