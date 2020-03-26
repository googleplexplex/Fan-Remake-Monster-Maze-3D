#pragma once
#include "game.hpp"

void Game_GameOver() //TODO
{

}

void Game_Win() //TODO
{

}

void Game_Game()
{
	generateGame();
	Game_Tick();
}

void Game_Menu()
{
	generateGame();
	return;
	Game_Game(); //Temp Debug

	HFONT oldFont = (HFONT)SelectObject(mainWindowHDC, hfont);
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOut(mainWindowHDC, screenSize.x / 3, screenSize.y / 5, "MONSTER MAZE 3D", strlen("MONSTER MAZE 3D"));
	SelectObject(mainWindowHDC, oldFont);
}