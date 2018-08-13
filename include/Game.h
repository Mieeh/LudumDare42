#pragma once

#include<map>
#include<array>

#include<sound\sound.h>
using namespace bear::sound;

#include<window\window.h>
using namespace bear::window;

#include<graphics\graphics.h>
#include<graphics\renderers\batch_renderer.h>
using namespace bear::graphics;

#include<core\clock.h>
#include<core\color.h>
using namespace bear::core;

#include"Constants.h"
#include"Enum.h"
#include"Player.h"
#include"Chest.h"
#include"Util.h"
#include"Item.h"
#include"Enemy.h"

struct map_struct {
	std::vector<std::vector<int>> tile_list;
	std::vector<Renderable*> renderable_tile_list; // ground & wall
	std::map<std::string, Renderable*> prop_tile_list; // Chest
	std::map<std::string, Enemy> enemy_tile_list; // Enemy 
};

class Game {
private:
	Window window;
	GAME_STATE game_state;
	Color clear_color = Color(19/255.0f, 18/255.0f, 14/255.0f);

	map_struct map;
	BatchRenderer renderer, ui_layer;
	Player player;
	Item* item;
	TextLabel* event_text;
	TextLabel* event_text_secondary;
	Font* font;
	View view;
	int current_level; 

	float shake_timer;
	int shake_strength;
	bool shake_flag;
	Vector2f origin_pos;

	SoundContext sound_context;
	SoundSource *bg;
	std::array<SoundSource*, SFX_COUNT> sfx_list;

	bool game_won = false;

private:
	void eventInput(bear::Event &event);

	bool removeFromPropTileList(Vector2i key);
	void clearCurrentMap();
	void loadNewMap(int index);
	void startWindowShake(float shake_timer, int shake_strength);

public:
	Game();
	~Game();

	// Method for switching the current game state 
	void switchGameState(GAME_STATE new_state);

	// Core main loop from where we use "game_state"
	void mainLoop();
	
	// Methods for the game states
	void menuLoop();
	void deadLoop();
	// Methods for the playing state
	void setupPlaying();
	void gameLoop();
};