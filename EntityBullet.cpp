#include "EntityBullet.h"
#include "GameScene.h"
#include "DataDef.h"
#include "GameApp.h"
#include "HREasyxUtil.h"
#include "HRUtil.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h>

IMAGE imgBullet[5]; // ¿‡–Õ

void LoadBulletRes()
{
	const int bufferSize = 256;
	char szBuffer[bufferSize] = { 0 };

	char name[128] = { 0 };
	for (int i = 0; i < 5; ++i)
	{
		sprintf_s(name, sizeof(name), "bullet/shot_%d.png", i + 1);
		loadimage(&imgBullet[i], GetImagePath(name, szBuffer, bufferSize));
	}
}

PlantBullet* CreateBullet(int type)
{
	struct PlantBullet* newEntity = (struct PlantBullet*)malloc(sizeof(struct PlantBullet));
	newEntity->type = type;
	newEntity->speed = 0.1;
	newEntity->pPrev = NULL;
	newEntity->pNext = NULL;

	return newEntity;
}

void UpdateBullet(PlantBullet* pBullet)
{
	pBullet->x += pBullet->speed * gAppParams.frameInterval;
}

IMAGE* GetBulletImage(PlantBullet* pBullet)
{
	return &imgBullet[pBullet->type];
}

