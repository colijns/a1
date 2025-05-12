#pragma once

struct GameParams
{
	long long createZombieTime = 0;
	int zombieFrameRecords[10][10] = { 0 }; 
	int plantFrameRecords[6] = {0};
	long long createSunshineTime = 0;
	int lbtnDown = 0;
	int selectCard = -1;
	int mouseX;
	int mouseY;
	int maxHp = 15;
	int killZombieNum = 0;
	int sunshine = 10000;
};
extern GameParams gameParams;

void LoadGameRes();

int InitGameScene();

int UpdateGameScene();

void DrawGameScene();


