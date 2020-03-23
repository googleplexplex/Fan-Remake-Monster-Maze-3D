#include <iostream>
#include <conio.h>
#include "map.hpp"

typedef enum direction {
	N = 0,
	W,
	E,
	S
};
//-N-
//W-E
//-S-

struct point
{
	unsigned int x, y;
};

class playerClass : public point
{
public:
	direction viewDirection;
	void makesMove()
	{
		//...
	}
}player;

class monsterClass : public point
{
public:
	point target;
	bool seesPlayer() //TOTEST
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
	map[mapXSize - 1][mapYSize - 1] = exit;
	monster.x = mapXSize / 2; monster.y = mapYSize / 2; //TOFIX
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

	while (true)
	{
		generateMap();
		debugShowMap();
	}

	return 0;
}