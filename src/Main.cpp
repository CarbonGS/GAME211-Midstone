#include <Engine.h>
#include <Scene.h>
#include <UI.h>
#include <GameState.h>
#include <RestartFlag.h>
#include <fmod.hpp>

int main(int argc, char* argv[]) {
	// Initialize SDL Video subsystem
	SDL_Init(SDL_INIT_VIDEO);

	// Create window
	int width;
	int height;

	width = 1280;
	height = 720;

	SDL_Log("Window Size: %dx%d", width, height);
	Engine::Window window("Revenir", width, height);

	// Initialize FMOD
	FMOD::System* fmodSystem = nullptr;
	FMOD::System_Create(&fmodSystem);
	fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	bool running = true;
	SDL_Event event;

	Scene* scene = new Scene(window.GetRenderer(), width, height, fmodSystem);
	UI* ui = new UI(window.GetRenderer());

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
			ui->HandleEvent(event);
			scene->EventHandler(event);
		}

		window.Clear();

		scene->Update(deltaTime);
		scene->Render(window.GetRenderer());
		ui->Render(window.GetRenderer(), scene->GetCamera(), *scene->GetPlayer());

		window.Present();

		// Handle Restart
		if (gRequestRestart) {
			delete scene;
			scene = new Scene(window.GetRenderer(), width, height, fmodSystem);
			gGameState = PLAYING;
			gRequestRestart = false;
		}
	}

	delete ui;
	delete scene;

	fmodSystem->close();
	fmodSystem->release();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	return 0;
}