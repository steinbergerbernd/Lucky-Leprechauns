//--------------------------------------------------------------------------------------
// File: Main.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN

#include "DXUT.h"
#include "resource.h"

#include <LuckyLeprechauns.h>

LuckyLeprechauns* game;

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	D3DPRESENT_PARAMETERS& pp = pDeviceSettings->d3d9.pp;

	// disable vsync
	//pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// enable antialiasing
	pp.SwapEffect      = D3DSWAPEFFECT_DISCARD;
	pp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;

	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	Window::setHandle(DXUTGetHWND());

	game->setGraphicsDevice(pd3dDevice);
	game->init();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	game->onResetDevice(pd3dDevice);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	if (game)
		game->onLostDevice();
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	game->update(GameTime(fElapsedTime, (float)fTime));
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	game->draw(GameTime(fElapsedTime, (float)fTime));
}



//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    return 0;
}



void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, 
	bool bSideButton2Down, INT nMouseWheelDelta, INT xPos, INT yPos, void* pUserContext)
{
	POINT pos;

	GetCursorPos(&pos);

	Mouse::setState(MouseState(bLeftButtonDown, bRightButtonDown, nMouseWheelDelta, Point(pos.x, pos.y)));
}


void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	Keyboard::setKey(nChar, bKeyDown);
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//DXUTSetCallbackD3D9DeviceAcceptable(IsD3D9DeviceAcceptable);
	DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
	//DXUTSetCallbackMsgProc(MsgProc);

	DXUTSetCallbackD3D9DeviceCreated(OnD3D9CreateDevice);
	//DXUTSetCallbackD3D9DeviceDestroyed(OnD3D9DestroyDevice);

	DXUTSetCallbackD3D9DeviceReset(OnD3D9ResetDevice);
	DXUTSetCallbackD3D9DeviceLost(OnD3D9LostDevice);
    
	DXUTSetCallbackFrameMove(OnFrameMove);
	DXUTSetCallbackD3D9FrameRender(OnD3D9FrameRender);

	DXUTSetCallbackMouse(OnMouse, true);
	DXUTSetCallbackKeyboard(OnKeyboard);

	try
	{
		// Perform any application-level initialization here
		game = new LuckyLeprechauns();

		// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
		DXUTInit(true, true);
		DXUTSetHotkeyHandling(true, true, true);
		DXUTSetCursorSettings(true, true);
		DXUTCreateWindow(L"Lucky Leprechauns");
		DXUTCreateDevice(true, 800, 600);

		// Disable constant frame time
		DXUTSetConstantFrameTime(false);

		DXUTMainLoop();
	
		game->onLostDevice();
		game->release();
		delete game;
		game = 0;
	}
	catch (const Exception& ex)
	{
		Console::out << ex.what() << Console::endl;
		Logging::log(ex.what());
	}

	return DXUTGetExitCode();
}
