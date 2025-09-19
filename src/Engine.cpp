#include "Engine.h"

#include <stdexcept>
#include <SDL3/SDL.h>

/// <summary>
/// Window class definitions
/// Handles window creation, rendering, and management
/// </summary>
Engine::Window::Window(const std::string& title, int width, int height)
{
	window = SDL_CreateWindow(title.c_str(), width, height, NULL);
	if (!window) {
		throw std::runtime_error("Failed to create window: " + std::string(SDL_GetError()));
	}

	renderer = SDL_CreateRenderer(window, "Main Renderer");
	if (!renderer) {
		throw std::runtime_error("Failed to create renderer: " + std::string(SDL_GetError()));
	}
}

Engine::Window::~Window()
{
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
}

void Engine::Window::Clear()
{
	SDL_RenderClear(renderer);
}

void Engine::Window::Present()
{
	SDL_RenderPresent(renderer);
}
/// End of Window class definitions




/// <summary>
/// Timer class definitions
/// Handles time tracking for frame updates and total elapsed time
/// </summary>
Engine::Timer::Timer()
{
	lastTicks = 0;
	currentTicks = 0;
	deltaTime = 0.0f;
	totalTime = 0.0f;
}

void Engine::Timer::Start()
{
	lastTicks = SDL_GetTicks();
	currentTicks = lastTicks; // Quicker initialization to avoid using SDL_GetTicks() twice
}

void Engine::Timer::UpdateTicks()
{
	lastTicks = currentTicks; // Move current ticks to last ticks
	currentTicks = SDL_GetTicks(); // Get new current ticks
	deltaTime = (currentTicks - lastTicks) / 1000.0f; // Convert milliseconds to seconds
	totalTime += deltaTime; // Accumulate total time
}
