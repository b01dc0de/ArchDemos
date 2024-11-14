#include "ArchDemos.h"

LRESULT WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define APPNAME_W() (L"ArchDemos")
#define APPNAME_A() ("ArchDemos")

static bool bRunning = true;

int WINAPI WinMain
(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	PSTR CmdLine,
	int ShowWnd
)
{
	(void)hPrevInst;
	(void)CmdLine;
	(void)ShowWnd;

	printf("%s -- INIT\n", APPNAME_A());

	int ExitCode = 0;

	WNDCLASSEX WndClassEx = {};
	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc = WinProc;
	WndClassEx.hInstance = hInst;
	WndClassEx.lpszClassName = APPNAME_A();
	RegisterClassEx(&WndClassEx);

	DWORD WndStyle = 0;
	DWORD WndExStyle = WS_EX_OVERLAPPEDWINDOW;

	UINT WindowWidth = 1280;
	UINT WindowHeight= 720;
	
	HWND hWindowEx = CreateWindowEx
	(
		WndExStyle,
		APPNAME_A(),
		APPNAME_A(),
		WndStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowWidth,
		WindowHeight,
		nullptr,
		nullptr,
		hInst,
		nullptr
	);

	if (hWindowEx)
	{
		bRunning = true;
		ShowWindow(hWindowEx, ShowWnd);

		while (bRunning)
		{
			MSG msg;
			while (BOOL bResult = PeekMessage(&msg, hWindowEx, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	else
	{
		ExitCode = 1;
		DebugBreak();
	}

	return ExitCode;
}

LRESULT WinProc
(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	LRESULT lResult = 0;
	switch (uMsg)
	{
		case WM_KEYUP:
		{
			if (VK_ESCAPE == wParam)
			{
				bRunning = false;
			}
		} break;
		default:
		{
			lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
		} break;
	}

	return lResult;
}
