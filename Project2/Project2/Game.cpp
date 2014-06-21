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

	black.frame = 0;
	black.health = 100;
	black.position = BLACKINITPOS;
	black.vector = ZEROVECTOR;
	black.frameDuration = GetTickCount() + FRAMEDURATION;

	grey.frame = 0;
	grey.health = 100;
	grey.position = GREYINITPOS;
	grey.vector = ZEROVECTOR;
	grey.frameDuration = GetTickCount() + FRAMEDURATION;
	floor1pos = D2D1::RectF(400, 0, pRT->GetSize().width - 400, pRT->GetSize().height);
	floor2pos = D2D1::RectF(400, -pRT->GetSize().height, pRT->GetSize().width - 400, 0);
	floor3pos = D2D1::RectF(400, -2 * pRT->GetSize().height, pRT->GetSize().width - 400, -pRT->GetSize().height);
	floorvec = D2D1::SizeF(0, 15);

	D2D1_RECT_F divider = D2D1::RectF(900, 0, 910, pRT->GetSize().height);
	D2D1_RECT_F leftwall = D2D1::RectF(397, 0, 403, pRT->GetSize().height);
	D2D1_RECT_F rightwall = D2D1::RectF(pRT->GetSize().width - 403, 0, pRT->GetSize().width - 397, pRT->GetSize().height);
	rects.push_front(divider);
	rects.push_front(leftwall);
	rects.push_front(rightwall);


	// Left Lane Rects

	for (size_t i = 0; i < NUMRECTS; i++)
	{
		Obstacle temp;
		temp.position.left = (rand() % 396) + 400;
		temp.position.right = temp.position.left + ((rand() % 30) + 75);
		temp.position.top = -(rand() % (int)(3 * pRT->GetSize().height));
		temp.position.bottom = temp.position.top + 20;
		temp.hitbox.left = temp.position.left + 18;
		temp.hitbox.right = temp.position.right - 18;
		temp.hitbox.top = temp.position.top +5;
		temp.hitbox.bottom = temp.position.bottom -5;
		obstacles.push_back(temp);
		
	}

	// Right Lane Rects
	for (size_t i = 0; i < NUMRECTS; i++)
	{

		Obstacle temp;
		temp.position.left = (rand() % 396) + 900;
		temp.position.right = temp.position.left + ((rand() % 30) + 75);
		temp.position.top = -(rand() % (int)(3 * pRT->GetSize().height));
		temp.position.bottom = temp.position.top + 20;
		temp.hitbox.left = temp.position.left + 15;
		temp.hitbox.right = temp.position.right - 15;
		temp.hitbox.top = temp.position.top;
		temp.hitbox.bottom = temp.position.bottom;
		obstacles.push_back(temp);

	}
	obstVec = D2D1::SizeF(0, OBSTACLESPEED);



	/*nNinjaFrame = 0;
	nGreyNinjaFrame = 0;
	playerpos = D2D1::RectF(300, 300, 350, 350);
	player2pos = D2D1::RectF(200, 200, 250, 250);
	dwGreyNinjaTime = GetTickCount() + 100;
	dwBlackNinjaTime = GetTickCount() + 100;*/
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

	//Black Ninja Input
	if (keys[VK_UP] & 0x80 && currLevel == Level2)
	{
		if (GetTickCount() > black.frameDuration && currLevel == Level2)
		{
			black.frame++;
			black.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		black.vector.height = -DEFAULTSPEED;
	}
	if (keys[VK_DOWN] & 0x80 && currLevel == Level2)
	{
		if (GetTickCount() > black.frameDuration && currLevel == Level2)
		{
			black.frame++;
			black.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		black.vector.height = DEFAULTSPEED;
	}
	if (keys[VK_LEFT] & 0x80)
	{
		if (GetTickCount() > black.frameDuration && currLevel == Level2)
		{
			black.frame++;
			black.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		black.vector.width = -DEFAULTSPEED;
	}
	if (keys[VK_RIGHT] & 0x80)
	{
		if (GetTickCount() > black.frameDuration && currLevel == Level2)
		{
			black.frame++;
			black.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		black.vector.width = DEFAULTSPEED;
	}
	if (black.frame == 5)
		black.frame = 0;

	// Grey Ninja Input
	if (keys['A'] & 0x80)
	{
		if (GetTickCount() > grey.frameDuration && currLevel == Level2)
		{
			grey.frame++;
			grey.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		grey.vector.width = -DEFAULTSPEED;

	}
	if (keys['W'] & 0x80 && currLevel == Level2)
	{
		if (GetTickCount() > grey.frameDuration && currLevel == Level2)
		{
			grey.frame++;
			grey.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		grey.vector.height = -DEFAULTSPEED;
	}
	if (keys['D'] & 0x80)
	{
		if (GetTickCount() > grey.frameDuration && currLevel == Level2)
		{
			grey.frame++;
			grey.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		grey.vector.width = DEFAULTSPEED;
	}
	if (keys['S'] & 0x80 && currLevel == Level2)
	{
		if (GetTickCount() > grey.frameDuration && currLevel == Level2)
		{
			grey.frame++;
			grey.frameDuration = GetTickCount() + FRAMEDURATION;
		}
		grey.vector.height = DEFAULTSPEED;
	}

	if (grey.frame == 5)
		grey.frame = 0;


	
}

void Game::Simulate(void)
{

	// Collision checks and moving
	if (!Collision(grey, black))		// first checking player vs player collision
	{
		bool collision, hurtcollision;
		
		iter = rects.begin();		// then check collision with walls/boundaries
		for (size_t i = 0; iter != rects.end(); iter++)
		{
			collision = Collision(grey, *iter);
			if (collision)
				break;

		}
		obstIter = obstacles.begin(); // then check collision with obstacles
		for (; obstIter != obstacles.end(); obstIter++)
		{
			hurtcollision = Collision(grey, obstIter->hitbox);
			if (hurtcollision)
			{
				Beep(500, 100);
			}


		}
		if (!collision)
		{
			grey.position.bottom += grey.vector.height;
			grey.position.left += grey.vector.width;
			grey.position.right += grey.vector.width;
			grey.position.top += grey.vector.height;
		}

		iter = rects.begin();
		for (size_t i = 0; iter != rects.end(); iter++)
		{
			collision = Collision(black, *iter);
			if (collision)
				break;

		}
		obstIter = obstacles.begin(); 
		for (; obstIter != obstacles.end(); obstIter++)
		{
			hurtcollision = Collision(black, obstIter->hitbox);
			if (hurtcollision)
			{
				Beep(500, 100);
			}


		}
		if (!collision)
		{
			black.position.bottom += black.vector.height;
			black.position.left += black.vector.width;
			black.position.right += black.vector.width;
			black.position.top += black.vector.height;
		}
	}
	black.vector = ZEROVECTOR;
	grey.vector = ZEROVECTOR;

	// Animating sprites
	if (GetTickCount() > black.frameDuration)
	{
		black.frame++;
		black.frameDuration = GetTickCount() + FRAMEDURATION;
	}

	if (black.frame == 5)
		black.frame = 0;

	if (GetTickCount() > grey.frameDuration)
	{
		grey.frame++;
		grey.frameDuration = GetTickCount() + FRAMEDURATION;
	}

	if (grey.frame == 5)
		grey.frame = 0;

	// Scrolling floor
	floor1pos.top += floorvec.height;
	floor1pos.bottom += floorvec.height;
	floor2pos.top += floorvec.height;
	floor2pos.bottom += floorvec.height;
	floor3pos.top += floorvec.height;
	floor3pos.bottom += floorvec.height;
	if (floor1pos.top >= pRT->GetSize().height)
	{
		floor1pos.top = -2 * pRT->GetSize().height;
		floor1pos.bottom = -pRT->GetSize().height;
	}
	if (floor2pos.top >= pRT->GetSize().height)
	{
		floor2pos.top = -2 * pRT->GetSize().height;
		floor2pos.bottom = -pRT->GetSize().height;
	}
	if (floor3pos.top >= pRT->GetSize().height)
	{
		floor3pos.top = -2 * pRT->GetSize().height;
		floor3pos.bottom = -pRT->GetSize().height;
	}


	obstIter = obstacles.begin();

	//Scrolling objects

	for (; obstIter != obstacles.end(); obstIter++)
	{
		obstIter->position.top += obstVec.height;
		obstIter->position.bottom += obstVec.height;
		if (obstIter->position.top > pRT->GetSize().height)
		{
			obstIter->position.top = -2 * pRT->GetSize().height;
			obstIter->position.bottom = obstIter->position.top + 20;
			if (obstIter->position.left > 900)
			{
				obstIter->position.left = (rand() % 396) + 900;
				obstIter->position.right = obstIter->position.left + ((rand() % 30) + 75);

			}
			else if (obstIter->position.right < 900)
			{
				obstIter->position.left = (rand() % 396) + 400;
				obstIter->position.right = obstIter->position.left + ((rand() % 30) + 75);
			}
		}
		obstIter->hitbox.top = obstIter->position.top;
		obstIter->hitbox.bottom = obstIter->position.bottom;
		obstIter->hitbox.left = obstIter->position.left + 15;
		obstIter->hitbox.right = obstIter->position.right + 15;
	}


	
	
	//Simulation here
}

void Game::Render(void)
{
	pRT->BeginDraw();

	
	pRT->Clear(D2DColor(CornflowerBlue));

	// Rendering Floor Textures
	pRT->DrawBitmap(floor1, floor1pos);
	pRT->DrawBitmap(floor2, floor2pos);
	pRT->DrawBitmap(floor3, floor3pos);


	// Render Black Ninja
	D2D1_RECT_F sourceRect;
	sourceRect.top = 0;
	sourceRect.bottom = 74;
	sourceRect.left =  88.0f * black.frame + 10;
	sourceRect.right = sourceRect.left + 74;

	pRT->DrawBitmap(black.sprite, black.position, 1.0F, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sourceRect);

	// Render Grey Ninja

	sourceRect.left = 88.0f * grey.frame + 10;
	sourceRect.right = sourceRect.left + 74;

	pRT->DrawBitmap(grey.sprite, grey.position, 1.0F, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sourceRect);

	// Render Rect
	pBrush->SetColor(D2DColor(Black));

	iter = rects.begin();
	for (size_t i = 0; iter != rects.end(); iter++)
	{
		pRT->FillRectangle(*iter, pBrush);
	}

	pBrush->SetColor(D2DColor(Yellow));
	obstIter = obstacles.begin();
	// Render Obstacles
	for (; obstIter != obstacles.end(); obstIter++)
	{
		pRT->FillRectangle(obstIter->position, pBrush);
	}

	// Rendering health bar
	blackhealth = D2D1::RectF(healthbar2.left, healthbar2.top, healthbar2.left + 4 * black.health, healthbar2.bottom);
	greyhealth = D2D1::RectF(healthbar1.left, healthbar1.top, healthbar1.left + 4 * grey.health, healthbar1.bottom);

	pBrush->SetColor(D2DColor(Red));
	pRT->FillRectangle(blackhealth, pBrush);
	pRT->FillRectangle(greyhealth, pBrush);
	pBrush->SetColor(D2DColor(Yellow));
	pRT->DrawRectangle(healthbar1, pBrush);
	pRT->DrawRectangle(healthbar2, pBrush);

	

	
	

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
	hr = LoadBitmapFromFile(L"\Images\\ninjasprite.png", &black.sprite);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load ninjasprite.png"),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Load Grey Ninja Sprite
	hr = LoadBitmapFromFile(L"Images\\greyninjasprite.png", &grey.sprite);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load greyninjasprite.png"),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Load Floor Textures
	hr = LoadBitmapFromFile(L"Images\\stonefloor.jpg", &floor1);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load stonefloor.jpg"),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	hr = LoadBitmapFromFile(L"Images\\stonefloor.jpg", &floor2);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load stonefloor.jpg"),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	hr = LoadBitmapFromFile(L"Images\\stonefloor.jpg", &floor3);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load stonefloor.jpg"),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	return S_OK; // Success!
}

void Game::DestroyGraphics(void)
{
	SafeRelease(&grey.sprite);
	SafeRelease(&black.sprite);
	SafeRelease(&pTF);
	SafeRelease(&pDWFactory);
	SafeRelease(&pBrush);
	SafeRelease(&pRT);
	// Release the Direct2D Factory.
	SafeRelease(&pD2DFactory);
}

/*********************\
COLLISION FUNCTIONS
\********************/



bool Game::Collision(Player _player1, Player _player2)
{
	D2D1_RECT_F nextpos1, nextpos2;
	nextpos1.top = _player1.position.top + _player1.vector.height;
	nextpos1.bottom = _player1.position.bottom + _player1.vector.height;
	nextpos1.left = _player1.position.left + _player1.vector.width;
	nextpos1.right = _player1.position.right + _player1.vector.width;

	nextpos2.top = _player2.position.top + _player2.vector.height;
	nextpos2.bottom = _player2.position.bottom + _player2.vector.height;
	nextpos2.left = _player2.position.left + _player2.vector.width;
	nextpos2.right = _player2.position.right + _player2.vector.width;

	if ((nextpos1.top <= nextpos2.bottom && nextpos1.top >= nextpos2.top) && (nextpos1.right <= nextpos2.right && nextpos1.right >= nextpos2.left))
		return true;
	if ((nextpos1.top <= nextpos2.bottom && nextpos1.top >= nextpos2.top) && (nextpos1.left <= nextpos2.right && nextpos1.left >= nextpos2.left))
		return true;
	if ((nextpos1.left <= nextpos2.right && nextpos1.left >= nextpos2.left) && (nextpos1.top <= nextpos2.bottom && nextpos1.top >= nextpos2.top))
		return true;
	if ((nextpos1.left <= nextpos2.right && nextpos1.left >= nextpos2.left) && (nextpos1.bottom <= nextpos2.top && nextpos1.bottom >= nextpos2.top))
		return true;
	if ((nextpos1.bottom >= nextpos2.top && nextpos1.bottom <= nextpos2.bottom) && (nextpos1.left <= nextpos2.right && nextpos1.left >= nextpos2.left))
		return true;
	if ((nextpos1.bottom >= nextpos2.top && nextpos1.bottom <= nextpos2.bottom) && (nextpos1.right <= nextpos2.right && nextpos1.right >= nextpos2.left))
		return true;
	if ((nextpos1.right >= nextpos2.left && nextpos1.right <= nextpos2.right) && (nextpos1.bottom >= nextpos2.top && nextpos1.bottom <= nextpos2.bottom))
		return true;
	if ((nextpos1.right >= nextpos2.left && nextpos1.right <= nextpos2.right) && (nextpos1.top <= nextpos2.bottom && nextpos1.top >= nextpos2.top))
		return true;

	return false;

}
bool Game::Collision(Player _player, D2D1_RECT_F _rect)
{

	D2D1_RECT_F nextpos1;
	nextpos1.top = _player.position.top + _player.vector.height;
	nextpos1.bottom = _player.position.bottom + _player.vector.height;
	nextpos1.left = _player.position.left + _player.vector.width;
	nextpos1.right = _player.position.right + _player.vector.width;


	if ((nextpos1.top <= _rect.bottom && nextpos1.top >= _rect.top) && (nextpos1.right <= _rect.right && nextpos1.right >= _rect.left))
		return true;
	if ((nextpos1.top <= _rect.bottom && nextpos1.top >= _rect.top) && (nextpos1.left <= _rect.right && nextpos1.left >= _rect.left))
		return true;
	if ((nextpos1.left <= _rect.right && nextpos1.left >= _rect.left) && (nextpos1.top <= _rect.bottom && nextpos1.top >= _rect.top))
		return true;
	if ((nextpos1.left <= _rect.right && nextpos1.left >= _rect.left) && (nextpos1.bottom <= _rect.top && nextpos1.bottom >= _rect.top))
		return true;
	if ((nextpos1.bottom >= _rect.top && nextpos1.bottom <= _rect.bottom) && (nextpos1.left <= _rect.right && nextpos1.left >= _rect.left))
		return true;
	if ((nextpos1.bottom >= _rect.top && nextpos1.bottom <= _rect.bottom) && (nextpos1.right <= _rect.right && nextpos1.right >= _rect.left))
		return true;
	if ((nextpos1.right >= _rect.left && nextpos1.right <= _rect.right) && (nextpos1.bottom >= _rect.top && nextpos1.bottom <= _rect.bottom))
		return true;
	if ((nextpos1.right >= _rect.left && nextpos1.right <= _rect.right) && (nextpos1.top <= _rect.bottom && nextpos1.top >= _rect.top))
		return true;
	return false;
}
