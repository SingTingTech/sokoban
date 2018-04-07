#include"my2d.h"

my2d::my2d(HWND hwnd) {
	// Obtain the size of the drawing area.
	GetClientRect(hwnd, &clientrc);
	this->hwnd = hwnd;

}

my2d::my2d() {


}
HRESULT my2d::init(HWND hwnd) {
	GetClientRect(hwnd, &clientrc);
	this->hwnd = hwnd;
	ID2D1Factory* pD2DFactory = NULL;
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
	if (SUCCEEDED(binit)) {
		binit = hr;

	}
	return binit;
}
HRESULT my2d::draw(void (*draw)())
{
	
	ID2D1SolidColorBrush* pBlackBrush = NULL;
	if (SUCCEEDED(binit))
	{

		pRT->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pBlackBrush
		);
	}
	pRT->BeginDraw();


	//pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	//bg
	ID2D1LinearGradientBrush *pLinearGradientBrush;
	// Create an array of gradient stops to put in the gradient stop
	// collection that will be used in the gradient brush.
	ID2D1GradientStopCollection *pGradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Black, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black, 1);
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
			D2D1::Point2F(400, 0),
			D2D1::Point2F(400, 600)),
		pGradientStops,
		&pLinearGradientBrush
	);
	D2D1_SIZE_F size = pRT->GetSize();
	D2D1_RECT_F r = { 0, 0, size.width, size.height };
	pRT->FillRectangle(&r, pLinearGradientBrush);
	(*draw)();




	HRESULT hr = pRT->EndDraw();
	return hr;
}