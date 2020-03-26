#include <Windows.h>

#define WINDOW_CLASS_NAME "Monster_Maze_3D_Fan_Remake"

HINSTANCE hInstanceApp;
HWND mainWindowHWND;
HDC mainWindowHDC;
constexpr POINT screenSize = { 800, 500 };

LOGFONT font;
HFONT hfont;

#include "menu.hpp"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Game_Init();
void Game_Main();
void Game_Shitdown();

int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{

	WNDCLASSEX winClass;
	HWND hwnd;
	MSG msg;
	HDC hdc;

	winClass.cbSize = sizeof(WNDCLASSEX);

	winClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WndProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = hInstance;
	winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = WINDOW_CLASS_NAME;
	winClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	hInstanceApp = hInstance;

	if (!RegisterClassEx(&winClass))
		return 0;

	if (!(hwnd = CreateWindowExA(NULL,
		WINDOW_CLASS_NAME,
		"Monster Maze 3D Fan Remake 0.1",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, screenSize.x, screenSize.y,
		NULL, NULL,
		hInstance,
		NULL)))
		return 0;

	mainWindowHWND = hwnd;

	Game_Init();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Game_Main();
	}

	Game_Shitdown();

	return msg.wParam;
}

//  ����: ������������ ��������� � ������� ����.
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		mainWindowHDC = BeginPaint(hWnd, &ps);
		showGameCanvas();
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}



void Game_Init()
{
	font.lfHeight = 24;// ������������� ������ ������ ��� �������
	font.lfWidth = 12;// ������������� ������� ������ �������� � ������
	//font.lfEscapement = 0;// ������������� ����, ����� �������� ������� � ���� X ����������
	//font.lfOrientation = 0;// ������������� ����, ����� �������� ������ ������� ������� � ���� X ����������
	//font.lfWeight = 100;// ������������� ������� ������ � ��������� �� 0 �� 1000
	//font.lfItalic = 0;// ������������� ��������� �����
	//font.lfUnderline = 0;// ������������� ������������ �����
	//font.lfStrikeOut = 0;// ������������� ����������� �����
	//font.lfCharSet = RUSSIAN_CHARSET;// ������������� ����� ��������
	//font.lfOutPrecision = 0;// ������������� �������� ������
	//font.lfClipPrecision = 0;// ������������� �������� ���������
	//font.lfQuality = 0;// ������������� �������� ������
	//font.lfPitchAndFamily = 0;// ������������� ������ �������� � ��������� ������
	//strcpy(font.lfFaceName, "VERDANA");//  ������������� �������� ������
	hfont = CreateFontIndirect(&font);
	srand(time(NULL));
	Game_Menu();
}

void Game_Main()
{
	RECT consoleWindowRect = { 0 };
	GetWindowRect(mainWindowHWND, &consoleWindowRect);
	MoveWindow(mainWindowHWND, consoleWindowRect.left, consoleWindowRect.top, screenSize.x + 4, screenSize.y + 30, NULL);

	gameState presentGameState = Game_Tick();
	if (presentGameState == win)
		Game_Win();
	else if (presentGameState == lose)
		Game_GameOver();
}

void Game_Shitdown()
{
	return;
}