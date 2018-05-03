#include"App.h"

LRESULT cris::App::windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

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
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW, (WNDPROC)this->windProc, 0L, 0L,
		hinst, NULL, NULL, NULL, NULL,
		CLASSNAME, NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	hwnd = CreateWindow(CLASSNAME, _T("SOKOBAN"),
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 100, 100, 800, 600,
		NULL, NULL, hinst, NULL);
}
cris::App::App(HINSTANCE hinst)
{
	this->hinst = hinst;

}
cris::App::~App()
{

}
