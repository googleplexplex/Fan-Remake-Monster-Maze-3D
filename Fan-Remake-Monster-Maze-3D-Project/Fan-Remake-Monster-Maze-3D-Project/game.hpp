#pragma once
#include <wingdi.h>
#include <time.h>
#include <atomic>
#include "map.hpp"
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

constexpr unsigned int monsterDelay = 1000;

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

		if (gameMap[goTo.x][goTo.y] != wall)
		{
			pos = goTo;

			if (player.inDoor())
				presentGameState = win;
		}

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
		return pointsEqual(pos, door);
	}
}player;
void showCompas() //TODO
{
	/*setTo(1, 1);
	cout << '-' << player.viewDirection << '-';
	setTo(1, 2);
	cout << turnDirectionLeft(player.viewDirection) << '#' << turnDirectionRight(player.viewDirection);
	setTo(1, 3);
	cout << '-' << turnDirectionAround(player.viewDirection) << '-';*/
}
void showMap() //TODO
{
	/*setTo(1, 1);
	cout << '-' << player.viewDirection << '-';
	setTo(1, 2);
	cout << turnDirectionLeft(player.viewDirection) << '#' << turnDirectionRight(player.viewDirection);
	setTo(1, 3);
	cout << '-' << turnDirectionAround(player.viewDirection) << '-';*/
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
		positionInfoStruct positionInfo(monster.pos); //Получаем всю информацию о окружении монстра

		if (positionInfo.sumOfPasses() == 1 && positionInfo.passDescriptions[(int)turnDirectionAround(target)].isPass) //Если место откуда мы вернулись не тупик, и других путей нет...
		{
			target = turnDirectionAround(target); //Разворачиваемся
			passDescriptionStruct wayBackDescription = positionInfo.passDescriptions[(int)target];
			targetRange = wayBackDescription.ranges[rand() % wayBackDescription.rangesCount];
			return;
		}

		//Если путей, учитывая обратный, несколько...
		positionInfo.passDescriptions[(int)turnDirectionAround(target)].isPass = false;
		if (positionInfo.sumOfPasses() == 1) //Если путей, не учитывая обратный, только один...
		{
			target = (direction)positionInfo.getFirstPass(); //Идём в него
			passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
			targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
			return;
		}

		//Если путей, не учитывая обратный, несколько...
		target = (direction)positionInfo.getRandomPass(); //Идём в случайный из них
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
		positionInfoStruct positionInfo(monster.pos); //Получаем всю информацию о окружении монстра

		if (positionInfo.sumOfPasses() == 1) //Если путь из нынешней точки, где монстр появился, только один...
		{
			target = (direction)positionInfo.getFirstPass(); //Идём в него
			passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
			targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
			return;
		}

		//Если таких путей несколько...
		target = (direction)positionInfo.getRandomPass(); //Идём в случайный из них
		passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
		targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
	}
}monster;



void generateGame()
{
	generateMap();
	player.pos.x = player.pos.y = 1; //TOFIX
	monster.pos = { mapXSize / 2, mapYSize / 2 };
	for (int i = mapYSize / 2; i != 0; i--)
	{
		if (gameMap[monster.pos.x][i] == none)
		{
			monster.pos.y = i;
			break;
		}
	}
	for (int i = mapXSize / 2; i != 0; i--)
	{
		if (gameMap[i][monster.pos.y] == none)
		{
			monster.pos.x = i;
			break;
		}
	}
	monster.setFirstTarget();
}


//TOLIB
void debugShowMap() //TOFIX
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));

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
	int rangeViewedPass = 0;
	switch (player.viewDirection)
	{
	case N:
		for (; gameMap[player.pos.x][player.pos.y - rangeViewedPass] != wall && rangeViewedPass < 6; rangeViewedPass++)
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
		if (door.x == player.pos.x && player.pos.y - rangeViewedPass == door.y)
			showDoor(rangeViewedPass);
		if (monster.pos.x == player.pos.x && player.pos.y - rangeViewedPass == monster.pos.y)
			showDoor(rangeViewedPass);
		break;
	case W:
		for (; gameMap[player.pos.x - rangeViewedPass][player.pos.y] != wall && rangeViewedPass < 6; rangeViewedPass++)
		{
			if (gameMap[player.pos.x - rangeViewedPass][player.pos.y + 1] == wall)
				showWall(rangeViewedPass, leftSide);
			else
				showNone(rangeViewedPass, leftSide);
			if (gameMap[player.pos.x - rangeViewedPass][player.pos.y - 1] == wall)
				showWall(rangeViewedPass, rightSide);
			else
				showNone(rangeViewedPass, rightSide);
		}
		if (door.y == player.pos.y && player.pos.x - rangeViewedPass == door.x)
			showDoor(rangeViewedPass);
		if (monster.pos.y == player.pos.y && player.pos.x - rangeViewedPass == monster.pos.x)
			showDoor(rangeViewedPass);
		break;
	case E:
		for (; gameMap[player.pos.x + rangeViewedPass][player.pos.y] != wall && rangeViewedPass < 6; rangeViewedPass++)
		{
			if (gameMap[player.pos.x + rangeViewedPass][player.pos.y - 1] == wall)
				showWall(rangeViewedPass, leftSide);
			else
				showNone(rangeViewedPass, leftSide);
			if (gameMap[player.pos.x + rangeViewedPass][player.pos.y + 1] == wall)
				showWall(rangeViewedPass, rightSide);
			else
				showNone(rangeViewedPass, rightSide);
		}
		if (door.y == player.pos.y && player.pos.x + rangeViewedPass == door.x)
			showDoor(rangeViewedPass);
		if (monster.pos.y == player.pos.y && player.pos.x + rangeViewedPass == monster.pos.x)
			showDoor(rangeViewedPass);
		break;
	case S:
		for (; gameMap[player.pos.x][player.pos.y + rangeViewedPass] != wall && rangeViewedPass < 6; rangeViewedPass++)
		{
			if (gameMap[player.pos.x + 1][player.pos.y + rangeViewedPass] == wall)
				showWall(rangeViewedPass, leftSide);
			else
				showNone(rangeViewedPass, leftSide);
			if (gameMap[player.pos.x - 1][player.pos.y + rangeViewedPass] == wall)
				showWall(rangeViewedPass, rightSide);
			else
				showNone(rangeViewedPass, rightSide);
		}
		if (door.x == player.pos.x && player.pos.y + rangeViewedPass == door.y)
			showDoor(rangeViewedPass);
		if (monster.pos.x == player.pos.x && player.pos.y + rangeViewedPass == monster.pos.y)
			showDoor(rangeViewedPass);
		break;
	}
	if (rangeViewedPass < 6)
		showWall(6 - rangeViewedPass + 1, frontSide);
	else
		showNone(1, frontSide);
}


std::atomic_bool callGameTick = true;
void Game_Tick()
{
	refreshCanvas();
	srand(time(NULL));
	Sleep(monsterDelay);
	monster.alifeTick();
	refreshCanvas();
	if (monster.catchPlayer())
		presentGameState = lose;
	callGameTick = true;
}