#pragma once

#include <graphics.h>

struct PlantBullet
{
	// ����
	int type = 0;
	
	// λ��
	float x = 0;
	float y = 0;

	int row = 0;

	float speed = 0.1f;

	PlantBullet* pPrev = NULL;
	PlantBullet* pNext = NULL;
};

void LoadBulletRes();
PlantBullet* CreateBullet(int type);

void UpdateBullet(PlantBullet* pBullet);

IMAGE* GetBulletImage(PlantBullet* pBullet);
