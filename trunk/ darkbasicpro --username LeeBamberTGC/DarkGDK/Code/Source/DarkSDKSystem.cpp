
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "..\Include\DarkSDKSystem.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int		TMEMAvailable				( void );
int		DMEMAvailable				( void );
int		SMEMAvailable				( void );

void	DisableEscapeKey			( void );
void	EnableEscapeKey				( void );
void	DisableSystemKeys			( void );
void	EnableSystemKeys			( void );
void	ExitPrompt					( DWORD pString, DWORD pString2 );

void	EmptyChecklist				( void );
int		ChecklistQuantity			( void );
int		ChecklistValueA				( int iIndex );
int		ChecklistValueB				( int iIndex );
int		ChecklistValueC				( int iIndex );
int		ChecklistValueD				( int iIndex );
DWORD	ChecklistFValueA			( int iIndex );
DWORD	ChecklistFValueB			( int iIndex );
DWORD	ChecklistFValueC			( int iIndex );
DWORD	ChecklistFValueD			( int iIndex );
DWORD	ChecklistString				( DWORD pDestStr, int iIndex );

void	DLLLoad						( DWORD pDLLFile, int dllid );
void	DLLDelete					( int dllid );
int		DLLExist					( int dllid );
int		DLLCallExist				( int dllid, DWORD pDLLFunction );
DWORD	CallDLL						( int dllid, DWORD pDLLFunction);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7,DWORD P8);
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7,DWORD P8,DWORD P9 );
DWORD	CallDLL						( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7,DWORD P8,DWORD P9,DWORD P10 );
DWORD	CallDLLX					( int dllid, DWORD pDLLFunction, DWORD pDataPtr, DWORD dwNumberOfDWORDS );
void*	GetGlobalStructure			( void );

void	ConstructorSystem			( void );
void	DestructorSystem			( void );
void	SetErrorHandlerSystem		( LPVOID pErrorHandlerPtr );
void	PassCoreDataSystem			( LPVOID pGlobPtr );
void	RefreshD3DSystem			( int iMode );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbLoadDLL( DWORD pDLLFile, int dllid )
{
	DLLLoad( pDLLFile,  dllid );
}

void dbDeleteDLL( int dllid )
{
	DLLDelete(  dllid );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction)
{
	return CallDLL(  dllid,  pDLLFunction);
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1)
{
	return CallDLL(  dllid,  pDLLFunction,  P1);
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2)
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3)
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4)
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3, P4 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5)
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3, P4, P5 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6)
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3, P4, P5, P6 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7)
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3, P4, P5, P6, P7 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7,DWORD P8)
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3, P4, P5, P6, P7, P8 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7,DWORD P8,DWORD P9 )
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3, P4, P5, P6, P7, P8, P9 );
}

DWORD dbCallDLL( int dllid, DWORD pDLLFunction, DWORD P1,DWORD P2,DWORD P3,DWORD P4,DWORD P5,DWORD P6,DWORD P7,DWORD P8,DWORD P9,DWORD P10 )
{
	return CallDLL(  dllid,  pDLLFunction,  P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 );
}

DWORD dbCallDLLX( int dllid, DWORD pDLLFunction, DWORD pDataPtr, DWORD dwNumberOfDWORDS )
{
	return CallDLLX( dllid,  pDLLFunction,  pDataPtr,  dwNumberOfDWORDS );
}

int dbDLLExist( int dllid )
{
	return DLLExist(  dllid );
}

int dbDLLCallExist( int dllid, DWORD pDLLFunction )
{
	return DLLCallExist(  dllid,  pDLLFunction );
}

void dbEmptyChecklist( void )
{
	EmptyChecklist ( );
}

int dbChecklistQuantity( void )
{
	return ChecklistQuantity( );
}

DWORD dbChecklistString( DWORD pDestStr, int iIndex )
{
	return ChecklistString(  pDestStr,  iIndex );
}

int dbChecklistValueA( int iIndex )
{
	return ChecklistValueA( iIndex );
}

int dbChecklistValueB( int iIndex )
{
	return ChecklistValueB( iIndex );
}

int dbChecklistValueC( int iIndex )
{
	return ChecklistValueC(  iIndex );
}

int dbChecklistValueD( int iIndex )
{
	return ChecklistValueD(  iIndex );
}

DWORD dbChecklistFValueA( int iIndex )
{
	return ChecklistFValueA(  iIndex );
}

DWORD dbChecklistFValueB( int iIndex )
{
	return ChecklistFValueB( iIndex );
}

DWORD dbChecklistFValueC( int iIndex )
{
	return ChecklistFValueC( iIndex );
}

DWORD dbChecklistFValueD( int iIndex )
{
	return ChecklistFValueD( iIndex );
}

void dbDisableEscapeKey(void)
{
	DisableEscapeKey ( );
}

void dbEnableEscapeKey(void)
{
	EnableEscapeKey ( );
}

void dbDisableSystemKeys(void)
{
	DisableSystemKeys ( );
}

void dbEnableSystemKeys(void)
{
	EnableSystemKeys ( );
}

void dbExitPrompt(char* pString, char* pString2)
{
	ExitPrompt(( DWORD ) pString, ( DWORD ) pString2);
}

int dbTMEMAvailable(void)
{
	return TMEMAvailable();
}

int dbDMEMAvailable(void)
{
	return DMEMAvailable();
}

int dbSMEMAvailable(void)
{
	return SMEMAvailable ( );
}

void* dbGetGlobalStructure ( void )
{
	return GetGlobalStructure ( );
}

void dbConstructorSystem ( void )
{
	ConstructorSystem ( );
}

void dbDestructorSystem ( void )
{
	DestructorSystem ( );
}

void dbSetErrorHandlerSystem ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerSystem ( pErrorHandlerPtr );
}

void dbPassCoreDataSystem( LPVOID pGlobPtr )
{
	PassCoreDataSystem( pGlobPtr );
}

void dbRefreshD3DSystem ( int iMode )
{
	RefreshD3DSystem ( iMode );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

  */