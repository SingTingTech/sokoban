#include "DXInput.h"

HRESULT DXInput::inputIni(HINSTANCE hinst,HWND hwnd,DWORD keyboardCoop,DWORD mouseCoop)
{
	HRESULT hr;
	HR(DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpDirectInput, NULL));
	
	HR(m_lpDirectInput->CreateDevice(GUID_SysKeyboard, &m_lpKeyboardDevice, NULL));
	HR(m_lpKeyboardDevice->SetCooperativeLevel(hwnd, keyboardCoop));
	HR(m_lpKeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(m_lpKeyboardDevice->Acquire());
	HR(m_lpKeyboardDevice->Poll());
	
	HR(m_lpDirectInput->CreateDevice(GUID_SysMouse, &m_lpMouseDevice, NULL));
	HR(m_lpMouseDevice->SetCooperativeLevel(hwnd, mouseCoop));
	HR(m_lpMouseDevice->SetDataFormat(&c_dfDIMouse));
	HR(m_lpMouseDevice->Acquire());
	HR(m_lpMouseDevice->Poll());

	return S_OK;
}
HRESULT DXInput::getInput() 
{
	HRESULT hr;
	hr = m_lpKeyboardDevice->GetDeviceState(sizeof m_keyBuffer, (LPVOID)&m_keyBuffer);

	if (FAILED(hr)) 
	{
		m_lpKeyboardDevice->Acquire();
		HR( m_lpKeyboardDevice->GetDeviceState(sizeof m_keyBuffer, (LPVOID)&m_keyBuffer));

	}

	hr = m_lpMouseDevice->GetDeviceState(sizeof m_mouseBuffer, (LPVOID)&m_mouseBuffer);

	if (FAILED(hr)) {
		m_lpMouseDevice->Acquire();
		HR(m_lpMouseDevice->GetDeviceState(sizeof m_mouseBuffer, (LPVOID)&m_mouseBuffer));
	}
}

BOOL DXInput::isKeyDown(INT iKey) 
{
	return m_keyBuffer[iKey] & 0x80;
}

BOOL DXInput::isMouseButtonDown(INT iButton)
{
	return m_mouseBuffer.rgbButtons[iButton] & 0x80;
}

LONG DXInput::getMouseXCoordinate()
{
	return m_mouseBuffer.lX;
}

LONG DXInput::getMouseYCoordinate()
{
	return m_mouseBuffer.lY;
}

LONG DXInput::getMouseZCoordinate()
{
	
	return m_mouseBuffer.lZ;
}

VOID DXInput::cleanUp()
{
	if (m_lpKeyboardDevice!=NULL)
		m_lpKeyboardDevice->Unacquire();
	if (m_lpMouseDevice != NULL)
		m_lpMouseDevice->Unacquire();
	safeRelease(m_lpMouseDevice);
	safeRelease(m_lpKeyboardDevice);
	safeRelease(m_lpDirectInput);
}
