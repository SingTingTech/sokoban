#pragma once
#include"stdafx.h"
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }     
class DXInput {

	LPDIRECTINPUT8 m_lpDirectInput;
	LPDIRECTINPUTDEVICE8 m_lpMouseDevice;
	LPDIRECTINPUTDEVICE8 m_lpKeyboardDevice;
	CHAR m_keyBuffer[256] = { 0 };
	DIMOUSESTATE m_mouseBuffer;
public:
	HRESULT inputIni(HINSTANCE hinst, HWND hwnd, DWORD keyboardCoop, DWORD mouseCoop);

	HRESULT getInput();

	BOOL isKeyDown(INT iKey);

	BOOL isMouseButtonDown(INT iButton);

	LONG getMouseXCoordinate();

	LONG getMouseYCoordinate();

	LONG getMouseZCoordinate();

	VOID safeRelease(IUnknown *p)
	{
		if (p != NULL)
			p->Release();
		p = NULL;
	}
	VOID cleanUp();
};