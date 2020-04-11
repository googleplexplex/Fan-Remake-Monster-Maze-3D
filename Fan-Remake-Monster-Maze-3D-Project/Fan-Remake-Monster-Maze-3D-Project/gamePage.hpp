#pragma once
#include "game.hpp"

void gamePage_onCalled()
{
	generateGame();
}

void gamePage_onPaint(HDC dispatheredHDC)
{
	showGameCanvas();
}

void gamePage_onKeyPressed(unsigned int key)
{
	switch (key)
	{
	case KB_Q:
		player.turnLeft();
		break;
	case KB_W:
		player.moveForward();
		if (monster.catchPlayer())
			goToPage(losePage);
		if (player.inDoor())
			goToPage(winPage);
		refreshCanvas();
		break;
	case KB_E:
		player.turnRight();
		break;
	case KB_S:
		player.turnAround();
		break;
	case KB_C:
		cheats = !cheats;
		break;
	default:
		break;
	}
}