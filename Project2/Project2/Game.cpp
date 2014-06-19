#include "Game.h"

Game::Game(void)
{
	bRunning = TRUE;
	hWnd = NULL;
	ZeroMemory(keys, sizeof(keys));

	pD2DFactory = NULL;
	pRT = NULL;
	pBrush = NULL;
	pDWFactory = NULL;
	pTF = NULL;

	// Zero out the frames per second variables:
	dwFrames = 0;
	dwCurrentTime = 0;
	dwLastUpdateTime = 0;
	dwElapsedTime = 0;
}

Game::~Game(void)
{
}

void Game::Startup(void)
{
	//TODO: Create Back Buffer

	// Initialize DirectX.
	HRESULT hr = CreateGraphics(hWnd);
	if (FAILED(hr))
	{
		return; // Return -1 to abort Window Creation.
	}

	nFrame = 0;
	playerpos = D2D1::RectF(300, 300, 350, 350);
	dwTime = GetTickCount() + 100;
	//TODO: Initialize Game Components


}

void Game::Shutdown(void)
{


	// Shutdown DirectX.
	DestroyGraphics();
}

void Game::Input(void)
{
	
	if (keys[VK_ESCAPE] & 0x80)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
	if (keys[VK_UP] & 0x80)
	{
		if (GetTickCount() > dwTime)
		{
			nFrame++;
			dwTime = GetTickCount() + 100;
		}
		playerpos.top -= 4;
		playerpos.bottom -= 4;
	}
	if (keys[VK_DOWN] & 0x80)
	{
		if (GetTickCount() > dwTime)
		{
			nFrame++;
			dwTime = GetTickCount() + 100;
		}
		playerpos.top += 4;
		playerpos.bottom += 4;
	}
	if (keys[VK_LEFT] & 0x80)
	{
		if (GetTickCount() > dwTime)
		{
			nFrame++;
			dwTime = GetTickCount() + 100;
		}
		playerpos.left -= 4;
		playerpos.right -= 4;
	}
	if (keys[VK_RIGHT] & 0x80)
	{
		if (GetTickCount() > dwTime)
		{
			nFrame++;
			dwTime = GetTickCount() + 100;
		}
		playerpos.left += 4;
		playerpos.right += 4;
	}
	if (nFrame == 5)
		nFrame = 0;


	
}

void Game::Simulate(void)
{
	
	//Simulation here
}

void Game::Render(void)
{
	pRT->BeginDraw();

	
	pRT->Clear(D2DColor(CornflowerBlue));

	D2D1_RECT_F sourceRect;
	sourceRect.top = 0;
	sourceRect.bottom = 88;
	sourceRect.left =  88.0f * nFrame;
	sourceRect.right = sourceRect.left + 88;


	pRT->DrawBitmap(ninja, playerpos, 1.0F, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sourceRect);
	

	HRESULT hr = pRT->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		DestroyGraphics();
		CreateGraphics(hWnd);
	}
}

HRESULT Game::LoadBitmapFromFile(LPCTSTR strFileName, ID2D1Bitmap** ppBitmap)
{
	HRESULT hr;

	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICFormatConverter *pConverter = NULL;

	hr = pWICFactory->CreateDecoderFromFilename(strFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	}
	if (SUCCEEDED(hr))
	{
		hr = pRT->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
	}
	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pConverter);
	return hr;
}

FMOD_RESULT Game::LoadFMOD()
{
	result = FMOD::System_Create(&sys);
	if (result != FMOD_OK)
	{
		MessageBox(HWND_DESKTOP, L"ERROR: Failed to create FMOD System.", L"FMOD Error", MB_OK | MB_ICONERROR);
		return result;
	}
	result = sys->getVersion(&version);
	if (result != FMOD_OK)
	{
		MessageBox(HWND_DESKTOP, L"ERROR: FMOD System out of date", L"FMOD Error", MB_OK | MB_ICONERROR);
		return result;
	}
	if (version < FMOD_VERSION)
	{
		MessageBox(HWND_DESKTOP, L"ERROR: FMOD System out of date", L"FMOD Error", MB_OK | MB_ICONERROR);
		return result;
	}
	result = sys->getNumDrivers(&numDrivers);
	if (result != FMOD_OK)
	{
		MessageBox(HWND_DESKTOP, L"ERROR: Drivers fucked up", L"FMOD Error", MB_OK | MB_ICONERROR);
		return result;
	}
	if (numDrivers == 0)
	{
		sys->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}

	result = sys->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		sys->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		sys->init(100, FMOD_INIT_NORMAL, 0);
	}
	return result;
}

HRESULT Game::CreateGraphics(HWND hWnd)
{
	// Initialize the Direct2D Factory.
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Factory."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Get the dimensions of the client.
	RECT rc;
	GetClientRect(hWnd, &rc);

	// Initialize a Direct2D Size Structure.
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);


	// Create the Direct2D Render Target.
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size), &pRT);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Render Target."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	pRT->SetDpi(96.0f, 96.0f);


	// Create the Direct2D Solid Color Brush.
	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(0x0), &pBrush);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Initialize the DirectWrite Factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		(IUnknown**)&pDWFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Factory."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Create the DirectWrite Text Format.
	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		24, _T(""), &pTF);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	

	// Create WIC Interface
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the WIC Interface!"),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Load Ninja Sprite
	hr = LoadBitmapFromFile(L"C:\\Users\\Seth\\Documents\\GitHub\\Project2\\Project2\\Project2\\Images\\ninjasprite.png", &ninja);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load ninjasprite.png"),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	return S_OK; // Success!
}

void Game::DestroyGraphics(void)
{
	SafeRelease(&ninja);
	SafeRelease(&pTF);
	SafeRelease(&pDWFactory);
	SafeRelease(&pBrush);
	SafeRelease(&pRT);
	// Release the Direct2D Factory.
	SafeRelease(&pD2DFactory);
}
