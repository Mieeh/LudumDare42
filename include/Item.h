#pragma once

#include<random>
#include<cmath>

#include"Constants.h"

/*
* The one item, player can only hold 1 at the time due to "running out of space"(LDJAM Theme)
* Items use case is based on its type
*/

enum ITEM_TYPE {
	DAMAGE = 0, // weapon of some sorts
	DEFENSE = 1, // some sorts of armor
	HEALING = 2 // healing potion of a varying degree
};

struct Item {
	// Members
	ITEM_TYPE type;
	std::string item_name;
	int weapon_value;
	int armor_value;

	const int max_weapon_attack = 10;
	const int max_defense_points = 5-1;

	// Methods
	void setTypeRandom() {
		int x = rand() % 3;
		type = static_cast<ITEM_TYPE>(x);
		if (type == ITEM_TYPE::DAMAGE) {
			int rand_index = rand() % weapon_names.size();
			item_name = weapon_names[rand_index];
			weapon_value = (rand() % max_weapon_attack) + 1;
			armor_value = 0;
		}
		else if (type == ITEM_TYPE::DEFENSE) {
			int rand_index = rand() % armor_names.size();
			item_name = armor_names[rand_index];
			armor_value = (rand() % max_defense_points) + 1;
			weapon_value = 0;
		}
		else if (type == ITEM_TYPE::HEALING) {
			item_name = "Heart";
		}
	}
};