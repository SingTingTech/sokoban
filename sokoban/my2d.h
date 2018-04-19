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


		pRT->Clear(D2D1::ColorF(D2D1::ColorF::ForestGreen));
		//bg
		ID2D1LinearGradientBrush *pLinearGradientBrush;
		// Create an array of gradient stops to put in the gradient stop
		// collection that will be used in the gradient brush.
		ID2D1GradientStopCollection *pGradientStops = NULL;

		D2D1_GRADIENT_STOP gradientStops[2];
		gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::LightYellow, 1);
		gradientStops[0].position = 0.0f;
		gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::LightBlue, 1);
		gradientStops[1].position = 1.0f;
		// Create the ID2D1GradientStopCollection from a previously
		// declared array of D2D1_GRADIENT_STOP structs.
		pRT->CreateGradientStopCollection(
			gradientStops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGradientStops
		);

		pRT->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(0, 0),
				D2D1::Point2F(800, 600)),
			pGradientStops,
			&pLinearGradientBrush
		);
		D2D1_SIZE_F size = pRT->GetSize();
		D2D1_RECT_F r = { 0, 0, size.width, size.height };
		pRT->FillRectangle(&r, pLinearGradientBrush);
		x();




		HRESULT hr = pRT->EndDraw();
		pGradientStops->Release();
		pLinearGradientBrush->Release();
		return hr;
	}
}