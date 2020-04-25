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
constexpr POINT miniMapPos = { 520, 120 };
constexpr POINT miniMapBlockSize = { 40, 40 };
void showMiniMap() //TODO
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, wallBrush);

	switch (player.viewDirection)
	{
	case N:
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (gameMap[player.pos.x - 1 + j][player.pos.y - 1 + i] == wall)
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
				else
				{
					HBRUSH prevBrush = (HBRUSH)SelectObject(mainWindowHDC, noneBrush);
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
					SelectObject(mainWindowHDC, prevBrush);
				}
			}
		}
		break;
	case W:
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (gameMap[player.pos.x + 1 - j][player.pos.y - 1 + i] == wall)
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
				else
				{
					HBRUSH prevBrush = (HBRUSH)SelectObject(mainWindowHDC, noneBrush);
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
					SelectObject(mainWindowHDC, prevBrush);
				}
			}
		}
		break;
	case E:
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (gameMap[player.pos.x - 1 + j][player.pos.y + 1 - i] == wall)
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
				else
				{
					HBRUSH prevBrush = (HBRUSH)SelectObject(mainWindowHDC, noneBrush);
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
					SelectObject(mainWindowHDC, prevBrush);
				}
			}
		}
		break;
	case S:
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (gameMap[player.pos.x + 1 - j][player.pos.y + 1 - i] == wall)
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
				else
				{
					HBRUSH prevBrush = (HBRUSH)SelectObject(mainWindowHDC, noneBrush);
					relativeRectangle(miniMapPos.x + (j * miniMapBlockSize.x), miniMapPos.y + (i * miniMapBlockSize.y), miniMapBlockSize.x, miniMapBlockSize.y);
					SelectObject(mainWindowHDC, prevBrush);
				}
			}
		}
		break;
	default:
		break;
	}

	SelectObject(mainWindowHDC, oldBrush);
}
double inline getRangeBehind(POINT f, POINT s)
{
	return sqrt(sqr(s.x - f.x) + sqr(s.y - f.y));
}

class monsterClass
{
public:
	POINT pos;
	direction target = null;
	unsigned short targetRange = 0;
	unsigned int speed = 1000;
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
		positionInfoStruct positionInfo(monster.pos); //�������� ��� ���������� � ��������� �������

		if (positionInfo.sumOfPasses() == 1 && positionInfo.passDescriptions[(int)turnDirectionAround(target)].isPass) //���� ����� ������ �� ��������� �� �����, � ������ ����� ���...
		{
			target = turnDirectionAround(target); //���������������
			passDescriptionStruct wayBackDescription = positionInfo.passDescriptions[(int)target];
			targetRange = wayBackDescription.ranges[rand() % wayBackDescription.rangesCount];
			return;
		}

		//���� �����, �������� ��������, ���������...
		positionInfo.passDescriptions[(int)turnDirectionAround(target)].isPass = false;
		if (positionInfo.sumOfPasses() == 1) //���� �����, �� �������� ��������, ������ ����...
		{
			target = (direction)positionInfo.getFirstPass(); //��� � ����
			passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
			targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
			return;
		}

		//���� �����, �� �������� ��������, ���������...
		target = (direction)positionInfo.getRandomPass(); //��� � ��������� �� ���
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
		std::pair<direction, unsigned short> seePlayer = seesPlayer();

