
#include"my2d.h"
#include"clientSocket.h"
#include"DXInput.h"
#include"user.h"
#include"BitmapLoader.h"
//constants
#define CLASSNAME _T("hello world")


//globle vars
cris::my2d my2ddraw = { 0 };
cris::DXInput input;
HWND hedit;
HWND hpasswd;
HWND hlist;
SOCKET client;
cris::BitmapLoader human;
cris::BitmapLoader target;
cris::BitmapLoader wall;
cris::BitmapLoader ground;
cris::BitmapLoader box;

//functions
LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



INT _stdcall WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {
	HCURSOR arrow = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYARROW));
	HCURSOR hand = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYHAND));
	cris::user u;
	cris::Timer timer;
	timer.start();
	int freq = 0;
	wchar_t buf[20] = { 0 };
	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW, windProc, 0L, 0L,
		hinst, NULL, NULL, NULL, NULL,
		CLASSNAME, NULL
	};
	wc.hCursor = arrow;
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hwnd = CreateWindow(CLASSNAME, _T("SOKOBAN"),
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 100, 100, 800, 600,
		NULL, NULL, hinst, NULL);

	my2ddraw.init(hwnd);
	clientConnectIni(&client);
	human.ini();
	wall.ini();
	ground.ini();
	target.ini();
	box.ini();
	human.loadPNG(hinst, my2ddraw.pRT, IDB_HUMAN, 50, 0);

	box.loadPNG(hinst, my2ddraw.pRT, IDB_BOX, 50, 0);

	target.loadPNG(hinst, my2ddraw.pRT, IDB_TARGET, 50, 0);

	wall.loadPNG(hinst, my2ddraw.pRT, IDB_WALL, 50, 0);

	ground.loadPNG(hinst, my2ddraw.pRT, IDB_GRUOND, 50, 0);
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

	int scene = 3;//默认主菜单
	int scenepre = -1;//此前界面用于win32api界面不清屏
	
	//wchar_t t[64] = L"";
	//char c[128];

	//std::cin >> c;
	//MultiByteToWideChar(0, 0, c, 128, t, 64);


	//存档选择界面渲染代码
	auto fun0 = []()//mutable
	{
		input.getInput();
	
		const wchar_t * t3 = L"使用一个账号登陆";
		const wchar_t * t1 = L"创建一个新存档";
		const wchar_t * t2 = L"选择这个存档";
		my2ddraw.pRT->DrawText(t3, wcslen(t3),
			my2ddraw.textNormal,
			D2D1::RectF(425, 150, 654, 175),
			my2ddraw.pGrayBrush);
		my2ddraw.pRT->DrawText(t1, wcslen(t1),
			my2ddraw.textNormal,
			D2D1::RectF(425, 250, 654, 275),
			my2ddraw.pGrayBrush);
		my2ddraw.pRT->DrawText(t2, wcslen(t2),
			my2ddraw.textNormal,
			D2D1::RectF(440, 350, 654, 375),
			my2ddraw.pGrayBrush);


		//调试输出
		//wchar_t buf[20] = { 0 };
		//wsprintf(buf, L"%d%d", input.isMouseButtonDown(DXInput::MIDBUTTON), y);
		//my2ddraw.pRT->DrawText(buf,20,
		//	text_normal,
		//	D2D1::RectF(0,0, size.width / 4, size.height/4),
		//	pbrush);

	};
	//注册界面渲染代码
	auto fun2 = []() 
	{
		const wchar_t * t1 = L"注册";
		const wchar_t * t2 = L"用户名:";
		const wchar_t * t3 = L"密码:";
		const wchar_t * t4 = L"提交";
		const wchar_t * t5 = L"返回";


		my2ddraw.pRT->DrawText(t1, wcslen(t1),
			my2ddraw.textNormal,
			D2D1::RectF(376,100,444,200),
			my2ddraw.pGrayBrush);
		my2ddraw.pRT->DrawText(t2, wcslen(t2),
			my2ddraw.textNormal,
			D2D1::RectF(200, 200, 300, 225),
			my2ddraw.pGrayBrush);
		my2ddraw.pRT->DrawText(t3, wcslen(t3),
			my2ddraw.textNormal,
			D2D1::RectF(225, 300, 300, 325),
			my2ddraw.pGrayBrush);
		my2ddraw.pRT->DrawText(t4, wcslen(t3),
			my2ddraw.textNormal,
			D2D1::RectF(300, 400, 350, 425),
			my2ddraw.pGrayBrush);		
		my2ddraw.pRT->DrawText(t5, wcslen(t3),
				my2ddraw.textNormal,
				D2D1::RectF(400, 400, 450, 425),
				my2ddraw.pGrayBrush);
	
	
	};

	//主界面渲染
	auto fun3 = [hand,hwnd,&scenepre,&scene,&u,&freq,&timer,&buf]() 
	{

		const wchar_t * t1 = L"主界面";
		my2ddraw.pRT->DrawText(t1, wcslen(t1),
			my2ddraw.textNormal,
			D2D1::RectF(300, 100, 375, 200),
			my2ddraw.pGrayBrush);

		const wchar_t * t2 = L"开始游戏";
		my2ddraw.pRT->DrawText(t2, wcslen(t2),
			my2ddraw.textNormal,
			D2D1::RectF(275, 200, 375, 225),
			my2ddraw.pGrayBrush);
		//帧数监测
		freq++;
		timer.stop();
		if (timer.timepassed * 1000 >= 1000) 
		{
			wsprintf(buf, L"%d", freq);
			freq = 0;
			timer.start();
		}
	
		my2ddraw.pRT->DrawText(buf, 20,
			my2ddraw.textNormal,
			D2D1::RectF(0, 0, 400, 25),
			my2ddraw.pGrayBrush);

		//鼠标监测	
		POINT p;
		input.getInput();
		GetCursorPos(&p);
		ScreenToClient(hwnd, &p);
		int x = p.x;
		int y = p.y;


		//开始游戏(275, 200, 375, 225),
		if (x < 375 && x>200 && y > 200 && y < 225)
		{
			SetCursor(hand);
			if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON))
			{
				scenepre = 3;
				scene = 4;
			}
		}
	

	};
	//地图缓存
	cris::map m(".\\maps\\screen.1");
	//游戏界面渲染
	int lastkey = -2;

	auto fun4 = [hinst,hwnd,&m,&freq,&timer,&buf,&lastkey]() 
	{
		POINT p;
		int x = 0;
		int y = 0;
		//只load一次

		int scaler;
		scaler = m.getHeight() < m.getWidth() ? m.getWidth() : m.getHeight();
		scaler = 600 / scaler;



		for (int i = 0; i < m.getHeight(); i++) 
		{
			for (int j = 0; j < m.getWidth(); j++) 
			{
				switch (m[i][j])
				{
					//箱子
				case 1:
					my2ddraw.pRT->DrawBitmap(box.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					break;
					//人
				case 2:
					//37x59,
					my2ddraw.pRT->DrawBitmap(ground.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					//(59-37)/2/59+j*scaler,i*scaler,(j + 1) * scaler-(59-37)/2/59,(i + 1) * scaler
					my2ddraw.pRT->DrawBitmap(human.pBitmap, D2D1::RectF((59.0 - 37) / 2 / 59*scaler + j*scaler + 100, i*scaler, (j + 1) * scaler - (59.0 - 37) / 2 / 59 * scaler + 100, (i + 1) * scaler));
					break;
					//箱子在目标点
				case 3:
					my2ddraw.pRT->DrawBitmap(box.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					break;
					//墙
				case 4:
					my2ddraw.pRT->DrawBitmap(wall.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100+ ( j+1) * scaler, (i+1) * scaler));
				
					break;
					//目标点
				case 5:
					my2ddraw.pRT->DrawBitmap(ground.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					my2ddraw.pRT->DrawBitmap(target.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					break;
					//地板
				case 6:
					my2ddraw.pRT->DrawBitmap(ground.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					break;
					//human on target
				case 7:
					my2ddraw.pRT->DrawBitmap(ground.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					my2ddraw.pRT->DrawBitmap(target.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					my2ddraw.pRT->DrawBitmap(human.pBitmap, D2D1::RectF((59.0 - 37) / 2 / 59 * scaler + j*scaler + 100, i*scaler, (j + 1) * scaler - (59.0 - 37) / 2 / 59 * scaler + 100, (i + 1) * scaler));
					break;
					
				}
			}
		}

		input.getInput();
		if (lastkey!=DIK_UP&& input.isKeyDown(DIK_UP)) 
		{
			m.step(cris::up);
			lastkey = DIK_UP;
		}
		if (lastkey != DIK_DOWN&&input.isKeyDown(DIK_DOWN))
		{
			m.step(cris::down);
			lastkey = DIK_DOWN;
		}
		if (lastkey != DIK_LEFT&&input.isKeyDown(DIK_LEFT))
		{
			m.step(cris::left);
			lastkey = DIK_LEFT;
		}
		if (lastkey != DIK_RIGHT&&input.isKeyDown(DIK_RIGHT))
		{

			m.step(cris::right);
			lastkey = DIK_RIGHT;
		}
		if (!(input.isKeyDown(DIK_RIGHT)| input.isKeyDown(DIK_LEFT)| input.isKeyDown(DIK_DOWN) | input.isKeyDown(DIK_UP) ))
		{
			lastkey = -2;
		}
		static bool click = false;
		if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON))
		{
			if (click) {

				input.getInput();
				GetCursorPos(&p);
				ScreenToClient(hwnd, &p);
				x = p.x;
				y = p.y;
				x = (x - 100) / scaler;
				y = y / scaler;
				m.jump(y, x);
				click = false;
			}
		}
		else 
		{
			click = true;
		}
		


		if (m.isComplete())
		{
			const wchar_t * t2 = L"完成";
			my2ddraw.pRT->DrawText(t2, wcslen(t2),
				my2ddraw.textNormal,
				D2D1::RectF(0, 100, 100, 125),
				my2ddraw.pGrayBrush);
		}
		freq++;
		timer.stop();
		if (timer.timepassed * 1000 >= 500)
		{
			wsprintf(buf, L"freq:%d", freq*2);
			freq = 0;
			timer.start();
		}

		my2ddraw.pRT->DrawText(buf, 20,
			my2ddraw.textNormal,
			D2D1::RectF(0, 0, 400, 25),
			my2ddraw.pGrayBrush);

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

			case 0://存档选择界面
			{
			
				if (scenepre != 0)
				{
					my2ddraw.draw(fun0);
					EnableWindow(hlist, true);
					scenepre = 0;
				}
				//鼠标监测	
				POINT p;
				input.getInput();
				GetCursorPos(&p);
				ScreenToClient(hwnd, &p);
				int x = p.x;
				int y = p.y;


				//登陆
				if (x > 200 && x < 400 && y>150 && y < 450)
					SetCursor(arrow);
				if (x < 600 && x>425 && y >150 && y < 175)
				{
					SetCursor(hand);
					if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON))
					{
						scenepre = 0;
						scene = 1;
						EnableWindow(hlist, false);
					}
				}

				//创建一个新存档
				if (x < 585 && x>425 && y > 250 && y < 275)
				{
					SetCursor(hand);
					if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON))
					{
						scenepre = 0;
						scene = 2;
						EnableWindow(hlist, false);
					}
				}
				//选择这个存档
				if (x <575 && x>440&& y > 350 && y < 375)
				{
					SetCursor(hand);
					if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON)) 
					{
						scenepre = 0;
						scene = 3;
						EnableWindow(hlist, false);
					}
				}
				Sleep(10);

				break;
			}
			case 2:
			{
				if (scenepre != 2)
				{
					my2ddraw.draw(fun2);
					EnableWindow(hedit, true);
					EnableWindow(hpasswd, true);
					scenepre = 2;

				}
				//鼠标监测	
				POINT p;
				input.getInput();
				GetCursorPos(&p);
				ScreenToClient(hwnd, &p);
				int x = p.x;
				int y = p.y;

				//提交
				if (x < 350 && x>300 && y > 400 && y < 425)
				{
					SetCursor(hand);
					if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON))
					{
						scenepre = 2;
						char buf[1024];
						char username[20];
						char passwd[30];
						wchar_t wusername[20];
						wchar_t wpasswd[30];
						buf[0] = 'r';
						GetWindowText(hedit, wusername, 20);
						GetWindowText(hpasswd, wpasswd, 30);
						WideCharToMultiByte(CP_ACP, 0, wusername, 20, username, 20, 0, 0);
						WideCharToMultiByte(CP_ACP, 0, wpasswd, 29, passwd, 29, 0, 0);
						strcpy_s(buf + 1, 20, username);
						strcpy_s(buf + 21, 29, passwd);

						//注册测试
						clientSend(client, buf, 1024);
						memset(buf, 0, 1024);
						clientRecv(client, buf, 1024);
						if (buf[0] == 0)
						{
							MessageBox(hwnd, L"注册失败！", L"注册失败！", MB_OK);
						}
						if (buf[1] == 't')
						{

							u.username = username;
							u.passwd = passwd;
							u.current = 0;
							u.lurd = "";
							u.save();
							//注册成功
							MessageBox(hwnd, L"注册成功！", L"注册成功！", MB_OK);
							scene = 3;
						}
						else
						{
							//注册失败
							MessageBox(hwnd, L"注册失败！", L"注册失败！", MB_OK);
						}


					}
				}
				//返回400, 400, 450, 425
				if (x < 450 && x>400 && y > 400 && y < 425)
				{
					SetCursor(hand);
					if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON))
					{
						scenepre = 2;
						scene = 0;
						EnableWindow(hedit, false);
						EnableWindow(hpasswd, false);
					}
				}
				Sleep(10);
				break;
			}
			//主界面
			case 3:
				//监测是否已有用户登陆
				if (!u.getLogin()) {
					scene = 0;
					scenepre = 3;
				}

				my2ddraw.draw(fun3);
				break;


			case 4:
				my2ddraw.draw(fun4);
				break;
				//test
			case -1:
				
				my2ddraw.draw([hinst,&timer]() 
				{

					
				});
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
		human.cleanup();
		box.cleanup();
		wall.cleanup();
		target.cleanup();
		ground.cleanup();


		input.cleanUp();
		my2ddraw.cleanup();
		clientCloseConnect(client);

		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		hedit = CreateWindow(L"edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 200, 200, 25, hwnd, 0, 0, 0);
		EnableWindow(hedit, false);
		hpasswd = CreateWindow(L"edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER|ES_PASSWORD,300, 300, 200, 25, hwnd, 0, 0, 0);
		EnableWindow(hpasswd, false);
		hlist = CreateWindow(L"listbox", NULL, LBS_NOTIFY | WS_CHILD | WS_VISIBLE | LBS_STANDARD,200, 150, 200, 300, hwnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		EnableWindow(hlist, false);


		
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
