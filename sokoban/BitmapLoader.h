#pragma once
#include"stdafx.h"
namespace cris {
	class BitmapLoader
	{	
		ID2D1RenderTarget *prt;
		// The factory pointer
		IWICImagingFactory *pFactory = NULL;
		IWICBitmapDecoder *pDecoder = NULL;
		IWICBitmapFrameDecode *pSource = NULL;
		IWICStream *pStream = NULL;
		IWICFormatConverter *pConverter = NULL;
		IWICBitmapScaler *pScaler = NULL;

	public:
		ID2D1Bitmap *pBitmap;
		void ini() 
		{
			// Initialize COM
			HRESULT hr = CoInitialize(NULL);
			// Create the COM imaging factory
			hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&pFactory)
			);
		}
		void loadPNG(HINSTANCE hinst,ID2D1RenderTarget *pRT,int resourcename,UINT targetHeight,UINT targetWidth)
		{
			// Initialize COM
			CoInitialize(NULL);
			// Create the COM imaging factory
			HRESULT hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&pFactory)
			);
			prt = pRT;
			HRSRC imageResHandle = NULL;
			HGLOBAL imageResDataHandle = NULL;
			void *pImageFile = NULL;
			DWORD imageFileSize = 0;

			// Locate the resource.
			imageResHandle = FindResource(hinst, MAKEINTRESOURCE(resourcename), L"PNG");
			hr = imageResHandle ? S_OK : E_FAIL;
			DWORD d = GetLastError();
			if (SUCCEEDED(hr))
			{
				// Load the resource.
				imageResDataHandle = LoadResource(hinst, imageResHandle);

				hr = imageResDataHandle ? S_OK : E_FAIL;
			}
			if (SUCCEEDED(hr))
			{
				// Lock it to get a system memory pointer.
				pImageFile = LockResource(imageResDataHandle);

				hr = pImageFile ? S_OK : E_FAIL;
			}
			if (SUCCEEDED(hr))
			{
				// Calculate the size.
				imageFileSize = SizeofResource(hinst, imageResHandle);

				hr = imageFileSize ? S_OK : E_FAIL;
			}


			
			if (SUCCEEDED(hr))
			{
				// Create a WIC stream to map onto the memory.
				hr = pFactory->CreateStream(&pStream);
			}
			if (SUCCEEDED(hr))
			{
				// Initialize the stream with the memory pointer and size.
				hr = pStream->InitializeFromMemory(
					reinterpret_cast<BYTE*>(pImageFile),
					imageFileSize
				);
			}
			if (SUCCEEDED(hr))
			{
				// Create a decoder for the stream.
				hr = pFactory->CreateDecoderFromStream(
					pStream,
					NULL,
					WICDecodeMetadataCacheOnLoad,
					&pDecoder
				);
			}
			if (SUCCEEDED(hr))
			{
				// Create the initial frame.
				hr = pDecoder->GetFrame(0, &pSource);
			}
			if (SUCCEEDED(hr))
			{
				// Convert the image format to 32bppPBGRA
				// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
				hr = pFactory->CreateFormatConverter(&pConverter);
			}

			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (targetWidth == 0)
			{
				FLOAT scalar = static_cast<FLOAT>(targetHeight) / static_cast<FLOAT>(originalHeight);
				targetWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
			}
			else if (targetHeight == 0)
			{
				FLOAT scalar = static_cast<FLOAT>(targetWidth) / static_cast<FLOAT>(originalWidth);
				targetHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
			}
			hr = pFactory->CreateBitmapScaler(&pScaler);
			if (SUCCEEDED(hr))
			{
				hr = pScaler->Initialize(
					pSource,
					targetWidth,
					targetHeight,
					WICBitmapInterpolationModeCubic
				);
			}



			if (SUCCEEDED(hr))
			{

				hr = pConverter->Initialize(
					pSource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}
			if (SUCCEEDED(hr))
			{
				//create a Direct2D bitmap from the WIC bitmap.
				hr = pRT->CreateBitmapFromWicBitmap(
					pConverter,
					NULL,
					&pBitmap
				);

			}
			

			
		}
		void reload(int targetHeight,int targetWidth) 
		{
			HRESULT hr;
			if (pConverter) 
			{
				pConverter->Release();
			}
		
				// Convert the image format to 32bppPBGRA
				// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
				hr = pFactory->CreateFormatConverter(&pConverter);
			

			UINT originalWidth, originalHeight;
			if (FAILED(hr))
			{
				return;
			}	
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (targetWidth == 0)
			{
				FLOAT scalar = static_cast<FLOAT>(targetHeight) / static_cast<FLOAT>(originalHeight);
				targetWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
			}
			else if (targetHeight == 0)
			{
				FLOAT scalar = static_cast<FLOAT>(targetWidth) / static_cast<FLOAT>(originalWidth);
				targetHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
			}
			if (pScaler)
				pScaler->Release();
			hr = pFactory->CreateBitmapScaler(&pScaler);
			if (SUCCEEDED(hr))
			{
				hr = pScaler->Initialize(
					pSource,
					targetWidth,
					targetHeight,
					WICBitmapInterpolationModeCubic
				);
			}



			if (SUCCEEDED(hr))
			{

				hr = pConverter->Initialize(
					pSource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}
			if (pBitmap)
				pBitmap->Release();
			if (SUCCEEDED(hr))
			{
				//create a Direct2D bitmap from the WIC bitmap.
				hr = prt->CreateBitmapFromWicBitmap(
					pConverter,
					NULL,
					&pBitmap
				);

			}


		}
		void cleanup() 
		{
			pBitmap->Release();
			pScaler->Release();
			pConverter->Release();
			pSource->Release();
			pStream->Release();
			pFactory->Release();
		}
	};


}