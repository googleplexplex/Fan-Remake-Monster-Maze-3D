#pragma once
#include <iostream>
#include <wingdi.h>
#include <conio.h>
#include "map.hpp"
using namespace std;
#include "output.hpp"

typedef enum direction {
	N = 'N',
	W = 'W',
	E = 'E',
	S = 'S'
};
//-N-
//W-E
//-S-

direction inline turnLeft(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return W;
	case W: return S;
	case S: return E;
	case E: return N;
	}
}
direction inline turnRight(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return E;
	case E: return S;
	case S: return W;
	case W: return N;
	}
}
direction inline turnAround(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return E;
	case E: return S;
	case S: return W;
	case W: return N;
	}
}
class playerClass : public POINT
{
public:
	direction viewDirection;
	void makesMove()
	{
		//...
	}
}player;
void showCompas()
{
	setTo(1, 1);
	cout << '-' << player.viewDirection << '-';
	setTo(1, 2);
	cout << turnLeft(player.viewDirection) << '#' << turnRight(player.viewDirection);
	setTo(1, 3);
	cout << '-' << turnAround(player.viewDirection) << '-';
}
void showMap()
{
	setTo(1, 1);
	cout << '-' << player.viewDirection << '-';
	setTo(1, 2);
	cout << turnLeft(player.viewDirection) << '#' << turnRight(player.viewDirection);
	setTo(1, 3);
	cout << '-' << turnAround(player.viewDirection) << '-';
}

class monsterClass : public POINT
{
public:
	POINT target;
	bool seesPlayer()
	{
		if (player.x == monster.x)
		{
			for (int i = monster.x; map[i][y] != wall; i++)
			{
				if (player.x == i)
				{
					return true;
				}
			}
			for (int i = monster.x; map[i][y] != wall; i--)
			{
				if (player.x == i)
				{
					return true;
				}
			}
		}
		if (player.y == monster.y)
		{
			for (int i = monster.y; map[x][i] != wall; i++)
			{
				if (player.y == i)
				{
					return true;
				}
			}
			for (int i = monster.y; map[x][i] != wall; i++)
			{
				if (player.y == i)
				{
					return true;
				}
			}
		}
		return false;
	}
	void makesMove()
	{
		//...
	}
}monster;


void generateGame()
{
	generateMap();
	player.x = player.y = 1;
	map[mapXSize - 2][mapYSize - 1] = door;

	monster.x = mapXSize / 2; monster.y = mapYSize / 2;
	for (int i = mapYSize / 2; i != 0; i--)
	{
		if (map[monster.x][i] == none)
		{
			monster.y = i;
			return;
		}
	}
	for (int i = mapXSize / 2; i != 0; i--)
	{
		if (map[i][monster.y] == none)
		{
			monster.x = i;
			return;
		}
	}
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

	setTo(player.x, player.y);
	switch (player.viewDirection)
	{
	case N: cout << '^'; break;
	case W: cout << '<'; break;
	case E: cout << '>'; break;
	case S: cout << 'v'; break;
	}
}


