#include"App.h"
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow)
{
	cris::App app(hinst);
	app.init();
	app.showandUpdateWnd();
	app.Run();
}