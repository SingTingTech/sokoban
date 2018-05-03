
#include"my2d.h"
#include"clientSocket.h"
#include"DXInput.h"
#include"user.h"
#include"tools.h"
#include"MapStream.h"
#include"Control.h"

using cris::direction;
using cris::down;
using cris::up;
using cris::left;
using cris::right;
//constants
#define CLASSNAME _T("sokoban")

#ifndef __WINMAIN
#define __WINMAIN
//globle vars
cris::my2d my2ddraw = { 0 };
cris::DXInput input;
//SOCKET client;
cris::MySocket client;


cris::EditControl username = { 300, 200, 200, 25, 1 ,cris::EditControl::NORMAL };
cris::EditControl passwd = { 300, 300, 200, 25, 2 ,cris::EditControl::PASSWD };
//地图缓存
cris::map m(".\\maps\\0.xsbs");
//当前用户信息
cris::user u;
cris::GameControl game = { u,m };
//初始化焦点
unsigned int cris::Control::focus = -1;
bool cris::Control::click = false;

//functions
//消息处理函数
LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//加载存档
void loadsaves(cris::ListControl &userlist);
void loadmaps(cris::ListControl &maplist);

//初始化
void init(HWND hwnd, HINSTANCE hinst);

//创建并更新窗口
void createWndAndUpdate(HWND *hwnd, HINSTANCE hinst, HCURSOR hCursor, int nCmdShow);

