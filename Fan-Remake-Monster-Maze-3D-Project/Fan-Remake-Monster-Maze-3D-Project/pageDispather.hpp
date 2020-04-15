#pragma once

void eventDispather_onCalled()
{

}

void eventDispather_onPaint(HDC dispatheredHDC)
{
	eriseWindow();
	switch (_presentPage)
	{
	case gamePage:
		gamePage_onPaint(dispatheredHDC);
		break;
	case losePage:
		losePage_onPaint(dispatheredHDC);
		break;
	case winPage:
		winPage_onPaint(dispatheredHDC);
		break;
	case menuPage:
		menuPage_onPaint(dispatheredHDC);
		break;
	}
	ValidateRect(mainWindowHWND, NULL);
}

void eventDispather_onKeyPressed(unsigned int key)
{
	switch (_presentPage)
	{
	case gamePage:
		gamePage_onKeyPressed(key);
		break;
	case winPage:
		winPage_onKeyPressed(key);
		break;
	case losePage:
		losePage_onKeyPressed(key);
		break;
	case menuPage:
		menuPage_onKeyPressed(key);
		break;
	}
}

void eventDispather_main()
{
	switch (_presentPage)
	{
	case gamePage:
		gamePage_main();
		break;
	case winPage:
		winPage_main();
		break;
	case losePage:
		losePage_main();
		break;
	case menuPage:
		menuPage_main();
		break;
	}
}

void goToPage(appPages newAppState)
{
	eriseWindow();

	_presentPage = newAppState;

	switch (newAppState)
	{
	case winPage:
		winPage_onCalled();
		break;
	case losePage:
		losePage_onCalled();
		break;
	case gamePage:
		gamePage_onCalled();
		break;
	case menuPage:
		menuPage_onCalled();
		break;
	}

	refreshCanvas();
}