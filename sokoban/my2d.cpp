#include"my2d.h"

cris::my2d::my2d(HWND hwnd) {
	// Obtain the size of the drawing area.
	GetClientRect(hwnd, &clientrc);
	this->hwnd = hwnd;

}

cris::my2d::my2d() {
	

}
HRESULT cris::my2d::init(HWND hwnd) {
	GetClientRect(hwnd, &clientrc);
	this->hwnd = hwnd;

	binit = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory
	);
	// Create a Direct2D render target	
	HRESULT hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(
				clientrc.right - clientrc.left,
				clientrc.bottom - clientrc.top)
		),
		&pRT
	);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown **>(&writeFactory));
	writeFactory->CreateTextFormat(L"·½Õýß÷ÎØÌå", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 25, L"", &textNormal);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &pGrayBrush);


	if (SUCCEEDED(binit)) {
		binit = hr;

	}
	
	return binit;
}

