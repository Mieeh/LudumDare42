#pragma once

class Enemy {
public:
	Enemy(int hp, int damage) {
		this->hp = hp;
		this->damage = damage;
	}

	int hp;
	int damage;
};