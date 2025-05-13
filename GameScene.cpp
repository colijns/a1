#include "GameScene.h"
#include "Data.h"
#include "GameApp.h"
#include "Zombie.h"
#include "Plant.h"
#include "Bullet.h"
#include "Sunshine.h"
#include "HRUtil.h"
#include "HREasyxUtil.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h> 
#include <chrono>
#include <ctime>

#define CREATE_ZOMBIE_TIME 3000
#define CREATE_SUNSHINE_TIME 5000

IMAGE imgGameBg;
IMAGE imgTopKuang;
IMAGE imgJiangshitupian;

IMAGE imgPeaShooterCard;
IMAGE imgSunFlowerCard;
IMAGE imgWallNutCard;
IMAGE imgSnowShooterCard;
IMAGE imgCherryBombCard;
IMAGE imgJalapenoCard;

const int startCardX = 80;
const int cardY = 8;

Zombie* gameZombie = NULL;
Plant* gamePlant = NULL;
PlantBullet* gameBullet = NULL;
Sunshine* gameSunshine = NULL;

int groundGrid[5][9] = { 0 };

GameParams gameParams;

void LoadGameRes();
void ReleaseEntity();
void InitEntity();

void LoadGameRes()
{
	const int bufferSize = 256;
	char szBuffer[bufferSize] = { 0 };

	loadimage(&imgGameBg, GetImagePath("game_bg.jpg", szBuffer, bufferSize));
	loadimage(&imgTopKuang, GetImagePath("kuang.png", szBuffer, bufferSize));
	loadimage(&imgJiangshitupian, GetImagePath("ZombiesRemaining.jpg", szBuffer, bufferSize));

	loadimage(&imgPeaShooterCard, GetImagePath("Card/PeaShooterCard.jpg", szBuffer, bufferSize), 50, 70);
	loadimage(&imgSunFlowerCard, GetImagePath("Card/SunFlowerCard.jpg", szBuffer, bufferSize), 50, 70);
	loadimage(&imgWallNutCard, GetImagePath("Card/wallNutCard.png", szBuffer, bufferSize), 50, 70);
	loadimage(&imgSnowShooterCard, GetImagePath("Card/snowShooterCard.png", szBuffer, bufferSize), 50, 70);
	loadimage(&imgCherryBombCard, GetImagePath("Card/CherryBomb.png", szBuffer, bufferSize), 50, 70);
	
	loadimage(&imgJalapenoCard, GetImagePath("Card/jalapeno.png", szBuffer, bufferSize), 50, 70);

	LoadZombieRes();
	LoadPlantRes();
	LoadBulletRes();
	LoadSunshieRes();
}

int GetEntityCount(int entity)
{
	int count = 0;

	Zombie* pCurEntity = gameZombie;
	while (pCurEntity != NULL)
	{
		++count;
		pCurEntity = pCurEntity->pNext;
	}

	return count;
}

void ReleaseEntity()
{
	if (gameZombie != NULL)
	{
		Zombie* pNext = gameZombie->pNext;
		free(gameZombie);
		while (true)
		{
			if (pNext)
			{
				Zombie* pCur = pNext;
				pNext = pNext->pNext;
				free(pCur);
			}
			else
			{
				break;
			}
		}
		gameZombie = NULL;
	}
}

float GetSpeed(int radius)
{
	if (0 <= radius && radius < 20)
	{
		return 0.1;
	}
	else if (radius < 30)
	{
		return 0.12;
	}
	else if (radius < 40)
	{
		return 0.13;
	}
	else
	{
		return 0.14;
	}
}

COLORREF GetFillColor(int radius)
{
	if (0 <= radius && radius < 20)
	{
		return RGB(0, 0, 255);
	}
	else if (radius < 30)
	{
		return RGB(0, 255, 0);
	}
	else if (radius < 40)
	{
		return RGB(255, 255, 0);
	}
	else
	{
		return RGB(255, 165, 0);
	}
}

void InitEntity()
{
	ReleaseEntity();
}

int InitGameScene()
{
	LoadGameRes();
	InitEntity();


	return 1;
}

void InitGameInfo()
{
	gameParams.sunshine = 500;
	gameParams.maxHp = 15;
	gameParams.killZombieNum = 0;
}

