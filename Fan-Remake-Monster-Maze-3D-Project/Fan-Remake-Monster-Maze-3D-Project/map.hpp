#pragma once
#include <time.h>

typedef enum block {
	none = 0,
	wall,
	door
};
constexpr unsigned int mapXSize = 25;
constexpr unsigned int mapYSize = 25;
block map[mapXSize][mapYSize];
constexpr POINT doorPos = { mapXSize - 2, mapYSize - 1 };

bool deadEnd(int x, int y) {
	int a = 0;

	if (x != 1) {
		if (map[y][x - 2] == none)
			a += 1;
	}
	else a += 1;

	if (y != 1) {
		if (map[y - 2][x] == none)
			a += 1;
	}
	else a += 1;

	if (x != mapXSize - 2) {
		if (map[y][x + 2] == none)
			a += 1;
	}
	else a += 1;

	if (y != mapYSize - 2) {
		if (map[y + 2][x] == none)
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

	for (int i = 0; i < mapYSize; i++) // ������ ����������� ������-��������
		for (int j = 0; j < mapXSize; j++)
			map[i][j] = wall;

	x = 3; y = 3; a = 0; // ����� ����������� ����� � �������
	while (a < 10000) { // ��, ��������, �������, ����� ���� ���, �� ����
		map[y][x] = none; a++;
		while (1) { // ����������� ����, ������� ����������� ������ �������
			c = rand() % 4; // ���������, ��� ���� ���������
			switch (c) {  // �� ��� ������ � ����� ����������� �� ������
			case 0: if (y != 1)
				if (map[y - 2][x] == wall) { // �����
					map[y - 1][x] = none;
					map[y - 2][x] = none;
					y -= 2;
				}
			case 1: if (y != mapYSize - 2)
				if (map[y + 2][x] == wall) { // ����
					map[y + 1][x] = none;
					map[y + 2][x] = none;
					y += 2;
				}
			case 2: if (x != 1)
				if (map[y][x - 2] == wall) { // ������
					map[y][x - 1] = none;
					map[y][x - 2] = none;
					x -= 2;
				}
			case 3: if (x != mapXSize - 2)
				if (map[y][x + 2] == wall) { // �������
					map[y][x + 1] = none;
					map[y][x + 2] = none;
					x += 2;
				}
			}
			if (deadEnd(x, y))
				break;
		}

		if (deadEnd(x, y)) // ����������� ����� �� ������
			do {
				x = 2 * (rand() % ((mapXSize - 1) / 2)) + 1;
				y = 2 * (rand() % ((mapYSize - 1) / 2)) + 1;
			} while (map[y][x] != none);
	} // �� ���� � ���.
}