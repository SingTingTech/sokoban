
#include"my2d.h"
#include"clientSocket.h"
//constants
#define CLASSNAME _T("hello world")


//globle vars
my2d my2ddraw = { 0 };
SOCKET client;
//functions
LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



INT _stdcall WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {


	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),CS_HREDRAW|CS_VREDRAW, windProc, 0L, 0L,
		hinst, NULL, NULL, NULL, NULL,
		CLASSNAME, NULL
	};
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hwnd = CreateWindow(CLASSNAME, _T("SOKOBAN"),
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME, 100, 100, 800, 600,
		NULL, NULL, hinst, NULL);
	ShowWindow(hwnd, nCmdShow);
	//*/
	my2ddraw.init(hwnd);

	clientConnectIni(&client);
	//*/
	UpdateWindow(hwnd);


	/*/²âÊÔµÇÂ½ºÍ×¢²á£»
	{
		char buf[1024];
		buf[0] = 'l';
		sprintf_s(buf+1, 20, "%s","hello");
		sprintf_s(buf + 21, 29, "%s", "world");
		//×¢²á²âÊÔ
		clientSend(client, buf, 1024);
		memset(buf, 0, 1024);
		clientRecv(client, buf, 1024);

	}
	//*/



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
//function implements
LRESULT _stdcall windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		//app.cleanUp();
		clientCloseConnect(client);
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		//app.update();
		my2ddraw.draw([]()
		{

			IDWriteFactory * write_factory;
			IDWriteTextFormat * text_title;
			IDWriteTextFormat * text_normal;
			ID2D1SolidColorBrush *pbrush;
			my2ddraw.pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray),&pbrush);
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(write_factory), reinterpret_cast<IUnknown **>(&write_factory));
			write_factory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50, L"", &text_title);
			write_factory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 25, L"", &text_normal);

			const wchar_t * title = L"Sokoban";
			const wchar_t * t1 = L"´´½¨Ò»¸öÐÂ´æµµ";
			const wchar_t * t2 = L"Ñ¡ÔñÄãµÄ´æµµ";

			D2D1_SIZE_F size = my2ddraw.pRT->GetSize();

			my2ddraw.pRT->DrawText(title, wcslen(title),
				text_title,
				D2D1::RectF(size.width/2-140,100, size.width / 2+ 154, size.height),
				pbrush);
			my2ddraw.pRT->DrawText(t1, wcslen(t1),
				text_normal,
				D2D1::RectF(size.width / 2 - 140, 200, size.width / 2 + 154, size.height),
				pbrush);
			my2ddraw.pRT->DrawText(t2, wcslen(t2),
				text_normal,
				D2D1::RectF(size.width / 2 - 140, 300, size.width / 2 + 154, size.height),
				pbrush);

		});
		ValidateRect(hwnd, NULL);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
