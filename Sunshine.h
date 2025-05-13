#pragma once

#include <graphics.h>

struct Sunshine
{	
	// λ��
	float x = 0;
	float y = 0;

	float speed = 0.1f;
	int targetY = 0;

	// ��ǰ����֡
	int curFrameindex = 0;
	int curMaxFrame = 0;

	Sunshine* pPrev = NULL;
	Sunshine* pNext = NULL;
};

void LoadSunshieRes();
Sunshine* CreateRandSunshine();

void UpdateSunshine(Sunshine* pSunshine);

IMAGE* GetSunshineImage(Sunshine* pSunshine);