int UpdateGameScene()
{
	if (gAppParams.needInitGame == 1)
	{
		gAppParams.needInitGame = 0;
		InitGameInfo();

		gAppParams.level += 1;
		gAppParams.gameOver = 0;
	}

	if (gAppParams.suspendGame == 1)
		return 1;

	for (size_t i = 0; i < gAppParams.exMessageSize; ++i)
	{
		if (gAppParams.exMessages[i].message == WM_LBUTTONDOWN)
		{
			int clickX = gAppParams.exMessages[i].x;
			int clickY = gAppParams.exMessages[i].y;

			for (int cardIndex = 0; cardIndex < PLANT_TYPE_NUM; ++cardIndex)
			{
				if (cardIndex == 5) {
					if ((startCardX + cardIndex * 50 - 10 <= clickX && clickX <= startCardX + cardIndex * 50 - 10 + 50)
						&& (cardY <= clickY && clickY <= cardY + 70))
					{
						Plant* tempPlant = CreatePlant(cardIndex);
						if (tempPlant) {
							int cost = tempPlant->cost;
							free(tempPlant);
							
							if (gameParams.sunshine >= cost) {
								gameParams.lbtnDown = 1;
								gameParams.selectCard = cardIndex;
								printf("select card %d (cost: %d, sunshine: %d)\n", cardIndex, cost, gameParams.sunshine);
							} else {
								printf("Not enough sunshine for card %d (cost: %d, sunshine: %d)\n", cardIndex, cost, gameParams.sunshine);
							}
						}
						break;
					}
				} else {
					// 其他卡片使用标准点击检测区域
					if ((startCardX + cardIndex * 50 <= clickX && clickX <= startCardX + cardIndex * 50 + 50)
						&& (cardY <= clickY && clickY <= cardY + 70))
					{
						Plant* tempPlant = CreatePlant(cardIndex);
						if (tempPlant) {
							int cost = tempPlant->cost;
							free(tempPlant); 
							
							if (gameParams.sunshine >= cost) {
								gameParams.lbtnDown = 1;
								gameParams.selectCard = cardIndex;
								printf("select card %d (cost: %d, sunshine: %d)\n", cardIndex, cost, gameParams.sunshine);
							} else {
								printf("Not enough sunshine for card %d (cost: %d, sunshine: %d)\n", cardIndex, cost, gameParams.sunshine);
							}
						}
						break;
					}
				}
			}

			if (gameParams.selectCard < 0)
			{
				Sunshine* pCurSunshine = gameSunshine;
				while (pCurSunshine != NULL)
				{
					if ((pCurSunshine->x <= clickX && clickX <= pCurSunshine->x + 80)
						&& (pCurSunshine->y <= clickY && clickY <= pCurSunshine->y + 80))
					{

						if (gameSunshine == pCurSunshine)
						{
							gameSunshine = pCurSunshine->pNext;
						}

						Sunshine* pTempSunshine = pCurSunshine;
						pCurSunshine = pCurSunshine->pNext;

						if (pTempSunshine->pPrev != NULL)
						{
							pTempSunshine->pPrev->pNext = pTempSunshine->pNext;
						}
						if (pTempSunshine->pNext != NULL)
						{
							pTempSunshine->pNext->pPrev = pTempSunshine->pPrev;
						}

						gameParams.sunshine += 50;

						free(pTempSunshine);
						printf("remove sunshine!\n");

						break;
					}
					pCurSunshine = pCurSunshine->pNext;
				}
			}
		}
		else if (gAppParams.exMessages[i].message == WM_LBUTTONUP)
		{
			int clickX = gAppParams.exMessages[i].x;
			int clickY = gAppParams.exMessages[i].y;

			if (gameParams.lbtnDown == 1 && gameParams.selectCard >= 0 && gameParams.selectCard < PLANT_TYPE_NUM)
			{
				
				int clickCol = (clickX - 40) / 80;
				int clickRow = (clickY - 80) / 100;

				int isGridBlocked = 0;
				Plant* pCurPlant = gamePlant;
				while (pCurPlant != NULL)
				{
					if (pCurPlant->row == clickRow
						&& pCurPlant->col == clickCol)
					{
						isGridBlocked = 1;
						break;
					}
					pCurPlant = pCurPlant->pNext;
				}

				if (isGridBlocked == 0)
				{
					Plant* tempPlant = CreatePlant(gameParams.selectCard);
					if (tempPlant) {
						int cost = tempPlant->cost;
						free(tempPlant);
						
						if (gameParams.sunshine >= cost) {
							Plant* pNewPlant = CreatePlant(gameParams.selectCard);
							if (pNewPlant) {
								// λ
								int plantX = 40 + clickCol * 80 + 40 - 40;
								int plantY = 80 + clickRow * 100 + 50 - 40;

								pNewPlant->x = plantX;
								pNewPlant->y = plantY;
								pNewPlant->row = clickRow;
								pNewPlant->col = clickCol;

								Plant* pTemp = gamePlant;
								gamePlant = pNewPlant;
								gamePlant->pNext = pTemp;
								if (pTemp)
									pTemp->pPrev = gamePlant;

								gameParams.lbtnDown = 0;
								gameParams.selectCard = -1;
								
								gameParams.sunshine -= pNewPlant->cost;
								
								printf("Plant placed successfully (remaining sunshine: %d)\n", gameParams.sunshine);
							}
						} else {
							gameParams.lbtnDown = 0;
							gameParams.selectCard = -1;
							printf("Not enough sunshine to place plant\n");
						}
					} else {
						gameParams.lbtnDown = 0;
						gameParams.selectCard = -1;
						printf("Invalid plant type\n");
					}
				} else {
					printf("Grid already occupied\n");
				}
			}
		}
		else if (gAppParams.exMessages[i].message == WM_MOUSEMOVE)
		{
			gameParams.mouseX = gAppParams.exMessages[i].x;
			gameParams.mouseY = gAppParams.exMessages[i].y;
		}
	}

	if (gameParams.createZombieTime <= 0)
	{
		Zombie* pNewZombie = CreateRandZombie();

		Zombie* pTemp = gameZombie;
		gameZombie = pNewZombie;
		gameZombie->pNext = pTemp;
		if (pTemp)
			pTemp->pPrev = pNewZombie;

		gameParams.createZombieTime = CREATE_ZOMBIE_TIME;
	}
	else
	{
		gameParams.createZombieTime -= gAppParams.frameInterval;
	}

	if (gameParams.createSunshineTime <= 0)
	{
		Sunshine* pNewSunshine = CreateRandSunshine();

		Sunshine* pTemp = gameSunshine;
		gameSunshine = pNewSunshine;
		gameSunshine->pNext = pTemp;
		if (pTemp)
			pTemp->pPrev = pNewSunshine;

		gameParams.createSunshineTime = CREATE_SUNSHINE_TIME;
	}
	else
	{
		gameParams.createSunshineTime -= gAppParams.frameInterval;
	}

	{
		Plant* pCurPlant = gamePlant;
		while (pCurPlant != NULL)
		{
			int removePlant = 0;
			int createBullet = 0;
			UpdatePlant(pCurPlant, createBullet);
			if (createBullet == 1)
			{
				if (pCurPlant->type == PLANT_TYPE_4 || pCurPlant->type == PLANT_TYPE_5)
				{
					int plantX = pCurPlant->x;
					int plantY = pCurPlant->y;
					// 保存植物类型，以便在释放后仍能使用
					int plantType = pCurPlant->type;

					if (gamePlant == pCurPlant)
					{
						gamePlant = pCurPlant->pNext;
					}

					Plant* pTempPlant = pCurPlant;
					pCurPlant = pCurPlant->pNext;

					if (pTempPlant->pPrev != NULL)
					{
						pTempPlant->pPrev->pNext = pTempPlant->pNext;
					}
					if (pTempPlant->pNext != NULL)
					{
						pTempPlant->pNext->pPrev = pTempPlant->pPrev;
					}
					free(pTempPlant);
					removePlant = 1;

					Zombie* pCurZombie = gameZombie;
					while (pCurZombie != NULL)
					{
						int zombieX = pCurZombie->x;
						int zombieY = pCurZombie->y;

						// 辣椒有更大的爆炸范围
						int explosionRange = 160;
						if (plantType == PLANT_TYPE_5) {
							// 辣椒的爆炸范围更大
							explosionRange = 240;
						}

						if (HRDistance2Pos(plantX + 40, plantY + 40, zombieX + 80, zombieY + 80) <= explosionRange)
						{
							pCurZombie->hp = 0;
							printf("zombie hp:%d\n", pCurZombie->hp);
							if (pCurZombie->hp <= 0)
							{
								printf("zombie die!\n");

								if (gameZombie == pCurZombie)
								{
									gameZombie = pCurZombie->pNext;
								}

								Zombie* pTempZombie = pCurZombie;
								pCurZombie = pCurZombie->pNext;

								if (pTempZombie->pPrev != NULL)
								{
									pTempZombie->pPrev->pNext = pTempZombie->pNext;
								}
								if (pTempZombie->pNext != NULL)
								{
									pTempZombie->pNext->pPrev = pTempZombie->pPrev;
								}
								free(pTempZombie);
								printf("remove zombie!\n");

								gameParams.killZombieNum++;
							}
						}
						else
						{
							pCurZombie = pCurZombie->pNext;
						}

					}
				}
				else if (pCurPlant->type == PLANT_TYPE_0)
				{
					Sunshine* pNewSunshine = CreateRandSunshine();
					pNewSunshine->x = HRGetRandNum(pCurPlant->x - 10, pCurPlant->x + 60);
					pNewSunshine->y = pCurPlant->y;
					pNewSunshine->targetY = pCurPlant->y + 50;

					Sunshine* pTemp = gameSunshine;
					gameSunshine = pNewSunshine;
					gameSunshine->pNext = pTemp;
					if (pTemp)
						pTemp->pPrev = pNewSunshine;
				}
				else
				{
					int hasZombieInRow = 0;
					Zombie* pCurZombie = gameZombie;
					while (pCurZombie != NULL)
					{
						if (pCurZombie->row == pCurPlant->row)
						{
							hasZombieInRow = 1;
							break;
						}
						pCurZombie = pCurZombie->pNext;
					}

					if (hasZombieInRow == 1)
					{
						PlantBullet* pBullet = CreateBullet(pCurPlant->type);
						pBullet->x = pCurPlant->x + 30;
						pBullet->y = pCurPlant->y;
						pBullet->row = pCurPlant->row;

						PlantBullet* pTemp = gameBullet;
						gameBullet = pBullet;
						gameBullet->pNext = pTemp;
						if (pTemp)
							pTemp->pPrev = gameBullet;
					}
				}

			}
			if (removePlant == 0)
				pCurPlant = pCurPlant->pNext;
		}

		PlantBullet* pCurBullet = gameBullet;
		while (pCurBullet != NULL)
		{
			UpdateBullet(pCurBullet);

			if (pCurBullet->x >= WINDOWS_WIDTH)
			{
				if (gameBullet == pCurBullet)
				{
					gameBullet = pCurBullet->pNext;
				}

				PlantBullet* pTempBullet = pCurBullet;
				pCurBullet = pCurBullet->pNext;

				if (pTempBullet->pPrev != NULL)
				{
					pTempBullet->pPrev->pNext = pTempBullet->pNext;
				}
				if (pTempBullet->pNext != NULL)
				{
					pTempBullet->pNext->pPrev = pTempBullet->pPrev;
				}
				free(pTempBullet);
			}
			else
			{
				pCurBullet = pCurBullet->pNext;
			}
		}

		Zombie* pCurZombie = gameZombie;
		while (pCurZombie != NULL)
		{
			UpdateZombie(pCurZombie);

			if (pCurZombie->x <= 0)
			{
				if (gameZombie == pCurZombie)
				{
					gameZombie = pCurZombie->pNext;
				}

				Zombie* pTempZombie = pCurZombie;
				pCurZombie = pCurZombie->pNext;

				if (pTempZombie->pPrev != NULL)
				{
					pTempZombie->pPrev->pNext = pTempZombie->pNext;
				}
				if (pTempZombie->pNext != NULL)
				{
					pTempZombie->pNext->pPrev = pTempZombie->pPrev;
				}
				free(pTempZombie);
				printf("remove zombie!\n");

				gameParams.maxHp -= 1;
				if (gameParams.maxHp <= 0)
				{
					gAppParams.gameScene = SCENE_OVER;
					return 0;
				}
			}
			else
			{
				pCurZombie = pCurZombie->pNext;
			}
		}

		Sunshine* pCurShunshine = gameSunshine;
		while (pCurShunshine != NULL)
		{
			UpdateSunshine(pCurShunshine);

			pCurShunshine = pCurShunshine->pNext;
		}
	}


	{
		PlantBullet* pCurBullet = gameBullet;
		while (pCurBullet != NULL)
		{
			int isBulletRemoved = 0;

			int bulletX = pCurBullet->x;
			int bulletY = pCurBullet->y;

			Zombie* pCurZombie = gameZombie;
			while (pCurZombie != NULL)
			{
				int zombieX = pCurZombie->x + 50;
				int zombieY = pCurZombie->y;

				if (pCurZombie->row == pCurBullet->row)
				{
					if (zombieX <= bulletX && bulletX <= (zombieX + 160)
						&& zombieY <= bulletY && bulletY <= (zombieY + 160))
					{
						pCurZombie->hp = pCurZombie->hp - 1;
						printf("zombie hp:%d\n", pCurZombie->hp);
						if (pCurZombie->hp <= 0)
						{
							printf("zombie die!\n");

							if (gameZombie == pCurZombie)
							{
								gameZombie = pCurZombie->pNext;
							}

							Zombie* pTempZombie = pCurZombie;
							pCurZombie = pCurZombie->pNext;

							if (pTempZombie->pPrev != NULL)
							{
								pTempZombie->pPrev->pNext = pTempZombie->pNext;
							}
							if (pTempZombie->pNext != NULL)
							{
								pTempZombie->pNext->pPrev = pTempZombie->pPrev;
							}
							free(pTempZombie);
							printf("remove zombie!\n");

							gameParams.killZombieNum++;
						}

						if (gameBullet == pCurBullet)
						{
							gameBullet = pCurBullet->pNext;
						}

						PlantBullet* pTempBullet = pCurBullet;
						pCurBullet = pCurBullet->pNext;

						if (pTempBullet->pPrev != NULL)
						{
							pTempBullet->pPrev->pNext = pTempBullet->pNext;
						}
						if (pTempBullet->pNext != NULL)
						{
							pTempBullet->pNext->pPrev = pTempBullet->pPrev;
						}
						free(pTempBullet);
						printf("remove bullet!\n");

						isBulletRemoved = 1;

						break;
					}
				}

				pCurZombie = pCurZombie->pNext;
			}
			if (isBulletRemoved == 0)
			{
				if (pCurBullet)
					pCurBullet = pCurBullet->pNext;
			}
		}
	}
	{
		Plant* pCurPlant = gamePlant;
		while (pCurPlant != NULL)
		{
			int isPlantRemoved = 0;

			int plantX = pCurPlant->x;
			int plantY = pCurPlant->y;

			Zombie* pCurZombie = gameZombie;
			while (pCurZombie != NULL)
			{
				int zombieTopLeftX = pCurZombie->x + 90;
				int zombieTopLeftY = pCurZombie->y;
				int zombieButtomRightX = pCurZombie->x + 160 + 90;
				int zombieButtomRightY = pCurZombie->y + 160;

				if (pCurZombie->row == pCurPlant->row)
				{
					if (HRIsRectangleCircleIntersect(zombieTopLeftX, zombieTopLeftY, zombieButtomRightX, zombieButtomRightY, plantX + 40, plantY + 40, 60))
					{
						pCurZombie->stop = 1;
						printf("zombie stop:%d\n", pCurZombie->hp);

						pCurPlant->hp -= 1;
						if (pCurPlant->hp <= 0)
						{
							if (gamePlant == pCurPlant)
							{
								gamePlant = pCurPlant->pNext;
							}

							Plant* pTempPlant = pCurPlant;
							pCurPlant = pCurPlant->pNext;

							if (pTempPlant->pPrev != NULL)
							{
								pTempPlant->pPrev->pNext = pTempPlant->pNext;
							}
							if (pTempPlant->pNext != NULL)
							{
								pTempPlant->pNext->pPrev = pTempPlant->pPrev;
							}
							free(pTempPlant);
							printf("remove plant!\n");

							isPlantRemoved = 1;

							break;
						}
					}
					else
					{
						pCurZombie->stop = 0;
					}
				}

				pCurZombie = pCurZombie->pNext;
			}

			if (isPlantRemoved == 0)
			{
				if (pCurPlant)
					pCurPlant = pCurPlant->pNext;
			}
		}
	}

	return 1;
}

