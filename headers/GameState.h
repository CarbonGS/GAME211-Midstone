#pragma once

enum GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    GAME_WON
};

extern GameState gGameState;
