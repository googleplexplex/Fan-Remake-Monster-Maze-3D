#pragma once

void losePage_onCalled()
{

}

void losePage_onPaint(HDC dispatheredHDC)
{
	TextOutCenter(mainWindowHDC, screenSize.y / 5, "YOU WAS CATHCED BY MONSTER");
	TextOutCenter(mainWindowHDC, screenSize.y / 3 * 2, "PRESS SPACE TO MENU");
}

void losePage_onKeyPressed(unsigned int key)
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

void losePage_main()
{

}