#include "HREasyxUtil.h"
#include "HRUtil.h"
#include <stdio.h>
#include <string.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void HRPutImageAlphaInternal(int x, int y, IMAGE* img, int alpha)
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(img); //获取picture的显存指针

	int pictureWidth = img->getwidth(); //获取picture的宽度，EASYX自带
	int pictureHeight = img->getheight(); //获取picture的高度，EASYX自带

	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < pictureHeight; iy++)
	{
		for (int ix = 0; ix < pictureWidth; ix++)
		{
			int srcX = ix + iy * pictureWidth; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度

			sa = sa * ((float)alpha / 255);

			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}

void HRPutImageAlpha(int x, int y, IMAGE* img)
{
	HRPutImageAlpha(x, y, img, 255);
}

void HRPutImageAlpha(int x, int y, IMAGE* img, int alpha)
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(img); //获取picture的显存指针

	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	IMAGE imgTmp, imgTmp2, imgTmp3;

	if ((x + img->getwidth() <= 0 || y + img->getheight() <= 0))
	{
		return;
	}
	else if (y >= graphHeight || x >= graphWidth)
	{
		return;
	}
	else
	{
		if (y < 0)
		{
			SetWorkingImage(img);
			getimage(&imgTmp, 0, -y, img->getwidth(), img->getheight() + y);
			SetWorkingImage();
			y = 0;
			img = &imgTmp;
		}
		else if (y + img->getwidth() > graphHeight)
		{
			SetWorkingImage(img);
			getimage(&imgTmp, 0, 0, img->getwidth(), graphHeight - y);
			SetWorkingImage();
		}

		if (x < 0)
		{
			SetWorkingImage(img);
			getimage(&imgTmp2, -x, 0, img->getwidth() + x, img->getheight());
			SetWorkingImage();
			x = 0;
			img = &imgTmp2;
		}
		else if (x + img->getwidth() > graphWidth)
		{
			SetWorkingImage(img);
			getimage(&imgTmp3, 0, 0, graphWidth - x, img->getheight());
			SetWorkingImage();
			img = &imgTmp3;
		}
	}

	HRPutImageAlphaInternal(x, y, img, alpha);
}

void HRPutImageEnhance(int x, int y, IMAGE* img)
{
	DWORD* pmem_input = GetImageBuffer(img);
	IMAGE temp(img->getwidth(), img->getheight());
	DWORD* pmem_temp = GetImageBuffer(&temp);
	for (int i = 0; i < img->getwidth() * img->getheight(); i++)
	{
		if (pmem_input[i] >= 0 && pmem_input[i] < 0x555555)
			pmem_temp[i] = WHITE;
		else
			pmem_temp[i] = BLACK;
	}
	putimage(x, y, &temp, SRCAND);
	putimage(x, y, img, SRCPAINT);
}

void HRSetFont(const char* fontName, int fontWidth, int fontHeight)
{
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = fontHeight;                      // 设置字体高度为 48
	f.lfWidth = fontWidth;
	strcpy(f.lfFaceName, fontName);    // 设置字体为"黑体"(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	setbkmode(TRANSPARENT);
	settextstyle(&f);
}

void HRPreLoadSound(const char* name)
{
	char cmd[512];
	snprintf(cmd, sizeof(cmd), "open %s alias %s-1", name, name);
	mciSendString(cmd, 0, 0, 0);
	snprintf(cmd, sizeof(cmd), "open %s alias %s-2", name, name);
	mciSendString(cmd, 0, 0, 0);
}

void HRPlaySound(const char* name)
{
	static int index = 1;
	char cmd[512];

	if (index == 1) 
	{
		snprintf(cmd, sizeof(cmd), "play %s-1", name);
		mciSendString(cmd, 0, 0, 0);
		snprintf(cmd, sizeof(cmd), "close %s-2", name);
		mciSendString(cmd, 0, 0, 0);
		snprintf(cmd, sizeof(cmd), "open %s alias %s-2", name, name);
		mciSendString(cmd, 0, 0, 0);
		index++;
	}
	else if (index == 2) 
	{
		snprintf(cmd, sizeof(cmd), "play %s-2", name);
		mciSendString(cmd, 0, 0, 0);
		snprintf(cmd, sizeof(cmd), "close %s-1", name);
		mciSendString(cmd, 0, 0, 0);
		snprintf(cmd, sizeof(cmd), "open %s alias %s-1", name, name);
		mciSendString(cmd, 0, 0, 0);
		index = 1;
	}
}

int IsMouseInBox(int x, int y, int left, int top, int right, int bottom)
{
	return x >= left && x <= right && y >= top && y <= bottom;
}

void HRSetInputBoxStyle1(HRInputBox* pInputBox)
{
	memset(pInputBox, 0, sizeof(HRInputBox));

	pInputBox->fontHeight = 16;
	strcpy(pInputBox->fontName, "微软雅黑");

	pInputBox->textInputBackground = RGB(21, 22, 23);
	pInputBox->textInputBorder = RGB(91, 92, 93);
	pInputBox->textInputText = RGB(255, 255, 255);
	pInputBox->textInputSelectedBackground = RGB(69, 86, 112);
	pInputBox->textInputCursorColor = RGB(255, 255, 255);
}

