#include <Engine.h>
#include <Scene.h>

int main() {
	// Initialize SDL Video subsystem
	SDL_Init(SDL_INIT_VIDEO);

	// Create window
	Engine::Window window("Game Window", 800, 600);

	bool running = true;
	SDL_Event event;

	// Main loop
	Scene scene;
	Engine::Timer timer;
	timer.Start();
	while (running) {

		timer.UpdateTicks();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		window.Clear();

		scene.Update(timer.GetDeltaTime());
		scene.Render();

		window.Present();
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	return 0;
}