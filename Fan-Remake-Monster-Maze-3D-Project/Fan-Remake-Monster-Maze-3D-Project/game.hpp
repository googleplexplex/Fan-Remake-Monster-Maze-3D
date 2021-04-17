#pragma once
#include <windows.h>
#include <time.h>
#include <math.h>
#include <atomic>
#include "map.hpp"
#include "output.hpp"
#include "direction.hpp"
POINT GetTextExtentPoint32Size(const char* str);
#define pointsEqual(f, s) (((f).x == (s).x) && ((f).y == (s).y))
#define sqr(x) ((x)*(x))


typedef enum playerBlock
{
	wasnot = 0,
	was = 1
};
playerBlock playerMap[mapSize.x][mapSize.y];
class playerClass
{
public:
	POINT pos;
	direction viewDirection = S;
	void moveForward()
	{
		POINT goTo = moveDirectionForward(pos, viewDirection);

		if (gameMap[goTo.x][goTo.y] != wall)
		{
			pos = goTo;

			if (gameMap[goTo.x][goTo.y] == door)
				goToPage(winPage);
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
}player;

constexpr POINT compasPos = { 520, 20 };
constexpr POINT compasFontSize = { 8, 16 };
constexpr POINT compasPosIndentation = { 20, 20 };
#define relativeRectangle(x, y, xsize, ysize) Rectangle(mainWindowHDC, x, y, x + xsize, y + ysize)
#define relativeRectangle(x, y, xsize, ysize) Rectangle(mainWindowHDC, x, y, x + xsize, y + ysize)
LPSTR toString(char charset)
{
	LPSTR result = new char[2];
	result[0] = charset;
	result[1] = NULL;
	return result;
}
void showCompas() //TODO
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)));

	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));

	relativeRectangle(compasPos.x + (0 * compasPosIndentation.x), compasPos.y + (0 * compasPosIndentation.y), compasFontSize.x, compasFontSize.y);
	TextOut(mainWindowHDC, compasPos.x + (1 * compasPosIndentation.x), compasPos.y + (0 * compasPosIndentation.y), toString(directionName(player.viewDirection)), 1);
	relativeRectangle(compasPos.x + (2 * compasPosIndentation.x), compasPos.y + (0 * compasPosIndentation.y), compasFontSize.x, compasFontSize.y);

	TextOut(mainWindowHDC, compasPos.x + (0 * compasPosIndentation.x), compasPos.y + (1 * compasPosIndentation.y), toString(directionName(turnDirectionLeft(player.viewDirection))), 1);
	relativeRectangle(compasPos.x + (1 * compasPosIndentation.x), compasPos.y + (1 * compasPosIndentation.y), compasFontSize.x, compasFontSize.y);
	TextOut(mainWindowHDC, compasPos.x + (2 * compasPosIndentation.x), compasPos.y + (1 * compasPosIndentation.y), toString(directionName(turnDirectionRight(player.viewDirection))), 1);

	relativeRectangle(compasPos.x + (0 * compasPosIndentation.x), compasPos.y + (2 * compasPosIndentation.y), compasFontSize.x, compasFontSize.y);
	TextOut(mainWindowHDC, compasPos.x + (1 * compasPosIndentation.x), compasPos.y + (2 * compasPosIndentation.y), toString(directionName(turnDirectionAround(player.viewDirection))), 1);
	relativeRectangle(compasPos.x + (2 * compasPosIndentation.x), compasPos.y + (2 * compasPosIndentation.y), compasFontSize.x, compasFontSize.y);

	SelectObject(mainWindowHDC, oldBrush);
}
double inline getRangeBehind(POINT f, POINT s)
{
	return sqrt(sqr(s.x - f.x) + sqr(s.y - f.y));
}