void HRDrawInputBox(HRInputBox* pInputBox)
{
	setbkmode(TRANSPARENT);//使背景透明
	
	int textLength = textwidth(pInputBox->value);

	setlinecolor(pInputBox->textInputBorder);
	setfillcolor(pInputBox->isActive == 1 ? pInputBox->textInputSelectedBackground : pInputBox->textInputBackground);
	fillrectangle(pInputBox->x, pInputBox->y, pInputBox->x + pInputBox->width, pInputBox->y + pInputBox->height);
	rectangle(pInputBox->x, pInputBox->y, pInputBox->x + pInputBox->width, pInputBox->y + pInputBox->height);

	settextcolor(pInputBox->textInputText);
	outtextxy(pInputBox->x + 5, pInputBox->y + 5, pInputBox->value);

	// 绘制光标
	if (pInputBox->isActive)
	{
		if (pInputBox->cursorShow == 1)
		{
			setlinecolor(pInputBox->textInputCursorColor);
			int cursorX = pInputBox->x + textLength + 8;
			int cursorY = pInputBox->y + 5;
			line(cursorX, cursorY, cursorX, cursorY + 20); // 画出光标
		}
	}
}

void HRUpdateInputBoxCursor(HRInputBox* pInputBox, long long timeElapse)
{
	if ((pInputBox->cursorDelayTime += timeElapse) >= 500)
	{
		pInputBox->cursorShow = pInputBox->cursorShow == 1 ? 0 : 1;
		pInputBox->cursorDelayTime = 0;
	}
}

void HRHandleInputBoxActivation(int mouseX, int mouseY, HRInputBox* pInputBox)
{
	if (IsMouseInBox(mouseX, mouseY, pInputBox->x, pInputBox->y, pInputBox->x + pInputBox->width, pInputBox->y + pInputBox->height))
	{
		pInputBox->isActive = 1;
		pInputBox->cursorPos = (int)strlen(pInputBox->value);//光标放在用户名末尾
	}
	else
	{
		pInputBox->isActive = 0;
	}
} 

void HRHandleInputBoxTexInput(int key, HRInputBox* pInputBox)
{
	if (pInputBox->isActive)
	{
		char* currentStr = pInputBox->value;//选择当前正在输入的字符串
		memmove(currentStr + pInputBox->cursorPos + 1, currentStr + pInputBox->cursorPos, strlen(currentStr) - pInputBox->cursorPos + 1);
		currentStr[pInputBox->cursorPos] = (char)key;
		++(pInputBox->cursorPos);
	}
}

void HRHandleInputBoxBackspace(HRInputBox* pInputBox)
{
	if (pInputBox->isActive)
	{
		if (pInputBox->cursorPos > 0)
		{
			char* currentStr = pInputBox->value;//选择当前正在输入的字符串
			memmove(currentStr + pInputBox->cursorPos - 1, currentStr + pInputBox->cursorPos, strlen(currentStr) - pInputBox->cursorPos + 1);
			--(pInputBox->cursorPos);
		}
	}
}

////////////////////////////////////////////////////////////////////////////

void HRSetButtonStyle1(HRButton* pButton)
{
	memset(pButton, 0, sizeof(HRButton));

	pButton->fontHeight = 16;
	strcpy(pButton->fontName, "微软雅黑");

	pButton->buttonBorder = RGB(39, 73, 114);
	pButton->buttonBackground = RGB(39, 73, 114);
	pButton->buttonText = RGB(255, 255, 255);
	pButton->buttonOnHoverBorder = RGB(49, 83, 124);
	pButton->buttonOnHoverBackground = RGB(49, 83, 124);
	pButton->buttonOnHoverText = RGB(255, 255, 255);
	pButton->buttonPressedBorder = RGB(59, 93, 134);
	pButton->buttonPressedBackground = RGB(59, 93, 134);
	pButton->buttonPressedText = RGB(255, 255, 255);
}

void HRDrawButton(HRButton* pButton)
{

	if (pButton->onPressed == 1)
	{
		setlinecolor(pButton->buttonPressedBorder);
		setfillcolor(pButton->buttonPressedBackground);//设置按钮背景颜色为蓝色
		settextcolor(pButton->buttonPressedText);
	}
	else
	{
		if (pButton->onHover == 1)
		{
			setlinecolor(pButton->buttonOnHoverBorder );
			setfillcolor(pButton->buttonOnHoverBackground );//设置按钮背景颜色为蓝色
			settextcolor(pButton->buttonOnHoverText);

		}
		else
		{
			setlinecolor(pButton->buttonBorder);
			setfillcolor(pButton->buttonBackground);//设置按钮背景颜色为蓝色
			settextcolor(pButton->buttonText);
		}
	}

	fillrectangle(pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height);//绘制填充矩形
	setbkmode(TRANSPARENT);

	int textWidth = textwidth(pButton->value);
	int textHeight = textheight(pButton->value);
	int textPosX = (pButton->width - textWidth) / 2 + pButton->x;
	int textPosY = (pButton->height - textHeight) / 2 + pButton->y;
	outtextxy(textPosX, textPosY, pButton->value);//在按钮内显示文本
}

void HRHandleMouseMoveButton(int mouseX, int mouseY, HRButton* pButton)
{
	if (IsMouseInBox(mouseX, mouseY, pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height))
	{
		pButton->onHover = 1;
	}
	else
	{
		pButton->onHover = 0;
	}
}

int HRHandleButtonLBtnDown(int mouseX, int mouseY, HRButton* pButton)
{
	if (IsMouseInBox(mouseX, mouseY, pButton->x, pButton->y, pButton->x + pButton->width, pButton->y + pButton->height))
	{
		pButton->onPressed = 1;
	}
	else
	{
		pButton->onPressed = 0;
	}

	return pButton->onPressed;
}

int HRHandleButtonLBtnUp(int mouseX, int mouseY, HRButton* pButton)
{
	if (pButton->onPressed == 1)
	{
		pButton->onPressed = 0;

		return 1;
	}

	return 0;
}



