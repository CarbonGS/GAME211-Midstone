#pragma once
#include <SDL3/SDL.h>
#include "Image.h"
#include "Player.h"
#include "Camera.h"

class UI {
public:
	UI(SDL_Renderer* renderer);
	~UI() {};
	void Render(SDL_Renderer* renderer, const Camera& camera);

private:
	// All UI images;
	Image* playButton;
	Image* quitButton;
	Image* restartButton;
	Image* menuButton;
	Image* pauseButton;

	Image* youWon;
	Image* youLost;

	Image* health0;
	Image* health10;
	Image* health20;
	Image* health30;
	Image* health40;
	Image* health50;
	Image* health60;
	Image* health70;
	Image* health80;
	Image* health90;
	Image* health100;

};

 