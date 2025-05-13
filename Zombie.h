#pragma once

#include <graphics.h>

extern IMAGE imgZombie[5][5][30]; // 僵尸类型=》状态=》帧数

#define ZOMBIE_TYPE_0 0
#define ZOMBIE_TYPE_1 1
#define ZOMBIE_TYPE_2 2
#define ZOMBIE_TYPE_3 3
#define ZOMBIE_TYPE_4 4

#define ZOMBIE_TYPE_NUM 5

struct Zombie
{
	// 类型
	int type = ZOMBIE_TYPE_0;
	
	// 位置
	float x = 800;
	float y = 80;

	int row = 0;

	// 血量
	int hp;

	int stop = 0;
	// 移动速度
	float moveSpeed = 10;

	// 当前序列帧
	int curFrameindex = 0;
	int curMaxFrame = 0;

	Zombie* pPrev = NULL;
	Zombie* pNext = NULL;
};

void LoadZombieRes();
Zombie* CreateRandZombie();
void UpdateZombie(Zombie* pZombie);

IMAGE* GetZombieImage(Zombie* pZombie);