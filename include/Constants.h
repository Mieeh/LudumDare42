#pragma once

#define GAME_TITLE "No Space"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 640

// 0 = walkable ground
// 1 = wall
// 8 = ground skirt
// 2 = chest
// 1+99 = weapon chest
// 2+99 = armor chest
// 3+99 = healing chest
// 10 = level win

// 3 = bat
// 4 = dog
// 5 = eyeball
// 6 = alien
// 7 = devil

// 9 = invicible wall

#define TILE_SIZE 16*4
#include<vector>

#define WC 1+99
#define AC 2+99					
#define HC 3+99

static std::vector<std::vector<int>> level1 = 
{
	{ 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 9, 0, 0, 0, 0, 0, WC, 0, 3, 4, 0, 0, 0, 9 },
	{ 9, 0, 0, 0, 0, 0, AC, HC, 3, 4, 0, 10, 0, 9 },
	{ 9, 0, 0, 0, 0, 0, WC, 0, 3, 0, 0, 4, 0, 9 },
	{ 9, 9, 8, 8, 9, 9, 8, 9, 9, 8, 9, 8, 9 }
};

static std::vector<std::vector<int>> level2 =
{
	{ 9, 1, 1, 1, 1, 1 },
	{ 9, 0, 0, 0, 0, HC, 9 },
	{ 9, 0, 0, 0, HC, WC, 9 },
	{ 9, 9, 8, 8, 0, 0, 9 },
	{ 9, 9, 9, 9, 0, WC, 9 },
	{ 9, 9, 9, 9, 0, 0, 9 },
	{ 9, 9, 1, 1, 0, 4, 9 },
	{ 9, 9, 0, 0, 0, 0, 1, 1, 1, 1 },
	{ 9, 9, WC, 0, 0, 0, 5, 0, 10, 0, 9 },
	{ 9, 9, 8, 8, 9, 9, 9, 9, 9, 9, 9 }
};

static std::vector<std::vector<int>> level3 =
{
	{ 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9 },
	{ 9, 0, 0, 0, 3, HC, 0, 0, 7, 10, 9 },
	{ 9, 0, 0, 0, 3, 3, 0, WC, 0, 0, 9 },
	{ 9, 8, 9, 8, 8, 9, 9, 8, 8, 8, 9 }
};

static std::vector<std::vector<int>> level4 = 
{
	{ 9, 1, 1, 1, 1, 1, 1, 9 },
	{ 9, 0, 0, 0, 0, 0, HC, 9 },
	{ 9, 0, 0, 0, 0, 0, HC, 9 },
	{ 9, 0, 0, WC, 0, 0, 3, 9 },
	{ 9, 4, 0, 0, 0, HC, 6, 9 },
	{ 9, WC, 4, 0, 3, 6, 10, 9 },
	{ 9, 8, 8, 9, 8, 8, 8, 9 }
};

static std::vector<std::vector<int>> level5 = 
{
	{ 9, 1, 1, 1, 1, 1, 1, 9 },
	{ 9, 0, WC, 0, 0, 0, 3, 9 },
	{ 9, AC, HC, 0, 0, 4, 0, 9 },
	{ 9, HC, HC, 9, 9, WC, 3, 9 },
	{ 9, HC, HC, 9, 9, 0, 0, 9 },
	{ 9, 0, 3, WC, 0, 0, 7, 9 },
	{ 9, 4, 0, 0, 0, 7, 10, 9 },
	{ 9, 8, 8, 8, 8, 8, 8, 9 }
};

static std::vector<std::vector<int>> level_list[] = { level1, level2, level3, level4, level5 };

struct weapon { std::string name; int damage; };
static std::vector<weapon> weapon_names =
{
	{ "Stick", 1 },
	{ "Axe", 2 },
	{ "Steel Sword", 3},
	{ "Chainsaw", 4 },
	{ "9mm", 5 },
	{ "Beam Rifle", 7},
	{ "Phaser", 10 },
	{ "B53", 15 },
	{ "God's Hand", 20 }
};

struct armor { std::string name; int defense; }; // defense = % chance of not taking any damage when hit
static std::vector<armor> armor_names = 
{
	{ "White Shirt", 10 },
	{ "Sweater", 15 },
	{ "Combat Armor", 20 },
	{ "Mark I", 30 },
	{ "God's T-Shirt", 90 }
};
					  
namespace enemy_state 
{

static int bat_hp = 3;
static int bat_dmg = 1;
	   
static int dog_hp = 5;
static int dog_dmg = 1;
	   
static int eyeball_hp = 9;
static int eyeball_dmg = 2;

static int alien_hp = 20;
static int alien_dmg = 2;

static int devil_hp = 30;
static int devil_dmg = 2;

}

// Sound
#define SFX_COUNT 8
#define LEVEL_WIN 0
#define ITEM_PICKUP 1
#define GAME_OVER 2
#define GAME_START 3
#define ENEMY_DIE 4
#define PLAYER_DIE 5
#define HEALED 6
#define ATTACK 7