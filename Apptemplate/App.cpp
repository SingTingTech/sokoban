#include"App.h"

LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void cris::App::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
void cris::App::init() 
{

}
cris::App::App(HINSTANCE hinst)
{
	this->hinst = hinst;
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW, (WNDPROC)windProc, 0L, 0L,
		hinst, NULL, NULL, NULL, NULL,
		CLASSNAME, NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	hwnd = CreateWindow(CLASSNAME, _T("SOKOBAN"),
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 100, 100, 800, 600,
		NULL, NULL, hinst, NULL);
}
cris::App::~App()
{

}
void cris::App::cleanup() 
{

}

void cris::App::showandUpdateWnd()
{
	ShowWindow(hwnd, 10);
	UpdateWindow(hwnd);
}
