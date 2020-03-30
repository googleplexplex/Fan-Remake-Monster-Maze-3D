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
	typedef enum passTypeStruct
	{
		let = 0,
		deadEnd,
		corridor,
		cameBackFromHere,
		turn
	};
	struct passDescription
	{
		passTypeStruct passType;
		unsigned short range;
	};
	passDescription getPassDescription(direction searchedDirection)
	{
		unsigned short rangeToWall = 1;
		passTypeStruct passType = deadEnd;
		switch (searchedDirection)
		{
		case N:
			if (gameMap[pos.x][pos.y - 1] == wall)
				return { let, 0 };
			while (gameMap[pos.x][pos.y - rangeToWall] != wall)
			{
				if (passType == deadEnd && (gameMap[pos.x - 1][pos.y - rangeToWall] == none || gameMap[pos.x + 1][pos.y - rangeToWall] == none))
				{
					if (gameMap[pos.x][pos.y - rangeToWall - 1] == wall)
						passType = turn;
					else
						passType = corridor;
				}
				rangeToWall++;
			}
			break;
		case W:
			if (gameMap[pos.x - 1][pos.y] == wall)
				return { let, 0 };
			while (gameMap[pos.x - rangeToWall][pos.y] != wall)
			{
				if (passType == deadEnd && (gameMap[pos.x - rangeToWall][pos.y - 1] == none || gameMap[pos.x - rangeToWall][pos.y + 1] == none))
				{
					if (gameMap[pos.x - rangeToWall - 1][pos.y] == wall)
						passType = turn;
					else
						passType = corridor;
				}
				rangeToWall++;
			}
			break;
		case E:
			if (gameMap[pos.x + 1][pos.y] == wall)
				return { let, 0 };
			while (gameMap[pos.x + rangeToWall][pos.y] != wall)
			{
				if (passType == deadEnd && (gameMap[pos.x + rangeToWall][pos.y - 1] == none || gameMap[pos.x + rangeToWall][pos.y + 1] == none))
				{
					if (gameMap[pos.x + rangeToWall + 1][pos.y] == wall)
						passType = turn;
					else
						passType = corridor;
				}
				rangeToWall++;
			}
			break;
		case S:
			if (gameMap[pos.x][pos.y + 1] == wall)
				return { let, 0 };
			while (gameMap[pos.x][pos.y + rangeToWall] != wall)
			{
				if (passType == deadEnd && (gameMap[pos.x - 1][pos.y + rangeToWall] == none || gameMap[pos.x + 1][pos.y + rangeToWall] == none))
				{
					if (gameMap[pos.x][pos.y + rangeToWall + 1] == wall)
						passType = turn;
					else
						passType = corridor;
				}
				rangeToWall++;
			}
			break;
		}
		return { passType, --rangeToWall };
	}
	unsigned short searchRangeToRandomCorridor(direction searchedDirection, int range) //range to random corridor
	{
	reRandom:
		switch (searchedDirection)
		{
		case N:
			for (int i = 1; i <= range; i++)
			{
				if ((bool)rand() % 2 && (gameMap[pos.x - 1][pos.y - i] == none || gameMap[pos.x + 1][pos.y - i] == none))
				{
					return i;
				}
			}
			goto reRandom;
		case W:
			for (int i = 1; i <= range; i++)
			{
				if ((bool)rand() % 2 && (gameMap[pos.x - i][pos.y - 1] == none || gameMap[pos.x - i][pos.y + 1] == none))
				{
					return i;
				}
			}
			goto reRandom;
		case E:
			for (int i = 1; i <= range; i++)
			{
				if ((bool)rand() % 2 && (gameMap[pos.x + i][pos.y - 1] == none || gameMap[pos.x + i][pos.y + 1] == none))
				{
					return i;
				}
			}
			goto reRandom;
		case S:
			for (int i = 1; i <= range; i++)
			{
				if ((bool)rand() % 2 && (gameMap[pos.x - 1][pos.y + i] == none || gameMap[pos.x + 1][pos.y + i] == none))
				{
					return i;
				}
			}
			goto reRandom;
		}
	}
	int firstPassNumOfType(passDescription passesDescriptions[4], passTypeStruct searchedPassType)
	{
		for (int i = 0; i < 4; i++)
		{
			if (passesDescriptions[i].passType == searchedPassType)
			{
				return i;
			}
		}
		return -1;
	}
	int randomPassNumOfType(passDescription passesDescriptions[4], passTypeStruct searchedPassType)
	{
		int numbersOfPasses[4];
		int numbersOfPassesCount = 0;

		for (int i = 0; i < 4; i++)
		{
			if (passesDescriptions[i].passType == searchedPassType)
			{
				numbersOfPasses[numbersOfPassesCount++] = i;
			}
		}

		if (numbersOfPassesCount == 0)
			return -1;

		while (true)
		{
			for (int i = 0; i < numbersOfPassesCount; i++)
			{
				if (bool(rand() % 2))
				{
					return numbersOfPasses[i];
				}
			}
		}
	}
	void setRandomTarget()
	{
		passDescription passesDescriptions[4] = { getPassDescription(N), getPassDescription(W), getPassDescription(E), getPassDescription(S) }; //Получаем информацию о окружении монстра
		unsigned short deadEndsCount = 0;
		unsigned short letsCount = 0;
		unsigned short coridorsCount = 0;
		unsigned short turnsCount = 0;
		unsigned short corridorsFromWhichWeReturnedCount = 0;

		for (int i = 0; i < 4; i++)
		{
			switch (passesDescriptions[i].passType)
			{
			case let:
				letsCount++;
				break;
			case deadEnd:
				deadEndsCount++;
				break;
			case corridor:
				coridorsCount++;
				break;
			case turn:
				turnsCount++;
				break;
			}
		}

		if (letsCount + deadEndsCount + corridorsFromWhichWeReturnedCount == 3) //Если сумма сторон, на которых стены и тупики = 3, значит единственный путь - вернутся
		{
			direction onlyWayDirection = turnDirectionAround(target);
			target = onlyWayDirection;
			if (passesDescriptions[(int)onlyWayDirection].passType == turn)
				targetRange = passesDescriptions[(int)onlyWayDirection].range;
			else
				targetRange = searchRangeToRandomCorridor(target, passesDescriptions[(int)onlyWayDirection].range);
			return;
		}

		passesDescriptions[(int)turnDirectionAround(target)].passType = cameBackFromHere; //Место из которого мы пришли - точно корридор
		corridorsFromWhichWeReturnedCount++;

		if (letsCount + deadEndsCount + corridorsFromWhichWeReturnedCount == 3) //Если сумма сторон, на которых стены, тупики и из которых мы пришли = 3, значит единственный путь - последний оставшийся
		{
			direction onlyCorridorDirection = (direction)firstPassNumOfType(passesDescriptions, turn); //TOFIX *random
			target = onlyCorridorDirection;
			if (onlyCorridorDirection != -1)
				targetRange = passesDescriptions[(int)onlyCorridorDirection].range;
			else
				targetRange = searchRangeToRandomCorridor(target, passesDescriptions[(int)onlyCorridorDirection].range);
			return;
		}

		direction randomCorridorDirection = (direction)randomPassNumOfType(passesDescriptions, corridor); //Оставшийся вариант - проходов несколько. Идём в случайный из
		target = randomCorridorDirection;
		targetRange = searchRangeToRandomCorridor(target, passesDescriptions[(int)randomCorridorDirection].range);
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
		passDescription passesDescriptions[4] = { getPassDescription(N), getPassDescription(W), getPassDescription(E), getPassDescription(S) }; //Получаем информацию о окружении монстра

		direction onlyWayDirection = (direction)randomPassNumOfType(passesDescriptions, turn); //И идём в случайную из сторон, где нет стены или тупика
		if (onlyWayDirection != -1)
		{
			target = onlyWayDirection;
			targetRange = passesDescriptions[onlyWayDirection].range;
			return;
		}

		onlyWayDirection = (direction)randomPassNumOfType(passesDescriptions, corridor); //И идём в случайную из сторон, где нет стены или тупика
		target = onlyWayDirection;
		targetRange = searchRangeToRandomCorridor(target, passesDescriptions[(int)onlyWayDirection].range);
	}
}monster;



void generateGame()
{
	generateMap();
	player.pos.x = player.pos.y = mapXSize - 1;
	gameMap[mapXSize - 2][mapYSize - 1] = door;

	//monster.pos.x = mapXSize / 2; monster.pos.y = mapYSize / 2;
	monster.pos = { 1, 1 };
	refreshCanvas();
	monster.setFirstTarget();
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