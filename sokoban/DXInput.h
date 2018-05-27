#pragma once
#include"stdafx.h"
#include"mytime.h"
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }   



namespace cris {
	class DXInput {
		//指示是否已初始化
		bool inited = false;
		LPDIRECTINPUT8 m_lpDirectInput;
		LPDIRECTINPUTDEVICE8 m_lpMouseDevice;
		LPDIRECTINPUTDEVICE8 m_lpKeyboardDevice;
		CHAR m_keyBuffer[256] = { 0 };
		DIMOUSESTATE m_mouseBuffer;
		//指示单击是否可用
		bool click = true;
		//计时器
		Timer t;
	public:
		//鼠标按键枚举
		enum buttons {
			LEFTBUTTON = 0,
			RIGHTBUTTON,
			MIDBUTTON
		};
		//初始化，指定协同级别
		HRESULT inputIni(HINSTANCE hinst, HWND hwnd, DWORD keyboardCoop, DWORD mouseCoop);
		//初始化，默认协同级别
		HRESULT inputIni(HINSTANCE hinst, HWND hwnd) 
		{

			return inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		}
		//更新缓存，获取当前控制器的状态
		HRESULT getInput();
		//判断某个指定的按键是否被按下
		BOOL isKeyDown(INT iKey);
		/*判断是否由单击消息
		isMouseOn  用于控制不在范围内的控件不响应单击消息
		*/
		bool isClick(bool isMouseOn);
		//判断指定鼠标按键是否被按下
		BOOL isMouseButtonDown(INT iButton);
		//x轴位移
		LONG getMouseXCoordinate();
		//y轴位移
		LONG getMouseYCoordinate();
		//z轴（转轮）位移
		LONG getMouseZCoordinate();
		

		#define safeRelease(p){if ((p) != NULL)(p)->Release();(p) = NULL;}
		//释放空间
		VOID cleanUp();
	};
}
