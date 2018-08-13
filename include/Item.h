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
	void setType(ITEM_TYPE type) {
		this->type = type;
		if (type == ITEM_TYPE::DAMAGE) {
			int rand_index = rand() % weapon_names.size();
			weapon random_weapon = weapon_names[rand_index];

			item_name = random_weapon.name;
			weapon_value = random_weapon.damage;
			armor_value = 0;
		}
		else if (type == ITEM_TYPE::DEFENSE) {
			int rand_index = rand() % armor_names.size();
			armor random_armor = armor_names[rand_index];

			item_name = random_armor.name;
			armor_value = random_armor.defense;
			weapon_value = 0;
		}
		else if (type == ITEM_TYPE::HEALING) {
			item_name = "Heart";
			weapon_value = 0;
			armor_value = 0;
		}
	}
};