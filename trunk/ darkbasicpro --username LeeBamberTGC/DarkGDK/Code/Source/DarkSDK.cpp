/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARKGDK //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Includes
#include "..\Include\DarkSDK.h"
#include "..\Include\DarkSDKCore.h"
#include "..\Include\globstruct.h"

// Prototypes
bool StartSDK             ( void );
bool EndSDK               ( void );
bool LoadSDKDLLs          ( void );
bool CreateSDKApplication ( void );

// Global for CL$ storage
LPSTR g_pUniqueCommandLineString = NULL;

// Global for runtime error recording
DWORD dwRuntimeErrorDWORD = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION LISTINGS ////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	g_pUniqueCommandLineString = lpCmdLine;
	if ( StartSDK ( )==true )
	{
		DarkGDK ( );
		EndSDK ( );
	}
	return 0;
}

bool StartSDK ( void )
{
	if ( !LoadSDKDLLs ( ) )
		return false;

	if ( !CreateSDKApplication ( ) )
		return false;

	return true;
}

bool EndSDK ( void )
{
	return true;
}

bool LoadSDKDLLs ( void )
{
	g_pGlob = ( GlobStruct* ) dbGetGlobPtr ( );
	return true;
}

bool CreateSDKApplication ( void )
{
	// pass DLLs in
	dbPassDLLs();

	// required for dbCL()
	dbPassCmdLineLineHandlerPtr ( ( LPVOID ) g_pUniqueCommandLineString );

	// pass error handler to return error code
	dbPassErrorHandlerPtr ( ( LPVOID ) &dwRuntimeErrorDWORD );

	// calls init display with settings
	DWORD dwDisplayType = 1;
	DWORD dwWidth = 640;
	DWORD dwHeight = 480;
	DWORD dwDepth = 32;
	HINSTANCE hInstance = GetModuleHandle ( NULL );
	LPSTR pApplicationName = "";
	HWND pParentHWND = NULL;
	DWORD dwInExStyle = 0;
	DWORD dwInStyle = 0;
	dbInitDisplayEx (dwDisplayType, dwWidth, dwHeight, dwDepth, hInstance, pApplicationName, pParentHWND, dwInExStyle, dwInStyle);

	// success
	return true;
}

bool LoopGDK ( void )
{
	if ( g_iDarkGameSDKQuit )
	{
		PostQuitMessage ( 0 );
		return false;
	}
	return true;
}
