#pragma once

#include <Windows.h> // Include the Win32 API
#include <tchar.h> // Include support for UNICODE
#include <sstream> // Include wostringstream support
using std::wostringstream;

#include <d2d1.h> // Include the Direct2D API.
#pragma comment(lib, "d2d1.lib") // Connect to the Direct2D Import Library.

#include <dwrite.h> // Include the DirectWrite API.
#pragma comment(lib, "dwrite.lib") // Connect to the DirectWrite Import Library.

#include <mmsystem.h> // Include the Multimedia header file.
#pragma comment(lib, "winmm.lib") // Connect to the Multimedia import library.

#include <wincodec.h> // WIC Header
#pragma comment(lib, "windowscodecs.lib")


class Game
{
	// Window Handle:
	HWND hWnd;

	// DirectX Helper Functions

	// The SafeRelease Pattern
	template <class T> void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	// Programmer defined macro to make using the known colors easier.
	#define D2DColor(clr) D2D1::ColorF(D2D1::ColorF::clr)

	HRESULT CreateGraphics(HWND hWnd);
	void DestroyGraphics(void);

	// DirectX Interfaces:
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush *pBrush;

	D2D1_COLOR_F custom;

	// DirectWrite Interfaces:
	IDWriteFactory* pDWFactory;
	IDWriteTextFormat* pTF;

	// WIC Interfaces
	IWICImagingFactory* pWICFactory;
	ID2D1Bitmap* image;


	// Frames shit
	DWORD dwFrames;
	DWORD dwCurrentTime;
	DWORD dwLastUpdateTime;
	DWORD dwElapsedTime;


public:
	bool bRunning;		
	HANDLE hGameThread; // Handle to the GameMain thread.
	BYTE keys[256];		// Current state of the keyboard.

	Game(void);
	virtual ~Game(void);

	void Startup(void);
	void Shutdown(void);
	void Input(void);
	void Simulate(void);
	void Render(void);

	HRESULT LoadBitmapFromFile(LPCTSTR strFileName, ID2D1Bitmap** ppBitmap);

	void SetHwnd(HWND _hWnd) {hWnd = _hWnd;}
};
