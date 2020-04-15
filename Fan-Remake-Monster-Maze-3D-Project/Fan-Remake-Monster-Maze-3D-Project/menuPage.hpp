#pragma once

void menuPage_onCalled()
{
	
}

void menuPage_onPaint(HDC dispatheredHDC)
{
	HFONT oldFont = (HFONT)SelectObject(dispatheredHDC, hfont);
	SetTextColor(dispatheredHDC, RGB(255, 255, 255));
	SetBkColor(dispatheredHDC, RGB(0, 0, 0));
	TextOutCenter(dispatheredHDC, screenSize.y / 5, "MONSTER MAZE 3D");
	TextOutCenter(dispatheredHDC, screenSize.y / 3 * 2, "PRESS SPACE TO START");
	SelectObject(dispatheredHDC, oldFont);
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

void menuPage_main()
{
	
}