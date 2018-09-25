#include"../include/Game.h"

#include<core\vector2.h>
#include"../include/Combat.h"
#include"../include/Util.h"
#include"../include/WorldGenerator.h"

using namespace bear;

#include<time.h>

Game::Game() :
	window(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE), // Create window
	player(),
	shake_timer(0.0f),
	sound_context()
{
	srand(time(NULL));

	//fpsClock.start();

	// Init sound
	sound_context.init();
	bg = new SoundSource("assets\\bg2.wav");
	bg->set_loooping(true);
	bg->setVolume(0.4f);

	sfx_list[LEVEL_WIN] = new SoundSource("assets\\level_win.wav");
	sfx_list[ITEM_PICKUP] = new SoundSource("assets\\item_pickup.wav");
	sfx_list[GAME_OVER] = new SoundSource("assets\\game_over.wav");
	sfx_list[GAME_START] = new SoundSource("assets\\game_start.wav");
	sfx_list[ENEMY_DIE] = new SoundSource("assets\\enemy_die.wav");
	sfx_list[PLAYER_DIE] = new SoundSource("assets\\player_die.wav");
	sfx_list[HEALED] = new SoundSource("assets\\healed.wav");
	sfx_list[ATTACK] = new SoundSource("assets\\attack.wav");
	for (int i = 0; i < SFX_COUNT; i++) {
		sfx_list[i]->setVolume(0.1f);
	}

	// Init graphics
	if (!Graphics::init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
		std::cout << "Failed to init Grap hics module\n";
	}

	renderer.init();
	ui_layer.init();

	font = new Font("assets\\kirsty.ttf", 30);
	event_text = new TextLabel("Nothing here yet!", *font, core::Vector2f(0, 50), core::Color::White());
	event_text_secondary = new TextLabel("Nothing here yet!", *font, core::Vector2f(0, 0), core::Color::White());

	switchGameState(GAME_STATE::MENU); // setup for default game state
	mainLoop();
}
				
Game::~Game()
{
	delete event_text;
	delete font;
	delete bg;
	for (int i = 0; i < SFX_COUNT; i++)
		delete sfx_list[i];
	Graphics::exit();
	sound_context.exit();
}

void Game::switchGameState(GAME_STATE new_state)
{
	bg->stop();
	game_state = new_state;
	switch (game_state) {
	case GAME_STATE::PLAYING:
		setupPlaying();
		break;
	}
}

void Game::mainLoop()
{
	core::Clock timer;
	unsigned int fps = 0;
	timer.start();

	while (window.isOpen()) {

		if (timer.getTicks() >= 1000) {
			std::cout << "FPS: " << fps << std::endl;
			fps = 0;
			timer.reset();
		}

		window.clear(clear_color);

		for (bear::Event event : window.getRegisteredEvents())
			eventInput(event);

		// State machine for what state the game is in and call the appropiate methods
		switch (game_state) {
		case GAME_STATE::PLAYING:
			gameLoop();
			break;
		case GAME_STATE::MENU:
			menuLoop();
			break;
		case GAME_STATE::DEAD:
			deadLoop();
			break;
		}

		window.display();

		fps++;
	}
}

void Game::menuLoop()
{
	static Font _font("assets\\kirsty.ttf", 70);
	static TextLabel titleText("No Space!", _font, core::Vector2f(window.getWindowSize().x*0.3f, 100), core::Color::White());
	static TextLabel startText("Press F to start", *font, core::Vector2f(280, 300), core::Color::Red());
	static TextLabel wonText("YOU'VE WON THE GAME!", *font, core::Vector2f(10, 30), core::Color::Green());

	startText.draw();
	titleText.draw();
	if(game_won == true)
		wonText.draw();
	
	static Renderable menu_player("assets\\player.png");
	menu_player.transform().m_Size = core::Vector2f(TILE_SIZE * 2, TILE_SIZE * 4);
	menu_player.transform().m_Position = core::Vector2f(50, window.getWindowSize().y - TILE_SIZE * 4);
	static Renderable enemy("assets\\dog.png");
	enemy.transform().m_Size = core::Vector2f(TILE_SIZE * 3, TILE_SIZE * 3);
	enemy.transform().m_Position = core::Vector2f(500, window.getWindowSize().y - TILE_SIZE * 3);
	static Renderable enemy2("assets\\bat.png");
	enemy2.transform().m_Size = core::Vector2f(TILE_SIZE * 1.5f, TILE_SIZE * 1.5f);
	enemy2.transform().m_Position = core::Vector2f(430, window.getWindowSize().y - TILE_SIZE * 3.5f);

	renderer.begin();
	renderer.submit(menu_player);
	renderer.submit(enemy);
	renderer.submit(enemy2);
	renderer.flush();
}

