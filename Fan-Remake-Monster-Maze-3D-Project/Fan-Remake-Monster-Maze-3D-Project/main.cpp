#include <iostream>
#include <conio.h>

typedef enum block {
	none = 0,
	player,
	wall,
	monster,
	exit
};
constexpr unsigned int mapXSize = 26;
constexpr unsigned int mapYSize = 26;
block map[mapXSize][mapYSize];

class playerClass
{
public:
	unsigned int x, y;
	void makesMove()
	{
		//...
	}
}player;

class monsterClass
{
public:
	unsigned int x, y;
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
}monster;

void generateMap()
{
	//...
}
void generateGame()
{
	generateMap();
	player.x = player.y = 1; //TOFIX
	map[mapXSize - 1][mapYSize - 1] = exit;
	monster.x = mapXSize / 2; monster.y = mapYSize / 2;
}


int main(void)
{
	while (true) //Game Loop
	{
		//...
	}

	return 0;
}