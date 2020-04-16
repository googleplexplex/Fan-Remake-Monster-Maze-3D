#pragma once

typedef enum menuPage_CursorEnum
{
	onStart = 0,
	onSecondButton,
	onExit
};
menuPage_CursorEnum menuPage_Cursor;

void menuPage_onCalled()
{
	menuPage_Cursor = onStart;
}

#define randInRange(from, to) (rand() % ((to) - (from)) + (from))
void menuPage_onPaint(HDC dispatheredHDC)
{
	COLORREF presentRandomColor = RGB(randInRange(125, 255), randInRange(125, 255), randInRange(125, 255));

	HFONT prevFont = (HFONT)SelectObject(dispatheredHDC, (HFONT)CreateFontA(50, 40, 0, 0, FW_BOLD, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, ("Courier New")));
	SetTextColor(mainWindowHDC, presentRandomColor);
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOutCenter(dispatheredHDC, screenSize.y / 10, "MONSTER MAZE 3D");
	SelectObject(dispatheredHDC, prevFont);

	prevFont = (HFONT)SelectObject(dispatheredHDC, (HFONT)CreateFontA(20, 15, 0, 0, FW_BOLD, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, ("Courier New")));
	SetTextColor(mainWindowHDC, presentRandomColor);
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOut(dispatheredHDC, screenSize.x / 3 * 2, screenSize.y / 10 * 4, "Start", strlen("Start"));
	TextOut(dispatheredHDC, screenSize.x / 3 * 2, screenSize.y / 10 * 5, "Too Exit", strlen("Too Exit"));
	TextOut(dispatheredHDC, screenSize.x / 3 * 2, screenSize.y / 10 * 6, "Exit", strlen("Exit"));
	if(menuPage_Cursor == onStart)
		TextOut(dispatheredHDC, screenSize.x / 3 * 2 - GetTextExtentPoint32Size(">").x, screenSize.y / 10 * 4, ">", 1);
	else if(menuPage_Cursor == onSecondButton)
		TextOut(dispatheredHDC, screenSize.x / 3 * 2 - GetTextExtentPoint32Size(">").x, screenSize.y / 10 * 5, ">", 1);
	else if (menuPage_Cursor == onExit)
		TextOut(dispatheredHDC, screenSize.x / 3 * 2 - GetTextExtentPoint32Size(">").x, screenSize.y / 10 * 6, ">", 1);
	SelectObject(dispatheredHDC, prevFont);

	TextOut(dispatheredHDC, screenSize.x / 8 * 1, screenSize.y / 10 * 3, "Management and Rules:", strlen("Management and Rules:"));
	TextOut(dispatheredHDC, screenSize.x / 7 * 1, screenSize.y / 10 * 4, "Q - Turn Right", strlen("Q - Turn Right"));
	TextOut(dispatheredHDC, screenSize.x / 7 * 1, screenSize.y / 10 * 5, "W - Move / Up", strlen("W - Move / Up"));
	TextOut(dispatheredHDC, screenSize.x / 7 * 1, screenSize.y / 10 * 6, "E - Turn Left", strlen("E - Turn Left"));
	TextOut(dispatheredHDC, screenSize.x / 7 * 1, screenSize.y / 10 * 7, "S - Turn Around / Down", strlen("S - Turn Around / Down"));
	TextOut(dispatheredHDC, screenSize.x / 7 * 1, screenSize.y / 10 * 8, "SPACE - Select / Down", strlen("SPACE - Select / Down"));
	TextOut(dispatheredHDC, screenSize.x / 8 * 1, screenSize.y / 10 * 9, "Just don't get eaten: D (And don't press C)", strlen("Just don't get eaten: D (And don't press C)"));
}

void menuPage_onKeyPressed(unsigned int key)
{
	switch (key)
	{
	case VK_SPACE:
		if(menuPage_Cursor == onStart)
			goToPage(gamePage);
		else if(menuPage_Cursor == onSecondButton)
			PostQuitMessage(0);
		else if (menuPage_Cursor == onExit)
			PostQuitMessage(0);
		break;
	case KB_W:
		if (menuPage_Cursor != onStart)
			menuPage_Cursor = menuPage_CursorEnum(menuPage_Cursor - 1);
		refreshCanvas();
		break;
	case KB_S:
		if (menuPage_Cursor != onExit)
			menuPage_Cursor = menuPage_CursorEnum(menuPage_Cursor + 1);
		refreshCanvas();
		break;
	default:
		break;
	}
}

void menuPage_main()
{
	
}