POINT inline cubeDPoint(LONG num)
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
VERTICAL_TRAPEZE getTrapezeCoords(short range, sidesEnum side) //TODO
{
	switch (range)
	{
	case 1://size = 2
		switch (side)
		{
		case leftSide:
			return { cubePoint(0, 0), cubePointVerticalMirrored(0), cubePointVerticalMirrored(2), cubePoint(2, 2) };
		case rightSide:
			return { cubePointHorizontalMirrored(0, 0), cubePointVerticalAndHorizontalMirrored(0), cubePointVerticalAndHorizontalMirrored(2), cubePointHorizontalMirrored(2, 2) };
		case frontSide:
			return cubeSquareInCenter(1);
		}
	case 2: //size = 8
		switch (side)
		{
		case leftSide:
			return { cubePoint(2, 2), cubePointVerticalMirrored(2), cubePointVerticalMirrored(2 + 8), cubePoint(2 + 8, 2 + 8) };
		case rightSide:
			return { cubePointHorizontalMirrored(2, 2), cubePointVerticalAndHorizontalMirrored(2), cubePointVerticalAndHorizontalMirrored(2 + 8), cubePointHorizontalMirrored(2 + 8, 2 + 8) };
		case frontSide:
			return cubeSquareInCenter(1 + 1);
		}
	case 3: //size=6
		switch (side)
		{
		case leftSide:
			return { cubePoint(2 + 8, 2 + 8), cubePointVerticalMirrored(2 + 8), cubePointVerticalMirrored(2 + 8 + 6), cubePoint(2 + 8 + 6, 2 + 8 + 6) };
		case rightSide:
			return { cubePointHorizontalMirrored(2 + 8, 2 + 8), cubePointVerticalAndHorizontalMirrored(2 + 8), cubePointVerticalAndHorizontalMirrored(2 + 8 + 6), cubePointHorizontalMirrored(2 + 8 + 6, 2 + 8 + 6) };
		case frontSide:
			return cubeSquareInCenter(1 + 1 + 2);
		}
	case 4: //size=4
		switch (side)
		{
		case leftSide:
			return { cubePoint(2 + 8 + 6, 2 + 8 + 6), cubePointVerticalMirrored(2 + 8 + 6), cubePointVerticalMirrored(2 + 8 + 6 + 4), cubePoint(2 + 8 + 6 + 4, 2 + 8 + 6 + 4) };
		case rightSide:
			return { cubePointHorizontalMirrored(2 + 8 + 6, 2 + 8 + 6), cubePointVerticalAndHorizontalMirrored(2 + 8 + 6), cubePointVerticalAndHorizontalMirrored(2 + 8 + 6 + 4), cubePointHorizontalMirrored(2 + 8 + 6 + 4, 2 + 8 + 6 + 4) };
		case frontSide:
			return cubeSquareInCenter(1 + 1 + 2 + 4);
		}
	case 5: //size=2
		switch (side)
		{
		case leftSide:
			return { cubePoint(2 + 8 + 6 + 4, 2 + 8 + 6 + 4), cubePointVerticalMirrored(2 + 8 + 6 + 4), cubePointVerticalMirrored(2 + 8 + 6 + 4 + 2), cubePoint(2 + 8 + 6 + 4 + 2, 2 + 8 + 6 + 4 + 2) };
		case rightSide:
			return { cubePointHorizontalMirrored(2 + 8 + 6 + 4, 2 + 8 + 6 + 4), cubePointVerticalAndHorizontalMirrored(2 + 8 + 6 + 4), cubePointVerticalAndHorizontalMirrored(2 + 8 + 6 + 4 + 2), cubePointHorizontalMirrored(2 + 8 + 6 + 4 + 2, 2 + 8 + 6 + 4 + 2) };
		case frontSide:
			return cubeSquareInCenter(1 + 1 + 2 + 4 + 6);
		}
	case 6: //size=1
		switch (side)
		{
		case leftSide:
			return { cubePoint(2 + 8 + 6 + 4 + 2, 2 + 8 + 6 + 4 + 2), cubePointVerticalMirrored(2 + 8 + 6 + 4 + 2), cubePointVerticalMirrored(2 + 8 + 6 + 4 + 2 + 1), cubePoint(2 + 8 + 6 + 4 + 2 + 1, 2 + 8 + 6 + 4 + 2 + 1) };
		case rightSide:
			return { cubePointHorizontalMirrored(2 + 8 + 6 + 4 + 2, 2 + 8 + 6 + 4 + 2), cubePointVerticalAndHorizontalMirrored(2 + 8 + 6 + 4 + 2), cubePointVerticalAndHorizontalMirrored(2 + 8 + 6 + 4 + 2 + 1), cubePointHorizontalMirrored(2 + 8 + 6 + 4 + 2 + 1, 2 + 8 + 6 + 4 + 2 + 1) };
		case frontSide:
			return cubeSquareInCenter(1 + 1 + 2 + 4 + 6 + 8);
		}
	}
}
void inline showWall(short range, sidesEnum side)
{
	trapeze(getTrapezeCoords(range, side), wallBrush);
}
unsigned short cubePos(unsigned short sizeInBlocks)
{
	return screenSize.y / 48 * sizeInBlocks - 1;
}
void inline showNone(short range, sidesEnum side) //TODO
{
	VERTICAL_TRAPEZE wallPrototype = getTrapezeCoords(range, side);
	trapeze(getTrapezeCoords(range, side), noneBrush);
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
	showWall(2, rightSide);
	showWall(3, rightSide);
	showWall(4, rightSide);
	showWall(5, rightSide);
	showWall(6, rightSide);

	showWall(1, frontSide);
	showWall(2, frontSide);
	showWall(3, frontSide);
	showWall(4, frontSide);
	showWall(5, frontSide);
	showWall(6, frontSide);

	EndPaint(mainWindowHWND, &ps);
}

void Game_Tick()
{
	while (true)
	{
		showGameCanvas(); //Temp Debug
	}
}