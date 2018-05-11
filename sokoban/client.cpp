
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
namespace app {
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

}
unsigned int cris::Control::focus = -1;
bool cris::Control::click = false;
HWND cris::Control::shwnd = 0;
HCURSOR cris::Control::shcur = 0;
cris::my2d &cris::Control::smy2d = app::my2ddraw;
cris::DXInput &cris::Control::sinput = app::input;
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
	cris::TextControl next = { 688,300,100,25,L"下一关" };
	//主界面
	cris::TextControl restore = { 275, 200,100,25,L"继续游戏" };
	cris::TextControl title = { 300, 100,75,25, L"推箱子" };
	cris::TextControl start = { 275, 225,100,25,L"关卡选择" };
	cris::TextControl usertitle = { 0,0,400,25,L"" };
	//地图选择界面
	cris::TextControl levelstart = { 425, 150,100,25,L"开始游戏" };
	cris::ListControl maplist = { 200, 150, 200, 300 };
	cris::TextControl upload = { 425, 200,100,25,L"上传地图" };
	cris::TextControl download = { 425, 250,100,25,L"下载地图" };
	
	//test
	cris::TextControl test = { 0,400,200,300,L"" };
	
	//
	int scene = 3;//默认主菜单
	HWND hwnd;
	createWndAndUpdate(&hwnd, hinst, arrow, nCmdShow);
	//ini
	{
		cris::Control::shwnd = hwnd;
		cris::Control::shcur = hand;
		title.setCursor(arrow);
		loadmaps(maplist);
		loadsaves(userlist);
		app::username.setCursor(ibeam);
		app::passwd.setCursor(ibeam);

		std::cout;
		init(hwnd, hinst);
	}



	//存档选择界面渲染代码
	auto fun0 = [&]()//mutable
	{
		app::input.getInput();
		userlist.draw();
		login.draw();
		create.draw();
		choose.draw();

	};
	//登陆界面渲染
	auto fun1 = [&]()
	{

		cris::TextControl title = { 376,100,50,25,L"登陆" };
		cris::TextControl username1 = { 225, 200,75,25,L"用户名:" };
		cris::TextControl password1 = { 225, 300,75,25,L"密码:" };
		title.setCursor(arrow);
		submit.draw();
		back.draw();
		title.draw();
		password1.draw();
		username1.draw();
		app::username.draw();
		app::passwd.draw();

	};
	//注册界面渲染代码
	auto fun2 = [&]()
	{

		cris::TextControl title = { 376,100,50,25,L"注册" };
		cris::TextControl username1 = { 225, 200,75,25,L"用户名:" };
		cris::TextControl password1 = { 225, 300,75,25,L"密码:" };
		title.setCursor(arrow);
		submit.draw();
		back.draw();
		title.draw();
		password1.draw();
		username1.draw();
		app::username.draw();
		app::passwd.draw();

	};
	//主界面渲染
	auto fun3 = [&]()
	{
		static cris::Timer timer;
		static int freq = 0;
		static cris::TextControl freqx = { 765, 0,35,25,L"" };
		freqx.setCursor(arrow);
		usertitle << app::u.username;
		title.draw();
		start.draw();
		usertitle.draw();
		restore.draw();
		//帧数监测
		freq++;
		timer.stop();
		if (timer.timepassed * 1000 >= 500)
		{
			freqx << freq * 2;
			freq = 0;
			timer.start();

		}

		freqx.draw();



	};
	//游戏界面渲染
	auto fun4 = [&]()
	{
		app::game.draw();
		gameback.draw();
		backward.draw();
		next.draw();
	};
	//地图选择界面
	auto fun5 = [&]()
	{
		maplist.draw();
		gameback.draw();
		levelstart.draw();
		upload.draw();
	};
	//登陆监测
	if (!app::u.getLogin()) {
		scene = 0;

	}
	else
	{
		char buf[1024];
		memset(buf, 0, 1024);
		buf[0] = 'l';
		strcpy_s(buf + 1, 20, app::u.username.c_str());
		strcpy_s(buf + 21, 29, app::u.passwd.c_str());

		app::client.clientSend(buf, 1024);
		memset(buf, 0, 1024);
		app::client.clientRecv(buf, 1024);

	}
	//地图对比；
	{
		//服务器获取地图列表
		char buf[1024];
		buf[0] = 'm';
		app::client.clientSend(buf, 1024);
		memset(buf, 0, 1024);
		app::client.clientRecv(buf, 1024);
		std::vector<std::string> maps;

		//本地地图列表
		wchar_t savepath[] = L".\\maps";
		cris::findXSBs(savepath, maps);
		//排序方便查找
		std::sort(maps.begin(), maps.end(), [](std::string a, std::string b)->bool
		{
			int x = std::stoi(a);
			int y = std::stoi(b);
			return x < y;
		});
		for (int i = 0; i < 1024; i++)
		{
			if (buf[i] == 0)
			{
				break;
			}
			//二分查找
			bool b = std::binary_search(maps.begin(), maps.end(), std::to_string((int)buf[i]) + ".xsbs");
			if (!b)
				maplist.addstring(std::to_string((int)buf[i]) + ".xsbs\t未下载");
		}
	}
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
				app::my2ddraw.draw(fun0);

				app::input.getInput();
				userlist.testKeys();
				create.testKeys([&scene]() {scene = 2; });
				login.testKeys([&scene]() {scene = 1; });
				choose.testKeys([&scene, &userlist]()
				{
					HRESULT hr;
					std::string s;
					userlist.getselect(s);
					app::u.read(s);
					app::u.login();
					scene = 3;
				});
				break;
			}
			//登陆界面
			case 1:
			{
				app::input.getInput();
				app::my2ddraw.draw(fun2);
				submit.testKeys([&scene, hwnd]()
				{
					char buf[1024];
					char lusername[20];
					char lpasswd[30];
					buf[0] = 'l';
					WideCharToMultiByte(CP_ACP, 0, app::username.controlText, 20, lusername, 20, 0, 0);
					WideCharToMultiByte(CP_ACP, 0, app::passwd.controlText, 29, lpasswd, 29, 0, 0);
					strcpy_s(buf + 1, 20, lusername);
					strcpy_s(buf + 21, 29, lpasswd);
					if (!strcmp(lpasswd, "")) {
						MessageBox(hwnd, L"密码不能为空！", L"登陆失败！", MB_OK);
						return;
					}
					//注册
					app::client.clientSend(buf, 1024);
					memset(buf, 0, 1024);
					app::client.clientRecv(buf, 1024);
					if (buf[0] == 0)
					{
						MessageBox(hwnd, L"登陆失败！", L"登陆失败！", MB_OK);
						return;
					}
					if (buf[1] == 't')
					{

						app::u.username = lusername;
						app::u.passwd = lpasswd;
						app::u.current = -1;
						app::u.lurd = "";
						app::u.save();
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
				back.testKeys([&scene]() {scene = 0; });

				app::username.testKeys();
				app::passwd.testKeys();
				break;
			}
			//*/注册界面
			case 2:
			{
				app::input.getInput();
				app::my2ddraw.draw(fun2);
				submit.testKeys([&scene, hwnd]()
				{
					char buf[1024];
					char lusername[20];
					char lpasswd[30];
					buf[0] = 'r';
					WideCharToMultiByte(CP_ACP, 0, app::username.controlText, 20, lusername, 20, 0, 0);
					WideCharToMultiByte(CP_ACP, 0, app::passwd.controlText, 29, lpasswd, 29, 0, 0);
					strcpy_s(buf + 1, 20, lusername);
					strcpy_s(buf + 21, 29, lpasswd);
					if (!strcmp(lpasswd, "")) {
						MessageBox(hwnd, L"密码不能为空！", L"注册失败！", MB_OK);
						return;
					}
					//注册
					app::client.clientSend(buf, 1024);
					memset(buf, 0, 1024);
					app::client.clientRecv(buf, 1024);
					if (buf[0] == 0)
					{
						MessageBox(hwnd, L"注册失败！", L"注册失败！", MB_OK);
						return;
					}
					if (buf[1] == 't')
					{

						app::u.username = lusername;
						app::u.passwd = lpasswd;
						app::u.current = -1;
						app::u.lurd = "";
						app::u.save();
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
				back.testKeys([&scene]() {scene = 0; });

				app::username.testKeys();
				app::passwd.testKeys();
				break;
			}
			//*/注册界面


			//*/主界面
			case 3:
			{
				//监测是否已有用户登陆

				app::input.getInput();
				app::my2ddraw.draw(fun3);
				start.testKeys([&scene]() {scene = 5; });
				usertitle.testKeys([&scene]() {scene = 0; });
				restore.testKeys([&]() 
				{
					maplist.cursor = app::u.current;
					std::string file;
					maplist.getselect(file);
					app::m.readMapFromFile(".\\maps\\" + file + ".xsbs");
					app::u.lurd = "";
					app::u.current = std::stoi(file);
					scene = 4;
				});
				break;

			}
			//*/游戏界面
			case 4:
				app::input.getInput();
				app::my2ddraw.draw(fun4);
				app::game.testkeys(app::input);
				gameback.testKeys([&scene]() {scene = 5; });
				next.testKeys([&]() 
				{
					//下一关
					maplist.cursor++;
					scene = 4;
					std::string file;
					maplist.getselect(file);
					app::m.readMapFromFile(".\\maps\\" + file + ".xsbs");
					app::u.lurd = "";
					app::u.current = std::stoi(file);
				});
				backward.testKeys([&scene]()
				{
					if (app::u.lurd.size() <= 0)
						return;
					char ch = app::u.lurd[app::u.lurd.size() - 1];
				
					app::u.lurd.erase(app::u.lurd.size() - 1);
					app::m.back(app::u);
					if (app::u.lurd.size() <= 0) {
						app::game.lastDirection = down;
						return;
					}
					ch = app::u.lurd[app::u.lurd.size() - 1];
					switch (ch)
					{
					case 'u':
						app::game.lastDirection = up;
						break;
					case 'd':
						app::game.lastDirection = down;
						break;
					case 'r':
						app::game.lastDirection = right;
						break;
					case 'l':
						app::game.lastDirection = left;
						break;
					}


				});
				break;
				//*/地图选择界面
			case 5:
				app::input.getInput();
				app::my2ddraw.draw(fun5);
				gameback.testKeys([&scene]() {scene = 3; });
				maplist.testKeys();
				levelstart.testKeys([&scene, &maplist]()
				{
					scene = 4;
					std::string file;
					maplist.getselect(file);
					app::m.readMapFromFile(".\\maps\\" + file + ".xsbs");
					app::u.lurd = "";
					app::u.current = std::stoi( file);
				});
				upload.testKeys([&maplist]()
				{
					std::string file;
					maplist.getselect(file);
					MapStream map = { ".\\maps\\" + file + ".xsbs" ,true };
					char buf[1024];
					do {

						map >> buf;
						app::client.clientSend(buf, 1024);
						memset(buf, 0, 1024);
						app::client.clientRecv(buf, 1024);

					} while (buf[0] == 'v');
					if (buf[0] == 'u'&&buf[1] == 't')
						MessageBox(0, L"上传成功", L"上传成功", 0);

				});


				break;

				//test
			case -1:

				app::my2ddraw.draw([hinst]()
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
		app::game.cleanup();
		app::input.cleanUp();
		app::my2ddraw.cleanup();
		app::client.clientCloseConnect();
		app::u.save();
		PostQuitMessage(0);
		return 0;

	case WM_CHAR:
		if (cris::EditControl::focus == app::passwd.id)
			app::passwd.onInput(wParam);
		if (cris::EditControl::focus == app::username.id)
			app::username.onInput(wParam);

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
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 100, 100, 850, 600,
		NULL, NULL, hinst, NULL);


	ShowWindow(*hwnd, nCmdShow);
	UpdateWindow(*hwnd);
}
void init(HWND hwnd, HINSTANCE hinst)
{

	app::client.clientConnectIni();
	bool b = app::client.isConnected();
	app::my2ddraw.init(hwnd);
	app::game.init(hwnd, hinst, app::my2ddraw);
	//*/
	//dinput
	app::input.inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


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
		return x < y;

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