void DrawGameScene()
{
	cleardevice();
	setbkmode(TRANSPARENT);

	putimage(0, 0, &imgGameBg);
	putimage(0, 0, &imgTopKuang);
	putimage(590, 0, &imgJiangshitupian);

	// 在顶部显示当前时间
	settextstyle(20, 0, _T("Arial"));
	settextcolor(BLACK);
	char szBuffer[64];
	
	// 获取并显示当前时间 (修复乱码问题)
	time_t now = time(NULL);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	strftime(szBuffer, sizeof(szBuffer), "%H:%M:%S", &timeinfo);
	
	// 绘制时间背景
	setfillcolor(LIGHTGREEN);
	setlinecolor(BLACK);
	fillrectangle(140, 8, 220, 30);
	
	// 绘制时间文本
	settextcolor(BLACK);
	outtextxy(145, 10, szBuffer);
	
	// 继续显示阳光数
	snprintf(szBuffer, sizeof(szBuffer), "%d", gameParams.sunshine);
	outtextxy(25, 65, szBuffer);

	settextstyle(24, 0, _T("微软雅黑"));
	setbkmode(OPAQUE);
	settextcolor(RED);
	snprintf(szBuffer, sizeof(szBuffer), "血:%d", gameParams.maxHp);
	outtextxy(620, 30, szBuffer);

	snprintf(szBuffer, sizeof(szBuffer), "杀僵:%d", gameParams.killZombieNum);
	outtextxy(620, 50, szBuffer);
	setbkmode(TRANSPARENT);

	setbkmode(TRANSPARENT);

	putimage(startCardX, cardY, &imgSunFlowerCard);
	putimage(startCardX + 50 * 1, cardY, &imgPeaShooterCard);
	putimage(startCardX + 50 * 2, cardY, &imgWallNutCard);
	putimage(startCardX + 50 * 3, cardY, &imgSnowShooterCard);
	putimage(startCardX + 50 * 4, cardY, &imgCherryBombCard);
	
	// 精确调整辣椒卡片位置
	putimage(startCardX + 50 * 5 - 10, cardY, &imgJalapenoCard);

	PlantBullet* pCurBullet = gameBullet;
	while (pCurBullet != NULL)
	{
		HRPutImageEnhance(pCurBullet->x, pCurBullet->y, GetBulletImage(pCurBullet));

		pCurBullet = pCurBullet->pNext;
	}

	Plant* pCurPlant = gamePlant;
	while (pCurPlant != NULL)
	{
		HRPutImageEnhance(pCurPlant->x, pCurPlant->y, GetPlantImage(pCurPlant));

		pCurPlant = pCurPlant->pNext;
	}

	Zombie* pCurZombie = gameZombie;
	while (pCurZombie != NULL)
	{
		HRPutImageEnhance(pCurZombie->x, pCurZombie->y, GetZombieImage(pCurZombie));

		pCurZombie = pCurZombie->pNext;
	}

	Sunshine* pCurSunshine = gameSunshine;
	while (pCurSunshine != NULL)
	{
		HRPutImageEnhance(pCurSunshine->x, pCurSunshine->y, GetSunshineImage(pCurSunshine));

		pCurSunshine = pCurSunshine->pNext;
	}

	if (gameParams.lbtnDown && gameParams.selectCard >= 0)
	{
		if (gameParams.selectCard < PLANT_TYPE_NUM) {
			// 所有植物使用相同的选择偏移
			HRPutImageAlpha(gameParams.mouseX - 35, gameParams.mouseY - 35, GetSelectPlantImage(gameParams.selectCard));
		} else {
			gameParams.lbtnDown = 0;
			gameParams.selectCard = -1;
			printf("Invalid plant selection detected and reset\n");
		}
	}

	FlushBatchDraw();
}


