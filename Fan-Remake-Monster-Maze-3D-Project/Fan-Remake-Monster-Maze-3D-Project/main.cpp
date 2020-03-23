#include <iostream>
#include <Windows.h>
#include <wingdi.h>
#include <conio.h>
#include "map.hpp"
using namespace std;
HANDLE thisWindowHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
HWND thisWindowHWND = GetForegroundWindow();
HDC thisWindowDC = GetDC(thisWindowHWND);
#include "output.hpp"

struct point
{
	unsigned int x, y;
};
point screenSize = { 32, 32};

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
class playerClass : public point
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

class monsterClass : public point
{
public:
	point target;
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

int main(void)
{
	srand((unsigned)time(NULL));

	trapeze({ { 0, 0 }, { 0, 200 } }, { { 40, 40 }, { 40, 160} }, grayBrush); //test

	generateMap();
	while (true)
	{
		
		debugShowMap();
	}

	return 0;
}