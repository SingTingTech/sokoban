#pragma once
#include"stdafx.h"
#include"my2d.h"
#include"DXInput.h"
#include"BitmapLoader.h"
#include"direction.h"
#include"map.h"
#pragma comment(lib,"imm32.lib")
namespace cris
{
	class Control
	{
		
	protected:
		static bool click;
	
		HWND hwnd;	//父窗口句柄
		HCURSOR hcursor;
		D2D1_RECT_F size;//窗口大小
	public:
		static HWND shwnd;
		static HCURSOR shcur;
		static cris::my2d &smy2d;
		static cris::DXInput &sinput;
		static unsigned int focus;
		Control(float x, float y, float width, float height) {
			size.left = x;
			size.right = x + width;
			size.top = y;
			size.bottom = y + height;
		}
		bool mouseOn()
		{
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hwnd, &p);
			return p.x<size.right&&p.x>size.left&&p.y > size.top&&p.y < size.bottom;
		}
		wchar_t controlText[1024];
		virtual void draw() //绘制
		{

			hwnd = shwnd;
			hcursor = hcursor == NULL ? shcur : hcursor;
			if (mouseOn())
			{
				SetCursor(hcursor);
			}


		}
		template<class T>
		void testKeys(T t)
		{

		};//获取按键信息
		virtual void setCursor(HCURSOR hcursor)
		{
			this->hcursor = hcursor;
		}
		virtual void setWnd(HWND hwnd)
		{
			this->hwnd = hwnd;
		}
	};
	
	class TextControl :public Control
	{
	public:
		TextControl(float x, float y, float width, float height, const wchar_t*text) :Control(x, y, width, height)
		{
			wcscpy_s(controlText, text);
		}
		void operator<<(std::string s)
		{
			memset(controlText, 0, 1024);
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length() + 1, controlText, 1024);
		}
		void operator<<(int i)
		{
			memset(controlText, 0, 1024);
			wsprintf(controlText, L"%d", i);
		}
		virtual void draw()
		{

			Control::draw();
			smy2d.pRT->DrawText(controlText, wcslen(controlText),
				smy2d.textNormal,
				size,
				smy2d.pGrayBrush);
		};
		template<class T>
		void testKeys(T t) {
			
			if (sinput.isMouseButtonDown(DXInput::LEFTBUTTON))
			{
				if (mouseOn() && click)
				{
					t();
					click = false;
				}

			}
			else
			{
				click = true;
			}
		}


	};	
	class EditControl :public Control
	{
		public:
		enum EditType
		{
			NORMAL,
			PASSWD
		};

		unsigned int id;
		EditType type;
		int cur = 0;//光标位置
		EditControl(float x, float y, float width, float height,unsigned int id,EditType type) :Control(x, y, width, height)
		{
			this->id = id;
			this->type = type;
		}
		//获取输入
		virtual void onInput(wchar_t ch)
		{

			if (ch == 8 && cur > 0)controlText[--cur] = 0;
			if ((ch > 6 && ch < 14) || ch == 27 || ch == 32)
				return;
			controlText[cur++] = ch;
		}
		virtual void draw()
		{

			Control::draw();
			
			wchar_t outtext[1024];
			memset(outtext, 0, 1024);
			if (type == PASSWD) 
			{
				for (int i = 0; i < wcslen(controlText); ++i) 
				{
					outtext[i] = L'*';
				}
			}
			else 
			{
				wcscpy_s(outtext, controlText);
			}
			smy2d.pRT->DrawText(outtext, wcslen(outtext),
				smy2d.textNormal,
				size,
				smy2d.pGrayBrush);
			smy2d.pRT->DrawLine(D2D1::Point2F(size.left, size.bottom), D2D1::Point2F(size.right, size.bottom), smy2d.pGrayBrush);
		/*	my2ddraw.pRT->DrawLine(D2D1::Point2F(wcslen(controlText)*10.5 + size.left, size.top), D2D1::Point2F(wcslen(controlText)*10.5 + size.left, size.bottom), my2ddraw.pGrayBrush);
		*/};
		void testKeys() {
			if (sinput.isMouseButtonDown(DXInput::LEFTBUTTON))
			{
				if (mouseOn() && click)
				{
					focus = this->id;
					click = false;
				}

			}
			else
			{
				click = true;
			}
		}
	
	};
	
	class ListControl :public Control
	{
	public:
		int cursor;
		std::vector<std::string> list;
		//0-1之间的小数 进度条
		float moveBar = 0;
		ListControl(float x, float y, float width, float height) :Control(x, y, width, height)
		{
			cursor = 0;
		}
		void addstring(std::string str) {
			list.push_back(str);
		}
		void getselect(std::string &s)
		{
			if (cursor != -1)
			{
				s = list[cursor];
			}
			else
			{
				s = "";
			}
		}
		void setselect(int i)
		{
			cursor = i;
		}
		void setMove(float i)
		{
			if (i >= 0 && i <= 1)
				moveBar = i;
		}
		void draw()
		{
			Control::draw();
			smy2d.pRT->FillRectangle(size, smy2d.pListBrush);
			float fullSize = list.size() * 20;
			float controlHeight = size.bottom - size.top;
			if (fullSize < controlHeight)
			{
				//无滚动条
				if (cursor >= 0)
					smy2d.pRT->FillRectangle(D2D1::Rect(size.left, size.top + 20.0f*cursor, size.right, size.top + 20.0f * cursor + 20.f), smy2d.pDarkBrush);
				float fullSize = list.size() * 20;

				wchar_t buffer[30] = { 0 };
				int i = 0;
				for (auto it = list.begin(); it != list.end(); it++, i++)
				{
					memset(buffer, 0, 30);
					MultiByteToWideChar(CP_ACP, 0, list[i].c_str(), list[i].length(), buffer, 30);
					smy2d.pRT->DrawText(buffer, 30,
						smy2d.textSmall,
						D2D1::Rect(size.left, size.top + 20.0f*i, size.right, size.top + 20.0f * (i + 1)),
						smy2d.pGrayBrush);
				}
			}
			else
			{
				//有滚动条
				//虚拟顶端
				float a = size.top - (fullSize - controlHeight)*moveBar;
				if (cursor >= 0)
				{
					float top,bottom;
					top = a + 20.0f*cursor < size.top ? size.top : a + 20.0f*cursor;
					bottom = a + 20.0f * (cursor + 1) > size.bottom ? size.bottom : a + 20.0f * (cursor + 1);
					if(top<bottom)
						smy2d.pRT->FillRectangle(D2D1::Rect(size.left, top, size.right - 25, bottom), smy2d.pDarkBrush);
				}
					
				float moveBarLength = controlHeight*(controlHeight / fullSize);

				float startPoint = (controlHeight - moveBarLength)*moveBar;
				//绘制list
				wchar_t buffer[30] = { 0 };
				int i = 0;
				for (auto it = list.begin(); it != list.end(); it++, i++)
				{
					//*/截面绘制
					if (a + 20.0f*i +20.f < size.top)
					{
						continue;
					}
					if (a + 20.0f*i > size.bottom)//向下截断
						break;

					memset(buffer, 0, 30);
					MultiByteToWideChar(CP_ACP, 0, list[i].c_str(), list[i].length(), buffer, 30);
					smy2d.pRT->DrawText(buffer, 30, smy2d.textSmall, D2D1::Rect(size.left, a + 20.0f*i, size.right - 25, a + 20.0f * (i + 1) > size.bottom ? size.bottom : a + 20.0f * (i + 1)), smy2d.pGrayBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				}
				//绘制滑块
				smy2d.pRT->FillRectangle(D2D1::Rect(size.right - 25, size.top + startPoint, size.right, size.top + startPoint + moveBarLength), smy2d.pLightBrush);
				//抹除顶部
				smy2d.pRT->FillRectangle(D2D1::Rect(size.left, size.top -20, size.right, size.top), smy2d.pBgBrush);


			}

		}
	
		void testKeys()
		{
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hwnd, &p);
			float fullSize = list.size() * 20;
			float controlHeight = size.bottom - size.top;
			float moveBarLength = controlHeight*(controlHeight / fullSize);
			float startPoint = (controlHeight - moveBarLength)*moveBar;
			float a = size.top - (fullSize - controlHeight)*moveBar;
			//*/鼠标监测
			static bool drag = false;
			if (sinput.isMouseButtonDown(DXInput::LEFTBUTTON))
			{
				if (click)
				{
					//监测在哪个区域单击


					//滚动条上
					if (fullSize > controlHeight &&p.x<size.right&&p.x>size.right - 25 && p.y > size.top + startPoint&&size.top + startPoint + moveBarLength > p.y)
					{
						drag = true;
						click= false;
					}

					//列表区域
					if (p.x<size.right - ((fullSize > controlHeight) ? 25 : 0) && p.x>size.left&&p.y > size.top&&p.y < size.bottom)
					{
						int i = (p.y - ((fullSize > controlHeight) ? a : size.top)) / 20;
						cursor = i > list.size()?cursor:i;

						click = false;
					}
				}
				if (drag)
				{
					int i = sinput.getMouseYCoordinate();
					moveBar += i / (50.0f*(fullSize/controlHeight));
					moveBar = moveBar > 1 ? 1 : moveBar < 0 ? 0 : moveBar;

				}

			}
			else
			{
				click = true;
				drag = false;
			}
			//*/鼠标监测
		}
	};
	class CheckBox:public Control
	{
public:
		bool isSelected;
		CheckBox(float x,float y) :Control(x, y, 15, 15), isSelected(false)
		{
			
		}
		void draw()
		{
			Control::draw();
			//bg
			smy2d.pRT->FillRectangle(size, smy2d.pGrayBrush);
			
		
			if(isSelected)//checked
				smy2d.pRT->FillRectangle(D2D1::RectF(size.left+2.5,size.top+2.5,size.right-2.5,size.bottom-2.5), smy2d.pDarkBrush);
			else//unchecked			
				smy2d.pRT->FillRectangle(D2D1::RectF(size.left + 2.5, size.top + 2.5, size.right - 2.5, size.bottom - 2.5), smy2d.pBgBrush);


		}
		void testKeys()
		{
		if ( sinput.isMouseButtonDown(DXInput::LEFTBUTTON))
			{
				if (mouseOn() && click)
				{
					isSelected = !isSelected;
					click = false;
				}

			}
			else
			{
				click = true;
			}
		}
	};
	class GameControl
	{
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
		HWND hwnd;
		user &u;
		map &m;
		cris::Timer timer;
		int freq = 0;
		wchar_t buf[20] = { 0 };
		cris::Timer t;
		bool phase1 = false;
		bool type1 = true;
		POINT p;
		int x = 0;
		int y = 0;
		int scaler;//startpoint
		POINT sp;
		my2d my2ddraw;

	public:
		direction lastDirection = down;

		void init(HWND hwnd,HINSTANCE hinst,my2d &my2ddraw)
		{
			this->hwnd = hwnd;
			this->my2ddraw = my2ddraw;
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
		}
		void cleanup() 
		{
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

		}
		void draw() 
		{
			scaler = m.getHeight() < m.getWidth() ? m.getWidth() : m.getHeight();
			scaler = 600 / scaler;

			cris::BitmapLoader *b = &d2;
			
			sp.x = 100 + (m.getHeight() < m.getWidth() ? 0 : (600.f - m.getWidth()*(600.f / m.getHeight())) / 2);
			sp.y = (m.getHeight() < m.getWidth() ? (600.f - m.getHeight()*(600.f / m.getWidth())) / 2 : 0) - 31;
			//*/画游戏区域 
			for (int i = 0; i < m.getHeight(); i++)
			{
				for (int j = 0; j < m.getWidth(); j++)
				{
					switch (m[i][j])
					{
#define RECTIJ(i,j) D2D1::RectF(j * scaler+sp.x, i * scaler+sp.y, (j + 1) * scaler+sp.x, (i + 1) * scaler+sp.y)
#define HUMANRECT(i,j)  D2D1::RectF((59.0 - 37) / 2 / 59 * scaler + j*scaler +sp.x, i*scaler+sp.y, (j + 1) * scaler - (59.0 - 37) / 2 / 59 * scaler+sp.x, (i + 1) * scaler+sp.y)
						
						//箱子
					case 1:
						my2ddraw.pRT->DrawBitmap(ground.pBitmap, RECTIJ(i, j));
						my2ddraw.pRT->DrawBitmap(box.pBitmap, RECTIJ(i, j));
						break;
						//人
					case 2:

						my2ddraw.pRT->DrawBitmap(ground.pBitmap, RECTIJ(i, j));
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
							my2ddraw.pRT->DrawBitmap(b->pBitmap, HUMANRECT(i, j));
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
							my2ddraw.pRT->DrawBitmap(b->pBitmap, HUMANRECT(i, j));

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
						my2ddraw.pRT->DrawBitmap(ground.pBitmap, RECTIJ(i, j));
						my2ddraw.pRT->DrawBitmap(bot.pBitmap, RECTIJ(i, j));

						break;
						//墙
					case 4:
						my2ddraw.pRT->DrawBitmap(wall.pBitmap, RECTIJ(i, j));

						break;
						//目标点
					case 5:
						my2ddraw.pRT->DrawBitmap(ground.pBitmap, RECTIJ(i, j));
						my2ddraw.pRT->DrawBitmap(target.pBitmap, RECTIJ(i, j));
						break;
						//地板
					case 6:
						my2ddraw.pRT->DrawBitmap(ground.pBitmap, RECTIJ(i, j));
						break;
						//human on target
					case 7:
						my2ddraw.pRT->DrawBitmap(ground.pBitmap, RECTIJ(i, j));
						my2ddraw.pRT->DrawBitmap(target.pBitmap, RECTIJ(i, j));
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
							my2ddraw.pRT->DrawBitmap(b->pBitmap, HUMANRECT(i, j));
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
							my2ddraw.pRT->DrawBitmap(b->pBitmap, HUMANRECT(i, j));
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
		}
		void testkeys(DXInput input) 
		{
			static int lastkey = -2;//指示按键是否有效

			if (lastkey == -2 && input.isKeyDown(DIK_UP))
			{
				m.step(cris::up);
				lastkey = DIK_UP;//有键按下后任何按键无效
				lastDirection = up;
				u.lurd += 'u';
				phase1 = true;
			}
			if (lastkey == -2 && input.isKeyDown(DIK_DOWN))
			{
				m.step(cris::down);
				lastkey = DIK_DOWN;
				lastDirection = down;
				u.lurd += 'd';
				phase1 = true;
			}
			if (lastkey == -2 && input.isKeyDown(DIK_LEFT))
			{
				m.step(cris::left);
				lastkey = DIK_LEFT;
				lastDirection = left;
				u.lurd += 'l';
				phase1 = true;
			}
			if (lastkey == -2 && input.isKeyDown(DIK_RIGHT))
			{
				m.step(cris::right);
				lastkey = DIK_RIGHT;
				lastDirection = right;
				u.lurd += 'r';
				phase1 = true;
			}
			if (!(input.isKeyDown(DIK_RIGHT) | input.isKeyDown(DIK_LEFT) | input.isKeyDown(DIK_DOWN) | input.isKeyDown(DIK_UP)))//若所有按键均松开则接下来的按键有效
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
					x = (x - sp.x) / scaler;
					y = (y - sp.y) / scaler;
					m.jump(y, x, u);
					click = false;
				}
			}
			else
			{
				click = true;
			}
		}
		GameControl(user &user,map&map) :u(user),m(map)
		{	
		}
	};
}