//__winmain
INT _stdcall WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {
	HCURSOR arrow = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYARROW));
	HCURSOR hand = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYHAND));
	HCURSOR ibeam = LoadCursor(hinst, MAKEINTRESOURCE(IDC_MYIBEAM));


	//界面元素
	//存档选择界面
	cris::ListControl userlist = { 200, 150, 200, 300 };
	cris::TextControl login = { 425, 150,200,25,L"使用一个账号登陆" };
	cris::TextControl create = { 425, 250,175,25, L"创建一个新存档" };
	cris::TextControl choose = { 425, 350,150,25, L"选择这个存档" };
	//注册界面
	cris::TextControl submit = { 300, 400,50,25, L"提交" };
	cris::TextControl back = { 400, 400,50,25, L"返回" };
	//游戏界面
	cris::TextControl gameback = { 0,0,50,25, L"返回" };
	cris::TextControl backward = { 688,400,100,25,L"后退一步" };
	//主界面
	cris::TextControl title = { 300, 100,75,25, L"推箱子" };
	cris::TextControl start = { 275, 200,100,25,L"关卡选择" };
	cris::TextControl usertitle = { 0,0,400,25,L"" };
	//地图选择界面
	cris::TextControl levelstart = { 425, 150,100,25,L"开始游戏" };
	cris::ListControl maplist = { 200, 150, 200, 300 };
	cris::TextControl upload = { 425, 200,100,25,L"上传地图" };
	//test
	cris::TextControl test = { 0,400,200,300,L"" };
	int scene = 3;//默认主菜单
	HWND hwnd;
	createWndAndUpdate(&hwnd, hinst, arrow, nCmdShow);
	//ini
	{
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

		gameback.setWnd(hwnd);
		gameback.setCursor(hand);

		username.setWnd(hwnd);
		username.setCursor(ibeam);

		passwd.setWnd(hwnd);
		passwd.setCursor(ibeam);

		usertitle.setWnd(hwnd);
		usertitle.setCursor(hand);

		backward.setWnd(hwnd);
		backward.setCursor(hand);

		maplist.setWnd(hwnd);
		maplist.setCursor(hand);
		loadmaps(maplist);

		levelstart.setWnd(hwnd);
		levelstart.setCursor(hand);

		upload.setWnd(hwnd);
		upload.setCursor(hand);

		init(hwnd, hinst);
	}



	//存档选择界面渲染代码
	auto fun0 = [&]()//mutable
	{
		input.getInput();
		userlist.draw(my2ddraw);
		login.draw(my2ddraw);
		create.draw(my2ddraw);
		choose.draw(my2ddraw);

	};
	//登陆界面渲染
	auto fun1 = [&]()
	{

		cris::TextControl title = { 376,100,50,25,L"登陆" };
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
		static cris::TextControl freqx = { 765, 0,35,25,L"" };

		usertitle << u.username;
		title.draw(my2ddraw);
		start.draw(my2ddraw);
		usertitle.draw(my2ddraw);
		//帧数监测
		freq++;
		timer.stop();
		if (timer.timepassed * 1000 >= 500)
		{
			freqx << freq * 2;
			freq = 0;
			timer.start();

		}

		freqx.draw(my2ddraw);



	};
	//游戏界面渲染
	auto fun4 = [&]()
	{
		game.draw(my2ddraw);
		gameback.draw(my2ddraw);
		backward.draw(my2ddraw);
	};
	//地图选择界面
	auto fun5 = [&]() 
	{
		maplist.draw(my2ddraw);
		gameback.draw(my2ddraw);
		levelstart.draw(my2ddraw);
		upload.draw(my2ddraw);
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
				userlist.testKeys(input);
				create.testKeys(input, [&scene]() {scene = 2;});
				login.testKeys(input, [&scene](){scene = 1;});
				choose.testKeys(input, [&scene,&userlist]()
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
			//登陆界面
			case 1:
			{
				input.getInput();
				my2ddraw.draw(fun2);
				submit.testKeys(input, [&scene,hwnd]()
				{
					char buf[1024];
					char lusername[20];
					char lpasswd[30];
					buf[0] = 'l';
					WideCharToMultiByte(CP_ACP, 0, username.controlText, 20, lusername, 20, 0, 0);
					WideCharToMultiByte(CP_ACP, 0, passwd.controlText, 29, lpasswd, 29, 0, 0);
					strcpy_s(buf + 1, 20, lusername);
					strcpy_s(buf + 21, 29, lpasswd);
					if (!strcmp(lpasswd, "")) {
						MessageBox(hwnd, L"密码不能为空！", L"登陆失败！", MB_OK);
						return;
					}
					//注册
					client.clientSend(buf, 1024);
					memset(buf, 0, 1024);
					client.clientRecv(buf, 1024);
					if (buf[0] == 0)
					{
						MessageBox(hwnd, L"登陆失败！", L"登陆失败！", MB_OK);
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
						MessageBox(hwnd, L"登陆失败！", L"登陆失败！", MB_OK);
					}
					else
					{
						//注册失败
						MessageBox(hwnd, L"登陆失败！", L"登陆失败！", MB_OK);
						return;
					}
				});
				back.testKeys(input, [&scene](){scene = 0;});

				username.testKeys(input);
				passwd.testKeys(input);
				break;
			}
			//*/注册界面
			case 2:
			{
				input.getInput();
				my2ddraw.draw(fun2);
				submit.testKeys(input, [&scene,hwnd]()
				{
					char buf[1024];
					char lusername[20];
					char lpasswd[30];
					buf[0] = 'r';
					WideCharToMultiByte(CP_ACP, 0, username.controlText, 20, lusername, 20, 0, 0);
					WideCharToMultiByte(CP_ACP, 0, passwd.controlText, 29, lpasswd, 29, 0, 0);
					strcpy_s(buf + 1, 20, lusername);
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
				back.testKeys(input, [&scene]() {scene = 0; });

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
				}
				input.getInput();
				my2ddraw.draw(fun3);
				start.testKeys(input, [&scene]() {scene = 5; });
				usertitle.testKeys(input, [&scene]() {scene = 0; });
				break;

			}
			//*/游戏界面
			case 4:
				input.getInput();
				my2ddraw.draw(fun4);
				game.testkeys(input);
				gameback.testKeys(input, [&scene]() {scene = 5; });
				backward.testKeys(input, [&scene]()
				{
					if (u.lurd.size() <= 0)
						return;
					char ch = u.lurd[u.lurd.size() - 1];
					m.back(game.lastDirection);
					u.lurd.erase(u.lurd.size() - 1);
					if (u.lurd.size() <= 0) {
						game.lastDirection = down;
						return;
					}
					ch = u.lurd[u.lurd.size() - 1];
					switch (ch)
					{
					case 'u':
						game.lastDirection = up;
						break;
					case 'd':
						game.lastDirection = down;
						break;
					case 'r':
						game.lastDirection = right;
						break;
					case 'l':
						game.lastDirection = left;
						break;
					}
				});
					break;
			//*/地图选择界面
			case 5:
				input.getInput();
				my2ddraw.draw(fun5);
				gameback.testKeys(input, [&scene]() {scene = 3; });
				maplist.testKeys(input);
				levelstart.testKeys(input, [&scene,&maplist]() 
				{
					scene = 4;
					std::string file;
					maplist.getselect(file);
					m.readMapFromFile(".\\maps\\" + file + ".xsbs");
				});
				upload.testKeys(input, [&maplist]()
				{
					std::string file;
					maplist.getselect(file);
					MapStream map = { ".\\maps\\" + file + ".xsbs" ,true};
					char buf[1024];
					do {
						
						map >> buf;
						client.clientSend(buf, 1024);
						memset(buf, 0, 1024);
						client.clientRecv(buf, 1024);

					} while (buf[0]=='v');
				});
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
		game.cleanup();
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
	game.init(hwnd, hinst, my2ddraw);
	//*/
	//dinput
	input.inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


}
void loadmaps(cris::ListControl &maplist)
{
	std::vector<std::string> maps;
	wchar_t savepath[] = L".\\maps";
	cris::findXSBs(savepath, maps);
	std::sort(maps.begin(), maps.end(), [](std::string a, std::string b)->bool
	{
		int x = std::stoi(a);
		int y = std::stoi(b);
		return x< y;

	});
	for (auto it = maps.begin(); it != maps.end(); it++)
	{
		it->erase(it->end() - 5, it->end());
		maplist.addstring(*it);
	}

}
void loadsaves(cris::ListControl &userlist)
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
#endif // !__WINMAIN
