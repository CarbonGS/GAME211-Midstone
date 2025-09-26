#include <Engine.h>
#include <Scene.h>

int main() {
	// Initialize SDL Video subsystem
	SDL_Init(SDL_INIT_VIDEO);

	// Create window
	int width = 800; // Default width
	int height = 600; // Default height

#ifdef _DEBUG
	width = 1280;
	height = 720;
#else
	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
		SDL_Log("SDL_GetCurrentDisplayMode failed: %s", SDL_GetError());
		return -1;
	}
	else {
		width = dm.w;
		height = dm.h;
	}
#endif
	SDL_Log("Window Size: %dx%d", width, height);
	Engine::Window window("Revinir", width, height);


	bool running = true;
	SDL_Event event;

	// Main loop
	Scene* scene = new Scene();

	Engine::Timer timer;
	timer.Start();
	while (running) {

		timer.UpdateTicks();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
			scene->EventHandler(event);
		}

		window.Clear();

		scene->Update(timer.GetDeltaTime());
		scene->Render();

		window.Present();
	}

	delete scene;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	return 0;
}