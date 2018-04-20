
#include"my2d.h"
#include"clientSocket.h"
#include"DXInput.h"
#include"user.h"
#include"BitmapLoader.h"
#include"tools.h"

#include"Control.h"
using cris::direction;
using cris::down;
using cris::up;
using cris::left;
using cris::right;
//constants
#define CLASSNAME _T("sokoban")

#ifndef __WINMAIN
//globle vars
cris::my2d my2ddraw = { 0 };
cris::DXInput input;
HWND hedit;
HWND hpasswd;
//SOCKET client;
cris::MySocket client;
cris::BitmapLoader d2;
cris::BitmapLoader d12;
cris::BitmapLoader d11;
cris::BitmapLoader u2;
cris::BitmapLoader u12;
cris::BitmapLoader u11;
cris::BitmapLoader l1;
cris::BitmapLoader l2;
cris::BitmapLoader r1;
cris::BitmapLoader r2;

cris::BitmapLoader target;
cris::BitmapLoader wall;
cris::BitmapLoader ground;
cris::BitmapLoader box;
cris::BitmapLoader bot;

cris::EditControl username = { 300, 200, 200, 25, 1 ,cris::EditControl::NORMAL };
cris::EditControl passwd = { 300, 300, 200, 25, 2 ,cris::EditControl::PASSWD };

//functions
LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline void loadsaves(cris::ListControl &userlist)
{
	std::vector<std::string> saves;
	wchar_t savepath[] = L".\\save";
	cris::findSaves(savepath, saves);
	for (auto it = saves.begin(); it != saves.end(); it++)
	{
		it->erase(it->end() - 5, it->end());
		userlist.addstring(*it);
	}

}
void init(HWND hwnd, HINSTANCE hinst);
unsigned int cris::EditControl::focus = -1;

void createWndAndUpdate(HWND *hwnd, HINSTANCE hinst, HCURSOR hCursor, int nCmdShow);
INT _stdcall WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {
	HCURSOR arrow = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYARROW));
	HCURSOR hand = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYHAND));
	HCURSOR ibeam = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYIBEAM));

	cris::map m(".\\maps\\screen.1");
	cris::user u;
	
	cris::ListControl userlist = { 200, 150, 200, 300 };
	cris::TextControl login = { 425, 150,200,25,L"使用一个账号登陆" };
	cris::TextControl create = { 425, 250,175,25, L"创建一个新存档" };
	cris::TextControl choose = { 425, 350,150,25, L"选择这个存档" };
	cris::TextControl submit = { 300, 400,50,25, L"提交" };
	cris::TextControl back = { 400, 400,50,25, L"返回" };
	cris::TextControl title = { 300, 100,75,25, L"推箱子"};
	cris::TextControl start = { 275, 200,100,25,L"开始游戏" };
	cris::TextControl usertitle = { 0,0,400,25,L""};
