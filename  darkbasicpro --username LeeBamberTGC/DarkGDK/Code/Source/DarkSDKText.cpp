
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKText.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void 		ConstructorText				( HINSTANCE );
void 		DestructorText				( void );
void 		SetErrorHandlerText			( LPVOID pErrorHandlerPtr );
void 		PassCoreDataText			( LPVOID pGlobPtr );
void 		RefreshD3DText				( int iMode );

void 		SetTextColor				( int iAlpha, int iRed, int iGreen, int iBlue );
void 		SetTextFont					( char* szTypeface, int iCharacterSet );
void 		Text						( int iX, int iY, char* szText );
int 		GetTextWidth				( char* szString );
int 		GetTextHeight				( char* szString );

int			Asc							( DWORD dwSrcStr);
DWORD 		Bin							( DWORD pDestStr, int iValue );
DWORD 		Chr							( DWORD pDestStr, int iValue );
DWORD 		Hex							( DWORD pDestStr, int iValue );
DWORD 		Left						( DWORD pDestStr, DWORD szText, int iValue );
int			Len							( DWORD dwSrcStr );
DWORD 		Lower						( DWORD szText, DWORD pDestStr );
DWORD 		Mid							( DWORD pDestStr, DWORD szText, int iValue );
DWORD 		Right						( DWORD pDestStr, DWORD szText, int iValue );
DWORD 		Str							( DWORD pDestStr, float fValue );
DWORD 		Str							( DWORD pDestStr, int iValue );
DWORD 		Upper						( DWORD pDestStr, DWORD szText );

DWORD 		ValF						( DWORD dwSrcStr );
DWORD 		StrDouble					( DWORD pDestStr, double dValue );
LONGLONG 	ValR						( DWORD dwSrcStr );

void		PerformChecklistForFonts	( void );
void 		BasicText					( int iX, int iY, DWORD szText );
void 		CenterText					( int iX, int iY, DWORD szText );
void 		SetBasicTextFont			( DWORD szTypeface );
void 		SetBasicTextFont			( DWORD szTypeface, int iCharacterSet );
void 		SetTextSize					( int iSize );
void 		SetTextToNormal				( void );
void 		SetTextToItalic				( void );
void 		SetTextToBold				( void );
void 		SetTextToBoldItalic			( void );
void 		SetTextToOpaque				( void );
void 		SetTextToTransparent		( void );

int 		TextBackgroundType			( void );
DWORD 		TextFont					( DWORD pDestStr );
int 		TextSize					( void );
int 		TextStyle					( void );
int 		TextWidth					( DWORD szString );
int 		TextHeight					( DWORD szString );

DWORD 		Spaces						( DWORD dwSrcStr, int iSpaces );

void 		Append						( DWORD dwA, DWORD dwB );
void 		Reverse						( DWORD dwA );
int 		FindFirstChar				( DWORD dwSource, DWORD dwChar );
int 		FindLastChar				( DWORD dwSource, DWORD dwChar );
int 		FindSubString				( DWORD dwSource, DWORD dwString );
int 		CompareCase					( DWORD dwA, DWORD dwB );
DWORD 		FirstToken					( DWORD dwReturn, DWORD dwSource, DWORD dwDelim );
DWORD 		NextToken					( DWORD dwReturn, DWORD dwDelim );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbText ( int iX, int iY, char* szText )
{
	BasicText ( iX, iY, ( DWORD ) szText );
}

void dbCenterText ( int iX, int iY, char* szText )
{
	CenterText ( iX, iY, ( DWORD ) szText );
}

void dbSetTextFont ( char* szTypeface )
{
	SetBasicTextFont ( ( DWORD ) szTypeface );
}

void dbSetTextFont ( char* szTypeface, int iCharacterSet )
{
	SetBasicTextFont ( ( DWORD ) szTypeface, iCharacterSet );
}

void dbSetTextSize ( int iSize )
{
	SetTextSize ( iSize );
}

void dbSetTextOpaque ( void )
{
	SetTextToOpaque ( );
}

void dbSetTextTransparent ( void )
{
	SetTextToTransparent ( );
}

void dbSetTextToNormal ( void )
{
	SetTextToNormal ( );
}

void dbSetTextToItalic ( void )
{
	SetTextToItalic ( );
}

void dbSetTextToBold ( void )
{
	SetTextToBold ( );
}

void dbSetTextToBoldItalic ( void )
{
	SetTextToBoldItalic ( );
}

void dbPerformChecklistForFonts ( void )
{
	PerformChecklistForFonts ( );
}

void dbSetTextColor ( int iAlpha, int iRed, int iGreen, int iBlue )
{
	SetTextColor ( iAlpha, iRed, iGreen, iBlue );
}

int dbTextBackgroundType ( void )
{
	return TextBackgroundType ( );
}

char* dbTextFont ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = TextFont ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbTextSize ( void )
{
	return TextSize ( );
}

int dbTextStyle ( void )
{
	return TextStyle ( );
}

int dbTextWidth ( char* szString )
{
	return TextWidth ( ( DWORD ) szString );
}

int dbTextHeight ( char* szString )
{
	return TextHeight ( ( DWORD ) szString );
}

int	dbAsc ( char* dwSrcStr )
{
	return Asc ( ( DWORD ) dwSrcStr );
}

char* dbBin ( int iValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Bin ( NULL, iValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbChr ( int iValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Chr ( NULL, iValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbHex ( int iValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Hex ( NULL, iValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbLeft ( char* szText, int iValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Left ( NULL, ( DWORD ) szText, iValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int	dbLen ( char* dwSrcStr )
{
	return Len ( ( DWORD ) dwSrcStr );
}

char* dbLower ( char* szText )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Lower ( ( DWORD ) szText, NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbMid ( char* szText, int iValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Mid ( NULL, ( DWORD ) szText, iValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbRight ( char* szText, int iValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Right ( NULL, ( DWORD ) szText, iValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbStr ( float fValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Str ( NULL, fValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbStr ( int iValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Str ( NULL, iValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbUpper ( char* szText )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Upper ( NULL, ( DWORD ) szText );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbValF ( char* dwSrcStr )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = ValF ( ( DWORD ) dwSrcStr );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbSpaces ( char* dwSrcStr, int iSpaces )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Spaces ( ( DWORD ) dwSrcStr, iSpaces );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbStrDouble ( double dValue )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = StrDouble ( NULL, dValue );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

void dbConstructorText ( HINSTANCE hSetup )
{
	ConstructorText ( hSetup );
}

void dbDestructorText ( void )
{
	DestructorText ( );
}

void dbSetErrorHandlerText ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerText ( pErrorHandlerPtr );
}

void dbPassCoreDataText ( LPVOID pGlobPtr )
{
	PassCoreDataText ( pGlobPtr );
}

void dbRefreshD3DText ( int iMode )
{
	RefreshD3DText ( iMode );
}

int dbGetTextWidth ( char* szString )
{
	return GetTextWidth ( szString );
}

int dbGetTextHeight ( char* szString )
{
	return GetTextHeight ( szString );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

*/