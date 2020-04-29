#pragma once
typedef enum direction {
	N = 0,
	W = 1,
	E = 2,
	S = 3,
	null
};
//-N-
//W-E
//-S-

char inline directionName(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return 'N';
	case E: return 'E';
	case S: return 'S';
	case W: return 'W';
	default: return NULL;
	}
}

direction inline turnDirectionLeft(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return W;
	case W: return S;
	case S: return E;
	default:
	case E: return N;
	}
}
direction inline turnDirectionRight(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return E;
	case E: return S;
	case S: return W;
	default:
	case W: return N;
	}
}
direction inline turnDirectionAround(direction rolledDirection)
{
	switch (rolledDirection)
	{
	case N: return S;
	case E: return W;
	case S: return N;
	default:
	case W: return E;
	}
}

POINT moveDirectionForward(POINT movedPoint, direction movedDirection)
{
	switch (movedDirection)
	{
	case N: return { movedPoint.x, movedPoint.y - 1 };
	case E: return { movedPoint.x + 1, movedPoint.y };
	case S: return { movedPoint.x, movedPoint.y + 1 };
	case W: return { movedPoint.x - 1, movedPoint.y };
	default: return { movedPoint.x, movedPoint.y };
	}
}
POINT moveDirectionBack(POINT movedPoint, direction movedDirection)
{
	switch (movedDirection)
	{
	case N: return { movedPoint.x, movedPoint.y + 1 };
	case E: return { movedPoint.x - 1, movedPoint.y };
	case S: return { movedPoint.x, movedPoint.y - 1 };
	case W: return { movedPoint.x + 1, movedPoint.y };
	default: return { movedPoint.x, movedPoint.y };
	}
}
POINT moveDirectionLeft(POINT movedPoint, direction movedDirection)
{
	switch (movedDirection)
	{
	case N: return { movedPoint.x - 1, movedPoint.y };
	case E: return { movedPoint.x, movedPoint.y - 1 };
	case S: return { movedPoint.x + 1, movedPoint.y };
	case W: return { movedPoint.x, movedPoint.y + 1 };
	default: return { movedPoint.x, movedPoint.y };
	}
}
POINT moveDirectionRight(POINT movedPoint, direction movedDirection)
{
	switch (movedDirection)
	{
	case N: return { movedPoint.x + 1, movedPoint.y };
	case E: return { movedPoint.x, movedPoint.y + 1 };
	case S: return { movedPoint.x - 1, movedPoint.y };
	case W: return { movedPoint.x, movedPoint.y - 1 };
	default: return { movedPoint.x, movedPoint.y };
	}
}