//收集存档信息


	int scene = 3;//默认主菜单
	int scenepre = -1;//此前界面用于win32api界面不清屏
	// 
	HWND hwnd;

	createWndAndUpdate(&hwnd, hinst, arrow, nCmdShow);

	start.setWnd(hwnd);
	start.setCursor(hand);

	userlist.setWnd(hwnd);
	userlist.setCursor(hand);
	loadsaves(userlist);

	login.setWnd(hwnd);
	login.setCursor(hand);
	
	create.setWnd(hwnd);
	create.setCursor(hand);
	
	choose.setWnd(hwnd);
	choose.setCursor(hand);

	submit.setWnd(hwnd);
	submit.setCursor(hand);

	back.setWnd(hwnd);
	back.setCursor(hand);

	username.setWnd(hwnd);
	username.setCursor(ibeam);

	passwd.setWnd(hwnd);
	passwd.setCursor(ibeam);


	init(hwnd,hinst);



	
	//存档选择界面渲染代码
	auto fun0 = [&]()//mutable
	{
		input.getInput();
		userlist.draw(my2ddraw);
		login.draw(my2ddraw);
		create.draw(my2ddraw);
		choose.draw(my2ddraw);

	};
	//注册界面渲染代码
	auto fun2 = [&]() 
	{

		cris::TextControl title = { 376,100,50,25,L"注册" };
		cris::TextControl username1 = { 225, 200,75,25,L"用户名:" };
		cris::TextControl password1 = { 225, 300,75,25,L"密码:" };
		submit.draw(my2ddraw);
		back.draw(my2ddraw);
		title.draw(my2ddraw);
		password1.draw(my2ddraw);
		username1.draw(my2ddraw);
		username.draw(my2ddraw);
		passwd.draw(my2ddraw);

	};

	//主界面渲染
	auto fun3 = [&]() 
	{
		static cris::Timer timer;
		static int freq = 0;
		static wchar_t buf[20] = { 0 };
		static cris::TextControl freqx = { 765, 0,35,25,L"" };

		usertitle << u.username;
		title.draw(my2ddraw);
		start.draw(my2ddraw);
		usertitle.draw(my2ddraw);
		//帧数监测
		freq++;
		timer.stop();
		if (timer.timepassed * 1000 >=500) 
		{
			freqx << freq*2;
			freq = 0;
			timer.start();
		
		}	

		freqx.draw(my2ddraw);

	

	};
	//地图缓存

	//游戏界面渲染
	auto fun4 = [hwnd,&m]() 
	{
		static cris::Timer timer;
		static int freq = 0;
		static wchar_t buf[20] = { 0 };
		static cris::Timer t;
		static bool phase1 = false;
		static bool type1 = true;
		static direction lastDirection = down;

		POINT p;
		int x = 0;
		int y = 0;

		int scaler;
		scaler = m.getHeight() < m.getWidth() ? m.getWidth() : m.getHeight();
		scaler = 600 / scaler;
		
		cris::BitmapLoader *b = &d2;

		//*/画游戏区域
		for (int i = 0; i < m.getHeight(); i++) 
		{
			for (int j = 0; j < m.getWidth(); j++) 
			{
				switch (m[i][j])
				{
					//箱子
				case 1:
					my2ddraw.pRT->DrawBitmap(ground.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					my2ddraw.pRT->DrawBitmap(box.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					break;
					//人
				case 2:
					
					my2ddraw.pRT->DrawBitmap(ground.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					if (!phase1) {
						switch (lastDirection)
						{
						case down:
							b = &d2;
							break;
						case right:
							b = &r2;
							break;
						case up:
							b = &u2;
							break;
						case left:
							b = &l2;
							break;
						}
						my2ddraw.pRT->DrawBitmap(b->pBitmap, D2D1::RectF((59.0 - 37) / 2 / 59 * scaler + j*scaler + 100, i*scaler, (j + 1) * scaler - (59.0 - 37) / 2 / 59 * scaler + 100, (i + 1) * scaler));
					}
					else 
					{
						switch (lastDirection)
						{
						case down:
							
							if (type1)
								b = &d11;
							else
								b = &d12;
							type1 = !type1;
					
							break;
						case right:
							b = &r1;
					
							break;
						case up:
							if(type1)
								b = &u12;
							else
								b = &u11;
							type1 = !type1;
					
							break;
						case left:
							b = &l1;
							break;
						}
						my2ddraw.pRT->DrawBitmap(b->pBitmap, D2D1::RectF((59.0 - 37) / 2 / 59 * scaler + j*scaler + 100, i*scaler, (j + 1) * scaler - (59.0 - 37) / 2 / 59 * scaler + 100, (i + 1) * scaler));

						t.stop();
						if (t.timepassed * 1000 > 500)
						{
							//enter phase2
							phase1 = false;
							t.start();
						}
					}
					break;
					//箱子在目标点
				case 3:
					my2ddraw.pRT->DrawBitmap(ground.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					my2ddraw.pRT->DrawBitmap(bot.pBitmap, D2D1::RectF(100 + j * scaler, i * scaler, 100 + (j + 1) * scaler, (i + 1) * scaler));
					
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
					if (!phase1) {
						switch (lastDirection)
						{
						case down:
							b = &d2;
							break;
						case right:
							b = &r2;
							break;
						case up:
							b = &u2;
							break;
						case left:
							b = &l2;
							break;
						}
						my2ddraw.pRT->DrawBitmap(b->pBitmap, D2D1::RectF((59.0 - 37) / 2 / 59 * scaler + j*scaler + 100, i*scaler, (j + 1) * scaler - (59.0 - 37) / 2 / 59 * scaler + 100, (i + 1) * scaler));
					}
					else
					{
						switch (lastDirection)
						{
						case down:

							if (type1)
								b = &d11;
							else
								b = &d12;
							type1 = !type1;
							break;
						case right:
							b = &r1;
							break;
						case up:
							if (type1)
								b = &u12;
							else
								b = &u11;
							type1 = !type1;
							break;
						case left:
							b = &l1;
							break;
						}
						my2ddraw.pRT->DrawBitmap(b->pBitmap, D2D1::RectF((59.0 - 37) / 2 / 59 * scaler + j*scaler + 100, i*scaler, (j + 1) * scaler - (59.0 - 37) / 2 / 59 * scaler + 100, (i + 1) * scaler));
						t.stop();
						if (t.timepassed * 1000 > 500)
						{
							//enter phase2
							phase1 = false;
							t.start();
						}
					}	break;
					
				}
			}
		}


		//监测是否已完成
		if (m.isComplete())
		{
			const wchar_t * t2 = L"完成";
			my2ddraw.pRT->DrawText(t2, wcslen(t2),
				my2ddraw.textNormal,
				D2D1::RectF(0, 100, 100, 125),
				my2ddraw.pGrayBrush);
		}

		//*/帧数统计
		freq++;
		timer.stop();
		if (timer.timepassed * 1000 >= 500)
		{
			//wsprintf(buf, L"freq:%d", freq * 2);
			freq = 0;
			timer.start();
		}

		//my2ddraw.pRT->DrawText(buf, 20,
		//	my2ddraw.textNormal,
		//	D2D1::RectF(0, 0, 400, 25),
		//	my2ddraw.pGrayBrush);
		//*/帧数统计

		//*/画游戏区域

		//*/键盘控制
		input.getInput();
		static int lastkey = -2;//指示按键是否有效

		if (lastkey==-2&& input.isKeyDown(DIK_UP)) 
		{
			m.step(cris::up);
			lastkey = DIK_UP;//有键按下后任何按键无效
			lastDirection = up;
			phase1 = true;
		}
		if (lastkey ==-2&&input.isKeyDown(DIK_DOWN))
		{
			m.step(cris::down);
			lastkey = DIK_DOWN;
			lastDirection = down;
			phase1 = true;
		}
		if (lastkey ==-2 &&input.isKeyDown(DIK_LEFT))
		{
			m.step(cris::left);
			lastkey = DIK_LEFT;
			lastDirection = left;
			phase1 = true;
		}
		if (lastkey ==-2&&input.isKeyDown(DIK_RIGHT))
		{
			m.step(cris::right);
			lastkey = DIK_RIGHT;
			lastDirection = right;
			phase1 = true;
		}
		if (!(input.isKeyDown(DIK_RIGHT)| input.isKeyDown(DIK_LEFT)| input.isKeyDown(DIK_DOWN) | input.isKeyDown(DIK_UP) ))//若所有按键均松开则接下来的按键有效
		{
			lastkey = -2;
		}
		//鼠标点击
		static bool click = false;//指示鼠标点击是否有效
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
			//*/存档选择界面
			case 0:
			{
				my2ddraw.draw(fun0);
			
				input.getInput();
				userlist.testKeys(input, []() {});
				create.testKeys(input, [&]() {
					scene = 2;
				});
				login.testKeys(input, [&]() 
				{
					scene = 1;
				});
				choose.testKeys(input, [&]() 
				{
					HRESULT hr;
					std::string s;
					userlist.getselect(s);
					u.read(s);
					u.login();
					scene = 3;
				});
			
				break;
			}

			//*/注册界面
			case 2:
			{
				input.getInput();
				my2ddraw.draw(fun2);
				submit.testKeys(input, [&]() 
				{
					char buf[1024];
					char lusername[20];
					char lpasswd[30];
					buf[0] = 'r';
					WideCharToMultiByte(CP_ACP, 0, username.controlText, 20, lusername, 20, 0, 0);
					WideCharToMultiByte(CP_ACP, 0, passwd.controlText, 29, lpasswd, 29, 0, 0);
					strcpy_s(buf + 1, 20,lusername);
					strcpy_s(buf + 21, 29, lpasswd);
				if (!strcmp(lpasswd, "")) {
						MessageBox(hwnd, L"密码不能为空！", L"注册失败！", MB_OK);
						return;
					}
					//注册
					client.clientSend(buf, 1024);
					memset(buf, 0, 1024);
					client.clientRecv(buf, 1024);
					if (buf[0] == 0)
					{
						MessageBox(hwnd, L"注册失败！", L"注册失败！", MB_OK);
						return;
					}
					if (buf[1] == 't')
					{

						u.username = lusername;
						u.passwd = lpasswd;
						u.current = -1;
						u.lurd = "";
						u.save();
						//注册成功
						MessageBox(hwnd, L"注册成功！", L"注册成功！", MB_OK);
					}
					else
					{
						//注册失败
						MessageBox(hwnd, L"注册失败！", L"注册失败！", MB_OK);
						return;
					}
				});
				back.testKeys(input, [&]() 
				{
					scene = 0;
				});

				username.testKeys(input);
				passwd.testKeys(input);
				break;
			}
			//*/注册界面


			//*/主界面
			case 3:
			{
				//监测是否已有用户登陆
				if (!u.getLogin()) {
					scene = 0;
					scenepre = 3;
				}
				input.getInput();
				my2ddraw.draw(fun3);
				start.testKeys(input, [&]() 
				{
					
					scene = 4;
				});
				usertitle.testKeys(input, [&]() 
				{
					scene = 0;
				});
				break;

			}
			//*/游戏界面
			case 4:
				my2ddraw.draw(fun4);
				break;


				//test
			case -1:
				
				my2ddraw.draw([hinst]() 
				{

					
				});
				break;
			}
			

			
		}

	}
	return 0;
}
//function implements
LRESULT _stdcall windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		//app.cleanUp();
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

		PostQuitMessage(0);
		return 0;

	case WM_CHAR:
		if (cris::EditControl::focus == passwd.id)
			passwd.onInput(wParam);
		if (cris::EditControl::focus == username.id)
			username.onInput(wParam);

		break;
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

	client.clientConnectIni();
	my2ddraw.init(hwnd);
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
	//*/
	//dinput
	input.inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	
}
#endif // !__WINMAIN