#pragma once

#include <graphics.h>

void HRPutImageAlpha(int x, int y, IMAGE* img);
void HRPutImageAlpha(int x, int y, IMAGE* img, int alpha);
void HRPutImageEnhance(int x, int y, IMAGE* img);

void HRSetFont(const char* fontName, int fontWidth, int fontHeight);

void HRPreLoadSound(const char* name);
void HRPlaySound(const char* name);

int IsMouseInBox(int x, int y, int left, int top, int right, int bottom);


struct HRSequenceFrame
{

};


struct HRInputBox
{
	int x;
	int y;
	int width;
	int height;
	char value[256];
	int isActive;
	int cursorPos;
	int isPwdType = 0;

	// 绘制光标
	int cursorShow = 0;
	float cursorDelayTime = 0;

	int fontHeight;
	char fontName[32];

	COLORREF textInputBackground;
	COLORREF textInputBorder;
	COLORREF textInputText;
	COLORREF textInputSelectedBackground;
	COLORREF textInputCursorColor;
};
void HRSetInputBoxStyle1(HRInputBox* pInputBox);

void HRDrawInputBox(HRInputBox* pInputBox);
void HRUpdateInputBoxCursor(HRInputBox* pInputBox, long long timeElapse);
void HRHandleInputBoxActivation(int mouseX, int mouseY, HRInputBox* pInputBox);
void HRHandleInputBoxTexInput(int key, HRInputBox* pInputBox);
void HRHandleInputBoxBackspace(HRInputBox* pInputBox);

struct HRButton
{
	int x;
	int y;
	int width;
	int height;
	char value[256];

	int fontHeight;
	char fontName[32];

	int onHover = 0;
	int onPressed = 0;

	COLORREF buttonBorder;
	COLORREF buttonBackground;
	COLORREF buttonText;
	COLORREF buttonOnHoverBorder;
	COLORREF buttonOnHoverBackground;
	COLORREF buttonOnHoverText;
	COLORREF buttonPressedBorder;
	COLORREF buttonPressedBackground;
	COLORREF buttonPressedText;
};

void HRSetButtonStyle1(HRButton* pButton);

void HRDrawButton(HRButton* pButton);
void HRHandleMouseMoveButton(int mouseX, int mouseY, HRButton* pButton);
int HRHandleButtonLBtnDown(int mouseX, int mouseY, HRButton* pButton);
int HRHandleButtonLBtnUp(int mouseX, int mouseY, HRButton* pButton);