block getObject(int x, int y);
block getObject(POINT gettedPoint);
void showObject(block obj, short range, sidesEnum side);
class monsterClass
{
public:
	POINT pos;
	direction target = null;
	unsigned short targetRange = 0;
	const unsigned int speed = 1000;
	bool seeThePlayer = false;
	std::pair<direction, unsigned short> seesPlayer() //return null, if false, direction if true
	{
		unsigned short rangeToPlayer = 1;
		if (player.pos.x == monster.pos.x)
		{
			if (player.pos.y > monster.pos.y)
			{
				for (; monster.pos.y + rangeToPlayer < player.pos.y; rangeToPlayer++)
				{
					if (gameMap[monster.pos.x][monster.pos.y + rangeToPlayer] == wall)
						return { null, NULL };
				}
				return { S, rangeToPlayer };
			}
			else {
				for (; monster.pos.y - rangeToPlayer > player.pos.y; rangeToPlayer++)
				{
					if (gameMap[monster.pos.x][monster.pos.y - rangeToPlayer] == wall)
						return { null, NULL };
				}
				return { N, rangeToPlayer };
			}
		}
		else if (player.pos.y == monster.pos.y)
		{
			if (player.pos.x > monster.pos.x)
			{
				for (; monster.pos.x + rangeToPlayer < player.pos.x; rangeToPlayer++)
				{
					if (gameMap[monster.pos.x + rangeToPlayer][monster.pos.y] == wall)
						return { null, NULL };
				}
				return { E, rangeToPlayer };
			}
			else {
				for (; monster.pos.x - rangeToPlayer > player.pos.x; rangeToPlayer++)
				{
					if (gameMap[monster.pos.x - rangeToPlayer][monster.pos.y] == wall)
						return { null, NULL };
				}
				return { W, rangeToPlayer };
			}
		}
		return { null, NULL };
	}
	bool inline monsterInTarget()
	{
		if (seeThePlayer)
			seeThePlayer = false;
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
			POINT presentScannedPos = moveDirectionForward(scannedPos, scannedDirection);

			while (getObject(presentScannedPos) != wall)
			{
				POINT viewPointLeft = moveDirectionLeft(presentScannedPos, scannedDirection);
				POINT viewPointRight = moveDirectionRight(presentScannedPos, scannedDirection);
				if (getObject(viewPointLeft) == none || getObject(viewPointRight) == none)
				{
					isPass = true;
					addRange(range);
				}

				presentScannedPos = moveDirectionForward(presentScannedPos, scannedDirection);
				range++;
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
		monster.pos = moveDirectionForward(monster.pos, target);
		targetRange--;
	}
	void alifeTick()
	{
		std::pair<direction, unsigned short> seePlayer = seesPlayer();

		if (seePlayer.first != null)
		{
			seeThePlayer = true;
			target = seePlayer.first;
			targetRange = seePlayer.second;
		}
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
constexpr const char* monsterStatuses[4] = { "RUN", "I hear steps...", "He is close", "He is far" };
void showStatus()
{
	if (monster.seesPlayer().first != null)
	{
		TextOutA(mainWindowHDC,
			screenSize.y / 2 - (GetTextExtentPoint32Size(monsterStatuses[0]).x) / 2,
			screenSize.y - GetTextExtentPoint32Size(monsterStatuses[0]).y - 30,
			monsterStatuses[0], strlen(monsterStatuses[0]));
		return;
	}

	unsigned int rangeBehingPlayerAndMonster = getRangeBehind(player.pos, monster.pos);
	if (rangeBehingPlayerAndMonster <= 5) {
		TextOutCenterGame(mainWindowHDC, screenSize.y - GetTextExtentPoint32Size(monsterStatuses[1]).y - 30, monsterStatuses[1]);
	}
	else if (rangeBehingPlayerAndMonster <= 10) {
		TextOutCenterGame(mainWindowHDC, screenSize.y - GetTextExtentPoint32Size(monsterStatuses[2]).y - 30, monsterStatuses[2]);
	}
	else {
		TextOutCenterGame(mainWindowHDC, screenSize.y - GetTextExtentPoint32Size(monsterStatuses[3]).y - 30, monsterStatuses[3]);
	}
}

//TOLIB
void inline clearPlayerMap()
{
	for (int i = 0; i < mapSize.x; i++)
	{
		for (int j = 0; j < mapSize.y; j++)
		{
			playerMap[i][j] = wasnot;
		}
	}
}
void inline updatePlayerMap(POINT updatedPoint)
{
	if (playerMap[updatedPoint.x][updatedPoint.y] == wasnot)
		playerMap[updatedPoint.x][updatedPoint.y] = was;
}
const POINT playerMapBlockSize = { 12, 12 };
const POINT playerMapPosOffset = { 15, 20 };
const POINT playerMapPos = { screenSize.x - mapSize.x * playerMapBlockSize.x - playerMapPosOffset.x, screenSize.y - mapSize.y * playerMapBlockSize.y - playerMapPosOffset.y };
const COLORREF playerMapBlockColor = RGB(255, 255, 255);
const COLORREF playerMapColor = RGB(0, 255, 0);
const COLORREF monsterMapColor = RGB(255, 0, 0);
void inline showPlayerBlock(int x, int y)
{
	POINT posBlockOnScreen = { playerMapPos.x + playerMapBlockSize.x * x, playerMapPos.y + playerMapBlockSize.y * y };
	Rectangle(mainWindowHDC, posBlockOnScreen.x, posBlockOnScreen.y, posBlockOnScreen.x + playerMapBlockSize.x, posBlockOnScreen.y + playerMapBlockSize.y);
}

void showPlayerMap()
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(playerMapBlockColor));
	if (usedCheats)
	{
		for (int i = 0; i < mapSize.y; i++)
		{
			for (int j = 0; j < mapSize.x; j++)
			{
				if (gameMap[j][i] == wall)
				{
					showPlayerBlock(j, i);
				}
			}
		}
	}
	else {
		for (int i = 0; i < mapSize.y; i++)
		{
			for (int j = 0; j < mapSize.x; j++)
			{
				if (playerMap[j][i] == was)
				{
					showPlayerBlock(j, i);
				}
			}
		}
	}
	SelectObject(mainWindowHDC, oldBrush);

	oldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(playerMapColor));

