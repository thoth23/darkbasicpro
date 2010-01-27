
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT COMMANDS ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKInput.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	ConstructorInput				( HINSTANCE );
void	DestructorInput					( void );
void	SetErrorHandlerInput			( LPVOID pErrorHandlerPtr );
void	PassCoreDataInput				( LPVOID pGlobPtr );
void	RefreshD3DInput					( int iMode );

char	InKey							( void );
void	ClearData						( void );

void	ClearEntryBuffer				( void );
DWORD	GetEntry						( DWORD pDestStr );

int		UpKey							( void );
int		DownKey							( void );
int		LeftKey							( void );
int		RightKey						( void );
int		ControlKey						( void );
int		ShiftKey						( void );
int		SpaceKey						( void );
int		EscapeKey						( void );
int		ReturnKey						( void );
int		KeyState						( int iKey );
int		ScanCode						( void );

void	WriteToClipboard				( LPSTR );
void	WriteToRegistry					( LPSTR, LPSTR, int );
DWORD	GetClipboard					( DWORD );
int		GetRegistry						( LPSTR, LPSTR );
void	WriteToRegistryS				( LPSTR pfolder, LPSTR valuekey, DWORD pString );
DWORD	GetRegistryS					( DWORD pDestStr, LPSTR pfolder, LPSTR valuekey );

void	HideMouse						( void );
void	ShowMouse						( void );
void	PositionMouse					( int, int );
void	ChangeMouse						( int );
int		GetMouseX						( void );
int		GetMouseY						( void );
int		GetMouseZ						( void );
int		GetMouseClick					( void );
int		GetMouseMoveX					( void );
int		GetMouseMoveY					( void );
int		GetMouseMoveZ					( void );

int		JoystickUp						( void );
int		JoystickDown					( void );
int		JoystickLeft					( void );
int		JoystickRight					( void );
int		JoystickX						( void );
int		JoystickY						( void );
int		JoystickZ						( void );
int		JoystickFireA					( void );
int		JoystickFireB					( void );
int		JoystickFireC					( void );
int		JoystickFireD					( void );
int		JoystickFireXL					( int iButton );
int		JoystickSliderA					( void );
int		JoystickSliderB					( void );
int		JoystickSliderC					( void );
int		JoystickSliderD					( void );
int		JoystickTwistX					( void );
int		JoystickTwistY					( void );
int		JoystickTwistZ					( void );
int		JoystickHatAngle				( int iHatID );

void	ForceUp							( int iMagnitude );
void	ForceDown						( int iMagnitude );
void	ForceLeft						( int iMagnitude );
void	ForceRight						( int iMagnitude );
void	ForceAngle						( int iMagnitude, int iAngle, int iDuration );
void	ForceChainsaw					( int iMagnitude, int iDuration );
void	ForceShoot						( int iMagnitude, int iDuration );
void	ForceImpact						( int iMagnitude, int iDuration );
void	ForceNoEffect					( void );
void	ForceWaterEffect				( int iMagnitude, int iDuration );
void	ForceAutoCenterOn				( void );
void	ForceAutoCenterOff				( void );

void	PerformChecklistControlDevices	( void );
void	SetControlDevice				( DWORD pName );
DWORD	GetControlDevice				( DWORD pDestStr );
int		ControlDeviceX					( void );
int		ControlDeviceY					( void );
int		ControlDeviceZ					( void );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbShowMouse ( void )
{
	ShowMouse ( );
}

void dbHideMouse ( void )
{
	HideMouse ( );
}

void dbPositionMouse ( int iX, int iY )
{
	PositionMouse ( iX, iY );
}

void dbChangeMouse ( int iCursorID )
{
	ChangeMouse ( iCursorID );
}

void dbClearEntryBuffer ( void )
{
	ClearEntryBuffer ( );
}

void dbWriteToClipboard ( LPSTR pString )
{
	WriteToClipboard ( pString );
}

void dbWriteToRegistry ( LPSTR pfolder, LPSTR valuekey, int iValue )
{
	WriteToRegistry ( pfolder, valuekey, iValue );
}

void dbWriteToRegistryS ( LPSTR pfolder, LPSTR valuekey, DWORD pString )
{
	WriteToRegistryS ( pfolder, valuekey, pString );
}

void dbPerformChecklistControlDevices( void )
{
	PerformChecklistControlDevices ( );
}

void dbSetControlDevice ( char* pName )
{
	SetControlDevice ( ( DWORD ) pName );
}

void dbForceUp ( int iMagnitude )
{
	ForceUp ( iMagnitude );
}

void dbForceDown ( int iMagnitude )
{
	ForceDown ( iMagnitude );
}

void dbForceLeft ( int iMagnitude )
{
	ForceLeft ( iMagnitude );
}

void dbForceRight ( int iMagnitude )
{
	ForceRight ( iMagnitude );
}

void dbForceAngle ( int iMagnitude, int iAngle, int iDuration )
{
	ForceAngle ( iMagnitude, iAngle, iDuration );
}

void dbForceNoEffect ( void )
{
	ForceNoEffect ( );
}

