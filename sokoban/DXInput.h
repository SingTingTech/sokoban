#pragma once
#include"stdafx.h"
#include"mytime.h"
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }   



namespace cris {
	class DXInput {
		LPDIRECTINPUT8 m_lpDirectInput;
		LPDIRECTINPUTDEVICE8 m_lpMouseDevice;
		LPDIRECTINPUTDEVICE8 m_lpKeyboardDevice;
		CHAR m_keyBuffer[256] = { 0 };
		DIMOUSESTATE m_mouseBuffer;
		Timer t;
	public:
		enum buttons {
			LEFTBUTTON = 0,
			RIGHTBUTTON,
			MIDBUTTON
		};

		HRESULT inputIni(HINSTANCE hinst, HWND hwnd, DWORD keyboardCoop, DWORD mouseCoop);

		HRESULT inputIni(HINSTANCE hinst, HWND hwnd) 
		{

			return inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		}
		HRESULT getInput();

		BOOL isKeyDown(INT iKey);

		BOOL isMouseButtonDown(INT iButton);
		
		LONG getMouseXCoordinate();
		
		LONG getMouseYCoordinate();

		LONG getMouseZCoordinate();
		

		#define safeRelease(p){if ((p) != NULL)(p)->Release();(p) = NULL;}
		VOID cleanUp();
	};
}
