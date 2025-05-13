#pragma once

#include <graphics.h>

extern IMAGE imgZombie[5][5][30]; // ��ʬ����=��״̬=��֡��

#define ZOMBIE_TYPE_0 0
#define ZOMBIE_TYPE_1 1
#define ZOMBIE_TYPE_2 2
#define ZOMBIE_TYPE_3 3
#define ZOMBIE_TYPE_4 4

#define ZOMBIE_TYPE_NUM 5

struct Zombie
{
	// ����
	int type = ZOMBIE_TYPE_0;
	
	// λ��
	float x = 800;
	float y = 80;

	int row = 0;

	// Ѫ��
	int hp;

	int stop = 0;
	// �ƶ��ٶ�
	float moveSpeed = 10;

	// ��ǰ����֡
	int curFrameindex = 0;
	int curMaxFrame = 0;

	Zombie* pPrev = NULL;
	Zombie* pNext = NULL;
};

void LoadZombieRes();
Zombie* CreateRandZombie();
void UpdateZombie(Zombie* pZombie);

IMAGE* GetZombieImage(Zombie* pZombie);