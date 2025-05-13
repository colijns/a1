#include "Sunshine.h"
#include "GameScene.h"
#include "Data.h"
#include "GameApp.h"
#include "HREasyxUtil.h"
#include "HRUtil.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h>

IMAGE imgSunshine[29]; // ¿‡–Õ

void LoadSunshieRes()
{
	const int bufferSize = 256;
	char szBuffer[bufferSize] = { 0 };

	char name[128] = { 0 };
	for (int i = 0; i < 29; ++i)
	{
		sprintf_s(name, sizeof(name), "sunshine/%d.jpg", i);
		loadimage(&imgSunshine[i], GetImagePath(name, szBuffer, bufferSize));
	}
}

Sunshine* CreateRandSunshine()
{
	struct Sunshine* newEntity = (struct Sunshine*)malloc(sizeof(struct Sunshine));
	newEntity->speed = 0.1;
	newEntity->x = HRGetRandNum(100, WINDOWS_WIDTH - 200);
	newEntity->y = 0;
	newEntity->targetY = 500;
	newEntity->curFrameindex = 0;
	newEntity->curMaxFrame = 29;
	newEntity->pPrev = NULL;
	newEntity->pNext = NULL;

	return newEntity;
}

void UpdateSunshine(Sunshine* pSunshine)
{
	if (gAppParams.frameCnt % 10 == 0)
	{
		++pSunshine->curFrameindex;
		if (pSunshine->curFrameindex >= pSunshine->curMaxFrame)
		{
			pSunshine->curFrameindex = 0;
		}
	}
	pSunshine->y += pSunshine->speed * gAppParams.frameInterval;
	if (pSunshine->y >= pSunshine->targetY)
	{
		pSunshine->y = pSunshine->targetY;
	}
}

IMAGE* GetSunshineImage(Sunshine* pSunshine)
{
	return &imgSunshine[pSunshine->curFrameindex];
}

