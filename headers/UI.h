#pragma once
#include "Image.h"
#include "Camera.h"
#include "Player.h"
#include "GameState.h"
#include <SDL3/SDL.h>

class UI {
public:
	UI(SDL_Renderer* renderer);
	void Render(SDL_Renderer* renderer, const Camera& camera, Player& player);
	void HandleEvent(const SDL_Event& event);

private:
	Image *pauseButton, *playButton, *restartButton, *quitButton, *menuButton;
	Image *youWon, *youLost;
	Image *health0, *health10, *health20, *health30, *health40, *health50, *health60, *health70, *health80, *health90, *health100;
	Image *currentHealth;
};