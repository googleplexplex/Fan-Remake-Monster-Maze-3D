#pragma once

HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH wallBrush = CreateSolidBrush(RGB(169, 169, 169));
HBRUSH noneBrush = CreateSolidBrush(RGB(128, 128, 128));
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
void eriseWindow()
{
	RECT rect;
	GetClientRect(mainWindowHWND, &rect);
	FillRect(mainWindowHDC, &rect, (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)));
}

void inline refreshCanvas()
{
	InvalidateRect(mainWindowHWND, NULL, NULL);
	SendMessage(mainWindowHWND, WM_PAINT, NULL, NULL);
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
void inline showMonster(short range) //TOFIX
{
	VERTICAL_TRAPEZE wallPrototype = getTrapezeCoords(6 - range + 1, frontSide);
	HBRUSH monsterBrush = CreateSolidBrush(RGB(255, 0, 0));
	wallPrototype.biggestBaseF.y = wallPrototype.smallestBaseF.y;
	wallPrototype.biggestBaseS.y = wallPrototype.smallestBaseS.y;
	trapeze(wallPrototype, monsterBrush);
}
void inline showDoor(short range) //TOFIX
{
	VERTICAL_TRAPEZE wallPrototype = getTrapezeCoords(6 - range + 1, frontSide);
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
