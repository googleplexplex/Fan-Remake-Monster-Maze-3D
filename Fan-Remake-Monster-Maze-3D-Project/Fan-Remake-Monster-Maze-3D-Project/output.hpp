#pragma once

void inline setTo(short x, short y)
{
	SetConsoleCursorPosition(mainWindowHWND, { x, y });
}
HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));
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
void trapeze(LINE biggestBase, LINE smallestBase, HBRUSH& brush)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(mainWindowHDC, brush);
	constexpr unsigned short trapezePointsCount = 4;
	const POINT trapezePolygons[trapezePointsCount] = { biggestBase.f, biggestBase.s, smallestBase.s, smallestBase.f };

	Polygon(mainWindowHDC, trapezePolygons, trapezePointsCount);

	SelectObject(mainWindowHDC, hOldBrush);
}
void trapeze(VERTICAL_TRAPEZE trapeze, HBRUSH& brush)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(mainWindowHDC, brush);
	constexpr unsigned short trapezePointsCount = 4;
	const POINT trapezePolygons[trapezePointsCount] = { trapeze.biggestBaseF, trapeze.biggestBaseS, trapeze.smallestBaseS, trapeze.smallestBaseF };

	Polygon(mainWindowHDC, trapezePolygons, trapezePointsCount);

	SelectObject(mainWindowHDC, hOldBrush);
}
void rectangle(LINE line, HBRUSH& brush)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(mainWindowHDC, brush);

	Rectangle(mainWindowHDC, line.f.x, line.f.y, line.s.x, line.s.y);

	SelectObject(mainWindowHDC, hOldBrush);
}