		if (seePlayer.first != null)
		{
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
		positionInfoStruct positionInfo(monster.pos); //�������� ��� ���������� � ��������� �������

		if (positionInfo.sumOfPasses() == 1) //���� ���� �� �������� �����, ��� ������ ��������, ������ ����...
		{
			target = (direction)positionInfo.getFirstPass(); //��� � ����
			passDescriptionStruct turnDescription = positionInfo.passDescriptions[(int)target];
			targetRange = turnDescription.ranges[rand() % turnDescription.rangesCount];
			return;
		}

		//���� ����� ����� ���������...
		target = (direction)positionInfo.getRandomPass(); //��� � ��������� �� ���
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
typedef enum playerBlock
{
	wasnot = 0,
	was
};
playerBlock playerMap[mapXSize][mapYSize];
void inline clearPlayerMap()
{
	for (int i = 0; i < mapXSize; i++)
	{
		memset(playerMap[i], 0, sizeof(playerBlock));
	}
}
void inline updatePlayerMap(POINT updatedPoint)
{
	if (playerMap[updatedPoint.x][updatedPoint.y] == wasnot)
		playerMap[updatedPoint.x][updatedPoint.y] = was;
}
void showMap()
{
	
}

void showObject(block obj, short range, sidesEnum side)
{
	if (obj == wall)
		showWall(range, side);
	else if (obj == none)
		showNone(range, side);
	else if (obj == door)
		showDoor(range, side);
}

void showGameCanvas()
{
	int rangeViewedPass = 0;
	switch (player.viewDirection) //��������� ����� � ������ ������ ���������
	{
	case N:
		for (; gameMap[player.pos.x][player.pos.y - rangeViewedPass] == none && rangeViewedPass < 6; rangeViewedPass++)
		{
			showObject(gameMap[player.pos.x - 1][player.pos.y - rangeViewedPass], rangeViewedPass, leftSide);
			showObject(gameMap[player.pos.x + 1][player.pos.y - rangeViewedPass], rangeViewedPass, rightSide);
		}
		break;
	case W:
		for (; gameMap[player.pos.x - rangeViewedPass][player.pos.y] == none && rangeViewedPass < 6; rangeViewedPass++)
		{
			showObject(gameMap[player.pos.x - rangeViewedPass][player.pos.y + 1], rangeViewedPass, leftSide);
			showObject(gameMap[player.pos.x - rangeViewedPass][player.pos.y - 1], rangeViewedPass, rightSide);
		}
		break;
	case E:
		for (; gameMap[player.pos.x + rangeViewedPass][player.pos.y] == none && rangeViewedPass < 6; rangeViewedPass++)
		{
			showObject(gameMap[player.pos.x + rangeViewedPass][player.pos.y - 1], rangeViewedPass, leftSide);
			showObject(gameMap[player.pos.x + rangeViewedPass][player.pos.y + 1], rangeViewedPass, rightSide);
		}
		break;
	case S:
		for (; gameMap[player.pos.x][player.pos.y + rangeViewedPass] == none && rangeViewedPass < 6; rangeViewedPass++)
		{
			showObject(gameMap[player.pos.x + 1][player.pos.y + rangeViewedPass], rangeViewedPass, leftSide);
			showObject(gameMap[player.pos.x - 1][player.pos.y + rangeViewedPass], rangeViewedPass, rightSide);
		}
		break;
	}

	switch (player.viewDirection) //��������� ����� ��������� ����� �������
	{
	case N:
		showObject(gameMap[player.pos.x][player.pos.y - rangeViewedPass], rangeViewedPass, frontSide);
		break;
	case W:
		showObject(gameMap[player.pos.x - rangeViewedPass][player.pos.y], rangeViewedPass, frontSide);
		break;
	case E:
		showObject(gameMap[player.pos.x + rangeViewedPass][player.pos.y], rangeViewedPass, frontSide);
		break;
	case S:
		showObject(gameMap[player.pos.x][player.pos.y + rangeViewedPass], rangeViewedPass, frontSide);
		break;
	}

	switch (player.viewDirection) //��������� ��������� ��������
	{
	case N:
		if (monster.pos.x == player.pos.x && player.pos.y > monster.pos.y && monster.pos.y > player.pos.y - rangeViewedPass)
			showMonster(player.pos.y - monster.pos.y);
		break;
	case W:
		if (monster.pos.y == player.pos.y && player.pos.x < monster.pos.x && monster.pos.x < player.pos.x - rangeViewedPass)
			showMonster(monster.pos.x - player.pos.x);
		break;
	case E:
		if (monster.pos.y == player.pos.y && player.pos.x > monster.pos.x && monster.pos.x > player.pos.x + rangeViewedPass)
			showMonster(player.pos.x - monster.pos.x);
		break;
	case S:
		if (monster.pos.x == player.pos.x && player.pos.y < monster.pos.y && monster.pos.y < player.pos.y + rangeViewedPass)
			showMonster(monster.pos.y - player.pos.y);
		break;
	}

	if (cheats) //���-����
		showMap();

	showCompas(); //����������� �������
	showMiniMap(); //��������� ����-�����
	showStatus();
}



void generateGame()
{
	generateMap();
	gameMap[mapXSize - 2][mapYSize - 1] = door;

	player.pos.x = player.pos.y = 1;
	clearPlayerMap();

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