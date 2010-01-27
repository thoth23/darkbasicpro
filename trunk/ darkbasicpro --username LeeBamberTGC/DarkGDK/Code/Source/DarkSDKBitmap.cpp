
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BITMAP COMMANDS //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKBitmap.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void				ConstructorBitmap							( HINSTANCE );
void				DestructorBitmap							( void );
void				SetErrorHandlerBitmap						( LPVOID pErrorHandlerPtr );
void				PassCoreDataBitmap							( LPVOID pGlobPtr );
void				RefreshD3DBitmap							( int iMode );
void				UpdateBitmapZeroOfNewBackbufferBitmap		( void );
LPDIRECT3DSURFACE9	MakeBitmap									( int iID, int iWidth, int iHeight );

void				CreateBitmap								( int iID, int iWidth, int iHeight );
void				LoadBitmap									( char* szFilename, int iID );
void				SaveBitmap									( char* szFilename, int iID );
void				LoadBitmapEx								( char* szFilename );
void				SaveBitmapEx								( char* szFilename );
void				CopyBitmap									( int iID, int iToID );
void				CopyBitmapEx								( int iID, int iX1, int iY1, int iX2, int iY2, int iToID, int iToX1, int iToY1, int iToX2, int iToY2 );
void				DeleteBitmapEx								( int iID );
void				FlipBitmap									( int iID );
void				MirrorBitmap								( int iID );
void				FadeBitmap									( int iID, int iFade );
void				BlurBitmap									( int iID, int iBlurLevel );
void				SetCurrentBitmap							( int iID );

int					CurrentBitmap								( void );
int					BitmapExist									( int iID );
int					BitmapWidth									( int iID );
int					BitmapHeight								( int iID );
int					BitmapDepth									( int iID );
int					BitmapMirrored								( int iID );
int					BitmapFlipped								( int iID );
int					BitmapExist									( void );
int					BitmapWidth									( void );
int					BitmapHeight								( void );
int					BitmapDepth									( void );
int					BitmapMirrored								( void );
int					BitmapFlipped								( void );

void				GetBitmapData								( int iID, DWORD* dwWidth, DWORD* dwHeight, DWORD* dwDepth, LPSTR* pData, DWORD* dwDataSize, bool bLockData );
void				SetBitmapData								( int bitmapindex, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, LPSTR pData, DWORD dwDataSize );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbLoadBitmap ( char* szFilename, int iID )
{
	LoadBitmap ( szFilename, iID );
}

void dbLoadBitmap ( char* szFilename )
{
	LoadBitmapEx ( szFilename );
}

void dbCreateBitmap ( int iID, int iWidth, int iHeight )
{
	CreateBitmap ( iID, iWidth, iHeight );
}

void dbDeleteBitmap ( int iID )
{
	DeleteBitmapEx ( iID );
}

void dbCopyBitmap ( int iID, int iToID )
{
	CopyBitmap ( iID, iToID );
}

void dbCopyBitmap ( int iID, int iX1, int iY1, int iX2, int iY2, int iToID, int iToX1, int iToY1, int iToX2, int iToY2 )
{
	CopyBitmapEx ( iID, iX1, iY1, iX2, iY2, iToID, iToX1, iToY1, iToX2, iToY2 );
}

void dbFlipBitmap ( int iID )
{
	FlipBitmap ( iID );
}

void dbMirrorBitmap ( int iID )
{
	MirrorBitmap ( iID );
}

void dbFadeBitmap ( int iID, int iFade )
{
	FadeBitmap ( iID, iFade );
}

void dbBlurBitmap ( int iID, int iBlurLevel )
{
	BlurBitmap ( iID, iBlurLevel );
}

void dbSetCurrentBitmap ( int iID )
{
	SetCurrentBitmap ( iID );
}

void dbSaveBitmap ( char* szFilename, int iID )
{
	SaveBitmap ( szFilename, iID );
}

void dbSaveBitmap ( char* szFilename )
{
	SaveBitmapEx ( szFilename );
}

int dbCurrentBitmap ( void )
{
	return CurrentBitmap ( );
}

int dbBitmapExist ( int iID )
{
	return BitmapExist ( iID );
}

int dbBitmapWidth ( int iID )
{
	return BitmapWidth ( iID );
}

int dbBitmapHeight ( int iID )
{
	return BitmapHeight ( iID );
}

int dbBitmapDepth ( int iID )
{
	return BitmapDepth ( iID );
}

int dbBitmapMirrored ( int iID )
{
	return BitmapMirrored ( iID );
}

int dbBitmapFlipped ( int iID )
{
	return BitmapFlipped ( iID );
}

int dbBitmapExist ( void )
{
	return BitmapExist ( );
}

int dbBitmapWidth ( void )
{
	return BitmapWidth ( );
}

int dbBitmapHeight ( void )
{
	return BitmapHeight ( );
}

int dbBitmapDepth ( void )
{
	return BitmapDepth ( );
}

int dbBitmapMirrored ( void )
{
	return BitmapMirrored ( );
}

int dbBitmapFlipped ( void )
{
	return BitmapFlipped ( );
}

void dbConstructorBitmap ( HINSTANCE hInstance )
{
	ConstructorBitmap ( hInstance );
}

void dbDestructorBitmap ( void )
{
	DestructorBitmap ( );
}

void dbSetErrorHandlerBitmap ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerBitmap ( pErrorHandlerPtr );
}

void dbPassCoreDataBitmap( LPVOID pGlobPtr )
{
	PassCoreDataBitmap ( pGlobPtr );
}

void dbRefreshD3DBitmap ( int iMode )
{
	RefreshD3DBitmap ( iMode );
}

void dbUpdateBitmapZeroOfNewBackbufferBitmap ( void )
{
	UpdateBitmapZeroOfNewBackbufferBitmap ( );
}

LPDIRECT3DSURFACE9 dbMakeBitmap ( int iID, int iWidth, int iHeight )
{
	return MakeBitmap ( iID, iWidth, iHeight );
}

void dbGetBitmapData ( int iID, DWORD* dwWidth, DWORD* dwHeight, DWORD* dwDepth, LPSTR* pData, DWORD* dwDataSize, bool bLockData )
{
	GetBitmapData ( iID, dwWidth, dwHeight, dwDepth, pData, dwDataSize, bLockData );
}

void dbSetBitmapData ( int bitmapindex, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, LPSTR pData, DWORD dwDataSize )
{
	SetBitmapData ( bitmapindex, dwWidth, dwHeight, dwDepth, pData, dwDataSize );
}

*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////