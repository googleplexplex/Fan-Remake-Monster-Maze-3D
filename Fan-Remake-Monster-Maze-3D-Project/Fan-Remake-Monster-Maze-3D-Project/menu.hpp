#pragma once
#include "game.hpp"

void Game_Menu()
{
	Game_Tick(); //Temp Debug

	PAINTSTRUCT ps;
	mainWindowHDC = BeginPaint(mainWindowHWND, &ps);

	HFONT oldFont = (HFONT)SelectObject(mainWindowHDC, hfont);
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOut(mainWindowHDC, screenSize.x / 3, screenSize.y / 5, "MONSTER MAZE 3D", strlen("MONSTER MAZE 3D"));
	SelectObject(mainWindowHDC, oldFont);

	EndPaint(mainWindowHWND, &ps);
}