void dbForceWaterEffect ( int iMagnitude, int iDuration )
{
	ForceWaterEffect ( iMagnitude, iDuration );
}

void dbForceShoot ( int iMagnitude, int iDuration )
{
	ForceShoot ( iMagnitude, iDuration );
}

void dbForceChainsaw ( int iMagnitude, int iDuration )
{
	ForceChainsaw ( iMagnitude, iDuration );
}

void dbForceImpact ( int iMagnitude, int iDuration )
{
	ForceImpact ( iMagnitude, iDuration );
}

void dbForceAutoCenterOn ( void )
{
	ForceAutoCenterOn ( );
}

void dbForceAutoCenterOff ( void )
{
	ForceAutoCenterOff ( );
}

int dbMouseClick ( void )
{
	return GetMouseClick ( );
}

int dbMouseMoveX ( void )
{
	return GetMouseMoveX ( );
}

int dbMouseMoveY ( void )
{
	return GetMouseMoveY ( );
}

int dbMouseMoveZ ( void )
{
	return GetMouseMoveZ ( );
}

int dbMouseX ( void )
{
	return GetMouseX ( );
}

int dbMouseY ( void )
{
	return GetMouseY ( );
}

int dbMouseZ ( void )
{
	return GetMouseZ ( );
}

int dbUpKey ( void )
{
	return UpKey ( );
}

int dbDownKey ( void )
{
	return DownKey ( );
}

int dbLeftKey ( void )
{
	return LeftKey ( );
}

int dbRightKey ( void )
{
	return RightKey ( );
}

int dbControlKey ( void )
{
	return ControlKey ( );
}

int dbShiftKey ( void )
{
	return ShiftKey ( );
}

int dbReturnKey ( void )
{
	return ReturnKey ( );
}

int dbEscapeKey ( void )
{
	return EscapeKey ( );
}

int dbSpaceKey ( void )
{
	return SpaceKey ( );
}

int dbScanCode ( void )
{
	return ScanCode ( );
}

int dbKeyState ( int iKey )
{
	return KeyState ( iKey );
}

char* dbGetEntry ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetEntry ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbGetClipboard ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetClipboard ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbGetRegistry ( LPSTR pfolder, LPSTR valuekey )
{
	return GetRegistry ( pfolder, valuekey );
}

char* dbGetRegistryS ( LPSTR pfolder, LPSTR valuekey )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetRegistryS ( NULL, pfolder, valuekey );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbJoystickUp ( void )
{
	return JoystickUp ( );
}

int dbJoystickDown ( void )
{
	return JoystickDown ( );
}

int dbJoystickLeft ( void )
{
	return JoystickLeft ( );
}

int dbJoystickRight ( void )
{
	return JoystickRight ( );
}

int dbJoystickX ( void )
{
	return JoystickX ( );
}

int dbJoystickY ( void )
{
	return JoystickY ( );
}

int dbJoystickZ ( void )
{
	return JoystickZ ( );
}

int dbJoystickFireA ( void )
{
	return JoystickFireA ( );
}

int dbJoystickFireB ( void )
{
	return JoystickFireB ( );
}

int dbJoystickFireC ( void )
{
	return JoystickFireC ( );
}

int dbJoystickFireD ( void )
{
	return JoystickFireD ( );
}

int dbJoystickFireXL ( int iButton )
{
	return JoystickFireXL ( iButton );
}

int dbJoystickSliderA ( void )
{
	return JoystickSliderA ( );
}

int dbJoystickSliderB ( void )
{
	return JoystickSliderB ( );
}

int dbJoystickSliderC ( void )
{
	return JoystickSliderC ( );
}

int dbJoystickSliderD ( void )
{
	return JoystickSliderD ( );
}

int dbJoystickTwistX ( void )
{
	return JoystickTwistX ( );
}

int dbJoystickTwistY ( void )
{
	return JoystickTwistY ( );
}

int dbJoystickTwistZ ( void )
{
	return JoystickTwistZ ( );
}

int dbJoystickHatAngle ( int iHatID )
{
	return JoystickHatAngle ( iHatID );
}

char* dbGetControlDevice ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetControlDevice ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbControlDeviceX ( void )
{
	return ControlDeviceX ( );
}

int dbControlDeviceY ( void )
{
	return ControlDeviceY ( );
}

int dbControlDeviceZ ( void )
{
	return ControlDeviceZ ( );
}

void dbConstructorInput ( HINSTANCE hInstance )
{
	ConstructorInput ( hInstance );
}
void dbDestructorInput ( void )
{
	DestructorInput ( );
}

void dbSetErrorHandlerInput ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerInput ( pErrorHandlerPtr );
}

void dbPassCoreDataInput( LPVOID pGlobPtr )
{
	PassCoreDataInput ( pGlobPtr );
}

void dbRefreshD3DInput ( int iMode )
{
	RefreshD3DInput ( iMode );
}

char dbInKeyInput ( void )
{
	return InKey ( );
}

void dbClearDataInput ( void )
{
	ClearData ( );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
*/