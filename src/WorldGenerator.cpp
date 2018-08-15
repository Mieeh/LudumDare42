#include"../include/WorldGenerator.h"

#include"../include/Util.h"
#include<core\vector2.h>
using namespace bear;

void WorldGenerator::GenWorld(map_struct & map)
{
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
			if (tileValue == 8) {
				map.renderable_tile_list.push_back(new Renderable("assets\\tjol.png"));
				placed = true;
			}
			if (tileValue == 10) {
				map.renderable_tile_list.push_back(new Renderable("assets\\ladder.png"));
				placed = true;
			}
			if (tileValue == 1 + 99 || tileValue == 2 + 99 || tileValue == 3 + 99) {
				// Chest insertion here please sir
				std::string chest_image_path;
				if (tileValue == 1 + 99) chest_image_path = "assets\\chest_weapon.png";
				else if (tileValue == 2 + 99) chest_image_path = "assets\\chest_shield.png";
				else if (tileValue == 3 + 99) chest_image_path = "assets\\chest_heart.png";
				std::string k = positionToStringKey(Vector2i(x, y));
				map.prop_tile_list.insert(std::pair<std::string, Renderable*>(k, new Renderable(chest_image_path)));
				map.prop_tile_list[k]->transform().m_Position = core::Vector2f(x, y)*TILE_SIZE;
				map.prop_tile_list[k]->transform().m_Size = core::Vector2f(1, 1)*TILE_SIZE;

				map.renderable_tile_list.push_back(new Renderable("assets\\ground.png"));
				placed = true;
			}
			if (tileValue == 3) { // bat
				std::string k = positionToStringKey(Vector2i(x, y));

				// Enemy registered to the map position k
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy("Bat", enemy_state::bat_hp, enemy_state::bat_dmg)));

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
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy("Dog", enemy_state::dog_hp, enemy_state::dog_dmg)));

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
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy("Eyeball", enemy_state::eyeball_hp, enemy_state::eyeball_dmg)));

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
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy("Alien", enemy_state::alien_hp, enemy_state::alien_dmg)));

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
				map.enemy_tile_list.insert(std::pair<std::string, Enemy>(k, Enemy("Devil", enemy_state::devil_hp, enemy_state::devil_dmg)));

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