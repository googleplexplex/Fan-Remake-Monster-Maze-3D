#pragma once
#include <iostream>
#include <wingdi.h>
#include <time.h>
#include <conio.h>
#include <atomic>
#include "map.hpp"
using namespace std;
#include "output.hpp"
#define pointsEqual(f, s) (((f).x == (s).x) && ((f).y == (s).y))

typedef enum direction {
	null = 0,
	N = 'N',
	W = 'W',
	E = 'E',
	S = 'S'
};
//-N-
//W-E
//-S-

direction inline turnDirectionLeft(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return W;
	case W: return S;
	case S: return E;
	case E: return N;
	}
}
direction inline turnDirectionRight(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return E;
	case E: return S;
	case S: return W;
	case W: return N;
	}
}
direction inline turnDirectionAround(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return S;
	case E: return W;
	case S: return N;
	case W: return E;
	}
}
class playerClass
{
public:
	POINT pos;
	direction viewDirection = E;
	void moveForward()
	{
		POINT goTo = pos;
		switch (viewDirection)
		{
		case N: goTo.y--; break;
		case W: goTo.x--; break;
		case E: goTo.x++; break;
		case S: goTo.y++; break;
		}

		if (getFromMap(goTo.x, goTo.y) != wall)
		{
			pos = goTo;
		}

		if (player.inDoor())
			presentGameState = win;

		refreshCanvas();
	}
	void inline turnLeft()
	{
		viewDirection = turnDirectionLeft(viewDirection);

		refreshCanvas();
	}
	void inline turnRight()
	{
		viewDirection = turnDirectionRight(viewDirection);

		refreshCanvas();
	}
	void inline turnAround()
	{
		viewDirection = turnDirectionAround(viewDirection);

		refreshCanvas();
	}
	bool inDoor()
	{
		return pointsEqual(pos, doorPos);
	}
}player;
void showCompas()
{
	setTo(1, 1);
	cout << '-' << player.viewDirection << '-';
	setTo(1, 2);
	cout << turnDirectionLeft(player.viewDirection) << '#' << turnDirectionRight(player.viewDirection);
	setTo(1, 3);
	cout << '-' << turnDirectionAround(player.viewDirection) << '-';
}
void showMap()
{
	setTo(1, 1);
	cout << '-' << player.viewDirection << '-';
	setTo(1, 2);
	cout << turnDirectionLeft(player.viewDirection) << '#' << turnDirectionRight(player.viewDirection);
	setTo(1, 3);
	cout << '-' << turnDirectionAround(player.viewDirection) << '-';
}

class monsterClass
{
public:
	POINT pos;
	direction target;
	direction seesPlayer() //return null, if false, direction if true
	{
		if (player.pos.x == monster.pos.x)
		{
			if (player.pos.y > monster.pos.y)
			{
				for (int i = monster.pos.y + 1; i < player.pos.y; i++)
				{
					if (gameMap[monster.pos.x][i] == wall)
						return null;
				}
				return S;
			}
			else {
				for (int i = monster.pos.y - 1; i >= 0; i--)
				{
					if (gameMap[monster.pos.x][i] == wall)
						return null;
				}
				return N;
			}
		}
		else if (player.pos.y == monster.pos.y)
		{
			if (player.pos.x > monster.pos.x)
			{
				for (int i = monster.pos.x + 1; i < player.pos.x; i++)
				{
					if (gameMap[i][monster.pos.y] == wall)
						return null;
				}
				return E;
			}
			else {
				for (int i = monster.pos.x - 1; i < player.pos.x; i--)
				{
					if (gameMap[i][monster.pos.y] == wall)
						return null;
				}
				return W;
			}
		}
		return null;
	}
	bool monsterInTarget()
	{
		switch (target)
		{
		case N: return (getFromMap(pos.x, pos.y - 1) == wall);
		case W: return (getFromMap(pos.x - 1, pos.y) == wall);
		case E: return (getFromMap(pos.x + 1, pos.y) == wall);
		case S: return (getFromMap(pos.x, pos.y + 1) == wall);
		}
	}
	void setRandomTarget()
	{
		bool wallsAround[4] = { getFromMap(pos.x - 1, pos.y) == wall, getFromMap(pos.x, pos.y - 1) == wall, getFromMap(pos.x + 1, pos.y) == wall, getFromMap(pos.x, pos.y + 1) == wall }; //WNES
		if (wallsAround[0] + wallsAround[1] + wallsAround[2] + wallsAround[3] == 3) //TOFIX
		{
			target = turnDirectionAround(target);
			return;
		}
	start:
		switch (rand() % 4)
		{
		case 0:
			if (target == turnDirectionAround(W) || wallsAround[0])
				goto start;
			target = W;
			break;
		case 1:
			if (target == turnDirectionAround(N) || wallsAround[1])
				goto start;
			target = N;
			break;
		case 2:
			if (target == turnDirectionAround(E) || wallsAround[2])
				goto start;
			target = E;
			break;
		case 3:
			if (target == turnDirectionAround(S) || wallsAround[3])
				goto start;
			target = S;
			break;
		}
	}
	void moveToTarget()
	{
		switch (target)
		{
		case N: pos.y--; break;
		case W: pos.x--; break;
		case E: pos.x++; break;
		case S: pos.y++; break;
		}
	}
	void alifeTick()
	{
		direction seePlayer = seesPlayer();

		if (seePlayer != null)
			target = seePlayer;
		else if (monsterInTarget())
			setRandomTarget();
		
		moveToTarget();
	}
	bool inline catchPlayer()
	{
		return (pointsEqual(player.pos, monster.pos));
	}
}monster;


