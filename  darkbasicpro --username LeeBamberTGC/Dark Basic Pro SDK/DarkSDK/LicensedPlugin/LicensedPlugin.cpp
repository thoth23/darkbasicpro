//
// Make Game TPC Prototype
//

// A Common Include
#include "windows.h"

// My Includes
#include "LicensedPlugin.h"

// Include Core (optional)
#include "core\globstruct.h"
GlobStruct* g_pGlob;

// Actual DLL Instance
HINSTANCE g_hDLLInstance = NULL;


//
// Constructors (optional)
//
void Constructor ( void )
{
	// Create memory here
}

void Destructor ( void )
{
	// Free memory here
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
	// 2=Sound
	return 0;
}


// 
// SPECIAL NOTE - The compiler will only add ESP data if the DLL ends with "Master.dll"
//                so we can still have protected DLLs that do not cludge exe with extras
//
//bool CASMWriter::AddProtectionToSelectedDLLs(LPSTR pDLLString)
//{
//	// ANYTHING ending with "Master.DLL" is protected
//	if ( strnicmp ( pDLLString + strlen ( pDLLString ) - 10, "Master.dll", 10 )==NULL )
//		return true;
//	else
//		return false;

//
// My Functions
//
void MakeGame ( LPSTR pStyle, LPSTR pHero, LPSTR pEnemy, LPSTR pStory )
{
	// Read the ESP value written by the main DBPRO EXECUTABLE
	// Stack pointer will reveal if the DLL has been wrapped!!
	DWORD * pErrPtr = (DWORD*)g_pGlob->g_pErrorHandlerRef;
	DWORD * pESPPtr = pErrPtr + 1;
	DWORD dwRecordedESP = *pESPPtr;

	// Read current ESP (was stored in EBP at the very start of the function 'at ASM level')
	// Sometimes, debug code in the compiler will change ESP substantially (in MSVC > 10240 bytes)
	DWORD dwCurrentESP;
	__asm mov dwCurrentESP, EBP;

	// Compare difference
	int iDifference = abs ( dwCurrentESP - dwRecordedESP );
	if ( iDifference > 12 )
		MessageBox(NULL, "This DLL has been tampered with!", "Internal Protection Error", MB_OK);

	DWORD* pLINEPtr = ((DWORD*)g_pGlob->g_pVariableSpace) + 5;
	DWORD dwRecordedLINE = *pLINEPtr;

	// Regular code for this command
	char pText[10240];
	wsprintf ( pText, "Stopped On Line Number : %d\n\n", dwRecordedLINE);
	DWORD* pPtr = (DWORD*)g_pGlob->g_pVariableSpace;
	for (int i=0; i<16; i++ ) wsprintf(pText, "%s%d : %d\n", pText, (pPtr+i), *(pPtr+i) );
	MessageBox(NULL, pText, "Variable Memory Dump", MB_OK);
}

void PlayGame ( void )
{
	g_hDLLInstance = LoadLibrary ( "dbproplugin.dll" );

	// Obtain path to this DLL being used
	char lpFilename[256];
	GetModuleFileName ( g_hDLLInstance, lpFilename, 256 );
	MessageBox(NULL, lpFilename, "Current Path To DLL File", MB_OK);

	FreeLibrary ( g_hDLLInstance );
}

void EndGame ( void )
{
	MessageBox(NULL, "...but got squashed by a big foot from the sky.", "", MB_OK);
}

void MessageBoxPopup ( DWORD pString )
{
	MessageBox ( NULL, (LPSTR)pString, "", MB_OK );
}

