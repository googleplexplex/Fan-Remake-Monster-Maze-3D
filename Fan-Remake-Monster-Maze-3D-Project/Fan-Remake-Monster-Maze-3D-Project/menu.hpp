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
	gameState presentGameState = inProcess;
	while (presentGameState == inProcess)
	{
		presentGameState = Game_Tick();
	}
	if(presentGameState == win)
		Game_Win();
	else if(presentGameState == lose)
		Game_GameOver();
}

void Game_Menu()
{
	Game_Game(); //Temp Debug

	PAINTSTRUCT ps;
	mainWindowHDC = BeginPaint(mainWindowHWND, &ps);

	HFONT oldFont = (HFONT)SelectObject(mainWindowHDC, hfont);
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
	TextOut(mainWindowHDC, screenSize.x / 3, screenSize.y / 5, "MONSTER MAZE 3D", strlen("MONSTER MAZE 3D"));
	SelectObject(mainWindowHDC, oldFont);

	EndPaint(mainWindowHWND, &ps);
}