#pragma once
#include"stdafx.h"
namespace cris{
	class my2d
	{

		HRESULT binit;//�Ƿ��ѳ�ʼ��
		HWND hwnd;//����Ŀ�괰�ھ��
		RECT clientrc;//��������
		ID2D1Factory* pD2DFactory = NULL;//d2d����
		IDWriteFactory * writeFactory;//Dwrite���幤��

	public:
		
		IDWriteTextFormat * textNormal;//ͨ������
		IDWriteTextFormat * textSmall;//��С����
		IDWriteTextFormat * textLarge;//�ϴ�����
		ID2D1LinearGradientBrush *pBgBrush;//�������仭ˢ
		ID2D1SolidColorBrush *pGrayBrush;//dark gray
		ID2D1SolidColorBrush *pLightBrush;//pink
		ID2D1SolidColorBrush *pDarkBrush;//dark blue
		ID2D1SolidColorBrush *pListBrush;//light green
		ID2D1HwndRenderTarget* pRT = NULL;//��Ⱦ����
		my2d(HWND hwnd);//ʹ��hwndָ��Ĵ��ڹ���һ����ͼ��
		my2d();//Ĭ�Ϲ���
		HRESULT init(HWND hwnd); //ʹ��hwndָ��Ĵ��ڳ�ʼ��һ����ͼ��
		//ģ���ͼ����TΪһ������ָ���һ��lambda����
		template<typename T>
		HRESULT draw(T);
		//�����ڴ�
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