	POINT playerCursorPoint_left_up = { playerMapPos.x + playerMapBlockSize.x * player.pos.x, playerMapPos.y + playerMapBlockSize.y * player.pos.y };
	POINT playerCursorPoint_right_up = { playerMapPos.x + playerMapBlockSize.x * (player.pos.x + 1), playerMapPos.y + playerMapBlockSize.y * player.pos.y };
	POINT playerCursorPoint_left_down = { playerMapPos.x + playerMapBlockSize.x * player.pos.x, playerMapPos.y + playerMapBlockSize.y * (player.pos.y + 1) };
	POINT playerCursorPoint_right_down = { playerMapPos.x + playerMapBlockSize.x * (player.pos.x + 1), playerMapPos.y + playerMapBlockSize.y * (player.pos.y + 1) };

	POINT playerCursorPoint_up = { playerMapPos.x + playerMapBlockSize.x * player.pos.x + playerMapBlockSize.x * 0.5, playerMapPos.y + playerMapBlockSize.y * player.pos.y };
	POINT playerCursorPoint_left = { playerMapPos.x + playerMapBlockSize.x * player.pos.x, playerMapPos.y + playerMapBlockSize.y * player.pos.y + playerMapBlockSize.y * 0.5 };
	POINT playerCursorPoint_down = { playerMapPos.x + playerMapBlockSize.x * player.pos.x + playerMapBlockSize.x * 0.5, playerMapPos.y + playerMapBlockSize.y * (player.pos.y + 1) };
	POINT playerCursorPoint_right = { playerMapPos.x + playerMapBlockSize.x * (player.pos.x + 1), playerMapPos.y + playerMapBlockSize.y * player.pos.y + playerMapBlockSize.y * 0.5 };

