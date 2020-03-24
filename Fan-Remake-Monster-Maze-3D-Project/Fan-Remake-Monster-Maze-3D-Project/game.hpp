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
			case wall: std::cout << "0"; break;
			case none: std::cout << " "; break;
			}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//int main(void)
//{
//	srand((unsigned)time(NULL));
//
//	trapeze({ { 0, 0 }, { 0, 200 } }, { { 40, 40 }, { 40, 160} }, grayBrush); //test
//
//	generateMap();
//	while (true)
//	{
//
//		debugShowMap();
//	}
//
//	return 0;
//}

unsigned int inline cubePos(unsigned int num)
{
	return screenSize.y / 48 * num;
}
POINT inline cubeDPoint(LONG num)
{
	LONG temp = screenSize.y / 48 * num;
	return { temp, temp };
}
POINT inline cubePoint(LONG fNum, LONG sNum)
{
	return { screenSize.y / 48 * fNum, screenSize.y / 48 * sNum };
}
typedef enum sidesEnum {
	leftSide = 0,
	rightSide,
	frontSide
};
VERTICAL_TRAPEZE getTrapezeCoords(short range, sidesEnum side) //TODO
{
	switch (range)
	{
	case 1:
		switch (side)
		{
		case leftSide:
			return { {0,0}, {0,screenSize.y}, cubePoint(2, 46), cubePoint(2, 2) };
		case rightSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} }; //TODO
		case frontSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		}
	case 2:
		switch (side)
		{
		case leftSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case rightSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case frontSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		}
	case 3:
		switch (side)
		{
		case leftSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case rightSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case frontSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		}
	case 4:
		switch (side)
		{
		case leftSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case rightSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case frontSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		}
	case 5:
		switch (side)
		{
		case leftSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case rightSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case frontSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		}
	case 6:
		switch (side)
		{
		case leftSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case rightSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		case frontSide:
			return { {0,0}, {0,0}, {0,0}, {0,0} };
		}
	}
}

void showGameCanvas()
{
	PAINTSTRUCT ps;
	mainWindowHDC = BeginPaint(mainWindowHWND, &ps);

	trapeze(getTrapezeCoords(1, leftSide), grayBrush); //Temp Debug
	trapeze(getTrapezeCoords(2, leftSide), grayBrush);
	trapeze(getTrapezeCoords(3, leftSide), grayBrush);
	trapeze(getTrapezeCoords(4, leftSide), grayBrush);
	trapeze(getTrapezeCoords(5, leftSide), grayBrush);
	trapeze(getTrapezeCoords(6, leftSide), grayBrush);

	trapeze(getTrapezeCoords(1, rightSide), grayBrush);
	trapeze(getTrapezeCoords(2, rightSide), grayBrush);
	trapeze(getTrapezeCoords(3, rightSide), grayBrush);
	trapeze(getTrapezeCoords(4, rightSide), grayBrush);
	trapeze(getTrapezeCoords(5, rightSide), grayBrush);
	trapeze(getTrapezeCoords(6, rightSide), grayBrush);

	trapeze(getTrapezeCoords(1, frontSide), grayBrush);
	trapeze(getTrapezeCoords(2, frontSide), grayBrush);
	trapeze(getTrapezeCoords(3, frontSide), grayBrush);
	trapeze(getTrapezeCoords(4, frontSide), grayBrush);
	trapeze(getTrapezeCoords(5, frontSide), grayBrush);
	trapeze(getTrapezeCoords(6, frontSide), grayBrush);

	EndPaint(mainWindowHWND, &ps);
}

void Game_Tick()
{
	while (true)
	{
		showGameCanvas(); //Temp Debug
	}
}