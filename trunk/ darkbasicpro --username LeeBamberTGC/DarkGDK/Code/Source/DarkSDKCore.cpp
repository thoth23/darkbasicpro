/////////////////////////////////////////////////////////////////////////////////////////////////////////
// CORE COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKCore.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void		Cls						( void );
void		SetCursor				( int iX, int iY );
void		PrintR					( LONGLONG lValue );
void		PrintO					( double dValue );
void		PrintS					( LPSTR pString );
void		Print0					( void );
void		PrintCR					( LONGLONG lValue );
void		PrintCO					( double dValue );
void		PrintCS					( LPSTR pString );
LONGLONG	InputR					( void );
double		InputO					( void );
DWORD		InputS					( DWORD pDestStr );
DWORD		AbsFF					( float fValue );
DWORD		IntLF					( float fValue );
DWORD		AcosFF					( float fValue );
DWORD		AsinFF					( float fValue );
DWORD		AtanFF					( float fValue );
DWORD		Atan2FFF				( float fA, float fB );
DWORD		CosFF					( float fAngle );
DWORD 		SinFF					( float fAngle );
DWORD 		TanFF					( float fAngle );
DWORD 		HcosFF					( float fAngle );
DWORD 		HsinFF					( float fAngle );
DWORD 		HtanFF					( float fAngle );
DWORD 		SqrtFF					( float fValue );
DWORD 		ExpFF					( float fExp );
void 		Randomize				( int iSeed );
int 		RndLL					( int r );
float 		wrapangleoffset			( float da );
DWORD 		CurveValueFFFF			( float a, float da, float sp );
DWORD 		WrapValueFF				( float da );
DWORD 		NewXValueFFFF			( float x, float a, float b );
DWORD 		NewYValueFFFF			( float x, float a, float b );
DWORD 		NewZValueFFFF			( float z, float a, float b );
DWORD 		CurveAngleFFFF			( float a, float da, float sp );
int 		TimerL					( void );
void 		SleepL					( int iDelay );
void 		WaitL					( int iDelay );
void 		WaitForKey				( void );
void 		WaitForMouse			( void );
DWORD 		Cl$						( DWORD dwDestStr );
DWORD 		GetDate$				( DWORD dwDestStr );
DWORD 		GetTime$				( DWORD dwDestStr );
DWORD 		InkeyS					( DWORD dwDestStr );
void 		SyncOn					( void );
void 		SyncOff					( void );
void 		Sync					( void );
void 		FastSync				( void );
void 		SyncRate				( int iRate );
void 		DrawToBack				( void );
void 		DrawToFront				( void );
void 		DrawToCamera			( void );
void 		DrawToScreen			( void );
void 		DrawSpritesFirst		( void );
void 		DrawSpritesLast			( void );
void 		AlwaysActiveOff			( void );
void 		AlwaysActiveOn			( void );
DWORD 		GetGlobPtr				( void );
void 		PassDLLs				( void );
void 		PassCmdLineHandlerPtr	( LPVOID pCmdLine );
void 		PassErrorHandlerPtr		( LPVOID pErrorPtr );
DWORD		InitDisplayEx			( DWORD dwDisplayType, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, HINSTANCE hInstance, LPSTR pApplicationName, HWND pParentHWND, DWORD dwInExStyle, DWORD dwInStyle);
DWORD 		InitDisplay				( DWORD dwDisplayType, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, HINSTANCE hInstance, LPSTR pApplicationName );
void		SyncMask				( DWORD dwMask );
void		SyncDisableQuad			( void );
void		SyncEnableQuad			( void );
void		SyncRenderQuad			( void );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD dbGetGlobPtr ( void )
{
	return GetGlobPtr ( );
}

void dbPassDLLs ( void )
{
	PassDLLs ( );
}

void dbPassCmdLineLineHandlerPtr ( LPVOID pCmdLinePtr )
{
	PassCmdLineHandlerPtr ( pCmdLinePtr );
}

void dbPassErrorHandlerPtr ( LPVOID pErrorPtr )
{
	PassErrorHandlerPtr ( pErrorPtr );
}

