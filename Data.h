#pragma once

#include <graphics.h>

// 初始化窗口大小
#define WINDOWS_WIDTH 800
#define WINDOWS_HEIGHT 600 

// 游戏FPS
#define GAME_FPS 120
// 计算每帧的时间
#define FRAME_ELAPSE 1000 / GAME_FPS

// 右边的游戏信息框
#define GAME_INFO_WINDOWNS_WIDTH 200


// 每帧移动的角度
#define ANGLE_PER_FRAME 160 / MAX_HOOK_ANGLE
#define PI 3.1415926535



#define RESOURCE_IMAGE_ROOT1 "./Resource/Images/"
#define RESOURCE_IMAGE_ROOT2 "../../Resource/Images/"

#define RESOURCE_AUDIO_ROOT1 "./Resource/audio/"
#define RESOURCE_AUDIO_ROOT2 "../../Resource/audio/"
#define SCENE_READY 0
// 主游戏场景
#define SCENE_GAME 1
// 游戏失败场景
#define SCENE_OVER 3

struct AppParams
{
	int exMessageSize = 0;
	struct ExMessage exMessages[100];

	// 标记游戏是否退出
	int gameExist = 0;

	int gameLevel = 0;
	// 当前场景
	int gameScene;

	long long frameCnt = 0;
	// 上一帧的时间
	long long lastFrameTime;
	long long frameInterval;


	// 是否暂停游戏
	int suspendGame = 0;

	// 是否初始化游戏
	int needInitGame = 1;
	// 是否初始化商店
	int needInitShop = 1;
	
	// 之前是否游戏失败
	int gameOver = 0;

	// 当前关卡
	int level = 0;
};

extern AppParams gAppParams;

