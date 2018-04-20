#pragma once
#include"stdafx.h"
namespace cris{
	class my2d
	{

		HRESULT binit;
		HWND hwnd;
		RECT clientrc;
		ID2D1Factory* pD2DFactory = NULL;




		IDWriteFactory * writeFactory;

	public:
		
		IDWriteTextFormat * textNormal;
		IDWriteTextFormat * textSmall;
		IDWriteTextFormat * textLarge;

		ID2D1LinearGradientBrush *pBgBrush;
		//dark gray
		ID2D1SolidColorBrush *pGrayBrush;
		//pink
		ID2D1SolidColorBrush *pLightBrush;
		//dark blue
		ID2D1SolidColorBrush *pDarkBrush;
		//light green
		ID2D1SolidColorBrush *pListBrush;
		ID2D1HwndRenderTarget* pRT = NULL;



		my2d(HWND hwnd);

		my2d();

		HRESULT init(HWND hwnd);

		template<typename T>
		HRESULT draw(T);
		void cleanup()
		{
			pBgBrush->Release();
			textLarge->Release();
			textSmall->Release();
			textNormal->Release();
			writeFactory->Release();
			pLightBrush->Release();
			pListBrush->Release();
			pGrayBrush->Release();
			pRT->Release();
			pD2DFactory->Release();
		}
	};

	template<class T>
	HRESULT my2d::draw(T x)
	{


		pRT->BeginDraw();


		D2D1_SIZE_F size = pRT->GetSize();
		D2D1_RECT_F r = { 0, 0, size.width, size.height };
		pRT->FillRectangle(&r, pBgBrush);
		x();




		HRESULT hr = pRT->EndDraw();
		return hr;
	}
}