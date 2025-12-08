#include "UI.h"

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
}

void UI::Render(SDL_Renderer* renderer, const Camera& camera)
{
	// Pause Button (Top-left)
	SDL_FRect pbDestRect = {
		(32),
		(32),
		static_cast<float>(64),
		static_cast<float>(64)
	};
	pauseButton->Render(renderer, nullptr, &pbDestRect);

	// Player Health (Top-right)
	SDL_FRect phDestRect = {
		(1280 - 200),
		(720 - 698),
		static_cast<float>(180),
		static_cast<float>(32)
	};
	health100->Render(renderer, nullptr, &phDestRect);

	// b1 and b2 correspond to button 1 and button 2 positions
	// b1 is middle higher, b2 is middle lower;
	SDL_FRect b1DestRect = {
	(1280 / 2.8),
	(720 / 3),
	static_cast<float>(360),
	static_cast<float>(64)
	};
	SDL_FRect b2DestRect = {
		(1280 / 2.8),
		(720 / 2),
		static_cast<float>(360),
		static_cast<float>(64)
	};

	//playButton->Render(renderer, nullptr, &b1DestRect); // b1
	//restartButton->Render(renderer, nullptr, &b1DestRect);
	//quitButton->Render(renderer, nullptr, &b2DestRect); // b2
	//menuButton->Render(renderer, nullptr, &b2DestRect);

	// Win/Lose png position
	SDL_FRect wlDestRect = {
		(1280 / 2.8),
		(720 / 7),
		static_cast<float>(360),
		static_cast<float>(64)
	};
	//youWon->Render(renderer, nullptr, &wlDestRect);
	//youLost->Render(renderer, nullptr, &wlDestRect);
}

