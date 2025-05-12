#include "HRUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#include <sys/stat.h> // stat()


int HRFindLastOf(const char* str, const char* chars)
{
    if (str == NULL || chars == NULL) {
        return -1;
    }
    
    int len = strlen(str);
    if (len == 0) {
        return -1;
    }
    
    for (int i = len - 1; i >= 0; i--)
    {
        for (int j = 0; chars[j] != '\0'; j++) {
            if (str[i] == chars[j])
            {
                return i;
            }
        }
    }
    return -1;
}

void HRRemoveNewLine(char* str)
{
    // 查找换行符
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        // 将换行符替换为字符串结束符
        str[len - 1] = '\0';
    }
}

int HRReadFileLines(const char* pFile, char* pBuffer, int m, int n)
{
    FILE* file = fopen(pFile, "r");
    if (file == NULL) 
    {
        return -1;
    }

    int lineNum = 0;
    char* pCurBuffer = pBuffer;
    // 按行读取文件，直到文件末尾
    while (fgets(pCurBuffer, m, file) != NULL)
    {
        HRRemoveNewLine(pCurBuffer);

        ++lineNum;
        pCurBuffer += m;
        if (lineNum > n)
        {
            // 如果已经超过了文件大小 先返回
            return 0;
        }
    }

    fclose(file);
    return lineNum;
}

int HRCheckFileExists(const char* pFile)
{
#ifdef _WIN32
    struct _stat path_stat;
    if (_stat(pFile, &path_stat) == 0) { // 路径存在
        if (path_stat.st_mode & _S_IFREG) { // 判断是否为普通文件
            return 1; // 是普通文件，返回 1
        }
    }
#else
    struct stat path_stat;
    if (stat(pFile, &path_stat) == 0) { // 路径存在
        if (S_ISREG(path_stat.st_mode)) { // 判断是否为普通文件
            return 1; // 是普通文件，返回 1
        }
    }
#endif
    return 0; // 不是普通文件，或者路径不存在，返回 0
}

int HRCheckDirExist(const char* pDir)
{
#ifdef _WIN32
    struct _stat path_stat;
    if (_stat(pDir, &path_stat) == 0) { // 路径存在
        if (path_stat.st_mode & _S_IFDIR) { // 判断是否为目录
            return 1; // 是目录，返回 1
        }
    }
#else
    struct stat path_stat;
    if (stat(pDir, &path_stat) == 0) { // 路径存在
        if (S_ISDIR(path_stat.st_mode)) { // 判断是否为目录
            return 1; // 是目录，返回 1
        }
    }
#endif
    return 0; // 不是目录，或者路径不存在，返回 0
}

int HRSplitString(const char* str, char delimiter, char* resultArray, int maxSubStrLen, int maxItems, int* returnSize)
{
    if (str == NULL || resultArray == NULL || returnSize == NULL || maxSubStrLen <= 0) {
        if (returnSize != NULL) {
            *returnSize = 0;
        }
        return -1; // 参数错误
    }

    // 创建一个临时缓冲区，因为 strtok 会修改输入字符串
    char* tempStr = _strdup(str);
    if (tempStr == NULL) {
        *returnSize = 0;
        return -1; // 内存分配失败
    }

    // 创建一个临时的分隔符字符串
    char delim_str[2] = {0};
    delim_str[0] = delimiter;
    delim_str[1] = '\0';

    // 使用strtok进行分割
    char* token = strtok(tempStr, delim_str);
    int index = 0;
    
    // 初始化结果数组
    memset(resultArray, 0, maxItems * maxSubStrLen);
    
    while (token != NULL && index < maxItems)
    {
        size_t token_len = strlen(token);
        if (token_len >= (size_t)maxSubStrLen) {
            token_len = maxSubStrLen - 1; // 截断过长的字符串
        }
        
        // 复制每个子串并保证以'\0'结尾
        strncpy(resultArray + index * maxSubStrLen, token, token_len);
        resultArray[index * maxSubStrLen + token_len] = '\0';
        
        token = strtok(NULL, delim_str);
        ++index;
    }

    *returnSize = index; // 返回实际分割得到的子串数量

    free(tempStr);
    return 0; // 成功
}

void HRGotoXY(int x, int y)
{
    HANDLE hout;
    COORD cor;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    cor.X = x;
    cor.Y = y;
    SetConsoleCursorPosition(hout, cor);
}

