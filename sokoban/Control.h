#pragma once
#include"stdafx.h"
#include"my2d.h"
#include"DXInput.h"
#pragma comment(lib,"imm32.lib")
namespace cris
{
	class Control 
	{

	protected:
		HWND hwnd;	//父窗口句柄
		HCURSOR hcursor;
		D2D1_RECT_F size;//窗口大小
	public:
		Control(float x,float y,float width,float height) {
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
		virtual void draw(my2d &my2ddraw) //绘制
		{

			if (mouseOn()) 
			{
				SetCursor(hcursor);
			}


		}
		template<class T>
		void testKeys(DXInput &d, T t) 
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
		TextControl(float x, float y, float width, float height,const wchar_t*text) :Control(x, y, width, height)
		{
			wcscpy_s(controlText, text);
		}
		virtual void draw(my2d &my2ddraw)
		{

			Control::draw(my2ddraw);
			my2ddraw.pRT->DrawText(controlText, wcslen(controlText),
				my2ddraw.textNormal,
				size,
				my2ddraw.pGrayBrush);
		};
		template<class T>
		void testKeys(DXInput &d,T t) {
			static bool click = false;
			d.getInput();
			if (mouseOn() && d.isMouseButtonDown(DXInput::LEFTBUTTON)) 
			{
				if(click)
				{
					t();
				}
				
			}
			else 
			{
				click = true;
			}
		}

	
	};
	class EditControl:public Control
	{
	public:
		
		int cur = 0;//光标位置
		EditControl(float x, float y, float width, float height) :Control(x, y, width, height)
		{
			
		}
		//获取输入
		virtual void onInput(wchar_t ch) 
		{

			if (ch == 8&&cur>0)controlText[--cur] = 0;
			if ((ch > 6 && ch < 14) || ch == 27||ch == 32)
				return;
			controlText[cur++] = ch;
		}
		virtual void draw(my2d &my2ddraw)
		{

			Control::draw(my2ddraw);
			my2ddraw.pRT->DrawText(controlText, wcslen(controlText),
				my2ddraw.textNormal,
				size,
				my2ddraw.pGrayBrush);
			my2ddraw.pRT->DrawLine(D2D1::Point2F(size.left, size.bottom), D2D1::Point2F(size.right, size.bottom), my2ddraw.pGrayBrush);
			my2ddraw.pRT->DrawLine(D2D1::Point2F(wcslen(controlText)*10.5 + size.left, size.top), D2D1::Point2F(wcslen(controlText)*10.5 + size.left, size.bottom), my2ddraw.pGrayBrush);
		};


	};
	class ListControl :public Control
	{
	public:
		int cursor;
		std::vector<std::string> list;
		//0-1之间的小数 进度条
		float moveBar = -1;
		ListControl(float x, float y, float width, float height) :Control(x, y, width, height)
		{
			cursor = -1;
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
			if(i>=0&&i<=1)
			moveBar = i;
		}
		void draw(my2d &my2ddraw) 
		{
			Control::draw(my2ddraw);
			my2ddraw.pRT->FillRectangle(size, my2ddraw.pListBrush);
			float fullSize = list.size() * 20;
			float controlHeight = size.bottom - size.top;
			if (fullSize < controlHeight)
			{
				//无滚动条
				if (cursor > 0)
					my2ddraw.pRT->FillRectangle(D2D1::Rect(size.left, size.top + 20.0f*cursor, size.right, size.top + 20.0f * cursor + 20.f), my2ddraw.pDarkBrush);
				float fullSize = list.size() * 20;
			
				wchar_t buffer[30] = { 0 };
				int i = 0;
				for (auto it = list.begin(); it != list.end(); it++, i++)
				{
					memset(buffer, 0, 30);
					MultiByteToWideChar(CP_ACP, 0, list[i].c_str(), list[i].length(), buffer, 30);
					my2ddraw.pRT->DrawText(buffer, 30,
						my2ddraw.textSmall,
						D2D1::Rect(size.left, size.top + 20.0f*i,  size.right , size.top + 20.0f * (i + 1)),
						my2ddraw.pLightBrush);
				}
			}
			else
			{
				//有滚动条
				//虚拟顶端
				float a = size.top - (fullSize - controlHeight)*moveBar;
				if (cursor>0)
					my2ddraw.pRT->FillRectangle(D2D1::Rect(size.left, a + 20.0f*cursor<size.top ? size.top : a + 20.0f*cursor, size.right - 25, a + 20.0f * (cursor + 1)>size.bottom ? size.bottom : a + 20.0f * (cursor + 1)), my2ddraw.pDarkBrush);
				float moveBarLength = controlHeight*(controlHeight / fullSize);

				float startPoint = (controlHeight - moveBarLength)*moveBar;
				//绘制list
				wchar_t buffer[30] = { 0 };
				int i = 0;
				for (auto it = list.begin(); it != list.end(); it++, i++)
				{
					//*/截面绘制
					if (a + 20.0f*i < size.top) 
					{
						continue;
					}
					if (a + 20.0f*i > size.bottom)//向下截断
						break;
					
					memset(buffer, 0, 30);
					MultiByteToWideChar(CP_ACP, 0, list[i].c_str(), list[i].length(), buffer, 30);
					my2ddraw.pRT->DrawText(buffer, 30,my2ddraw.textSmall,D2D1::Rect(size.left, a + 20.0f*i, size.right - 25,a + 20.0f * (i + 1)>size.bottom?size.bottom: a + 20.0f * (i + 1)),my2ddraw.pGrayBrush,D2D1_DRAW_TEXT_OPTIONS_CLIP);
				}
				//绘制滑块
				my2ddraw.pRT->FillRectangle(D2D1::Rect(size.right - 25, size.top+startPoint,size.right , size.top + startPoint+moveBarLength), my2ddraw.pLightBrush);


			}

		}

		template<class T>
		void testKeys(DXInput &d, T t) {
			
			//*/鼠标监测
			static bool canClick = false;
			static bool drag = false;
			d.getInput();
			if (mouseOn() && d.isMouseButtonDown(DXInput::LEFTBUTTON))
			{
				if (canClick)
				{
					//监测在哪个区域单击
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(hwnd, &p);
					float fullSize = list.size() * 20;
					float controlHeight = size.bottom - size.top;
					float moveBarLength = controlHeight*(controlHeight / fullSize);
					float startPoint = (controlHeight - moveBarLength)*moveBar;
					//滚动条上
					if (p.x<size.right&&p.y>size.right-25&& p.y>size.top + startPoint&&size.top + startPoint + moveBarLength>p.y) 
					{
						drag = true;
					}
					//列表区域
				//	if () {}

				}
				if (drag) 
				{
					int i = d.getMouseXCoordinate();
					moveBar += i / 100.0f;
				}

			}
			else
			{
				canClick = true;
				drag = false;
			}
			//*/鼠标监测
		}
	};
}