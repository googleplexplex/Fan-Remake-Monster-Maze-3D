#include <Windows.h>
#include <thread>

#define WINDOW_CLASS_NAME "Monster_Maze_3D_Fan_Remake"

#define KB_Q 0x51
#define KB_W 0x57
#define KB_E 0x45
#define KB_S 0x53
#define KB_C 0x43

HINSTANCE hInstanceApp;
HWND mainWindowHWND;
HDC mainWindowHDC;
constexpr POINT screenSize = { 800, 500 };
void synchronizeWindowSize();
void eriseWindow();
void inline refreshCanvas();
POINT GetTextExtentPoint32Size(const char* str);
#define TextOutWithDynamicLength(hdc, x, y, str) TextOut((hdc), (x), (y), (str), strlen(str))
#define TextOutCenter(hdc, y, str) TextOutWithDynamicLength((hdc), screenSize.x / 2 - (GetTextExtentPoint32Size(str).x) / 2, (y), (str));
#define TextOutCenterGame(hdc, Y, str) TextOutWithDynamicLength((hdc), screenSize.y / 2 - (GetTextExtentPoint32Size(str).x) / 2, (Y), (str));
#define randInRange(from, to) (rand() % ((to) - (from)) + (from))

typedef enum appPages
{
	winPage = 0,
	losePage,
	gamePage,
	menuPage
};
appPages _presentPage = menuPage;
bool usedCheats = false;

void goToPage(appPages newAppState);
#include "winPage.hpp"
#include "losePage.hpp"
#include "gamePage.hpp"
#include "menuPage.hpp"
#include "pageDispather.hpp"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void App_Init();
void App_Main();
void App_Shitdown();

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

	App_Init();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		App_Main();
	}

	App_Shitdown();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		mainWindowHDC = BeginPaint(hWnd, &ps);
		eventDispather_onPaint(mainWindowHDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		eventDispather_onKeyPressed((unsigned int)wParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}


void synchronizeWindowSize()
{
	RECT consoleWindowRect = { 0 };
	GetWindowRect(mainWindowHWND, &consoleWindowRect);
	MoveWindow(mainWindowHWND, consoleWindowRect.left, consoleWindowRect.top, screenSize.x + 4, screenSize.y + 30, NULL);
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

POINT GetTextExtentPoint32Size(const char* str)
{
	SIZE size;
	GetTextExtentPoint32A(GetDC(mainWindowHWND), str, strlen(str), &size);
	POINT resut = { size.cx, size.cy };
	return resut;
}



void App_Init()
{
	srand(time(NULL));
	SetTextColor(mainWindowHDC, RGB(255, 255, 255));
	SetBkColor(mainWindowHDC, RGB(0, 0, 0));
}

void App_Main()
{
	synchronizeWindowSize();

	eventDispather_main();
}

void App_Shitdown()
{
	return;
}