#include"../include/Game.h"

#include<core\vector2.h>

using namespace bear;

#include<time.h>

Game::Game() :
	window(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE), // Create window
	player()
{
	srand(time(NULL));

	// Init graphics
	if (!Graphics::init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
		std::cout << "Failed to init Grap hics module\n";
	}

	renderer.init();
	ui_layer.init();

	font = new Font("assets\\kirsty.ttf", 30);
	event_text = new TextLabel("Nothing here yet!", *font, core::Vector2f(0, 50), core::Color::White());
	event_text_secondary = new TextLabel("Nothing here yet!", *font, core::Vector2f(0, 0), core::Color::White());

	switchGameState(GAME_STATE::PLAYING); // setup for default game state
	mainLoop();
}
				
Game::~Game()
{
	delete event_text;
	delete font;
	Graphics::exit();
}

void Game::switchGameState(GAME_STATE new_state)
{
	game_state = new_state;
	switch (game_state) {
	case GAME_STATE::PLAYING:
		setupPlaying();
		break;
	}
}

void Game::mainLoop()
{
	while (window.isOpen()) {
		window.clear(clear_color);

		for (bear::Event event : window.getRegisteredEvents())
			eventInput(event);

		// State machine for what state the game is in and call the appropiate methods
		switch (game_state) {
		case GAME_STATE::PLAYING:
			gameLoop();
		}

		window.display();
	}
}

void Game::eventInput(bear::Event &event)
{
	if (event.type == EventType::WindowReiszed) {
		Graphics::window_resize_callback(event.size.x, event.size.y);
	}

	if (game_state == GAME_STATE::PLAYING) {
		// Process player move input
		if (event.type == EventType::KeyPressed) {
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
						}
						event_text->setStringLiteral("Used heal item");
					}
				}
			}
			
			newTileValue = map.tile_list.at(newPos.y).at(newPos.x-1);

			// Logic for after we have the new positon & tile value
			if (newTileValue == 0) { // New tile is a ground tile so we can move onto it with no problem
				player.position = newPos;
			}
			else if (newTileValue == 2) { // Chest
				// Handle chest logic
				// Chest is at position newPos in the map list
				bool chest_was_removed = removeFromPropTileList(Vector2i(newPos.x - 1, newPos.y));
				if(!chest_was_removed) // Chest wasn't on the tile let the player freely move to the tile
					player.position = newPos; 
				else {
					// Give the player the item found inside the chest
					delete item; // remove current item
					item = new Item();
					item->setTypeRandom();
					event_text->setStringLiteral("Found a " + item->item_name);
					//event_text_secondary->setStringLiteral(std::string())
				}
			}
			else if (newTileValue == 3 || newTileValue == 4 || newTileValue == 5 || newTileValue == 6 || newTileValue == 7) {
				std::string k = positionToStringKey(core::Vector2i(newPos.x - 1, newPos.y));
				Enemy& enemy = map.enemy_tile_list.at(k);
				enemy.hp--;
				std::cout << enemy.hp << std::endl;
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

void Game::setupPlaying()
{
	std::cout << "setupPlaying() executed\n";

	map.renderable_tile_list.clear();
	map.tile_list = level1;
	for (int y = 0; y < map.tile_list.size(); y++) {
		for (int x = 0; x < map.tile_list.at(y).size(); ++x) {
			int tileValue = map.tile_list.at(y).at(x);
			bool placed = false;

			if (tileValue == 0) {
				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}
			if (tileValue == 1) {
				map.renderable_tile_list.push_back(new Renderable("assets\\wall.png"));
				placed = true;
			}
			if (tileValue == 2) {
				// Chest insertion here please sir
				std::string k = positionToStringKey(Vector2i(x, y));
				map.prop_tile_list.insert(std::pair<std::string, Renderable*>(k, new Renderable("assets\\chest_closed.png")));
				map.prop_tile_list[k]->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.prop_tile_list[k]->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;

				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}
			if (tileValue == 3) { // bat
				std::string k = positionToStringKey(Vector2i(x, y));
				
				// Enemy registered to the map position k
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy(enemy_state::bat_hp, enemy_state::bat_dmg)));

				// Placing the bat as a prop
				map.prop_tile_list.insert(std::pair<std::string, Renderable*>(k, new Renderable("assets\\bat.png")));
				map.prop_tile_list[k]->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.prop_tile_list[k]->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;

				// Placing the tile
				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}
			if (tileValue == 4) { // dog
				// Placing the dog as a prop
				std::string k = positionToStringKey(Vector2i(x, y));

				// Enemy registered to the map position k
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy(enemy_state::dog_hp, enemy_state::dog_dmg)));

				map.prop_tile_list.insert(std::pair<std::string, Renderable*>(k, new Renderable("assets\\dog.png")));
				map.prop_tile_list[k]->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.prop_tile_list[k]->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;

				// Placing the tile
				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}
			if (tileValue == 5) { // eyeball
				// Placing the eyeball as a prop
				std::string k = positionToStringKey(Vector2i(x, y));

				// Enemy registered to the map position k
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy(enemy_state::eyeball_hp, enemy_state::eyeball_dmg)));

				map.prop_tile_list.insert(std::pair<std::string, Renderable*>(k, new Renderable("assets\\eyeball.png")));
				map.prop_tile_list[k]->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.prop_tile_list[k]->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;

				// Placing the tile
				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}
			if (tileValue == 6) { // alien
				// Placing the alien as a prop
				std::string k = positionToStringKey(Vector2i(x, y));

				// Enemy registered to the map position k
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy(enemy_state::alien_hp, enemy_state::alien_dmg)));

				map.prop_tile_list.insert(std::pair<std::string, Renderable*>(k, new Renderable("assets\\alien.png")));
				map.prop_tile_list[k]->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.prop_tile_list[k]->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;

				// Placing the tile
				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}
			if (tileValue == 7) { // devil
				// Placing the devil as a prop
				std::string k = positionToStringKey(Vector2i(x, y));

				// Enemy registered to the map position k
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy(enemy_state::devil_hp, enemy_state::devil_dmg)));

				map.prop_tile_list.insert(std::pair<std::string, Renderable*>(k, new Renderable("assets\\devil.png")));
				map.prop_tile_list[k]->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.prop_tile_list[k]->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;

				// Placing the tile
				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}

			if (placed) {
				map.renderable_tile_list.back()->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.renderable_tile_list.back()->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;
			}
		}
	}
}

