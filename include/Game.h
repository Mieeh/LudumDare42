#pragma once

#include<window\window.h>
using namespace bear::window;

#include<graphics\graphics.h>
using namespace bear::graphics;

#include<core\color.h>
using namespace bear::core;

#include"Constants.h"
#include"Enum.h"

class Game {
private:
	Window window;
	GAME_STATE game_state;
	Color clear_color = Color(19/255.0f, 18/255.0f, 14/255.0f);

private:
	void eventInput(bear::Event &event);

public:
	Game();
	~Game();

	// Method for switching the current game state 
	void switchGameState(GAME_STATE new_state);

	// Core main loop from where we use "game_state"
	void mainLoop();
	
	// Methods for the playing state
	void setupPlaying();
	void gameLoop();
};