	POINT playerCursor_up[3] = { playerCursorPoint_up, playerCursorPoint_left_down, playerCursorPoint_right_down };
	POINT playerCursor_left[3] = { playerCursorPoint_left, playerCursorPoint_right_up, playerCursorPoint_right_down };
	POINT playerCursor_right[3] = { playerCursorPoint_right, playerCursorPoint_left_up, playerCursorPoint_left_down };
	POINT playerCursor_down[3] = { playerCursorPoint_down, playerCursorPoint_left_up, playerCursorPoint_right_up };

	switch (player.viewDirection)
	{
	case N:
		Polygon(mainWindowHDC, playerCursor_up, 3);
		break;
	case W:
		Polygon(mainWindowHDC, playerCursor_left, 3);
		break;
	case E:
		Polygon(mainWindowHDC, playerCursor_right, 3);
		break;
	case S:
		Polygon(mainWindowHDC, playerCursor_down, 3);
		break;
	}
	SelectObject(mainWindowHDC, oldBrush);

	if (usedCheats || monster.seeThePlayer)
	{
		oldBrush = (HBRUSH)SelectObject(mainWindowHDC, (HBRUSH)CreateSolidBrush(monsterMapColor));
		showPlayerBlock(monster.pos.x, monster.pos.y);
		SelectObject(mainWindowHDC, oldBrush);
	}
}


block getObject(int x, int y)
{
	if (monster.pos.x == x && monster.pos.y == y)
		return monsterUM;
	return gameMap[x][y];
}
block getObject(POINT gettedPoint)
{
	return getObject(gettedPoint.x, gettedPoint.y);
}
void showObject(block obj, short range, sidesEnum side)
{
	if (obj == wall)
		showWall(range, side);
	else if (obj == none)
		showNone(range, side);
	else if (obj == door)
		showDoor(range, side);
	else if (obj == monsterUM)
		showMonster(range, side);
}

void showGameCanvas()
{
	int rangeViewedPass = 0;
	POINT viewPoint = player.pos;

	bool isMonstorInFront = false;
	int monsterInFrontRange = -1;
	while (rangeViewedPass < 6) //Отображение всех объектов карты
	{
		block presentBlock = getObject(viewPoint);
		if (presentBlock != none)
		{
			if (presentBlock == monsterUM)
			{
				isMonstorInFront = true;
				monsterInFrontRange = rangeViewedPass;
			}
			else 
				break;
		}

		updatePlayerMap(viewPoint);

		POINT viewPointLeft = moveDirectionLeft(viewPoint, player.viewDirection);
		POINT viewPointRight = moveDirectionRight(viewPoint, player.viewDirection);
		showObject(getObject(viewPointLeft), rangeViewedPass, leftSide);
		showObject(getObject(viewPointRight), rangeViewedPass, rightSide);

		viewPoint = moveDirectionForward(viewPoint, player.viewDirection);
		rangeViewedPass++;
	}
	showObject(getObject(viewPoint), rangeViewedPass, frontSide);
	if(isMonstorInFront)
		showObject(monsterUM, monsterInFrontRange, frontSide);

	showPlayerMap(); //Отрисовка мини-карты
	showCompas(); //Отображение компаса
	showStatus();
}



void generateGame()
{
	generateMap();
	gameMap[mapSize.x - 2][mapSize.y - 1] = door;

	player.pos.x = 1;
	player.pos.y = 1;
	clearPlayerMap();

	monster.seeThePlayer = false;
	monster.pos = { mapSize.x / 2, mapSize.y / 2 };
	for (int i = mapSize.y / 2; i != 0; i--)
	{
		if (gameMap[monster.pos.x][i] == none)
		{
			monster.pos.y = i;
			break;
		}
	}
	for (int i = mapSize.x / 2; i != 0; i--)
	{
		if (gameMap[i][monster.pos.y] == none)
		{
			monster.pos.x = i;
			break;
		}
	}
	monster.setFirstTarget();
}

std::atomic_bool callGameTick = true;
void Game_Tick()
{
	refreshCanvas();
	srand(time(NULL));
	Sleep(monster.speed);

	monster.alifeTick();
	refreshCanvas();
	if (monster.catchPlayer())
		goToPage(losePage);

	callGameTick = true;
}