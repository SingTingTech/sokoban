#pragma once
#include"stdafx.h"
#include"mytime.h"
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }   



namespace cris {
	class DXInput {
		//ָʾ�Ƿ��ѳ�ʼ��
		bool inited = false;
		LPDIRECTINPUT8 m_lpDirectInput;
		LPDIRECTINPUTDEVICE8 m_lpMouseDevice;
		LPDIRECTINPUTDEVICE8 m_lpKeyboardDevice;
		CHAR m_keyBuffer[256] = { 0 };
		DIMOUSESTATE m_mouseBuffer;
		//ָʾ�����Ƿ����
		bool click = true;
		//��ʱ��
		Timer t;
	public:
		//��갴��ö��
		enum buttons {
			LEFTBUTTON = 0,
			RIGHTBUTTON,
			MIDBUTTON
		};
		//��ʼ����ָ��Эͬ����
		HRESULT inputIni(HINSTANCE hinst, HWND hwnd, DWORD keyboardCoop, DWORD mouseCoop);
		//��ʼ����Ĭ��Эͬ����
		HRESULT inputIni(HINSTANCE hinst, HWND hwnd) 
		{

			return inputIni(hinst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		}
		//���»��棬��ȡ��ǰ��������״̬
		HRESULT getInput();
		//�ж�ĳ��ָ���İ����Ƿ񱻰���
		BOOL isKeyDown(INT iKey);
		/*�ж��Ƿ��ɵ�����Ϣ
		isMouseOn  ���ڿ��Ʋ��ڷ�Χ�ڵĿؼ�����Ӧ������Ϣ
		*/
		bool isClick(bool isMouseOn);
		//�ж�ָ����갴���Ƿ񱻰���
		BOOL isMouseButtonDown(INT iButton);
		//x��λ��
		LONG getMouseXCoordinate();
		//y��λ��
		LONG getMouseYCoordinate();
		//z�ᣨת�֣�λ��
		LONG getMouseZCoordinate();
		

		#define safeRelease(p){if ((p) != NULL)(p)->Release();(p) = NULL;}
		//�ͷſռ�
		VOID cleanUp();
	};
}
