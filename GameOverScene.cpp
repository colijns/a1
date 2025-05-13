#include "GameOverScene.h"
#include "Data.h"
#include "GameApp.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h> //º¯ÊýÉùÃ÷//  
#include <chrono>
#include <ctime>

IMAGE imgGameOver;
IMAGE imgGameWin;

void LoadGameOverRes()
{
	const int bufferSize = 256;
	char szBuffer[bufferSize] = { 0 };

	loadimage(&imgGameOver, GetImagePath("lose.jpg", szBuffer, bufferSize));
}

int InitGameOverScene()
{
	LoadGameOverRes();
	
	return 1;
}

int UpdateGameOverScene()
{
	for (size_t i = 0; i < gAppParams.exMessageSize; ++i)
	{
		if (gAppParams.exMessages[i].message == WM_KEYDOWN)
		{
			switch (gAppParams.exMessages[i].vkcode)
			{
			case VK_ESCAPE:
				gAppParams.gameExist = true;
				return 0;

			}
		}
	}

	return 1;
}

void DrawGameOverScene()
{
	cleardevice();
	setbkmode(TRANSPARENT);

	//ÓÎÏ·±³¾°Í¼
	putimage(0, 0, &imgGameOver);

	settextcolor(0x00FF00);
	settextstyle(28, 0, _T("Î¢ÈíÑÅºÚ"));
	outtextxy(WINDOWS_WIDTH / 2 - 100, 400, "°´ESC¼üÍË³öÓÎÏ·");

	FlushBatchDraw();
}