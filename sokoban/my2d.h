#pragma once
#include"stdafx.h"
namespace cris{
	class my2d
	{

		HRESULT binit;//是否已初始化
		HWND hwnd;//绘制目标窗口句柄
		RECT clientrc;//窗口区域
		ID2D1Factory* pD2DFactory = NULL;//d2d工厂
		IDWriteFactory * writeFactory;//Dwrite字体工厂

	public:
		
		IDWriteTextFormat * textNormal;//通常字体
		IDWriteTextFormat * textSmall;//较小字体
		IDWriteTextFormat * textLarge;//较大字体
		ID2D1LinearGradientBrush *pBgBrush;//背景渐变画刷
		ID2D1SolidColorBrush *pGrayBrush;//dark gray
		ID2D1SolidColorBrush *pLightBrush;//pink
		ID2D1SolidColorBrush *pDarkBrush;//dark blue
		ID2D1SolidColorBrush *pListBrush;//light green
		ID2D1HwndRenderTarget* pRT = NULL;//渲染对象
		my2d(HWND hwnd);//使用hwnd指向的窗口构造一个绘图器
		my2d();//默认构造
		HRESULT init(HWND hwnd); //使用hwnd指向的窗口初始化一个绘图器
		//模板绘图函数T为一个函数指针或一个lambda对象
		template<typename T>
		HRESULT draw(T);
		//清理内存
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