void Game::gameLoop()
{
	// Game Logic
	// Set player position on the map
   	player.sprite.transform().m_Position = core::Vector2f((player.position.x * TILE_SIZE)-player.sprite.transform().m_Size.x*1.25f, (player.position.y * TILE_SIZE)-player.sprite.transform().m_Size.y/1.8f);

	{
		// UI Rendering
		static TextLabel text("10/10", *font, core::Vector2f(TILE_SIZE, window.getWindowSize().y - 20), core::Color::White());
		static TextLabel text2("1", *font, core::Vector2f(TILE_SIZE * 3.6f, window.getWindowSize().y - 20), core::Color::White());
		static TextLabel text3("0", *font, core::Vector2f(TILE_SIZE * 5.2f, window.getWindowSize().y - 20), core::Color::White());
		static TextLabel heart_item_text("F to heal!", *font, core::Vector2f(TILE_SIZE, window.getWindowSize().y - 75), core::Color::White());

		text.setPosition(core::Vector2f(TILE_SIZE, window.getWindowSize().y - 20));
		text2.setPosition(core::Vector2f(TILE_SIZE * 3.6f, window.getWindowSize().y - 20));
		text3.setPosition(core::Vector2f(TILE_SIZE * 5.2f, window.getWindowSize().y - 20));
		heart_item_text.setPosition(core::Vector2f(TILE_SIZE, window.getWindowSize().y - 75));
		event_text->setPosition(core::Vector2f(385, window.getWindowSize().y - 35));
		event_text_secondary->setPosition(core::Vector2f(385, window.getWindowSize().y - 5));

		std::string attackText;
		std::string shieldText;
		if (item != nullptr) {
			attackText = std::to_string(player.attack_points + item->weapon_value);
			shieldText = std::to_string(player.defense_points + item->armor_value);
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
		sword.transform().m_Position = core::Vector2f(TILE_SIZE * 2.5f, window.getWindowSize().y - TILE_SIZE);
		static Renderable armor = Renderable("assets\\shield.png");
		armor.transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;
		armor.transform().m_Position = core::Vector2f(TILE_SIZE * 4, window.getWindowSize().y - TILE_SIZE);

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

	{
		view.setPosition(core::Vector2f(player.sprite.transform().m_Position.x-250, player.sprite.transform().m_Position.y-175) * core::Vector2f(-1,-1));

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


}