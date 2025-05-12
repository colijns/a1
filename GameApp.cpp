#include "GameApp.h"
#include "DataDef.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "GameReadyScene.h"
#include "HRUtil.h"
#include <stdio.h>  
#include <string.h>  
#include <malloc.h> 
#include <conio.h> //函数声明//  
#include <vector>
#include <chrono>
#include <ctime>
#include <format>

AppParams gAppParams;

// 原有的窗口消息处理函数
WNDPROC gOriginWndProc = NULL;

// 初始化游戏
void InitGame();
// 总的Update和Draw
int Update();
void Draw();

void RunGame()
{
	InitGame();
	
	gAppParams.needInitGame = 1;
	gAppParams.gameScene = SCENE_READY;

	BeginBatchDraw();

	while (1)
	{
		if (Update() != 0)
		{
			Draw();
		}

		if (gAppParams.gameExist == 1)
		{
			return;
		}
	}
}

int Update()
{
	gAppParams.frameCnt++;

	if (gAppParams.gameExist == 1)
		return 0;

	// 控制帧频
	if (!CheckFrameElapse())
	{
		return 0;
	}

	// easyx 消息
	gAppParams.exMessageSize = 0;
	ExMessage message{};
	while (peekmessage(&message)) {
		gAppParams.exMessages[gAppParams.exMessageSize] = message;
		++gAppParams.exMessageSize;
	}

	int rtUpdate = 0;
	switch (gAppParams.gameScene)
	{
	case SCENE_READY:
		rtUpdate = UpdateGameReadyScene();
		break;
	case SCENE_GAME:
		rtUpdate = UpdateGameScene();
		break;
	case SCENE_OVER:
		rtUpdate = UpdateGameOverScene();
		break;
	}

	return rtUpdate;
}

void Draw()
{
	switch (gAppParams.gameScene)
	{
	case SCENE_READY:
		DrawGameReadyScene();
		break;
	case SCENE_GAME:
		DrawGameScene();
		break;
	case SCENE_OVER:
		DrawGameOverScene();
		break;
	}
}


LRESULT CALLBACK NewWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		gAppParams.gameExist = 1;
		break;

	default:
		return gOriginWndProc(hWnd, uMsg, wParam, lParam);		// 执行默认的消息处理函数
	}
	return 0;
}


void InitGame()
{
	srand(time(NULL));
	 
	// 初始化窗口大小
	HWND wnd = initgraph(800, 600, SHOWCONSOLE);
	gOriginWndProc = (WNDPROC)GetWindowLongPtr(wnd, GWLP_WNDPROC);
	SetWindowLongPtr(wnd, GWLP_WNDPROC, (LONG_PTR)NewWndProc);

	// 初始化游戏场景
	InitGameScene();
	InitGameOverScene();
	InitGameReadyScene();

	// 获取当前时间点
	auto now = std::chrono::system_clock::now();
	// 获取自纪元以来的持续时间，并转换为毫秒
	gAppParams.lastFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

char* GetImagePath(const char* pRes, char* pBuffer, int bufLength)
{
	if (pRes == NULL || pBuffer == NULL || bufLength <= 0)
	{
		return NULL;
	}

	// 首先清空缓冲区
	memset(pBuffer, 0, bufLength);
	
	// 计算源路径的长度，确保不会溢出
	size_t root1Len = strlen(RESOURCE_IMAGE_ROOT1);
	size_t resLen = strlen(pRes);
	
	// 检查是否会溢出
	if (root1Len + resLen + 1 > (size_t)bufLength)
	{
		printf("Path too long for buffer: %s%s\n", RESOURCE_IMAGE_ROOT1, pRes);
		return NULL;
	}
	
	// 复制根路径
	strcpy(pBuffer, RESOURCE_IMAGE_ROOT1);
	// 连接资源路径
	strcat(pBuffer, pRes);
	
	if (!HRCheckFileExists(pBuffer))
	{
		// 清空缓冲区重新尝试第二个路径
		memset(pBuffer, 0, bufLength);
		
		// 计算第二个源路径的长度
		size_t root2Len = strlen(RESOURCE_IMAGE_ROOT2);
		
		// 检查是否会溢出
		if (root2Len + resLen + 1 > (size_t)bufLength)
		{
			printf("Path too long for buffer: %s%s\n", RESOURCE_IMAGE_ROOT2, pRes);
			return NULL;
		}
		
		// 复制根路径
		strcpy(pBuffer, RESOURCE_IMAGE_ROOT2);
		// 连接资源路径
		strcat(pBuffer, pRes);
		
		if (!HRCheckFileExists(pBuffer))
		{
			printf("File not found: %s\n", pRes);
			return NULL;
		}
	}

	return pBuffer;
}

char* GetAudioPath(const char* pRes, char* pBuffer, int bufLength)
{
	if (pRes == NULL || pBuffer == NULL || bufLength <= 0)
	{
		return NULL;
	}

	// 首先清空缓冲区
	memset(pBuffer, 0, bufLength);
	
	// 计算源路径的长度，确保不会溢出
	size_t root1Len = strlen(RESOURCE_AUDIO_ROOT1);
	size_t resLen = strlen(pRes);
	
	// 检查是否会溢出
	if (root1Len + resLen + 1 > (size_t)bufLength)
	{
		printf("Audio path too long for buffer: %s%s\n", RESOURCE_AUDIO_ROOT1, pRes);
		return NULL;
	}
	
	// 复制根路径
	strcpy(pBuffer, RESOURCE_AUDIO_ROOT1);
	// 连接资源路径
	strcat(pBuffer, pRes);
	
	if (!HRCheckFileExists(pBuffer))
	{
		// 清空缓冲区重新尝试第二个路径
		memset(pBuffer, 0, bufLength);
		
		// 计算第二个源路径的长度
		size_t root2Len = strlen(RESOURCE_AUDIO_ROOT2);
		
		// 检查是否会溢出
		if (root2Len + resLen + 1 > (size_t)bufLength)
		{
			printf("Audio path too long for buffer: %s%s\n", RESOURCE_AUDIO_ROOT2, pRes);
			return NULL;
		}
		
		// 复制根路径
		strcpy(pBuffer, RESOURCE_AUDIO_ROOT2);
		// 连接资源路径
		strcat(pBuffer, pRes);
		
		if (!HRCheckFileExists(pBuffer))
		{
			printf("Audio file not found: %s\n", pRes);
			return NULL;
		}
	}

	return pBuffer;
}


//检查用户数据是否匹配
int CheckUserData(const char* username, const char* password)
{
	for (int i = 0; i < 100; ++i)
	{
		if (strlen(gAppParams.accounts[i]) > 0)
		{
			if (strcmp(gAppParams.accounts[i], username) == 0
				&& strcmp(gAppParams.pwds[i], password) == 0)
			{
				return 1;
			}
		}
		else
		{
			break;
		}
	}
	return 0;
}

bool CheckFrameElapse()
{
	auto now = std::chrono::system_clock::now();
	auto millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	if (millisecs - gAppParams.lastFrameTime < FRAME_ELAPSE)
	{
		return false;
	}
	gAppParams.frameInterval = millisecs - gAppParams.lastFrameTime;
	gAppParams.lastFrameTime = millisecs;

	return true;
}


