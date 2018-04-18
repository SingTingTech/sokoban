#include"my2d.h"
#include"Control.h"
#include"stdafx.h"


#ifdef __WINMAIN
//constants
#define CLASSNAME _T("sokoban")

//globle vars
cris::my2d my2ddraw = { 0 };
cris::EditControl edit = { 300,200,200,25 };
//functions
LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void init(HWND hwnd, HINSTANCE hinst);

void createWndAndUpdate(HWND *hwnd, HINSTANCE hinst, HCURSOR hCursor, int nCmdShow);
INT _stdcall WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {
	HCURSOR arrow = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYARROW));
	HCURSOR hand = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYHAND));
	edit.setCursor(hand);
	//收集存档信息
	// 
	HWND hwnd;

	createWndAndUpdate(&hwnd, hinst, arrow, nCmdShow);
	edit.setWnd(hwnd);
	init(hwnd, hinst);



	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			my2ddraw.draw([]()
			{
				edit.draw(my2ddraw);
			});
			Sleep(100);

		}

	}

}
//function implements
LRESULT _stdcall windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
		edit.onInput(wParam);
		break;
	case WM_DESTROY:

		my2ddraw.cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_CREATE: {
		POINT p;
		p.x = 400;
		p.y = 300;
		ClientToScreen(hwnd, &p);
		HIMC himc = ImmGetContext(hwnd);
		CANDIDATEFORM cand;
		cand.dwIndex = 0;
		cand.dwStyle = CFS_CANDIDATEPOS;
		cand.ptCurrentPos = p;
		int i = ImmSetCandidateWindow(himc, &cand);
		ImmReleaseContext(hwnd, himc);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
void createWndAndUpdate(HWND *hwnd, HINSTANCE hinst, HCURSOR hCursor, int nCmdShow)
{
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW, windProc, 0L, 0L,
		hinst, NULL, NULL, NULL, NULL,
		CLASSNAME, NULL
	};
	wc.hCursor = hCursor;
	RegisterClassEx(&wc);

	// Create the application's window
	*hwnd = CreateWindow(CLASSNAME, _T("SOKOBAN"),
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 100, 100, 800, 600,
		NULL, NULL, hinst, NULL);


	ShowWindow(*hwnd, nCmdShow);
	UpdateWindow(*hwnd);
}
void init(HWND hwnd, HINSTANCE hinst)
{

	my2ddraw.init(hwnd);
}
#endif // __WINMAIN

