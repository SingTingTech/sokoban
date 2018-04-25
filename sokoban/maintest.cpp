#include"my2d.h"
#include"Control.h"
#include"stdafx.h"


#ifdef __WINMAIN
//constants
#define CLASSNAME _T("sokoban")

//globle vars
cris::DXInput dinput;
cris::my2d my2ddraw = { 0 };
cris::EditControl edit = { 300,200,200,25 ,1,cris::EditControl::NORMAL};
cris::TextControl text = { 100,200,200,25,L"hello" };
cris::ListControl list = { 300,300,200,50};
cris::CheckBox check = { 100,400};
//functions
LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void init(HWND hwnd, HINSTANCE hinst);

void createWndAndUpdate(HWND *hwnd, HINSTANCE hinst, HCURSOR hCursor, int nCmdShow);

INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {
	HCURSOR arrow = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYARROW));
	HCURSOR hand = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYHAND));
	
	//收集存档信息
	// 
	HWND hwnd;

	createWndAndUpdate(&hwnd, hinst, arrow, nCmdShow);

	init(hwnd, hinst);
	edit.setCursor(hand);
	edit.setWnd(hwnd);
	text.setCursor(hand);
	text.setWnd(hwnd);
	list.setWnd(hwnd);
	list.setCursor(hand);

	list.addstring("hello");
	list.addstring("world");
	list.addstring("i'm");
	list.addstring("cirs");
	list.setselect(2);
	list.setMove(1);
	check.setCursor(hand);
	check.setWnd(hwnd);

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
				dinput.getInput();
				edit.draw(my2ddraw);
				text.draw(my2ddraw);
				text.testKeys(dinput,[]() {
					MessageBox(0, L"", 0, 0 );				
				});
				
				list.draw(my2ddraw);
				list.testKeys(dinput, []() {
					int i=0;
				});
				char s[1024];
				memset(s, 0, 1024);
				sprintf_s(s, "%f%d", list.moveBar,list.cursor);
				text << s;
				check.draw(my2ddraw);
				check.testKeys(dinput);
			});
		
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
		dinput.cleanUp();
		my2ddraw.cleanup();
		PostQuitMessage(0);
		return 0;

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
	dinput.inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}
#endif // __WINMAIN

