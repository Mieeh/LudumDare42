#pragma once

#include"Player.h"
#include"Item.h"
#include"Enemy.h"

#include<random>

struct combat_result {
	combat_result(){ }

	int new_player_health;
	int new_enemy_health;
};

class Combat {
public:
	static combat_result performCombtWithEnemy(Player& player, Enemy& enemy, Item* item) {
		int player_dmg = item != nullptr ? player.attack_points + item->weapon_value : player.attack_points;
		int enemy_dmg = enemy.damage;

		int num = rand() % 100;
		if (item != nullptr) {
			if (item->type == ITEM_TYPE::DEFENSE) {
				if (num < item->armor_value) {
					enemy_dmg = 0;
				}
			}
		}

		combat_result result;
		result.new_player_health = player.hearts_left - enemy_dmg;
		result.new_enemy_health = enemy.hp - player_dmg;
		return result;
	}
};