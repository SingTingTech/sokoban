#pragma once
#include"stdafx.h"

#define CLASSNAME _T("sokoban")

LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace cris {
	class App {
		HWND hwnd;
		HINSTANCE hinst;
	public:
		App(HINSTANCE hinst);
		void init();
		void Run();
		void cleanup();
		void showandUpdateWnd();
		~App();
	
	};
}
