#pragma once
#include <windows.h>

HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH wallBrush = CreateSolidBrush(RGB(169, 169, 169));
HBRUSH noneBrush = CreateSolidBrush(RGB(128, 128, 128));
HBRUSH doorBrush = CreateSolidBrush(RGB(0, 255, 0));
HBRUSH monsterBodyBrush = CreateSolidBrush(RGB(41, 41, 41));
HBRUSH monsterEyesBrush = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH monsterMouthBrush = CreateSolidBrush(RGB(15, 15, 15));
struct LINE
{
	POINT f;
	POINT s;
};
struct VERTICAL_TRAPEZE
{
	POINT biggestBaseF;
	POINT biggestBaseS;
	POINT smallestBaseS;
	POINT smallestBaseF;
};
void trapeze(VERTICAL_TRAPEZE trapeze, HBRUSH& brush)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(mainWindowHDC, brush);
	constexpr unsigned short trapezePointsCount = 4;
	const POINT trapezePolygons[trapezePointsCount] = { trapeze.biggestBaseF, trapeze.biggestBaseS, trapeze.smallestBaseS, trapeze.smallestBaseF };

	Polygon(mainWindowHDC, trapezePolygons, trapezePointsCount);

	SelectObject(mainWindowHDC, hOldBrush);
}
void rectangle(POINT f, POINT s, HBRUSH& brush)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(mainWindowHDC, brush);

	Rectangle(mainWindowHDC, f.x, f.y, s.x, s.y);

	SelectObject(mainWindowHDC, hOldBrush);
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
unsigned short factorialSizes2DWalls[8] = { 0, 2, 2 + 8, 2 + 8 + 6, 2 + 8 + 6 + 4, 2 + 8 + 6 + 4 + 2, 2 + 8 + 6 + 4 + 2 + 1 };
VERTICAL_TRAPEZE getTrapezeCoords(short range, sidesEnum side)
{
	if (side == frontSide)
	{
		switch (range) //TOFIX
		{
		case 6:
			return cubeSquareInCenter(1); //1
		case 5:
			return cubeSquareInCenter(1 + 1); //2
		case 4:
			return cubeSquareInCenter(1 + 1 + 2); //4
		case 3:
			return cubeSquareInCenter(1 + 1 + 2 + 4); //8
		case 2:
			return cubeSquareInCenter(1 + 1 + 2 + 4 + 6); //14
		default:
			return cubeSquareInCenter(1 + 1 + 2 + 4 + 6 + 8); //22
		}
	}
	else if (side == leftSide)
		return { cubePoint(factorialSizes2DWalls[range], factorialSizes2DWalls[range]), cubePointVerticalMirrored(factorialSizes2DWalls[range]), cubePointVerticalMirrored(factorialSizes2DWalls[range + 1]), cubePoint(factorialSizes2DWalls[range + 1], factorialSizes2DWalls[range + 1]) };
	else if (side == rightSide)
		return { cubePointHorizontalMirrored(factorialSizes2DWalls[range], factorialSizes2DWalls[range]), cubePointVerticalAndHorizontalMirrored(factorialSizes2DWalls[range]), cubePointVerticalAndHorizontalMirrored(factorialSizes2DWalls[range + 1]), cubePointHorizontalMirrored(factorialSizes2DWalls[range + 1], factorialSizes2DWalls[range + 1]) };
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
void inline showDoor(short range, sidesEnum side)
{
	trapeze(getTrapezeCoords(range, side), doorBrush);
}
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



 char monsterSprite[11][11] = {
	"   ###    ",
	"  #####   ",
	"  #.#.### ",
	" #########",
	"###MMM####",
	"###MMM### ",
	"# ####### ",
	"   #######",
	"  ###  ## ",
	" ###   ## "
};

void Cube(HDC hdc, int x, int y, int size)
{
	//Rectangle(hdc, x, y, screenSize.x - (x + size), screenSize.y - (y + size));
	constexpr unsigned short cubePointsCount = 4;
	const POINT cubePolygons[cubePointsCount] = { { x, y }, { x + size, y }, { x + size, y + size }, { x, y + size } };

	Polygon(mainWindowHDC, cubePolygons, cubePointsCount);
}

void inline showMonster(short range, sidesEnum side)
{
	if (side == frontSide)
	{
		VERTICAL_TRAPEZE monsterTextureSizes = getTrapezeCoords(range, side);
		POINT upLeftSpritePoint = monsterTextureSizes.biggestBaseF;
		POINT spriteSize = { monsterTextureSizes.smallestBaseS.x - monsterTextureSizes.biggestBaseF.x, monsterTextureSizes.smallestBaseS.y - monsterTextureSizes.biggestBaseF.y };
		POINT oneSpriteCubeSize = { spriteSize.x / 10, spriteSize.y / 10 };

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				if (monsterSprite[y][x] == '#')
				{
					HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, monsterBodyBrush);
					Cube(mainWindowHDC, upLeftSpritePoint.x + oneSpriteCubeSize.x * x + 1, upLeftSpritePoint.y + oneSpriteCubeSize.y * y + 1, oneSpriteCubeSize.x);
					SelectObject(mainWindowHDC, oldBrush);
				}
				else if (monsterSprite[y][x] == '.')
				{
					HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, monsterEyesBrush);
					Cube(mainWindowHDC, upLeftSpritePoint.x + oneSpriteCubeSize.x * x + 1, upLeftSpritePoint.y + oneSpriteCubeSize.y * y + 1, oneSpriteCubeSize.x);
					SelectObject(mainWindowHDC, oldBrush);
				}
				else if (monsterSprite[y][x] == 'M')
				{
					HBRUSH oldBrush = (HBRUSH)SelectObject(mainWindowHDC, monsterMouthBrush);
					Cube(mainWindowHDC, upLeftSpritePoint.x + oneSpriteCubeSize.x * x + 1, upLeftSpritePoint.y + oneSpriteCubeSize.y * y + 1, oneSpriteCubeSize.x);
					SelectObject(mainWindowHDC, oldBrush);
				}
			}
		}
	}
	else
		trapeze(getTrapezeCoords(range, side), monsterBodyBrush);
}