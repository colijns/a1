#include "EntityZombie.h"
#include "GameScene.h"
#include "DataDef.h"
#include "GameApp.h"
#include "HREasyxUtil.h"
#include "HRUtil.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h> //��������//  

IMAGE imgZombie[5][5][30]; // ��ʬ����=״̬=֡��

int zombieRow[] = { 0, 100, 200, 300, 400 };

void LoadZombieRes()
{
	const int bufferSize = 256;
	char szBuffer[bufferSize] = { 0 };

	char name[128] = { 0 };
	memset(gameParams.zombieFrameRecords, 0, sizeof(gameParams.zombieFrameRecords));

	for (int i = 0; i < 10; ++i) // ����
	{
		for (int j = 0; j < 10; ++j) // ״̬
		{
			sprintf_s(name, sizeof(name), "zm/zm_%d/zm_%d_%d/zm_%d_%d_0.gif", i + 1, i + 1, j + 1, i + 1, j + 1);
			if (GetImagePath(name, szBuffer, bufferSize)) //״̬��������֡
			{
				for (int frameIndex = 0; frameIndex < 100; ++frameIndex)
				{
					sprintf_s(name, sizeof(name), "zm/zm_%d/zm_%d_%d/zm_%d_%d_%d.gif", i + 1, i + 1, j + 1, i + 1, j + 1, frameIndex);
					if (GetImagePath(name, szBuffer, bufferSize))
					{
						gameParams.zombieFrameRecords[i][j]++;
					}
					else
					{
						break;
					}
				}
			}
		}
	}


	// ���ض�Ӧ��ͼ
	for (int i = 0; i < 10; ++i) // ����
	{
		for (int j = 0; j < 10; ++j) // ״̬
		{
			for (int frameIndex = 0; frameIndex < gameParams.zombieFrameRecords[i][j]; ++frameIndex)
			{
				sprintf_s(name, sizeof(name), "zm/zm_%d/zm_%d_%d/zm_%d_%d_%d.gif", i + 1, i + 1, j + 1, i + 1, j + 1, frameIndex);
				loadimage(&imgZombie[i][j][frameIndex], GetImagePath(name, szBuffer, bufferSize), 160, 160);
			}
		}
	}
}

Zombie* CreateRandZombie()
{
	struct Zombie* newEntity = (struct Zombie*)malloc(sizeof(struct Zombie));

	// �������
	newEntity->type = HRGetRandNum(ZOMBIE_TYPE_0, ZOMBIE_TYPE_NUM - 1);
	int randRow = HRGetRandNum(0, 4);
	newEntity->x = 800;
	newEntity->y = zombieRow[randRow];
	newEntity->row = randRow;

	// ��ʬѪ��
	newEntity->hp = 3 + newEntity->type * 3;
	newEntity->stop = 0;
	newEntity->moveSpeed = 0.05;
	
	newEntity->curFrameindex = 0;
	newEntity->curMaxFrame = gameParams.zombieFrameRecords[newEntity->type][0];

	newEntity->pPrev = NULL;
	newEntity->pNext = NULL;

	return newEntity;
}

void UpdateZombie(Zombie* pZombie)
{
	if (gAppParams.frameCnt % 20 == 0)
	{
		++pZombie->curFrameindex;
		if (pZombie->curFrameindex >= pZombie->curMaxFrame)
		{
			pZombie->curFrameindex = 0;
		}
	}

	if (pZombie->stop == 0)
	{
		pZombie->x -= pZombie->moveSpeed * gAppParams.frameInterval;
	}

	// �����ã�ÿ֡�ж� 
	pZombie->stop = 0;
}

IMAGE* GetZombieImage(Zombie* pZombie)
{
	return &imgZombie[pZombie->type][0][pZombie->curFrameindex];
}
