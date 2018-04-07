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

