#pragma once

#include <Windows.h> // Include the Win32 API
#include <tchar.h> // Include support for UNICODE
#include <sstream> // Include wostringstream support
#include <iterator>
#include <list>
using namespace std;


using std::wostringstream;

#include <d2d1.h> // Include the Direct2D API.
#pragma comment(lib, "d2d1.lib") // Connect to the Direct2D Import Library.

#include <dwrite.h> // Include the DirectWrite API.
#pragma comment(lib, "dwrite.lib") // Connect to the DirectWrite Import Library.

#include <mmsystem.h> // Include the Multimedia header file.
#pragma comment(lib, "winmm.lib") // Connect to the Multimedia import library.

#include <wincodec.h> // WIC Header
#pragma comment(lib, "windowscodecs.lib")

#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmodex_vc.lib")



//*******Defaults for ease of access******************
#define GREYINITPOS D2D1::RectF(300, 300, 380, 380)
#define BLACKINITPOS D2D1::RectF(200, 200, 280, 280)
#define ZEROVECTOR D2D1::SizeF(0,0)
#define FRAMEDURATION 100
#define DEFAULTSPEED 2
//*****************************************************//



enum Level {Level1, NUMLEVELS};
enum State {Intro, Menu, Playing, Paused};

class Game
{

	struct Player
	{
		ID2D1Bitmap* sprite;
		D2D1_RECT_F position;
		D2D1_SIZE_F vector;
		int frame;
		DWORD frameDuration;
	};
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
	FMOD_RESULT LoadFMOD();
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


	// FMOD Interface
	FMOD::System* sys;
	FMOD_RESULT result;
	unsigned int version;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS caps;
	char name[256];


	// Frames shit
	DWORD dwFrames;
	DWORD dwCurrentTime;
	DWORD dwLastUpdateTime;
	DWORD dwElapsedTime;

	// Game Components
	Player black, grey;

	// Game Functions
	bool Collision(D2D1_RECT_F _rectA, D2D1_RECT_F _rectB);
	bool Collision(Player _player1, Player _player2);
	bool Collision(Player _player, D2D1_RECT_F _rect);

	//Game Variables
	Level currLevel = Level1;
	State currState = Playing;
	list<D2D1_RECT_F> rects;
	std::list<D2D1_RECT_F>::iterator iter;
	
	


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