DWORD dbInitDisplayEx ( DWORD dwDisplayType, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, HINSTANCE hInstance, LPSTR pApplicationName, HWND pParentHWND, DWORD dwInExStyle, DWORD dwInStyle)
{
	// dwDisplayType
	// =============
	// 0=Hidden Mode
	// 1=Window Mode
	// 2=Desktop Fullscreen Mode
	// 3=Exclusive Fullscreen Mode
	// 4=Desktop Fullscreen Mode (No Taskbar)
	// 5=Custom Settings (pass in EX and STYLE)
	return InitDisplayEx ( dwDisplayType, dwWidth, dwHeight, dwDepth, hInstance, pApplicationName, pParentHWND, dwInExStyle, dwInStyle );
}

DWORD dbInitDisplay ( DWORD dwDisplayType, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, HINSTANCE hInstance, LPSTR pApplicationName )
{
	// see above
	return InitDisplay ( dwDisplayType, dwWidth, dwHeight, dwDepth, hInstance, pApplicationName );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbSyncOn ( void )
{
	SyncOn ( );
}

void dbSyncOff ( void )
{
	SyncOff ( );
}

void dbSync ( void )
{
	Sync ( );
}

void dbSyncRate ( int dbiRate )
{
	SyncRate ( dbiRate );
}

void dbFastSync ( void )
{
	FastSync ( );
}

void dbCLS ( void )
{
	Cls ( );
}

void dbSetCursor ( int iX, int iY )
{
	SetCursor ( iX, iY );
}

void dbPrint ( LONGLONG lValue )
{
	PrintR ( lValue );
}

void dbPrint ( double dValue )
{
	PrintO ( dValue );
}

void dbPrint ( LPSTR pString )
{
	PrintS ( pString );
}

void dbPrint ( void )
{
	Print0 ( );
}

void dbPrintC ( LONGLONG lValue )
{
	PrintCR ( lValue );
}

void dbPrintC ( double dValue )
{
	PrintCO ( dValue );
}

void dbPrintC ( LPSTR pString )
{
	PrintCS ( pString );
}

char* dbInput ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = InputS ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

void dbWait ( int dbiDelay )
{
	SleepL ( dbiDelay );
}

void dbSleep ( int dbiDelay )
{
	SleepL ( dbiDelay );
}

void dbWaitMouse ( void )
{
	WaitForMouse ( );
}

void dbWaitKey ( void )
{
	WaitForKey ( );
}

void dbSuspendForKey ( void )
{
	WaitForKey ( );
}

void dbSuspendForMouse ( void )
{
	WaitForMouse ( );
}

void dbDrawToBack ( void )
{
	DrawToBack ( );
}

void dbDrawToFront ( void )
{
	DrawToFront ( );
}

void dbDrawToCamera(void)
{
	DrawToCamera();
}

void dbDrawToScreen(void)
{
	DrawToScreen();
}

void dbDrawSpritesFirst ( void )
{
	DrawSpritesFirst ( );
}

void dbDrawSpritesLast ( void )
{
	DrawSpritesLast ( );
}

void dbRandomize ( int dbiSeed )
{
	Randomize ( dbiSeed );
}

float dbCos ( float fValue )
{
	DWORD dwReturn = CosFF ( fValue );
	
	return *( float* ) &dwReturn;
}

float dbSin ( float fAngle )
{
	DWORD dwReturn = SinFF ( fAngle );

	return *( float* ) &dwReturn;
}

float dbTan ( float fAngle )
{
	DWORD dwReturn = TanFF ( fAngle );

	return *( float* ) &dwReturn;
}

float dbAcos ( float fValue )
{
	DWORD dwReturn = AcosFF ( fValue );

	return *( float* ) &dwReturn;
}

float dbAsin ( float fValue )
{
	DWORD dwReturn = AsinFF ( fValue );

	return *( float* ) &dwReturn;
}

float dbAtan ( float fValue )
{
	DWORD dwReturn = AtanFF ( fValue );

	return *( float* ) &dwReturn;
}

float dbAtanFull ( float fA, float fB )
{
	DWORD dwReturn = Atan2FFF ( fA, fB );

	return *( float* ) &dwReturn;
}

float dbHcos ( float fAngle )
{
	DWORD dwReturn = HcosFF ( fAngle );

	return *( float* ) &dwReturn;
}

float dbHsin ( float fAngle )
{
	DWORD dwReturn = HsinFF ( fAngle );

	return *( float* ) &dwReturn;
}

float dbHtan ( float fAngle )
{
	DWORD dwReturn = HtanFF ( fAngle );

	return *( float* ) &dwReturn;
}

float dbSqrt ( float fValue )
{
	DWORD dwReturn = SqrtFF ( fValue );

	return *( float* ) &dwReturn;
}

float dbAbs ( float fValue )
{
	DWORD dwReturn = AbsFF ( fValue );

	return *( float* ) &dwReturn;
}

float dbInt ( float fValue )
{
	DWORD dwReturn = IntLF ( fValue );

	return *( float* ) &dwReturn;
}

float dbExp ( float fValue )
{
	DWORD dwReturn = ExpFF ( fValue );

	return *( float* ) &dwReturn;
}

int dbRnd ( int dbr )
{
	return RndLL ( dbr );
}

int dbTimer ( void )
{
	return TimerL ( );
}

char* dbInKey ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = InkeyS ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbCl$ ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Cl$ ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbGetDate$ ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetDate$ ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbGetTime$ ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetTime$ ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

float dbCurveValue ( float a, float da, float sp )
{
	DWORD dwReturn = CurveValueFFFF ( a, da, sp );

	return *( float* ) &dwReturn;
}

float dbWrapValue ( float da )
{
	DWORD dwReturn = WrapValueFF ( da );

	return *( float* ) &dwReturn;
}

float dbNewXValue ( float x, float a, float b )
{
	DWORD dwReturn = NewXValueFFFF ( x, a, b );

	return *( float* ) &dwReturn;
}

float dbNewYValue ( float y, float a, float b )
{
	DWORD dwReturn = NewYValueFFFF ( y, a, b );

	return *( float* ) &dwReturn;
}

float dbNewZValue ( float z, float a, float b )
{
	DWORD dwReturn = NewZValueFFFF ( z, a, b );

	return *( float* ) &dwReturn;
}

float dbCurveAngle ( float a, float da, float sp )
{
	DWORD dwReturn = CurveAngleFFFF ( a, da, sp );

	return *( float* ) &dwReturn;
}

void dbAlwaysActiveOff ( void )
{
	AlwaysActiveOff ( );
}

void dbAlwaysActiveOn ( void )
{
	AlwaysActiveOn ( );
}

void dbSyncMask ( DWORD dwMask )
{
	SyncMask ( dwMask );
}

void dbSyncDisableQuad(void)
{
	SyncDisableQuad();
}

void dbSyncEnableQuad(void)
{
	SyncEnableQuad();
}

void dbSyncRenderQuad(void)
{
	SyncRenderQuad();
}

int			dbRND					( int dbr )				{ return dbRnd ( dbr ); }
float		dbCOS					( float fValue )		{ return dbCos ( fValue ); }
float		dbSIN					( float fAngle )		{ return dbSin ( fAngle ); }
float		dbTAN					( float fAngle )		{ return dbTan ( fAngle ); }
float		dbACOS					( float fValue )		{ return dbAcos ( fValue ); }
float		dbASIN					( float fValue )		{ return dbAsin ( fValue ); }
float		dbATAN					( float fValue )		{ return dbAtan ( fValue ); }
float		dbATANFULL				( float fA, float fB )	{ return dbAtanFull ( fA, fB ); }
float		dbHCOS					( float fAngle )		{ return dbHcos ( fAngle ); }
float		dbHSIN					( float fAngle )		{ return dbHsin ( fAngle ); }
float		dbHTAN					( float fAngle )		{ return dbHtan ( fAngle ); }
float		dbSQRT					( float fValue )		{ return dbSqrt ( fValue ); }
float		dbABS					( float fValue )		{ return dbAbs ( fValue ); }
float		dbINT					( float fValue )		{ return dbInt ( fValue ); }
float		dbEXP					( float fExp )			{ return dbExp ( fExp ); }
char*		dbCL					( void )				{ return dbCl$ ( ); }
char*		dbGetDate				( void )				{ return dbGetDate$ ( ); }
char*		dbGetTime				( void )				{ return dbGetTime$ ( ); }
