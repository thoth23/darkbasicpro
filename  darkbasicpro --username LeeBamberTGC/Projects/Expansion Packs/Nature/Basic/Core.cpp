
////////////////////////////////////////////////////////////////////
// INFORMATION /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

/*
	CORE SET UP COMMANDS
*/

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

/*
// type definition
	

	// variables
	
	
*/

////////////////////////////////////////////////////////////////////
// DEFINES AND INCLUDES ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#define DARKSDK	__declspec ( dllexport )
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include "core.h"

// lee - 130206 - Local Data Ptr (used to be including globstruct directly)
GlobStruct*				g_pGlobForTerrain = NULL;

char					g_szErrorList [ 256 ] [ 256 ];
bool					g_bErrorFile = false;

#define SAFE_DELETE( p )		{ if ( p ) { delete ( p );       ( p ) = NULL; } }
#define SAFE_RELEASE( p )		{ if ( p ) { ( p )->Release ( ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p )	{ if ( p ) { delete [ ] ( p );   ( p ) = NULL; } }

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// GLOBALS /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// FUNCTIONS ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

DARKSDK void ReceiveCoreDataPtr ( LPVOID pCore );
DARKSDK int  GetAssociatedDLLs  ( void );
DARKSDK void Destructor         ( void );
DARKSDK void RefreshD3D         ( int iMode );

extern void SetupTerrain ( void );
extern void DestroyTerrain ( void );

void SetupErrorCodes ( void );

void ReceiveCoreDataPtr ( LPVOID pCore )
{
	g_pGlobForTerrain = ( GlobStruct* ) pCore;

	SetupErrorCodes ( );

	if ( !g_pGlobForTerrain )
		Error ( 1 );

	SetupTerrain ( );
}

// lee - 130206 - when this is static, DBP cannot find this function for DLL use
#ifndef DARKSDK_COMPILE
 void RefreshD3D ( int iMode )
#else
 static void RefreshD3D ( int iMode )
#endif
{
	// mode zero - close down
	if ( iMode == 0 )
	{
// lee - 130206 - done in destructor
//		// destroy any terrain resources
//		DestroyTerrain ( );

		// call final desctructor of terrain class
		Destructor ( );
	}

	// mode one - start up again
	if ( iMode == 1 )
	{
		// ensure glob and ptrs are still valid
		ReceiveCoreDataPtr ( g_pGlobForTerrain );
	}
}

int GetAssociatedDLLs ( void )
{
	// lee - 130206 - set 1 which means use basic3d and friends
	return 1;
}

// lee - 130206 - destructor must be available to DBP DLL caller
#ifndef DARKSDK_COMPILE
 void Destructor ( void )
#else
 static void Destructor ( void )
#endif
{
	// free resources used by terrain
	DestroyTerrain ( );
}

bool Check ( int iID, int iMax )
{
	if ( iID >= iMax || iID <= 0 )
		Error ( 2 );

	return true;
}

void SetupErrorCodes ( void )
{
	// set up error codes for when things fail

	{
		HKEY hKey1;
		char szProductType[80];
		DWORD dwBufLen1;

		RegOpenKeyEx (HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey1);
		RegQueryValueEx (hKey1, "ProductType", NULL, NULL, (LPBYTE) szProductType, &dwBufLen1);
		RegCloseKey (hKey1);
	}

	char  szPath [ 255 ] = "";
	char  szDir  [ 255 ] = "";
	HKEY  hKey;
	DWORD dwBufLen;

	// open the registry key
	RegOpenKeyEx    ( HKEY_LOCAL_MACHINE, "SOFTWARE\\Dark Basic\\Dark Basic Pro", 0, KEY_QUERY_VALUE, &hKey );
	RegQueryValueEx ( hKey, "install-path", NULL, NULL, ( LPBYTE ) szPath, &dwBufLen );
	RegCloseKey     ( hKey );

	// append our path
	strcat ( szPath, "\\compiler\\plugins-licensed\\" );

	GetCurrentDirectory ( 255, szDir );
	SetCurrentDirectory ( szPath );

	// clear out the error table
	memset ( g_szErrorList, 0, sizeof ( g_szErrorList ) );

	// attempt to open the "loc" file
	FILE* fp = fopen ( "AdvancedTerrainExpansionPack.loc", "r" );

	// if the file is valid we should be running from DB Pro
	if ( fp )
	{
		// read in each line of the file
		char szTemp [ 256 ];
		int  iIndex = 0;
		
		while ( fgets ( szTemp, sizeof ( szTemp ), fp ) )
		{
			char* token = strtok ( szTemp, "\n" );

			while ( token )
			{
				strcpy ( g_szErrorList [ iIndex++ ], token );
				token = strtok ( 0, "\n" );
			}
		}

		// close the file
		fclose ( fp );

		// save state as we have the error file loaded
		g_bErrorFile = true;
	}

	// restory directory
	SetCurrentDirectory ( szDir );
}

void Error ( int iID )
{
	if ( g_bErrorFile )
		MessageBox ( NULL, g_szErrorList [ iID ], g_szErrorList [ 0 ], MB_ICONERROR | MB_OK );
	else
	{
		char szNum [ 3 ];

		_itoa ( iID, szNum, 10 );

		MessageBox ( NULL, szNum, "Error", MB_ICONERROR | MB_OK );
	}

	PostQuitMessage ( 0 );
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
