
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BASIC 2D COMMANDS /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKBasic2D.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	ConstructorBasic2D			( HINSTANCE );
void	DestructorBasic2D			( void );
void	SetErrorHandlerBasic2D		( LPVOID pErrorHandlerPtr );
void	PassCoreDataBasic2D			( LPVOID pGlobPtr );
void	RefreshD3DBasic2D			( int iMode );
void	UpdateBPPBasic2D			( void );

void	Basic2DClear				( void );
void	Basic2DClear				( int iRed, int iGreen, int iBlue );
void	Basic2DInk					( int iRedF, int iGreenF, int iBlueF, int iRedB, int iGreenB, int iBlueB );
bool	Basic2DGetLockable			( void );

void	Basic2DClearRgb				( DWORD RGBBackColor );
void	Basic2DInk					( DWORD RGBForeColor, DWORD RGBBackColor );
void	Basic2DDot					( int iX, int iY );
void	Basic2DBox					( int iLeft, int iTop, int iRight, int iBottom );
void	Basic2DLine					( int iXa, int iYa, int iXb, int iYb );
void	Basic2DCircle				( int iX, int iY, int iRadius );
void	Basic2DElipse				( int iX, int iY, int iXRadius, int iYRadius );
DWORD	Basic2DRgb					( int iRed, int iGreen, int iBlue );
int		Basic2DRgbR					( DWORD iRGB );
int		Basic2DRgbG					( DWORD iRGB );
int		Basic2DRgbB					( DWORD iRGB );
DWORD	Basic2DGetPoint				( int iX, int iY );

void	Basic2DDot					( int iX, int iY, DWORD dwColor );
void	Basic2DBoxGradient			( int iLeft, int iTop, int iRight, int iBottom, DWORD dw1, DWORD dw2, DWORD dw3, DWORD dw4 );
void	Basic2DLock					( void );
void	Basic2DUnlock				( void );
DWORD	Basic2DGetPixelPtr			( void );
DWORD	Basic2DGetPixelPitch		( void );

void	Basic2DCopyArea				( int iDestX, int iDestY, int iWidth, int iHeight, int iSourceX, int iSourceY );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbInk ( DWORD RGBForeColor, DWORD RGBBackColor )
{
	Basic2DInk ( RGBForeColor, RGBBackColor );
}

void dbCLS ( DWORD RGBBackColor )
{
	Basic2DClearRgb ( RGBBackColor );
}

void dbDot ( int iX, int iY )
{
	Basic2DDot ( iX, iY );
}

void dbDot ( int iX, int iY, DWORD dwColor )
{
	Basic2DDot ( iX, iY, dwColor );
}

void dbLine ( int iXa, int iYa, int iXb, int iYb )
{
	Basic2DLine ( iXa, iYa, iXb, iYb );
}

void dbBox ( int iLeft, int iTop, int iRight, int iBottom )
{
	Basic2DBox ( iLeft, iTop, iRight, iBottom );
}

void dbBox ( int iLeft, int iTop, int iRight, int iBottom, DWORD dw1, DWORD dw2, DWORD dw3, DWORD dw4 )
{
	Basic2DBoxGradient ( iLeft, iTop, iRight, iBottom, dw1, dw2, dw3, dw4 );
}

void dbCircle ( int iX, int iY, int iRadius )
{
	Basic2DCircle ( iX, iY, iRadius );
}

void dbElipse ( int iX, int iY, int iXRadius, int iYRadius )
{
	Basic2DElipse ( iX, iY, iXRadius, iYRadius );
}

void dbLockPixels ( void )
{
	Basic2DLock ( );
}

void dbUnLockPixels ( void )
{
	Basic2DUnlock ( );
}

DWORD dbRGB ( int iRed, int iGreen, int iBlue )
{
	return Basic2DRgb ( iRed, iGreen, iBlue );
}

int dbRGBR ( DWORD iRGB )
{
	return Basic2DRgbR ( iRGB );
}

int dbRGBG ( DWORD iRGB )
{
	return Basic2DRgbG ( iRGB );
}

int dbRGBB ( DWORD iRGB )
{
	return Basic2DRgbB ( iRGB );
}

DWORD dbPoint ( int iX, int iY )
{
	return Basic2DGetPoint ( iX, iY );
}

DWORD dbGetPixelsPointer ( void )
{
	return Basic2DGetPixelPtr ( );
}

DWORD dbGetPixelsPitch ( void )
{
	return Basic2DGetPixelPitch ( );
}

void dbConstructorBasic2D ( HINSTANCE hInstance )
{
	ConstructorBasic2D ( hInstance );
}

void dbDestructorBasic2D ( void )
{
	DestructorBasic2D ( );
}

void dbSetErrorHandlerBasic2D ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerBasic2D ( pErrorHandlerPtr );
}

void dbPassCoreDataBasic2D ( LPVOID pGlobPtr )
{
	PassCoreDataBasic2D ( pGlobPtr );
}

void dbRefreshD3DBasic2D ( int iMode )
{
	RefreshD3DBasic2D ( iMode );
}

void dbUpdateBPPBasic2D ( void )
{
	UpdateBPPBasic2D ( );
}

void dbClear ( void )
{
	Basic2DClear ( );
}

void dbClear ( int iRed, int iGreen, int iBlue )
{
	Basic2DClear ( iRed, iGreen, iBlue );
}

void dbInk ( int iRedF, int iGreenF, int iBlueF, int iRedB, int iGreenB, int iBlueB )
{
	Basic2DInk ( iRedF, iGreenF, iBlueF, iRedB, iGreenB, iBlueB );
}

bool dbGetLockable ( void )
{
	return Basic2DGetLockable ( );
}

void dbCopyArea ( int iDestX, int iDestY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	Basic2DCopyArea ( iDestX, iDestY, iWidth, iHeight, iSourceX, iSourceY );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
*/