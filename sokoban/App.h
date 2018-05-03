#pragma once
#include"stdafx.h"

#define CLASSNAME _T("sokoban")


namespace cris {
	class App {
		HWND hwnd;
		HINSTANCE hinst;
	public:

	
		LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		App(HINSTANCE hinst);
		void init();
		void Run();
		~App();
	
	};
}
HRESULT WINAPI WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) 
{
	cris::App app(hinst);
	app.Run();
	
}