#include <Engine.h>
#include <Scene.h>
#include <fmod.hpp>

int main(int argc, char* argv[]) {
	// Initialize SDL Video subsystem
	SDL_Init(SDL_INIT_VIDEO);

	// Create window
	int width;
	int height;

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
	Engine::Window window("Revenir", width, height);

	// Initialize FMOD
	FMOD::System* fmodSystem = nullptr;
	FMOD::System_Create(&fmodSystem);
	fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	bool running = true;
	SDL_Event event;

	// Main loop
	Scene* scene = new Scene(window.GetRenderer(), width, height, fmodSystem);

	Engine::Timer timer;
	timer.Start();
	while (running) {

		timer.UpdateTicks();

		float deltaTime = timer.GetDeltaTime();
		const float MAX_DELTA_TIME = 1.0f / 60.0f;
		if (deltaTime > MAX_DELTA_TIME) deltaTime = MAX_DELTA_TIME;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
			scene->EventHandler(event);
		}

		window.Clear();

		scene->Update(deltaTime);
		scene->Render(window.GetRenderer()); // Render needs the SDL_Renderer from the window to draw

		window.Present();
	}

	delete scene;

	fmodSystem->close();
	fmodSystem->release();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	return 0;
}