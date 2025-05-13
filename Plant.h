#pragma once
#define PLANT_TYPE_5 5
#include <graphics.h>

extern IMAGE imgPlants[6][30]; // =֡

#define PLANT_TYPE_0 0
#define PLANT_TYPE_1 1
#define PLANT_TYPE_2 2
#define PLANT_TYPE_3 3
#define PLANT_TYPE_4 4
#define PLANT_TYPE_5 5

#define PLANT_TYPE_NUM 6

struct Plant
{
	// 
	int type = PLANT_TYPE_0;
	
	// λ��
	float x = 800;
	float y = 80;

	int row = 0;
	int col = 0;

	// Ѫ��
	int hp;

	int cost;

	// ��ǰ����֡
	int curFrameindex = 0;
	int curMaxFrame = 0;

	// �������
	int shootTimeInterval = 0;
	int shootTimeRemain = 0;

	Plant* pPrev = NULL;
	Plant* pNext = NULL;
};

void LoadPlantRes();
Plant* CreatePlant(int type);

void UpdatePlant(Plant* pPlant, int& createBullet);

IMAGE* GetPlantImage(Plant* pPlant);
IMAGE* GetSelectPlantImage(int cardIndex);