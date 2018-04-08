
#include"my2d.h"
#include"clientSocket.h"
#include"DXInput.h"
#include"resource.h"
//constants
#define CLASSNAME _T("hello world")


//globle vars
my2d my2ddraw = { 0 };
DXInput input;
SOCKET client;

//functions
LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



INT _stdcall WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {
	HCURSOR arrow = LoadCursor(hinst,MAKEINTRESOURCE(IDC_MYARROW));
	HCURSOR hand = LoadCursor(hinst,MAKEINTRESOURCE(IDC_MYHAND));

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),CS_HREDRAW|CS_VREDRAW, windProc, 0L, 0L,
		hinst, NULL, NULL, NULL, NULL,
		CLASSNAME, NULL
	};
	wc.hCursor = arrow;
	RegisterClassEx(&wc);
	
	// Create the application's window
	HWND hwnd = CreateWindow(CLASSNAME, _T("SOKOBAN"),
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME, 100, 100, 800, 600,
		NULL, NULL, hinst, NULL);

	my2ddraw.init(hwnd);
	clientConnectIni(&client);
	
	
	ShowWindow(hwnd, nCmdShow);
	//*/



	//*/
	UpdateWindow(hwnd);

	input.inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	/*/测试登陆和注册；
	{
		char buf[1024];
		buf[0] = 'l';
		sprintf_s(buf+1, 20, "%s","hello");
		sprintf_s(buf + 21, 29, "%s", "world");
		//注册测试
		clientSend(client, buf, 1024);
		memset(buf, 0, 1024);
		clientRecv(client, buf, 1024);

	}
	//*/

	int scene = 0;//默认存档选择界面
	//监测是否存在存档如果有跳至主菜单
	scene = 3;
	
	//存档选择界面
	auto fun0 = [hand, hwnd]()
	{
		input.getInput();
		IDWriteFactory * write_factory;
		IDWriteTextFormat * text_normal;
		ID2D1SolidColorBrush *pbrush;
		my2ddraw.pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &pbrush);
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(write_factory), reinterpret_cast<IUnknown **>(&write_factory));
		write_factory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 25, L"", &text_normal);

		const wchar_t * t3 = L"使用一个账号登陆";
		const wchar_t * t1 = L"创建一个新存档";
		const wchar_t * t2 = L"选择这个存档";

		D2D1_SIZE_F size = my2ddraw.pRT->GetSize();

		my2ddraw.pRT->DrawText(t3, wcslen(t3),
			text_normal,
			D2D1::RectF(size.width / 2 - 140, 100, size.width / 2 + 154, size.height),
			pbrush);
		my2ddraw.pRT->DrawText(t1, wcslen(t1),
			text_normal,
			D2D1::RectF(size.width / 2 - 140, 200, size.width / 2 + 154, size.height),
			pbrush);
		my2ddraw.pRT->DrawText(t2, wcslen(t2),
			text_normal,
			D2D1::RectF(size.width / 2 - 75, 300, size.width / 2 + 75, size.height),
			pbrush);

		//鼠标监测
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hwnd, &p);
		int x = p.x;
		int y = p.y;

		if (x< size.width / 2 + 75 && x>size.width / 2 - 75
			&& y > 300 && y < 325)
		{
			SetCursor(hand);
			if (input.isMouseButtonDown(DXInput::LEFTBUTTON))
				;
		}
		//调试输出
		//wchar_t buf[20] = { 0 };
		//wsprintf(buf, L"%d%d", input.isMouseButtonDown(DXInput::MIDBUTTON), y);
		//my2ddraw.pRT->DrawText(buf,20,
		//	text_normal,
		//	D2D1::RectF(0,0, size.width / 4, size.height/4),
		//	pbrush);

		write_factory->Release();
		pbrush->Release();

	};


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
			switch (scene) {

			case 3://存档选择界面

				my2ddraw.draw(fun0);

				//break;
			case 1:
				
				break;
			
			}
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
		input.cleanUp();
		my2ddraw.cleanup();
		clientCloseConnect(client);

		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
