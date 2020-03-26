#pragma once

void inline setTo(short x, short y)
{
	SetConsoleCursorPosition(mainWindowHWND, { x, y });
}
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