void Game::deadLoop()
{
	static Font deadFont("assets\\kirsty.ttf", 80);
	static TextLabel text("You died!", deadFont, core::Vector2f(window.getWindowSize().x*0.3f, 100), Color::Red());
	static TextLabel text2("Press F to continue", *font, core::Vector2f(280, 300), core::Color::White());

	text2.draw();
	text.draw();

	static Renderable dead_player("assets\\dead_player.png");
	dead_player.transform().m_Size = Vector2f(TILE_SIZE*5, TILE_SIZE*2.5f);
	dead_player.transform().m_Position = Vector2f(20, window.getWindowSize().y - TILE_SIZE*2.6f);
	
	static Renderable cross("assets\\religious_cross.png");
	cross.transform().m_Size = Vector2f(TILE_SIZE * 3, TILE_SIZE * 3);
	cross.transform().m_Position = Vector2f(450, window.getWindowSize().y - TILE_SIZE * 4);

	renderer.begin();
	renderer.submit(dead_player);
	renderer.submit(cross);
	renderer.flush();
}

void Game::eventInput(bear::Event &event)
{
	if (event.type == EventType::WindowReiszed) {
		Graphics::window_resize_callback(event.size.x, event.size.y);
	}
	if (game_state == GAME_STATE::MENU) {
		if (event.type == EventType::KeyPressed) {
			if (event.key == Key::F)
				switchGameState(GAME_STATE::PLAYING);
		}
	}
	if (game_state == GAME_STATE::DEAD) {
		if (event.type == EventType::KeyPressed) {
			if (event.key == Key::F)
				switchGameState(GAME_STATE::MENU);
		}
	}

	if (game_state == GAME_STATE::PLAYING) {
		// Process player move input
		if (event.type == EventType::KeyPressed) {
			if (event.key == Key::M) {
				static bool bgPlaying = true;
				if (bgPlaying) {
					bgPlaying = false;
					bg->setVolume(0);
				}
				else if (!bgPlaying) {
					bgPlaying = true;
					bg->setVolume(0.4f);
				}
			}

			int newTileValue;
			core::Vector2i newPos = player.position;
			if (event.key == Key::D)
				newPos.x++;
			else if (event.key == Key::A)
				newPos.x--;
			else if (event.key == Key::S)
				newPos.y++;
			else if (event.key == Key::W)
				newPos.y--;
			else if (event.key == Key::F) {
				// Consume healing item if we have one
				if (item != nullptr) {
					if (item->type == ITEM_TYPE::HEALING) {
						delete item;
						item = nullptr;
						if (player.hearts_left < 10) {
							player.hearts_left++;
							sfx_list[HEALED]->play();
						}
						event_text->setStringLiteral("Used heal item");
					}
				}
			}
			else if (event.key == Key::X) {
				// Give the player shit
				player.attack_points = 99;
				player.hearts_left = 99;
				player.defense_points = 99;
			}
			else if (event.key == Key::Z) {
				current_level++;
				clearCurrentMap();
				loadNewMap(current_level);
			}

			newTileValue = map.tile_list.at(newPos.y).at(newPos.x-1);

			// Logic for after we have the new positon & tile value
			if (newTileValue == 0) { // New tile is a ground tile so we can move onto it with no problem
				player.position = newPos;
			}
			if (newTileValue == 10) { // Level end ladder
				sfx_list[LEVEL_WIN]->play();
				clearCurrentMap();
				current_level++;
				loadNewMap(current_level);
			}
			else if (newTileValue == 1+99 || newTileValue == 2+99 || newTileValue == 3+99) { // Chest
				// Handle chest logic
				// Chest is at position newPos in the map list
				bool chest_was_removed = removeFromPropTileList(Vector2i(newPos.x - 1, newPos.y));
				if(!chest_was_removed) // Chest wasn't on the tile let the player freely move to the tile
					player.position = newPos; 
				else {
					sfx_list[ITEM_PICKUP]->play();
					// Give the player the item found inside the chest
					delete item; // remove current item
					item = new Item();
					switch (newTileValue) { case 1 + 99: item->setType(ITEM_TYPE::DAMAGE); break; case 2 + 99: item->setType(ITEM_TYPE::DEFENSE); break; case 3 + 99: item->setType(ITEM_TYPE::HEALING); break; }
					event_text->setStringLiteral("Found a " + item->item_name);
				}
			}
			else if (newTileValue == 3 || newTileValue == 4 || newTileValue == 5 || newTileValue == 6 || newTileValue == 7) {
				std::string k = positionToStringKey(core::Vector2i(newPos.x - 1, newPos.y));
				auto it = map.enemy_tile_list.find(k);
				if (it != map.enemy_tile_list.end()) { // Check if the enemy exists
					Enemy& enemy = map.enemy_tile_list.at(k); // The actual enemy object on the map (not the prop we have to remove the after the enemy dies)
					std::string ename = enemy.name;
					combat_result result = Combat::performCombtWithEnemy(player, enemy, item);
					player.hearts_left = result.new_player_health;
					enemy.hp = result.new_enemy_health;

					event_text->setStringLiteral("Hit the " + enemy.name);
					std::string temp = enemy.name + " HP: " + std::to_string(enemy.hp);
					event_text_secondary->setStringLiteral(temp);

					// Shake screen
					startWindowShake(150, 5);

					// Game over condition
					if (player.hearts_left <= 0) {
						sfx_list[PLAYER_DIE]->play();
						clearCurrentMap();
						switchGameState(GAME_STATE::DEAD);
					}
					// Enemy died?
					else if (enemy.hp <= 0) {
						sfx_list[ENEMY_DIE]->play();
						map.enemy_tile_list.erase(k);
						removeFromPropTileList(core::Vector2i(newPos.x - 1, newPos.y));
						event_text->setStringLiteral("Enemy died!");
						int num = rand() % 100;
						if (num > 50) {
							if (ename == "Devil" || ename == "Alien")
								player.hearts_left += 3;
							if (ename == "Eyeball")
								player.hearts_left++;
							player.attack_points++;
							event_text_secondary->setStringLiteral("Base Attack Increased!");
						}
						else {
							player.defense_points += 5;
							event_text_secondary->setStringLiteral("Base Defense Increased!");
						}
					}
					else {
						sfx_list[ATTACK]->play();
					}
				}
				else {
					player.position = newPos;
				}
			}
		}
	}
}

