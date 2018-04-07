#pragma once
#include"stdafx.h"

class my2d 
{

	HRESULT binit;
	HWND hwnd;
	RECT clientrc;
	


	

public:
	ID2D1HwndRenderTarget* pRT = NULL;
	my2d(HWND hwnd);

	my2d();

	HRESULT init(HWND hwnd);


	HRESULT draw(void(*drawfuc)());

};