void generateGame()
{
	generateMap();
	player.pos.x = player.pos.y = mapXSize - 1;
	gameMap[mapXSize - 2][mapYSize - 1] = door;

	//monster.pos.x = mapXSize / 2; monster.pos.y = mapYSize / 2;
	monster.pos = { 1, 1 };
	//monster.setRandomTarget();
	/*for (int i = mapYSize / 2; i != 0; i--)
	{
		if (getFromMap(monster.pos.x, i) == none)
		{
			monster.pos.y = i;
			return;
		}
	}
	for (int i = mapXSize / 2; i != 0; i--)
	{
		if (getFromMap(i, monster.pos.y) == none)
		{
			monster.pos.x = i;
			return;
		}
	}*/
}


POINT inline cubeDPoint(LONG num) //TODOC
{
	LONG temp = screenSize.y / 48 * num;
	return { temp, temp };
}
POINT inline cubePoint(LONG fNum, LONG sNum)
{
	return { screenSize.y / 48 * fNum - 1, screenSize.y / 48 * sNum };
}
POINT inline cubePointVerticalMirrored(LONG num)
{
	return { screenSize.y / 48 * num - 1, screenSize.y / 48 * (48 - num) };
}
POINT inline cubePointHorizontalMirrored(LONG fNum, LONG sNum)
{
	return { screenSize.y / 48 * (48 - fNum) - 1, screenSize.y / 48 * sNum };
}
POINT inline cubePointVerticalAndHorizontalMirrored(LONG num)
{
	return { screenSize.y / 48 * (48 - num) - 1, screenSize.y / 48 * (48 - num) };
}
VERTICAL_TRAPEZE inline cubeSquareInCenter(LONG size)
{
	return {
		{ screenSize.y / 48 * (24 - size) - 1, screenSize.y / 48 * (24 - size) },
		{ screenSize.y / 48 * (24 - size) - 1, screenSize.y / 48 * (24 + size) },
		{ screenSize.y / 48 * (24 + size) - 1, screenSize.y / 48 * (24 + size) },
		{ screenSize.y / 48 * (24 + size) - 1, screenSize.y / 48 * (24 - size) }
	};
}
typedef enum sidesEnum {
	leftSide = 0,
	rightSide,
	frontSide
};
unsigned short wall2DSizes[6] = { 2, 8, 6, 4, 2, 1 };
unsigned short factorialSizes2DWalls[7] = { 0, 2, 2 + 8, 2 + 8 + 6, 2 + 8 + 6 + 4, 2 + 8 + 6 + 4 + 2, 2 + 8 + 6 + 4 + 2 + 1 };
VERTICAL_TRAPEZE getTrapezeCoords(short range, sidesEnum side)
{
	if (side == frontSide)
	{
		switch (range) //TOFIX
		{
		case 1:
			return cubeSquareInCenter(1); //1
		case 2:
			return cubeSquareInCenter(1 + 1); //2
		case 3:
			return cubeSquareInCenter(1 + 1 + 2); //4
		case 4:
			return cubeSquareInCenter(1 + 1 + 2 + 4); //8
		case 5:
			return cubeSquareInCenter(1 + 1 + 2 + 4 + 6); //14
		case 6:
			return cubeSquareInCenter(1 + 1 + 2 + 4 + 6 + 8); //22
		}
	}
	else if (side == leftSide)
		return { cubePoint(factorialSizes2DWalls[range - 1], factorialSizes2DWalls[range - 1]), cubePointVerticalMirrored(factorialSizes2DWalls[range - 1]), cubePointVerticalMirrored(factorialSizes2DWalls[range]), cubePoint(factorialSizes2DWalls[range], factorialSizes2DWalls[range]) };
	else if (side == rightSide)
		return { cubePointHorizontalMirrored(factorialSizes2DWalls[range - 1], factorialSizes2DWalls[range - 1]), cubePointVerticalAndHorizontalMirrored(factorialSizes2DWalls[range - 1]), cubePointVerticalAndHorizontalMirrored(factorialSizes2DWalls[range]), cubePointHorizontalMirrored(factorialSizes2DWalls[range], factorialSizes2DWalls[range]) };
}
void inline showWall(short range, sidesEnum side)
{
	trapeze(getTrapezeCoords(range, side), wallBrush);
}
int cubePos(int sizeInBlocks)
{
	return (screenSize.y / 48) * sizeInBlocks;
}
void inline showNone(short range, sidesEnum side)
{
	VERTICAL_TRAPEZE wallPrototype = getTrapezeCoords(range, side);
	wallPrototype.biggestBaseF.y = wallPrototype.smallestBaseF.y;
	wallPrototype.biggestBaseS.y = wallPrototype.smallestBaseS.y;
	trapeze(wallPrototype, noneBrush);
}
void inline showMonster(short range) //TOFIX
{
	VERTICAL_TRAPEZE wallPrototype = getTrapezeCoords(range, frontSide);
	HBRUSH monsterBrush = CreateSolidBrush(RGB(255, 0, 0));
	wallPrototype.biggestBaseF.y = wallPrototype.smallestBaseF.y;
	wallPrototype.biggestBaseS.y = wallPrototype.smallestBaseS.y;
	trapeze(wallPrototype, monsterBrush);
}
void inline showDoor(short range) //TOFIX
{
	VERTICAL_TRAPEZE wallPrototype = getTrapezeCoords(range, frontSide);
	HBRUSH doorBrush = CreateSolidBrush(RGB(0, 255, 0));
	wallPrototype.biggestBaseF.y = wallPrototype.smallestBaseF.y;
	wallPrototype.biggestBaseS.y = wallPrototype.smallestBaseS.y;
	trapeze(wallPrototype, doorBrush);
}
//debug zone
void inline showCube(unsigned int x, unsigned int y)
{
	Rectangle(mainWindowHDC, cubePos(x), cubePos(y), cubePos(x + 1), cubePos(y + 1));
}
void inline showCube(unsigned int x, unsigned int y, COLORREF color)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(color));
	Rectangle(mainWindowHDC, cubePos(x), cubePos(y), cubePos(x + 1), cubePos(y + 1));
	SelectObject(mainWindowHDC, oldBrush);
}
void inline showCube(POINT pos)
{
	Rectangle(mainWindowHDC, cubePos(pos.x), cubePos(pos.y), cubePos(pos.x + 1), cubePos(pos.y + 1));
}
void inline showCube(POINT pos, COLORREF color)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(color));
	Rectangle(mainWindowHDC, cubePos(pos.x), cubePos(pos.y), cubePos(pos.x + 1), cubePos(pos.y + 1));
	SelectObject(mainWindowHDC, oldBrush);
}
void debugShowMap() //TOFIX
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(RGB(rand()%255, rand() % 255, rand() % 255)));

	for (int i = 0; i < mapYSize; i++)
	{
		for (int j = 0; j < mapXSize; j++)
		{
			if (gameMap[i][j] == wall)
			{
				showCube(j, i);
			}
		}
	}
	showCube(player.pos, RGB(0, 255, 0));
	showCube(monster.pos, RGB(255, 0, 0));

	SelectObject(mainWindowHDC, hOldBrush);
}

void showGameCanvas()
{
	debugShowMap();
}

std::atomic_bool callGameTick = true;
void Game_Tick()
{
	srand(time(NULL));
	Sleep(1000);
	monster.alifeTick();
	refreshCanvas();
	if (monster.catchPlayer())
		presentGameState = lose;
	callGameTick = true;
}