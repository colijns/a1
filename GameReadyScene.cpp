#include "GameOverScene.h"
#include "DataDef.h"
#include "GameApp.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h> //函数声明//  
#include <chrono>
#include <ctime>

IMAGE imgGameReady;

void LoadGameReadyRes()
{
	const int bufferSize = 256;
	char szBuffer[bufferSize] = { 0 };

	loadimage(&imgGameReady, GetImagePath("StartScene.jpg", szBuffer, bufferSize));
}

int InitGameReadyScene()
{
	LoadGameReadyRes();
	
	return 1;
}

int UpdateGameReadyScene()
{
	for (size_t i = 0; i < gAppParams.exMessageSize; ++i)
	{
		if (gAppParams.exMessages[i].message == WM_LBUTTONDOWN)
		{
			gAppParams.gameScene = SCENE_GAME;
			gAppParams.needInitGame = 1;

			return 0;
		}
	}

	return 1;
}

void DrawGameReadyScene()
{
	cleardevice();
	setbkmode(TRANSPARENT);

	//游戏背景图
	putimage(0, 0, &imgGameReady);

	settextcolor(BLACK);
	settextstyle(28, 0, _T("微软雅黑"));
	outtextxy(WINDOWS_WIDTH / 2 - 100, 400, "点击鼠标开始游戏");

	FlushBatchDraw();
}