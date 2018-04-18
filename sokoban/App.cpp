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
	for (auto it = scenelist.begin(); it != scenelist.end(); it++) 
	{
		if (it->SceneNum == scene) 
		{
			it->run();
		}
	}
}
void cris::App::winLoop()
{
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
			Run();
		}
	}
}
cris::App::App(HINSTANCE hinst,int nCmdShow)
{
	arrow = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYARROW));
	hand = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYHAND));
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW, (WNDPROC) windProc, 0L, 0L,
		hinst, NULL, NULL, NULL, NULL,
		CLASSNAME, NULL
	};
	wc.hCursor = arrow;
	RegisterClassEx(&wc);

	// Create the application's window
	hwnd = CreateWindow(CLASSNAME, _T("SOKOBAN"),
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 100, 100, 800, 600,
		NULL, NULL, hinst, NULL);
	//
	hedit = CreateWindow(L"edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 200, 200, 25, hwnd, 0, 0, 0);
	EnableWindow(hedit, false);
	hpasswd = CreateWindow(L"edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD, 300, 300, 200, 25, hwnd, 0, 0, 0);
	EnableWindow(hpasswd, false);
	hlist = CreateWindow(L"listbox", NULL, LBS_NOTIFY | WS_CHILD | WS_VISIBLE | LBS_STANDARD, 200, 150, 200, 300, hwnd, 0, hinst, 0);
	EnableWindow(hlist, false);
	
	//d2d
	my2ddraw.init(hwnd);
	//sokect
	client.clientConnectIni();

	//png
	d2.ini();
	d11.ini();
	d12.ini();
	u11.ini();
	u12.ini();
	u2.ini();
	l1.ini();
	l2.ini();
	r1.ini();
	r2.ini();
	wall.ini();
	ground.ini();
	target.ini();
	box.ini();
	bot.ini();
	d2.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_D2, 50, 0);
	d12.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_D12, 50, 0);
	d11.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_D11, 50, 0);
	u2.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_U2, 50, 0);
	u12.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_U12, 50, 0);
	u11.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_U11, 50, 0);
	l2.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_L2, 50, 0);
	l1.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_L1, 50, 0);
	r2.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_R2, 50, 0);
	r1.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN_R1, 50, 0);
	box.loadPNG(hinst, my2ddraw.pRT, IDB_BOX, 50, 0);
	target.loadPNG(hinst, my2ddraw.pRT, IDB_TARGET, 50, 0);
	wall.loadPNG(hinst, my2ddraw.pRT, IDB_WALL, 50, 0);
	ground.loadPNG(hinst, my2ddraw.pRT, IDB_GRUOND, 50, 0);
	bot.loadPNG(hinst, my2ddraw.pRT, IDB_BOX_ON_TAR, 50, 0);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//dxinput
	input.inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

}
cris::App::~App()
{
	d2.cleanup();
	d12.cleanup();
	d11.cleanup();
	u2.cleanup();
	u12.cleanup();
	u11.cleanup();
	l1.cleanup();
	l2.cleanup();
	r1.cleanup();
	r2.cleanup();
	box.cleanup();
	wall.cleanup();
	target.cleanup();
	ground.cleanup();
	bot.cleanup();

	input.cleanUp();
	my2ddraw.cleanup();
	client.clientCloseConnect();
}
