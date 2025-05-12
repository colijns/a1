#pragma once
#include <graphics.h>

void RunGame();

void InitGame();

int IsUserExisted(const char* username);
void SaveUserData(const char* username, const char* password);
int CheckUserData(const char* username, const char* password);

char* GetImagePath(const char* pRes, char* pBuffer, int bufLength);
char* GetAudioPath(const char* pRes, char* pBuffer, int bufLength);

bool CheckFrameElapse();


