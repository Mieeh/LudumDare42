#pragma once

#include<core\vector2.h>
using namespace bear::core;

#include<graphics\graphics.h>
using namespace bear::graphics;

#include"Constants.h"

struct Player {
	Player() : position(2, 2), sprite("assets\\player.png")
	{
		sprite.transform().m_Size = Vector2f(0.75f, 1.5f)*TILE_SIZE;
	}

	// members
	int hearts_left = 10;
	int attack_points = 1;
	int defense_points = 0;
	// Bear lib members
	Vector2i position;
	Renderable sprite;
};