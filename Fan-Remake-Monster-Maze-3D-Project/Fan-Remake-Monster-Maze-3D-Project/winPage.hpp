#pragma once

COLORREF winPage_presentRandomColor;
byte winPage_presentRandomColorRGB[3];
#define R 0
#define G 1
#define B 2

void winPage_onCalled()
{
	winPage_presentRandomColorRGB[R] = randInRange(125, 255);
	winPage_presentRandomColorRGB[G] = randInRange(125, 255);
	winPage_presentRandomColorRGB[B] = randInRange(125, 255);
	winPage_presentRandomColor = RGB(winPage_presentRandomColorRGB[R],
		winPage_presentRandomColorRGB[G],
		winPage_presentRandomColorRGB[B]);
}

void winPage_onPaint(HDC dispatheredHDC)
{
	SetTextColor(mainWindowHDC, winPage_presentRandomColor);
	TextOutCenter(mainWindowHDC, screenSize.y / 5, "YOU RAN AWAY");
	TextOutCenter(mainWindowHDC, screenSize.y / 3 * 2, "PRESS SPACE TO MENU");
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
}

void winPage_onKeyPressed(unsigned int key)
{
	switch (key)
	{
	case VK_ESCAPE:
	case VK_SPACE:
		goToPage(menuPage);
	default:
		break;
	}
}

typedef enum colorEditCommandEnum
{
	add = 0,
	sub
};

void winPage_main()
{
	colorEditCommandEnum colorEditCommand = colorEditCommandEnum(rand() % 2);
	byte randomColor = rand() % 3;

	if (colorEditCommand == add && winPage_presentRandomColorRGB[randomColor] < UCHAR_MAX)
		winPage_presentRandomColorRGB[randomColor]++;
	else if(colorEditCommand == sub && winPage_presentRandomColorRGB[randomColor] > CHAR_MIN)
		winPage_presentRandomColorRGB[randomColor]--;

	winPage_presentRandomColor = RGB(winPage_presentRandomColorRGB[R],
		winPage_presentRandomColorRGB[G],
		winPage_presentRandomColorRGB[B]);

	winPage_onPaint(GetDC(mainWindowHWND));
}