void HRHideCursor()
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);    //hout是一个句柄，用来指挥一个设备或者对象，这里是STD_OUTPUT_HANDLE，就是标准输出设备（控制台窗口）
    CONSOLE_CURSOR_INFO cor_info = { 1, 0 };         //windows api定义的结构体，用以管理光标属性，第一个是大小，第二个是是否可见
    SetConsoleCursorInfo(hout, &cor_info);          //告诉函数，设置该句柄所对应的光标
}

void HRPrintLine(char c, int n)
{
    for (int i = 0; i < n; i++)
    {
        putchar(c);
    }
    putchar('\n');
}

void HRClearStdin()
{
    int c;
    // 不断读取 stdin 中的字符，直到遇到换行符或文件结束符
    while ((c = getchar()) != '\n' && c != EOF);
}

int HRGetLocalTimeString(char* pBuffer, int size)
{
    memset(pBuffer, 0, size);

    time_t rawtime;
    struct tm timeinfo;

    // 获取当前时间
    if (time(&rawtime) == (time_t)-1) 
    {
        return 1; // 获取时间失败
    }

    // 根据不同平台选择合适的本地时间转换函数
#ifdef _WIN32
    if (localtime_s(&timeinfo, &rawtime) != 0) 
    {
        return 1; // 转换本地时间失败
    }
#else
    if (localtime_r(&rawtime, &timeinfo) == NULL) {
        return 1; // 转换本地时间失败
    }
#endif

    // 格式化时间为指定的字符串格式
    if (strftime(pBuffer, size, "%Y-%m-%d %H:%M:%S", &timeinfo) == 0) {
        return 1; // 格式化时间失败
    }

    return 0; // 成功获取并格式化时间
}

#define EPOCH_BIAS 11644473600000LL

long long HRGetMillisecondTimestamp()
{
#ifdef _WIN32
    // Windows 平台使用 GetSystemTimeAsFileTime
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    // FILETIME 是一个64位值，单位是100纳秒
    ULARGE_INTEGER time;
    time.LowPart = ft.dwLowDateTime;
    time.HighPart = ft.dwHighDateTime;

    // 转换为毫秒
    return (long long)(time.QuadPart / 10000); // 1毫秒 = 10000个100纳秒
#else
    // Linux/Unix 平台使用 gettimeofday
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_sec) * 1000 + (tv.tv_usec / 1000); // 转换为毫秒
#endif
}

float HRDistance2Pos(float posX1, float posY1, float posX2, float posY2)
{
    float result = sqrt((posX1 - posX2) * (posX1 - posX2) + (posY1 - posY2) * (posY1 - posY2));

    return result;
}

int HRGetRandNum(int min, int max)
{
    int randValue = rand();
    int temp = randValue % (max + 1 - min);

    return min + temp;
}

float HRClamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int HRPos2MoveTowards(float* posX, float* posY, float posXTarget, float posYTarget, float maxDistance)
{
    float dx = posXTarget - *posX;
    float dy = posYTarget - *posY;
    float value = (dx * dx) + (dy * dy);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance * maxDistance)))
    {
        return 0;
    }

    float dist = sqrtf(value);

    *posX = *posX + dx / dist * maxDistance;
    *posY = *posY + dy / dist * maxDistance;

    return 1;
}

void HRVector2Normalize(int* x, int* y)
{
    int tempX = *x;
    int tempY = *y;
    float length = sqrtf((tempX * tempX) + (tempY * tempY));
    if (length > 0)
    {
        float ilength = 1.0f / length;
        *x = *x * ilength;
        *y = *y * ilength;
    }
}

int HRIsRectangleCircleIntersect(float rect_x1, float rect_y1, float rect_x2, float rect_y2, float circle_x, float circle_y, float radius)
{
    // 找到矩形中距离圆心最近的点
    float closest_x = HRClamp(circle_x, rect_x1, rect_x2);
    float closest_y = HRClamp(circle_y, rect_y1, rect_y2);

    // 计算最近点到圆心的距离的平方
    float distance_sq = (closest_x - circle_x) * (closest_x - circle_x) +
        (closest_y - circle_y) * (closest_y - circle_y);

    // 判断距离是否小于等于半径的平方
    return distance_sq <= radius * radius;
}

int HRIsCircleIntersect(float x1, float y1, float r1, float x2, float y2, float r2)
{
    // 计算圆心之间的平方距离
    float distance_sq = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);

    // 计算两圆半径之和的平方
    float radius_sum_sq = (r1 + r2) * (r1 + r2);

    // 判断是否相交
    return distance_sq <= radius_sum_sq;
}
