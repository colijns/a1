#pragma once

#include <graphics.h>

struct Sunshine
{	
	// 位置
	float x = 0;
	float y = 0;

	float speed = 0.1f;
	int targetY = 0;

	// 当前序列帧
	int curFrameindex = 0;
	int curMaxFrame = 0;

	Sunshine* pPrev = NULL;
	Sunshine* pNext = NULL;
};

void LoadSunshieRes();
Sunshine* CreateRandSunshine();

void UpdateSunshine(Sunshine* pSunshine);

IMAGE* GetSunshineImage(Sunshine* pSunshine);
