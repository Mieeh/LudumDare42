#pragma once

#define GAME_TITLE "No Space"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 640

// 0 = walkable ground
// 1 = wall
// 2 = chest

// 3 = bat
// 4 = dog
// 5 = eyeball
// 6 = alien
// 7 = devil

// 9 = invicible wall

#define TILE_SIZE 16*4
#include<vector>

static std::vector<std::vector<int>> level1 = 
{
	{ 9, 1, 1, 1, 1, 1, 1, 9 },
	{ 9, 0, 0, 0, 4, 0, 0, 1, 1, 1, 1, 1 },
	{ 9, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 3, 9 },
	{ 9, 0, 0, 0, 0, 4, 0, 0, 2, 2, 2, 2, 9 },
	{ 9, 0, 0, 0, 4, 0, 0, 0, 2, 0, 0, 3, 9 },
	{ 9, 0, 4, 0, 0, 0, 0, 9, 9, 9, 9, 9 },
	{ 9, 9, 9, 9, 9, 9, 9, 9 }
};

static std::vector<std::string> weapon_names =
{
	"Just a Stick",
	"Heavy Stone",
	"Play Sword",
	"Dull Sword",
	"Iron Sword",
	"Sabertooth",
	"Chainsaw",
	"Musket",
	"Switchblade",
	"Boxing Gloves",
	"9mm",
	"M4 Rifle",
	"870 Shooter",
	"Beam Rifle",
	"Particle Destroyer",
	"Strong Force Destroyer",
	"Universe Incinerator"
};

static std::vector<std::string> armor_names = 
{
	"Cover Leaf",
	"T-Shirt",
	"Wife Beater",
	"Power Armor"
};

namespace enemy_state 
{

static int bat_hp = 3;
static int bat_dmg = 1;
	   
static int dog_hp = 5;
static int dog_dmg = 2;
	   
static int eyeball_hp = 10;
static int eyeball_dmg = 2;

static int alien_hp = 20;
static int alien_dmg = 3;

static int devil_hp = 30;
static int devil_dmg = 3;

}