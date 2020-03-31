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
	N = 0,
	W = 1,
	E = 2,
	S = 3,
	null
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
	default:
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
	default:
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
	default:
	case W: return E;
	}
}
POINT door;
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
void showCompas() //TODO
{
	setTo(1, 1);
	cout << '-' << player.viewDirection << '-';
	setTo(1, 2);
	cout << turnDirectionLeft(player.viewDirection) << '#' << turnDirectionRight(player.viewDirection);
	setTo(1, 3);
	cout << '-' << turnDirectionAround(player.viewDirection) << '-';
}
void showMap() //TODO
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
	direction target = null;
	unsigned short targetRange = 0;
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
	bool inline monsterInTarget()
	{
		return (targetRange == 0);
	}

	struct passDescriptionStruct
	{
		bool isPass = false;
		unsigned short* ranges;
		unsigned short rangesCount = 0;
		void addRange(unsigned short addedRange)
		{
			unsigned short* temp = ranges;
			ranges = new unsigned short[rangesCount + 1];
			memcpy(ranges, temp, rangesCount * sizeof(unsigned short));
			ranges[rangesCount++] = addedRange;
		}
		void fill(POINT scannedPos, direction scannedDirection)
		{
			unsigned short range = 1;
			switch (scannedDirection)
			{
			case N:
				while (gameMap[scannedPos.x][scannedPos.y - range] != wall)
				{
					if (gameMap[scannedPos.x - 1][scannedPos.y - range] == none || gameMap[scannedPos.x + 1][scannedPos.y - range] == none)
					{
						isPass = true;
						addRange(range);
					}
					range++;
				}
				return;
			case W:
				while (gameMap[scannedPos.x - range][scannedPos.y] != wall)
				{
					if (gameMap[scannedPos.x - range][scannedPos.y - 1] == none || gameMap[scannedPos.x - range][scannedPos.y + 1] == none)
					{
						isPass = true;
						addRange(range);
					}
					range++;
				}
				return;
			case E:
				while (gameMap[scannedPos.x + range][scannedPos.y] != wall)
				{
					if (gameMap[scannedPos.x + range][scannedPos.y - 1] == none || gameMap[scannedPos.x + range][scannedPos.y + 1] == none)
					{
						isPass = true;
						addRange(range);
					}
					range++;
				}
				return;
			case S:
				while (gameMap[scannedPos.x][scannedPos.y + range] != wall)
				{
					if (gameMap[scannedPos.x - 1][scannedPos.y + range] == none || gameMap[scannedPos.x + 1][scannedPos.y + range] == none)
					{
						isPass = true;
						addRange(range);
					}
					range++;
				}
				return;
			}
		}
	};
	struct positionInfoStruct
	{
		passDescriptionStruct passDescriptions[4];
		positionInfoStruct(POINT scannedPoint)
		{
			for (int i = 0; i < 4; i++)
			{
				passDescriptions[i].fill(scannedPoint, (direction)i);
			}
		}
		unsigned short sumOfPasses()
		{
			unsigned short res = 0;
			for (int i = 0; i < 4; i++)
			{
				if (passDescriptions[i].isPass)
					res++;
			}
			return res;
		}
		int getFirstPass()
		{
			for (int i = 0; i < 4; i++)
			{
				if (passDescriptions[i].isPass)
					return i;
			}
		}
		int getRandomPass()
		{
			rerandom:
			for (int i = 0; i < 4; i++)
			{
				if (passDescriptions[i].isPass && bool(rand() % 2))
					return i;
			}
			goto rerandom;
		}
	};
	void setRandomTarget()
	{
		positionInfoStruct positionInfo(monster.pos);

		if (positionInfo.sumOfPasses() == 1 && positionInfo.passDescriptions[(int)turnDirectionAround(target)].isPass)
		{
			target = turnDirectionAround(target);
			passDescriptionStruct wayBackDescription = positionInfo.passDescriptions[(int)target];
			targetRange = wayBackDescription.ranges[rand() % wayBackDescription.rangesCount];
			return;
		}

		positionInfo.passDescriptions[(int)turnDirectionAround(target)].isPass = false;
		if (positionInfo.sumOfPasses() == 1)
		{
			target = (direction)positionInfo.getFirstPass();
			passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
			targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
			return;
		}

		target = (direction)positionInfo.getRandomPass();
		passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
		targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
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
		targetRange--;
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
	void setFirstTarget()
	{
		positionInfoStruct positionInfo(monster.pos);

		if (positionInfo.sumOfPasses() == 1)
		{
			target = (direction)positionInfo.getFirstPass();
			passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
			targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
			return;
		}

		target = (direction)positionInfo.getRandomPass();
		passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
		targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
	}
}monster;



void generateGame()
{
	generateMap();
	player.pos.x = player.pos.y = mapXSize - 1;

	monster.pos.x = mapXSize / 2; monster.pos.y = mapYSize / 2;
	//monster.pos = { 1, 1 };
	for (int i = mapYSize / 2; i != 0; i--)
	{
		if (getFromMap(monster.pos.x, i) == none)
		{
			monster.pos.y = i;
			break;
		}
	}
	for (int i = mapXSize / 2; i != 0; i--)
	{
		if (getFromMap(i, monster.pos.y) == none)
		{
			monster.pos.x = i;
			break;
		}
	}
	monster.setFirstTarget();
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
			if (gameMap[j][i] == wall)
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
	switch (player.viewDirection)
	{
	case N:
		int rangeViewedPass = 0;
		for (; gameMap[player.pos.x][player.pos.y - rangeViewedPass] != wall; rangeViewedPass++)
		{
			if (gameMap[player.pos.x - 1][player.pos.y - rangeViewedPass] == wall)
				showWall(rangeViewedPass, leftSide);
			else
				showNone(rangeViewedPass, leftSide);
			if (gameMap[player.pos.x + 1][player.pos.y - rangeViewedPass] == wall)
				showWall(rangeViewedPass, rightSide);
			else
				showNone(rangeViewedPass, rightSide);
		}
		showWall(rangeViewedPass, frontSide);
		if (door.x == player.pos.x && player.pos.y - rangeViewedPass == door.y)
			showDoor(rangeViewedPass);
		if (monster.pos.x == player.pos.x && player.pos.y - rangeViewedPass == monster.pos.y)
			showDoor(rangeViewedPass);
		break;
	case W:
		break;
	case E:
		break;
	case S:
		break;
	}

	debugShowMap();
}

std::atomic_bool callGameTick = true;
void Game_Tick()
{
	refreshCanvas();
	srand(time(NULL));
	Sleep(1000);
	monster.alifeTick();
	refreshCanvas();
	if (monster.catchPlayer())
		presentGameState = lose;
	callGameTick = true;
}