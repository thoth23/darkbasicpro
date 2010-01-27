//
// Make Game TPC Prototype
//

// A Common Include
#include "windows.h"

// My Includes
#include "MakeGame.h"

// Include Core (optional)
#include "core\globstruct.h"
GlobStruct* g_pGlob;

//
// Constructors (optional)
//
void Constructor ( void )
{
	// Create memory here
	int started=42;
}

void Destructor ( void )
{
	// Free memory here
}

void PreDestructor ( void )
{
	// pre destructor
}

void ReceiveCoreDataPtr ( LPVOID pCore )
{
	// Get Core Data Pointer here
	g_pGlob = (GlobStruct*)pCore;
}

int GetAssociatedDLLs ( void )
{
	// return a code indicating which DLLs are required
	// 1=Basic3D (and friends)
	return 0;
}

int GetNumDependencies ( void )
{
	MessageBox ( NULL, "hello", "hello test", MB_OK );

	// no dependences
	return 0;

	/* if we wanted to associate third party DLLs to this plugin, we specify how many
	return 5;
	*/
}

const char * GetDependencyID ( int n )
{
	// return an empty string, the string is safely read-only, no dependencies
	return "";

	/* if we wanted to associate third party DLLs to this plugin, we return the name of each
	switch ( n )
	{
		case 0 : return "MyFirstLibrary.DLL";
		case 1 : return "MySecondLibrary.DLL";
		case 2 : return "MyThirdLibrary.DLL";
	}
	*/
}

void ReceiveDependenciesHinstance ( LPSTR pDLLName, HINSTANCE hModule )
{
	// got the hinstance for this DLL
	HINSTANCE DLL = hModule;
}

void OptionalSecurityCode ( int iInitCode )
{
	// Load Core DLL (get code from core)
	typedef int ( *DLL_CoreCall ) ( void );
	HINSTANCE hDLL = LoadLibrary("DBProCore.dll");
	if ( hDLL )
	{
		DLL_CoreCall g_DLL_GetSecurityCode;
		g_DLL_GetSecurityCode = ( DLL_CoreCall ) GetProcAddress ( hDLL, "?GetSecurityCode@@YAHXZ" );
		if ( g_DLL_GetSecurityCode )
		{
			int iSecurityCode = g_DLL_GetSecurityCode();
			if ( iSecurityCode!=-1 )
			{
				if ( iInitCode==iSecurityCode )
				{
					// if they match, can activate this PLUGIN
					MessageBox ( NULL, "ACTIVATED", "ME", MB_OK );
				}
			}
		}
		FreeLibrary(hDLL);
	}
}

static int iD3DDeviceLostControlVariable = 0;

void D3DDeviceLost ( void )
{
	if ( iD3DDeviceLostControlVariable==0 )
	{
		MessageBox ( NULL, "D3DDeviceLost", "device lost", MB_OK );
		iD3DDeviceLostControlVariable = 1;
	}
}

void D3DDeviceNotReset ( void )
{
	if ( iD3DDeviceLostControlVariable==1 )
	{
		MessageBox ( NULL, "D3DDeviceNotReset", "device not reset", MB_OK );
		iD3DDeviceLostControlVariable = 0;
	}
}

//
// My Functions
//
void MakeGame ( LPSTR pStyle, LPSTR pHero, LPSTR pEnemy, LPSTR pStory )
{
	char pText[10240];
	DWORD dwLineNumber = *((DWORD*)g_pGlob->g_pErrorHandlerRef+0);
	wsprintf(pText, "You are at line %d in the program.\n", dwLineNumber );
	wsprintf(pText, "%s Your name is %s from the planet Zob. You have come to earth\n", pText, pHero);
	wsprintf(pText, "%s to rid it from the evil %s in order to %s.\n", pText, pEnemy, pStory);
	MessageBox(NULL, pText, pStyle, MB_OK);
}

void PlayGame ( void )
{
	MessageBox(NULL, "The fought with valour...", "", MB_OK);
}

void EndGame ( void )
{
	MessageBox(NULL, "...but got squashed by a big foot from the sky.", "", MB_OK);
}