bool Game::removeFromPropTileList(Vector2i key)
{
	auto itr = map.prop_tile_list.find(positionToStringKey(key));
	if (itr != map.prop_tile_list.end()) {
		// Found entry, delete it
		delete itr->second;
		map.prop_tile_list.erase(itr);
		return true;
	}
	return false;
}

void Game::clearCurrentMap()
{
	// Clear the tiles (vector of pointers)
	clearVectorOfPointers<Renderable>(map.renderable_tile_list);

	// Clear the prop map (map of pointers)
	clearMapOfPointers<std::string, Renderable>(map.prop_tile_list);

	// Enemy map
	map.enemy_tile_list.clear();

	event_text->setStringLiteral("");
	event_text->setStringLiteral("");
}

void Game::loadNewMap(int index)
{
	// Win condition
	if (index == 5) {
		game_won = true;
		switchGameState(GAME_STATE::MENU);
		return;
	}
	event_text_secondary->setStringLiteral("Welcome to level " + std::to_string(index + 1) + "!");

	player.position = Vector2i(2, 1);

	map.tile_list = level_list[index];
	WorldGenerator::GenWorld(map);
}

void Game::startWindowShake(float shake_timer, int shake_strength)
{
	shake_flag = true;
	this->shake_timer = shake_timer;
	this->shake_strength = shake_strength;
	this->origin_pos = view.getPosition();
}

void Game::setupPlaying()
{
	current_level = 0;
	player.hearts_left = 10;
	player.attack_points = 1;
	player.defense_points = 0;
	item = nullptr;
	loadNewMap(0);
	bg->play();
}

