#include "UI.h"
#include "GameState.h"
#include "RestartFlag.h"
#include <SDL3/SDL.h>
#include <iostream>

UI::UI(SDL_Renderer* renderer) {
	pauseButton = new Image();
	pauseButton->LoadTexture(renderer, "assets/Pause_Button.png");
	playButton = new Image();
	playButton->LoadTexture(renderer, "assets/Play_Button.png");
	restartButton = new Image();
	restartButton->LoadTexture(renderer, "assets/Restart_Button.png");
	quitButton = new Image();
	quitButton->LoadTexture(renderer, "assets/Quit_Button.png");
	menuButton = new Image();
	menuButton->LoadTexture(renderer, "assets/Menu_Button.png");

	youWon = new Image();
	youWon->LoadTexture(renderer, "assets/You_Won.png");
	youLost = new Image();
	youLost->LoadTexture(renderer, "assets/You_Lost.png");

	health0 = new Image();
	health0->LoadTexture(renderer, "assets/Health_0.png");
	health10 = new Image();
	health10->LoadTexture(renderer, "assets/Health_10.png");
	health20 = new Image();
	health20->LoadTexture(renderer, "assets/Health_20.png");
	health30 = new Image();
	health30->LoadTexture(renderer, "assets/Health_30.png");
	health40 = new Image();
	health40->LoadTexture(renderer, "assets/Health_40.png");
	health50 = new Image();
	health50->LoadTexture(renderer, "assets/Health_50.png");
	health60 = new Image();
	health60->LoadTexture(renderer, "assets/Health_60.png");
	health70 = new Image();
	health70->LoadTexture(renderer, "assets/Health_70.png");
	health80 = new Image();
	health80->LoadTexture(renderer, "assets/Health_80.png");
	health90 = new Image();
	health90->LoadTexture(renderer, "assets/Health_90.png");
	health100 = new Image();
	health100->LoadTexture(renderer, "assets/Health_100.png");

	currentHealth = health100;
}

void UI::Render(SDL_Renderer* renderer, const Camera& camera, Player& player)
{
	// Button rects
	SDL_FRect pbDestRect = { 32, 32, 64, 64 };
	SDL_FRect phDestRect = { 1280 - 200, 720 - 698, 180, 32 };
	SDL_FRect b1DestRect = { 1280 / 2.8f, 720 / 3.0f, 360, 64 };
	SDL_FRect b2DestRect = { 1280 / 2.8f, 720 / 2.0f, 360, 64 };
	SDL_FRect wlDestRect = { 1280 / 2.8f, 720 / 7.0f, 360, 64 };

	// Health bar
	if (player.health >= 100) currentHealth = health100;
	else if (player.health >= 90) currentHealth = health90;
	else if (player.health >= 80) currentHealth = health80;
	else if (player.health >= 70) currentHealth = health70;
	else if (player.health >= 60) currentHealth = health60;
	else if (player.health >= 50) currentHealth = health50;
	else if (player.health >= 40) currentHealth = health40;
	else if (player.health >= 30) currentHealth = health30;
	else if (player.health >= 20) currentHealth = health20;
	else if (player.health >= 10) currentHealth = health10;
	else currentHealth = health0;

	// --- UI by Game State ---
	if (gGameState == MAIN_MENU) {
		playButton->Render(renderer, nullptr, &b1DestRect);
		quitButton->Render(renderer, nullptr, &b2DestRect);
	}
	else if (gGameState == PLAYING) {
		pauseButton->Render(renderer, nullptr, &pbDestRect);
		currentHealth->Render(renderer, nullptr, &phDestRect);
	}
	else if (gGameState == PAUSED) {
		restartButton->Render(renderer, nullptr, &b1DestRect);
		quitButton->Render(renderer, nullptr, &b2DestRect);
		menuButton->Render(renderer, nullptr, &pbDestRect);
		currentHealth->Render(renderer, nullptr, &phDestRect);
	}
	else if (gGameState == GAME_OVER) {
		youLost->Render(renderer, nullptr, &wlDestRect);
		restartButton->Render(renderer, nullptr, &b1DestRect);
		quitButton->Render(renderer, nullptr, &b2DestRect);
		menuButton->Render(renderer, nullptr, &pbDestRect);
		currentHealth->Render(renderer, nullptr, &phDestRect);
	}
	else if (gGameState == GAME_WON) {
		youWon->Render(renderer, nullptr, &wlDestRect);
		restartButton->Render(renderer, nullptr, &b1DestRect);
		quitButton->Render(renderer, nullptr, &b2DestRect);
		menuButton->Render(renderer, nullptr, &pbDestRect);
		currentHealth->Render(renderer, nullptr, &phDestRect);
	}
}

void UI::HandleEvent(const SDL_Event& event)
{
	if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN || event.button.button != SDL_BUTTON_LEFT)
		return;
	float mx = static_cast<float>(event.button.x);
	float my = static_cast<float>(event.button.y);
	SDL_FRect b1DestRect = { 1280 / 2.8f, 720 / 3.0f, 360, 64 };
	SDL_FRect b2DestRect = { 1280 / 2.8f, 720 / 2.0f, 360, 64 };
	SDL_FRect pbDestRect = { 32, 32, 64, 64 };

	if (gGameState == MAIN_MENU) {
		if (mx >= b1DestRect.x && mx <= b1DestRect.x + b1DestRect.w && my >= b1DestRect.y && my <= b1DestRect.y + b1DestRect.h) {
			gGameState = PLAYING;
			std::cout << "Play pressed" << std::endl;
		}
		else if (mx >= b2DestRect.x && mx <= b2DestRect.x + b2DestRect.w && my >= b2DestRect.y && my <= b2DestRect.y + b2DestRect.h) {
			std::cout << "Quit pressed" << std::endl;
			SDL_Event quitEvent; quitEvent.type = SDL_EVENT_QUIT; SDL_PushEvent(&quitEvent);
		}
	}
	else if (gGameState == PLAYING) {
		if (mx >= pbDestRect.x && mx <= pbDestRect.x + pbDestRect.w && my >= pbDestRect.y && my <= pbDestRect.y + pbDestRect.h) {
			gGameState = PAUSED;
			std::cout << "Pause pressed" << std::endl;
		}
	}
	else if (gGameState == PAUSED || gGameState == GAME_OVER || gGameState == GAME_WON) {
		if (mx >= b1DestRect.x && mx <= b1DestRect.x + b1DestRect.w && my >= b1DestRect.y && my <= b1DestRect.y + b1DestRect.h) {
			gRequestRestart = true;
			std::cout << "Restart pressed" << std::endl;
		}
		else if (mx >= b2DestRect.x && mx <= b2DestRect.x + b2DestRect.w && my >= b2DestRect.y && my <= b2DestRect.y + b2DestRect.h) {
			std::cout << "Quit pressed" << std::endl;
			SDL_Event quitEvent; quitEvent.type = SDL_EVENT_QUIT; SDL_PushEvent(&quitEvent);
		}
		else if (mx >= pbDestRect.x && mx <= pbDestRect.x + pbDestRect.w && my >= pbDestRect.y && my <= pbDestRect.y + pbDestRect.h) {
			gGameState = MAIN_MENU;
			std::cout << "Menu pressed" << std::endl;
		}
	}
}

