#include"stdafx.h"
#include"App.h"

class Scene1 :cris::Scene
{
	cris::DXInput input;
	cris::my2d my2ddraw;
	Scene1(cris::DXInput input,cris::my2d my2ddraw) {
		this->input = input;
		this->my2ddraw = my2ddraw;
		SceneNum = 0;
	}

	void run()
	{
		auto fun0 = [&]()
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
		};

		if (scenepre != 0)
		{
			my2ddraw.draw(fun0);
			EnableWindow(hlist, true);
			scenepre = 0;

			//收集存档信息
			std::vector<std::string> saves;
			wchar_t savepath[] = L".\\save";
			cris::findSaves(savepath, saves);
			for (auto it = saves.begin(); it != saves.end(); it++)
			{
				wchar_t c[20] = { 0 };
				MultiByteToWideChar(CP_ACP, 0, it->c_str(), it->length() - 5, c, 260);
				SendMessage(hlist, LB_ADDSTRING, 0, (LPARAM)c);
			}
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
		if (x <575 && x>440 && y > 350 && y < 375)
		{
			SetCursor(hand);
			if (input.isMouseButtonDown(cris::DXInput::LEFTBUTTON))
			{
				wchar_t	wselected[20] = { 0 };
				HRESULT hr;
				hr = SendMessage(hlist, LB_GETCURSEL, 0, 0);
				SendMessage(hlist, LB_GETTEXT, hr, (LPARAM)wselected);
				char selected[20] = { 0 };
				WideCharToMultiByte(CP_ACP, 0, wselected, 20, selected, 20, NULL, NULL);
				u.read(selected);
				u.login();

				scenepre = 0;
				scene = 3;
				EnableWindow(hlist, false);
			}
		}
		Sleep(10);

	
	
	};
};



INT _stdcall WinMain(HINSTANCE hinst, HINSTANCE hPreinst
	, LPSTR cmdLine, int nCmdShow) {
	cris::App a(hinst,nCmdShow);

	a.winLoop();
}