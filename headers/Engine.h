#pragma once
#include <string>
#include <SDL3\SDL.h>

namespace Engine {

	/// <summary>
	/// Window class definitions
	/// Handles window creation, rendering, and management
	/// </summary>
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

	/// <summary>
	/// Timer class definitions
	/// Handles time tracking for frame updates and total elapsed time
	/// </summary>
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
