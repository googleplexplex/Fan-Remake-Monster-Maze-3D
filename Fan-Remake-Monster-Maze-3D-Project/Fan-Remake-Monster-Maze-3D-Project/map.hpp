#pragma once
#include <time.h>

typedef enum block {
	none = 0,
	wall,
	door
};
constexpr unsigned int mapXSize = 25;
constexpr unsigned int mapYSize = 25;
block gameMap[mapXSize][mapYSize];
block getFromMap(unsigned int x, unsigned int y)
{
	return gameMap[y + 1][x + 1];
}
void setToMap(unsigned int x, unsigned int y, block val)
{
	gameMap[y + 1][x + 1] = val;
}
constexpr POINT doorPos = { mapXSize - 2, mapYSize - 1 };

bool deadEnd(int x, int y) {
	int a = 0;

	if (x != 1) {
		if (gameMap[y][x - 2] == none)
			a += 1;
	}
	else a += 1;

	if (y != 1) {
		if (gameMap[y - 2][x] == none)
			a += 1;
	}
	else a += 1;

	if (x != mapXSize - 2) {
		if (gameMap[y][x + 2] == none)
			a += 1;
	}
	else a += 1;

	if (y != mapYSize - 2) {
		if (gameMap[y + 2][x] == none)
			a += 1;
	}
	else a += 1;

	if (a == 4)
		return 1;
	else
		return 0;
}
void generateMap()
{
	int x, y, c, a;
	bool b;

	for (int i = 0; i < mapYSize; i++) // Массив заполняется землей-ноликами
		for (int j = 0; j < mapXSize; j++)
			gameMap[i][j] = wall;

	x = 3; y = 3; a = 0; // Точка приземления крота и счетчик
	while (a < 10000) { // Да, простите, костыль, иначе есть как, но лень
		gameMap[y][x] = none; a++;
		while (1) { // Бесконечный цикл, который прерывается только тупиком
			c = rand() % 4; // Напоминаю, что крот прорывает
			switch (c) {  // по две клетки в одном направлении за прыжок
			case 0: if (y != 1)
				if (gameMap[y - 2][x] == wall) { // Вверх
					gameMap[y - 1][x] = none;
					gameMap[y - 2][x] = none;
					y -= 2;
				}
			case 1: if (y != mapYSize - 2)
				if (gameMap[y + 2][x] == wall) { // Вниз
					gameMap[y + 1][x] = none;
					gameMap[y + 2][x] = none;
					y += 2;
				}
			case 2: if (x != 1)
				if (gameMap[y][x - 2] == wall) { // Налево
					gameMap[y][x - 1] = none;
					gameMap[y][x - 2] = none;
					x -= 2;
				}
			case 3: if (x != mapXSize - 2)
				if (gameMap[y][x + 2] == wall) { // Направо
					gameMap[y][x + 1] = none;
					gameMap[y][x + 2] = none;
					x += 2;
				}
			}
			if (deadEnd(x, y))
				break;
		}

		if (deadEnd(x, y)) // Вытаскиваем крота из тупика
			do {
				x = 2 * (rand() % ((mapXSize - 1) / 2)) + 1;
				y = 2 * (rand() % ((mapYSize - 1) / 2)) + 1;
			} while (gameMap[y][x] != none);
	} // На этом и все.
}