void Game::gameLoop()
{
	//if (fpsClock.getTicks() >= 1000) {
	//	std::cout << fps << "\n";
	//	fps = 0;
	//	fpsClock.reset();
	//	fpsClock.start();
	//}

	// Game Logic
	// Set player position on the map
   	player.sprite.transform().m_Position = core::Vector2f((player.position.x * TILE_SIZE)-player.sprite.transform().m_Size.x*1.25f, (player.position.y * TILE_SIZE)-player.sprite.transform().m_Size.y/1.8f);

	{
		// Camera position
		float dt = window.getDeltaTime();
		if (window.isKeyDown(Key::RIGHT))
			view.translate(Vector2f(-1, 0)*dt);
		else if (window.isKeyDown(Key::LEFT))
			view.translate(Vector2f(1, 0)*dt);
		else if (window.isKeyDown(Key::UP))
			view.translate(Vector2f(0, 1)*dt);
		else if (window.isKeyDown(Key::DOWN))
			view.translate(Vector2f(0, -1)*dt);
	}

	// Logic for the camera movement 
	{
		static Vector2d last_frame_mouse_pos;
		if (window.isMouseDown(Mouse::MOUSE_BUTTON_LEFT)) {
			float dx = last_frame_mouse_pos.x - window.getMousePosition().x;
			float dy = last_frame_mouse_pos.y - window.getMousePosition().y;
			view.translate(Vector2f(-dx, -dy));
		}
		last_frame_mouse_pos = window.getMousePosition();
	}

	// Window shake logic
	{
		if (shake_flag) {
			if (shake_timer > 0) {
				shake_timer -= 1 * window.getDeltaTime();
				float x = view.getPosition().x + (rand() % shake_strength) - (shake_strength / 2);
				float y = view.getPosition().y + (rand() % shake_strength) - (shake_strength / 2);
				view.setPosition(Vector2f(x, y));
			}
			if (shake_timer <= 0) {
				shake_flag = false;
				view.setPosition(origin_pos);
			}
		}
	}

	{
		renderer.begin();
		// Render
		renderer.submit(player.sprite);
		// Render the map tiles 
		for (auto const&x : map.prop_tile_list)
			renderer.submit(*x.second);
		for (int i = 0; i < map.renderable_tile_list.size(); i++)
			renderer.submit(*map.renderable_tile_list.at(i));
		renderer.flush(view);
	}

	{
		// UI Rendering
		static TextLabel text("10/10", *font, core::Vector2f(TILE_SIZE, window.getWindowSize().y - 20), core::Color::White());
		static TextLabel text2("1", *font, core::Vector2f(TILE_SIZE * 3.6f, window.getWindowSize().y - 20), core::Color::White());
		static TextLabel text3("0", *font, core::Vector2f(TILE_SIZE * 5.7f, window.getWindowSize().y - 20), core::Color::White());
		static TextLabel heart_item_text("F to heal!", *font, core::Vector2f(TILE_SIZE, window.getWindowSize().y - 75), core::Color::White());

		text.setPosition(core::Vector2f(TILE_SIZE, window.getWindowSize().y - 20));
		text2.setPosition(core::Vector2f(TILE_SIZE * 3.3f, window.getWindowSize().y - 20));
		text3.setPosition(core::Vector2f(TILE_SIZE * 5.5f, window.getWindowSize().y - 20));
		heart_item_text.setPosition(core::Vector2f(TILE_SIZE, window.getWindowSize().y - 75));
		event_text->setPosition(core::Vector2f(450, window.getWindowSize().y - 35));
		event_text_secondary->setPosition(core::Vector2f(450, window.getWindowSize().y - 5));

		std::string attackText;
		std::string shieldText;
		if (item != nullptr) {
			attackText = std::to_string(player.attack_points) + " + " + std::to_string(item->weapon_value);
			shieldText = std::to_string(player.defense_points) + " + " + std::to_string(item->armor_value);
		}
		else {
			attackText = std::to_string(player.attack_points);
			shieldText = std::to_string(player.defense_points);
		}

		std::string heartText = std::to_string(player.hearts_left) + "/10";
		text.setStringLiteral(heartText);
		text2.setStringLiteral(attackText);
		text3.setStringLiteral(shieldText);

		text.draw();
		text2.draw();
		text3.draw();
		event_text->draw();
		event_text_secondary->draw();
		if (item != nullptr) {
			if (item->type == ITEM_TYPE::HEALING) {
				heart_item_text.draw();
			}
		}

		static Renderable heart = Renderable("assets\\heart.png");
		heart.transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;
		heart.transform().m_Position = core::Vector2f(0, window.getWindowSize().y - TILE_SIZE);
		static Renderable heart_item = Renderable("assets\\heart.png");
		heart_item.transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;
		heart_item.transform().m_Position = core::Vector2f(0, window.getWindowSize().y - TILE_SIZE - 65);
		static Renderable sword = Renderable("assets\\sword.png");
		sword.transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;
		sword.transform().m_Position = core::Vector2f(TILE_SIZE * 2.25f, window.getWindowSize().y - TILE_SIZE);
		static Renderable armor = Renderable("assets\\shield.png");
		armor.transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;
		armor.transform().m_Position = core::Vector2f(TILE_SIZE * 4.4f, window.getWindowSize().y - TILE_SIZE);

		ui_layer.begin();
		ui_layer.submit(heart);
		ui_layer.submit(sword);
		ui_layer.submit(armor);
		if (item != nullptr) {
			if (item->type == ITEM_TYPE::HEALING) {
				ui_layer.submit(heart_item);
			}
		}
		ui_layer.flush();
	}

	//fps++;
}