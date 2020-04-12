#pragma once

void menuPage_onCalled()
{

}

void menuPage_onPaint(HDC dispatheredHDC)
{
	HFONT oldFont = (HFONT)SelectObject(mainWindowHDC, hfont);
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOutCenter(mainWindowHDC, screenSize.y / 5, "MONSTER MAZE 3D");
	TextOutCenter(mainWindowHDC, screenSize.y / 3 * 2, "PRESS SPACE TO START");
	SelectObject(mainWindowHDC, oldFont);
}

void menuPage_onKeyPressed(unsigned int key)
{
	switch (key)
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
	case VK_SPACE:
		goToPage(gamePage);
	default:
		break;
	}
}