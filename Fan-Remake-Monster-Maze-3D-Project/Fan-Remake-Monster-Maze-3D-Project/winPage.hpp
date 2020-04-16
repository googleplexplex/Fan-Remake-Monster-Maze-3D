#pragma once

void winPage_onCalled()
{

}

void winPage_onPaint(HDC dispatheredHDC)
{
	TextOutCenter(mainWindowHDC, screenSize.y / 5, "YOU RAN AWAY");
	TextOutCenter(mainWindowHDC, screenSize.y / 3 * 2, "PRESS SPACE TO MENU");
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

void winPage_main()
{

}