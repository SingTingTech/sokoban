#pragma once
#include"stdafx.h"
#include"my2d.h"
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
		wchar_t controlText[1024];
		virtual void draw(my2d &my2ddraw) //绘制
		{
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hwnd, &p);
			if (p.x<size.right&&p.x>size.left&&p.y > size.top&&p.y < size.bottom) 
			{
				SetCursor(hcursor);
			}
			//wchar_t buf[20];
			//wsprintf(buf, L"%d,%d", p.x, p.y);
			//my2ddraw.pRT->DrawText(buf, wcslen(buf),
			//	my2ddraw.textNormal,
			//	D2D1::RectF(size.left,size.top+100,size.right,size.bottom+100),
			//	my2ddraw.pGrayBrush);

		}
		virtual void testKeys() = 0;//获取按键信息
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
		virtual void testKeys() 
		{

		}
	};
}