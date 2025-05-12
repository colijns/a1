#pragma once

// 从后往前查找字符位置
int HRFindLastOf(const char* str, const char* chars);

// 移除换行符
void HRRemoveNewLine(char* str);

// 读取文本信息
int HRReadFileLines(const char* pFile, char* pBuffer, int m, int n);

// 判断文件是否存在
int HRCheckFileExists(const char* pFile);
int HRCheckDirExist(const char* pDir);


// 拆分字符串
int HRSplitString(const char* str, char delimiter, char* resultArray, int maxSubStrLen, int maxItems, int* returnSize);

//光标定位函数，将光标定位到(x,y)坐标位置  
void HRGotoXY(int x, int y);

// 隐藏光标
void HRHideCursor();

//实现一个画横线的函数line, 用于绘制由n个字符c输出组成的行线,函数原形如下:
void HRPrintLine(char c, int n);

//清除stdin
void HRClearStdin();

//获取当前时间格式化字符串
int HRGetLocalTimeString(char* pBuffer, int size);

// 获取当前时间的毫秒时间戳
long long HRGetMillisecondTimestamp();

// 获取随机数
int HRGetRandNum(int min, int max);

float HRClamp(float value, float min, float max);
///////////////////////////////////////////////////////////////////////////////
float HRDistance2Pos(float posX1, float posY1, float posX2, float posY2);

// 向目标移动的线性差值
int HRPos2MoveTowards(float* posX, float* posY, float posXTarget, float posYTarget, float maxDistance);

void HRVector2Normalize(int* x, int* y);

///////////////////////////////////////////////////////////////////////////////

// 判断矩形和圆形是否相交
int HRIsRectangleCircleIntersect(
    float rect_x1, float rect_y1, // 矩形左上角坐标
    float rect_x2, float rect_y2, // 矩形右下角坐标
    float circle_x, float circle_y, // 圆心坐标
    float radius);
int HRIsCircleIntersect(float x1, float y1, float r1, // 圆1的圆心和半径
    float x2, float y2, float r2);