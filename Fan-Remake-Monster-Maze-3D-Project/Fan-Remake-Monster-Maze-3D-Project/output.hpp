#pragma once

void inline setTo(short x, short y)
{
	SetConsoleCursorPosition(thisWindowHANDLE, { x, y });
}
HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));
struct LINE
{
	POINT f;
	POINT s;
};
void trapeze(LINE biggestLine, LINE smallestLine, HBRUSH& brush)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(thisWindowDC, brush);
	constexpr unsigned short trapezePointsCount = 4;
	const POINT trapezePolygons[trapezePointsCount] = { biggestLine.f, smallestLine.f, smallestLine.s, biggestLine.s };

	Polygon(thisWindowDC, trapezePolygons, trapezePointsCount);

	SelectObject(thisWindowDC, hOldBrush);
}
void rectangle(LINE line, HBRUSH& brush)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(thisWindowDC, brush);

	Rectangle(thisWindowDC, line.f.x, line.f.y, line.s.x, line.s.y);

	SelectObject(thisWindowDC, hOldBrush);
}