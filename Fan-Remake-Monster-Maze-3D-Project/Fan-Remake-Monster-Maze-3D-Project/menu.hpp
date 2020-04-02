#pragma once
#include "game.hpp"


void Game_GenerateGame()
{
	generateGame();
}

void Game_ShowMenu()
{
	HFONT oldFont = (HFONT)SelectObject(mainWindowHDC, hfont);
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOut(mainWindowHDC, screenSize.x / 3, screenSize.y / 5, "MONSTER MAZE 3D", strlen("MONSTER MAZE 3D"));
	TextOut(mainWindowHDC, screenSize.x / 7 * 2, screenSize.y / 3 * 2, "PRESS SPACE TO START", strlen("PRESS SPACE TO START"));
	SelectObject(mainWindowHDC, oldFont);
}

void Game_ShowWin()
{
	HFONT oldFont = (HFONT)SelectObject(mainWindowHDC, hfont);
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOut(mainWindowHDC, screenSize.x / 3, screenSize.y / 5, "YOU WAS ESCAPED", strlen("YOU WAS ESCAPED"));
	TextOut(mainWindowHDC, screenSize.x / 7 * 2, screenSize.y / 3 * 2, "PRESS SPACE TO MENU", strlen("PRESS SPACE TO MENU"));
	SelectObject(mainWindowHDC, oldFont);
}

void Game_ShowLose()
{
	HFONT oldFont = (HFONT)SelectObject(mainWindowHDC, hfont);
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOut(mainWindowHDC, screenSize.x / 3, screenSize.y / 5, "YOU WAS CATHCED BY MONSTER", strlen("YOU WAS CATHCED BY MONSTER"));
	TextOut(mainWindowHDC, screenSize.x / 7 * 2, screenSize.y / 3 * 2, "PRESS SPACE TO MENU", strlen("PRESS SPACE TO MENU"));
	SelectObject(mainWindowHDC, oldFont);
}