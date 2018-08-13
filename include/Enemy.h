#pragma once

#include<string>

class Enemy {
public:
	Enemy(std::string name, int hp, int damage) {
		this->hp = hp;
		this->damage = damage;
		this->name = name;
	}

	std::string name;
	int hp;
	int damage;
};