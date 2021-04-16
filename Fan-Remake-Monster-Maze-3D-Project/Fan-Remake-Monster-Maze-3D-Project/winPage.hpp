#pragma once

COLORREF winPage_presentRandomColor;
byte winPage_presentRandomColorRGB[3];
#define R 0
#define G 1
#define B 2

void winPage_onCalled()
{
	winPage_presentRandomColorRGB[R] = 0;
	winPage_presentRandomColorRGB[G] = 0;
	winPage_presentRandomColorRGB[B] = 0;
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

byte presentEditedColor = R;
bool editColorUp = true;
const int slowKoef = 50;

int presentKoef = 0;
void winPage_main()
{
	if (presentKoef == slowKoef)
	{
		if (winPage_presentRandomColorRGB[presentEditedColor] == UCHAR_MAX && editColorUp)
		{
			presentEditedColor = (presentEditedColor + 1) % 3;

			if (winPage_presentRandomColorRGB[presentEditedColor] == UCHAR_MAX)
				editColorUp = false;
		}
		else if (winPage_presentRandomColorRGB[presentEditedColor] == 0 && editColorUp == false)
		{
			presentEditedColor = (presentEditedColor + 1) % 3;

			if (winPage_presentRandomColorRGB[presentEditedColor] == 0)
				editColorUp = true;
		}

		if (editColorUp)
			winPage_presentRandomColorRGB[presentEditedColor]++;
		else
			winPage_presentRandomColorRGB[presentEditedColor]--;

		presentKoef = 0;
	}

	presentKoef++;

	winPage_presentRandomColor = RGB(winPage_presentRandomColorRGB[R],
		winPage_presentRandomColorRGB[G],
		winPage_presentRandomColorRGB[B]);

	winPage_onPaint(GetDC(mainWindowHWND));
}