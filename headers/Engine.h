#pragma once
#include <string>
#include <SDL3\SDL.h>

namespace Engine {

	// Created By Stojan B N01717368: 2025-09-19
	class Window {
	public:

		Window(const std::string& title, int width, int height);
		~Window();

		void Clear();
		void Present();

		SDL_Renderer* GetRenderer() const { return renderer; }
		SDL_Window* GetSDLWindow() const { return window; }

	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

	};

	// Created By Stojan B N01717368: 2025-09-19
	class Timer {
	public:
		Timer();
		~Timer() {};

 		void Start();
		void UpdateTicks();
		float GetDeltaTime() const { return deltaTime; }
		float GetTotalTime() const { return totalTime; }

	private:
		uint64_t lastTicks; // Ticks at the last frame
		uint64_t currentTicks; // Ticks at the current frame
		float deltaTime; // Time between the last two frames
		float totalTime; // Time since the timer started
	};

}
