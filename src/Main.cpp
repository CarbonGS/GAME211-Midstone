#include <Engine.h>
#include <Scene.h>
#include <Audio.h>
#include <iostream>

int main() {
	// Initialize SDL Video subsystem
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_AUDIO);

	// Create window
	int width;
	int height;
	
	// AUDIO TEST AREA
	//
		// Create Audio object and load WAV
		Audio sound("C:/Users/calam/OneDrive/Documents/file_example_WAV_1MG.wav");   // Make sure "sound.wav" exists in working directory

		// Start playback
		sound.play();

		// Wait while the audio is playing
		while (sound.isPlaying()) {
			SDL_Delay(100);
		}
	
	//
	//

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


	bool running = true;
	SDL_Event event;

	// Main loop
	Scene* scene = new Scene(window.GetRenderer());

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
		scene->Render(window.GetRenderer(), width, height); // Render needs the SDL_Renderer from the window to draw

		window.Present();
	}

	delete scene;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	return 0;
}