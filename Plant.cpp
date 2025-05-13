#include "Plant.h"
#include "GameScene.h"
#include "Data.h"
#include "GameApp.h"
#include "HREasyxUtil.h"
#include "HRUtil.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h>

IMAGE imgPlants[6][30]; // 植物动画帧
IMAGE imgPlantSelect[6]; // 选择时图片

int plantRows[] = { 0, 100, 150, 250, 350 };

void LoadPlantRes()
{
	const int bufferSize = 256;
	char szBuffer[bufferSize] = { 0 };

	char name[256] = { 0 };  // 增加name缓冲区大小以防止溢出
	memset(gameParams.plantFrameRecords, 0, sizeof(gameParams.plantFrameRecords));

	printf("Loading plant resources for %d plants\n", 6);
	
	for (int i = 0; i < 6; ++i) // 修改上限为6
	{
		// 樱桃和辣椒使用png
		if (i == 4 || i == 5)
		{
			snprintf(name, sizeof(name), "plant/plant_%d/plant_%d (1).png", i + 1, i + 1);
			printf("Loading plant type %d from: %s\n", i, name);
		}
		else
		{
			snprintf(name, sizeof(name), "plant/plant_%d/plant_%d (1).gif", i + 1, i + 1);
			printf("Loading plant type %d from: %s\n", i, name);
		}
		if (GetImagePath(name, szBuffer, bufferSize)) // 状态帧
		{
			for (int frameIndex = 0; frameIndex < 100; ++frameIndex)
			{
				if (i == 4 || i == 5)
				{
					snprintf(name, sizeof(name), "plant/plant_%d/plant_%d (%d).png", i + 1, i + 1, frameIndex + 1);
				}
				else
				{
					snprintf(name, sizeof(name), "plant/plant_%d/plant_%d (%d).gif", i + 1, i + 1, frameIndex + 1);
				}
				if (GetImagePath(name, szBuffer, bufferSize))
				{
					gameParams.plantFrameRecords[i]++;
				}
				else
				{
					break;
				}
			}
		}
		printf("Loaded %d frames for plant type %d\n", gameParams.plantFrameRecords[i], i);
	}

	// 加载对应图像
	for (int i = 0; i < 6; ++i) // 修改上限为6
	{
		for (int frameIndex = 0; frameIndex < gameParams.plantFrameRecords[i]; ++frameIndex)
		{
			if (i == 4 || i == 5)
			{
				snprintf(name, sizeof(name), "plant/plant_%d/plant_%d (%d).png", i + 1, i + 1, frameIndex + 1);
			}
			else
			{
				snprintf(name, sizeof(name), "plant/plant_%d/plant_%d (%d).gif", i + 1, i + 1, frameIndex + 1);
			}
			loadimage(&imgPlants[i][frameIndex], GetImagePath(name, szBuffer, bufferSize), 67, 78);
		}
	}

	// Load selection images with consistent size to prevent UI issues
	for (int i = 0; i < 6; ++i) // 修改上限为6
	{
		snprintf(name, sizeof(name), "plant/plant_%d/plant_%d.png", i + 1, i + 1);
		printf("Loading selection image for plant type %d: %s\n", i, name);
		
		// 确保所有植物选择图像尺寸一致
		if (i == 5) {
			// 辣椒植物现在使用与其他植物相同的尺寸
			loadimage(&imgPlantSelect[i], GetImagePath(name, szBuffer, bufferSize), 70, 70);
		} else {
			// 其他植物使用标准尺寸
			loadimage(&imgPlantSelect[i], GetImagePath(name, szBuffer, bufferSize), 70, 70);
		}
	}
}

Plant* CreatePlant(int type)
{
	struct Plant* newEntity = (struct Plant*)malloc(sizeof(struct Plant));
	if (!newEntity) {
		return NULL;
	}

	// 初始化属性
	newEntity->type = type;
	newEntity->curFrameindex = 0;
	newEntity->curMaxFrame = gameParams.plantFrameRecords[type];
	newEntity->shootTimeInterval = 0;
	newEntity->shootTimeRemain = 0;

	if (newEntity->type == PLANT_TYPE_0)
	{
		newEntity->hp = 10;
		newEntity->shootTimeInterval = 5000;
		newEntity->shootTimeRemain = newEntity->shootTimeInterval;
		newEntity->cost = 50;
	}
	else if (newEntity->type == PLANT_TYPE_1
		|| newEntity->type == PLANT_TYPE_3)
	{
		newEntity->hp = 10;
		newEntity->shootTimeInterval = 2000;
		newEntity->shootTimeRemain = newEntity->shootTimeInterval;
		if (newEntity->type == PLANT_TYPE_1)
			newEntity->cost = 100;
		else if (newEntity->type == PLANT_TYPE_3)
			newEntity->cost = 175;
	}
	else if (newEntity->type == PLANT_TYPE_2)
	{
		newEntity->hp = 1000;
		newEntity->cost = 50;
	}
	else if (newEntity->type == PLANT_TYPE_4)
	{
		newEntity->hp = 1000000;
		newEntity->cost = 150;
	}
	else if (newEntity->type == PLANT_TYPE_5)
	{
		// 辣椒是一种爆炸性植物，放置后会在一段时间后爆炸并消失
		// 爆炸会杀死周围的僵尸
		newEntity->hp = 500;
		// 辣椒的爆炸延迟时间
		newEntity->shootTimeInterval = 6000;  // 6秒后爆炸
		newEntity->shootTimeRemain = newEntity->shootTimeInterval;
		newEntity->cost = 125;
	}
	newEntity->pPrev = NULL;
	newEntity->pNext = NULL;

	return newEntity;
}

void UpdatePlant(Plant* pPlant, int& createBullet)
{
	// 更新动画帧
	if (gAppParams.frameCnt % 10 == 0)
	{
		++pPlant->curFrameindex;
		if (pPlant->curFrameindex >= pPlant->curMaxFrame)
		{
			if (pPlant->type == PLANT_TYPE_4 || pPlant->type == PLANT_TYPE_5)
			{
				createBullet = 1;
				return;
			}
			else
			{
				pPlant->curFrameindex = 0;
			}
		}
	}

	// 更新攻击状态
	createBullet = 0;
	if (pPlant->shootTimeInterval > 0)
	{
		pPlant->shootTimeRemain -= gAppParams.frameInterval;
		
		// 对于辣椒，当剩余时间少于2秒时提示即将爆炸
		if (pPlant->type == PLANT_TYPE_5 && pPlant->shootTimeRemain < 2000) {
			// 每0.2秒提示一次
			if (gAppParams.frameCnt % 12 == 0) {
				printf("警告：辣椒即将爆炸！\n");
			}
		}
		
		if (pPlant->shootTimeRemain <= 0)
		{
			createBullet = 1;
			pPlant->shootTimeRemain = pPlant->shootTimeInterval;
		}
	}
}

IMAGE* GetPlantImage(Plant* pPlant)
{
	return &imgPlants[pPlant->type][pPlant->curFrameindex];
}

IMAGE* GetSelectPlantImage(int cardIndex)
{
	// Make sure the index is within bounds
	if (cardIndex < 0 || cardIndex >= PLANT_TYPE_NUM) {
		printf("Error: Invalid plant card index %d\n", cardIndex);
		// Return first plant as fallback
		return &imgPlantSelect[0];
	}
	return &imgPlantSelect[cardIndex];
}
