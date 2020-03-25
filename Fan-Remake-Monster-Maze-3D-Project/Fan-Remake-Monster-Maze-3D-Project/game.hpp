#pragma once
#include <iostream>
#include <wingdi.h>
#include <time.h>
#include <conio.h>
#include "map.hpp"
using namespace std;
#include "output.hpp"
#define pointsEqual(f, s) (((f).x == (s).x) && ((f).y == (s).y))

typedef enum direction {
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
	case N: return E;
	case E: return S;
	case S: return W;
	case W: return N;
	}
}
class playerClass
{
public:
	POINT pos;
	direction viewDirection;
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

		if (map[goTo.x][goTo.y] != wall)
		{
			pos = goTo;
		}
	}
	void inline turnLeft()
	{
		viewDirection = turnDirectionLeft(viewDirection);
	}
	void inline turnRight()
	{
		viewDirection = turnDirectionRight(viewDirection);
	}
	void inline turnAround()
	{
		viewDirection = turnDirectionAround(viewDirection);
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
	POINT target;
	bool seesPlayer()
	{
		if (player.pos.x == monster.pos.x)
		{
			for (int i = monster.pos.x; map[i][pos.y] != wall; i++)
			{
				if (player.pos.x == i)
				{
					return true;
				}
			}
			for (int i = monster.pos.x; map[i][pos.y] != wall; i--)
			{
				if (player.pos.x == i)
				{
					return true;
				}
			}
		}
		if (player.pos.y == monster.pos.y)
		{
			for (int i = monster.pos.y; map[pos.x][i] != wall; i++)
			{
				if (player.pos.y == i)
				{
					return true;
				}
			}
			for (int i = monster.pos.y; map[pos.x][i] != wall; i++)
			{
				if (player.pos.y == i)
				{
					return true;
				}
			}
		}
		return false;
	}
	void setRandomTarget()
	{
		start:
		switch (rand() % 4)
		{
		case 0:
			for (int i = monster.pos.x; ; i++)
			{
				if (map[i][pos.y] == wall)
				{
					target.x = i - 1;
				}
			}
		case 1:
			for (int i = monster.pos.x; ; i--)
			{
				if (map[i][pos.y] == wall)
				{
					target.x = i + 1;
				}
			}
		case 2:
			for (int i = monster.pos.y; ; i++)
			{
				if (map[pos.x][i] == wall)
				{
					target.y = i - 1;
				}
			}
		case 3:
			for (int i = monster.pos.y; ; i--)
			{
				if (map[pos.x][i] == wall)
				{
					target.y = i + 1;
				}
			}
		}
		if (pointsEqual(pos, target))
			goto start;
	}
	void moveToTarget()
	{
		if (pos.x == target.x && pos.y > target.y)
			pos.y--;
		else if (pos.x == target.x && pos.y < target.y)
			pos.y++;
		else if (pos.x > target.x && pos.y == target.y)
			pos.x--;
		else if (pos.x < target.x && pos.y == target.y)
			pos.x++;
	}
	void alifeTick()
	{
		bool seePlayer = seesPlayer();
		bool monsterInTarget = pointsEqual(pos, target);

		if (seePlayer)
			target = player.pos;
		else if (monsterInTarget)
			setRandomTarget();

		moveToTarget();
	}
	bool inline catchPlayer()
	{
		return (abs(monster.pos.x - player.pos.x) == 1 || abs(monster.pos.y - player.pos.y) == 1);
	}
}monster;


void generateGame()
{
	generateMap();
	player.pos.x = player.pos.y = 1;
	map[mapXSize - 2][mapYSize - 1] = door;

	monster.pos.x = mapXSize / 2; monster.pos.y = mapYSize / 2;
	for (int i = mapYSize / 2; i != 0; i--)
	{
		if (map[monster.pos.x][i] == none)
		{
			monster.pos.y = i;
			return;
		}
	}
	for (int i = mapXSize / 2; i != 0; i--)
	{
		if (map[i][monster.pos.y] == none)
		{
			monster.pos.x = i;
			return;
		}
	}
	monster.setRandomTarget();
}



void debugShowMap() {
	for (int i = 0; i < mapYSize; i++) {
		for (int j = 0; j < mapXSize; j++)
			switch (map[i][j]) {
			case wall: std::cout << "1"; break;
			case none: std::cout << " "; break;
			}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	setTo(player.pos.x, player.pos.y);
	switch (player.viewDirection)
	{
	case N: cout << '^'; break;
	case W: cout << '<'; break;
	case E: cout << '>'; break;
	case S: cout << 'v'; break;
	}
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
unsigned short cubePos(unsigned short sizeInBlocks)
{
	return screenSize.y / 48 * sizeInBlocks - 1;
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

void showGameCanvas()
{
	PAINTSTRUCT ps;
	mainWindowHDC = BeginPaint(mainWindowHWND, &ps);

	showWall(1, leftSide);
	showNone(2, leftSide);
	showWall(3, leftSide);
	showNone(4, leftSide);
	showWall(5, leftSide);
	showNone(6, leftSide);

	showWall(1, rightSide);
	showNone(2, rightSide);
	showWall(3, rightSide);
	showNone(4, rightSide);
	showWall(5, rightSide);
	showNone(6, rightSide);

	showWall(1, frontSide);
	showMonster(2);
	showDoor(3);
	showWall(4, frontSide);
	showWall(5, frontSide);
	showWall(6, frontSide);

	EndPaint(mainWindowHWND, &ps);
}

typedef enum gameState
{
	win = 0,
	lose,
	inProcess
};
gameState Game_Tick()
{
	for (int i = 0; i < 20; i++, Sleep(1000 / 50))
	{
		if (bool(kbhit()))
		{
			char pressedKey = getch();
			switch (pressedKey)
			{
			case 'W':
			case 'w':
				player.moveForward();
				break;
			case 'Q':
			case 'q':
				player.turnLeft();
				break;
			case 'E':
			case 'e':
				player.turnRight();
				break;
			case 'S':
			case 's':
				player.turnAround();
				break;
			}
			showGameCanvas();
			if (monster.catchPlayer())
				return lose;
			if (player.inDoor())
				return win;
		}
	}
	monster.alifeTick();
	showGameCanvas();
	if (monster.catchPlayer())
		return lose;
	return inProcess;
}