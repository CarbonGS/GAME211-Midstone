#include <Engine.h>

int main() {
	// Initialize SDL Video subsystem
	SDL_Init(SDL_INIT_VIDEO);

	// Create window
	Engine::Window window("Game Window", 800, 600);

	bool running = true;
	SDL_Event event;

	// Main loop
	while (running) {
				while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		window.Clear();



		window.Present();
	}

	SDL_Quit();
	return 0;
}