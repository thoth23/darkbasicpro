
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKWorld.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	LoadEx								( SDK_LPSTR szFilename, SDK_LPSTR szMap );
void	DeleteEx							( void );
void	SetupCullingCameraEx				( int iID );
void	SetupCameraCollision				( int iID, int iEntityID, float fRadius, int iResponse );
void	SetupObjectCollision				( int iID, int iEntityID, float fRadius, int iResponse );
void	SetCollisionThreshholdEx			( int iID, float fSensitivity );
void	SetupCollisionOffEx					( int iID );
void	SetCameraCollisionRadiusEx			( int iID, int iEntityID, float fX, float fY, float fZ );
void	SetObjectCollisionRadiusEx			( int iID, int iEntityID, float fX, float fY, float fZ );
void	SetCollisionHeightAdjustmentEx		( int iID, float fHeight );
void	SetHardwareMultiTexturingOnEx		( void );
void	SetHardwareMultiTexturingOffEx		( void );
void	ProcessCollisionEx					( int iID );

int		GetCollisionResponseEx				( int iID );
DWORD	GetCollisionXEx						( int iID );
DWORD	GetCollisionYEx						( int iID );
DWORD	GetCollisionZEx						( int iID );

void	ConstructorWorld					( HINSTANCE hSetup, HINSTANCE hImage, HINSTANCE hCamera, HINSTANCE hObject );
void	DestructorWorld						( void );
void	SetErrorHandlerWorld				( LPVOID pErrorHandlerPtr );
void	PassCoreDataWorld					( LPVOID pGlobPtr );
void	RefreshD3DWorld						( int iMode );

void	UpdateBSP							( void );
void	Delete								( void );
void	StartBSP							( void );
void	EndBSP								( void );
void	EndWorld							( void );
bool	Load								( char* szFilename, char* szMap );
void	SetupCullingCamera					( int iID );
void	UpdateEx							( void );
void	StartEx								( void );
void	EndEx								( void );
void	SetupCameraCollisionEx				( int iID, int iEntityID, float fRadius, int iResponse );
void	SetupObjectCollisionEx				( int iID, int iEntityID, float fRadius, int iResponse );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbUpdateBSP ( void )
{
	UpdateBSP ( );
}

void dbStartBSP ( void )
{
	StartBSP ( );
}

void dbEndBSP ( void )
{
	EndBSP ( );
}

void dbLoadBSP ( char* szFilename, char* szMap )
{
	LoadEx ( ( DWORD ) szFilename, ( DWORD ) szMap );
}

void dbDeleteBSP ( void )
{
	DeleteEx ( );
}

void dbSetBSPCamera ( int iID )
{
	SetupCullingCameraEx ( iID );
}

void dbSetBSPCameraCollision ( int iID, int iEntityID, float fRadius, int iResponse )
{
	SetupCameraCollision (  iID,  iEntityID,  fRadius,  iResponse );
}

void dbSetBSPObjectCollision ( int iID, int iEntityID, float fRadius, int iResponse )
{
	SetupObjectCollision (  iID, iEntityID,  fRadius,  iResponse );
}

void dbSetBSPCollisionThreshhold ( int iID, float fSensitivity )
{
	SetCollisionThreshholdEx ( iID, fSensitivity );
}

void dbSetBSPCollisionOff ( int iID )
{
	SetupCollisionOffEx ( iID );
}

void dbSetBSPCameraCollisionRadius ( int iID, int iEntityID, float fX, float fY, float fZ )
{
	SetCameraCollisionRadiusEx ( iID, iEntityID,  fX,  fY,  fZ );
}

void dbSetBSPObjectCollisionRadius ( int iID, int iEntityID, float fX, float fY, float fZ )
{
	SetObjectCollisionRadiusEx (  iID,  iEntityID, fX, fY, fZ );
}

void dbSetBSPCollisionHeightAdjustment ( int iID, float fHeight )
{
	SetCollisionHeightAdjustmentEx	( iID, fHeight );
}

void dbSetBSPMultiTexturingOn ( void )
{
	SetHardwareMultiTexturingOnEx ( );
}

void dbSetBSPMultiTexturingOff ( void )
{
	SetHardwareMultiTexturingOffEx ( );
}

void dbProcessBSPCollision ( int iID )
{
	ProcessCollisionEx ( iID );
}

int dbBSPCollisionHit ( int iID )
{
	return GetCollisionResponseEx ( iID );
}

float dbBSPCollisionX ( int iID )
{
	DWORD dwReturn = GetCollisionXEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbBSPCollisionY ( int iID )
{
	DWORD dwReturn = GetCollisionYEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbBSPCollisionZ ( int iID )
{
	DWORD dwReturn = GetCollisionZEx ( iID );
	
	return *( float* ) &dwReturn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbConstructorWorld ( HINSTANCE hSetup, HINSTANCE hImage, HINSTANCE hCamera, HINSTANCE hObject )
{
	ConstructorWorld (  hSetup, hImage, hCamera, hObject );
}

void dbDestructorWorld ( void )
{
	DestructorWorld ( );
}

void dbSetErrorHandlerWorld ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerWorld ( pErrorHandlerPtr );
}

void dbPassCoreDataWorld( LPVOID pGlobPtr )
{
	PassCoreDataWorld ( pGlobPtr );
}

void dbRefreshD3DWorld ( int iMode )
{
	RefreshD3DWorld ( iMode );
}

/*
void dbUpdate ( void )
{
	Update ( );
}

void dbDelete ( void )
{
	Delete ( );
}

void dbStart ( void )
{
	Start ( );
}

void dbEndWorld ( void )
{
	End ( );
}

bool dbLoad ( char* szFilename, char* szMap )
{
	return Load ( szFilename, szMap );
}

void dbSetupCullingCamera ( int iID )
{
	SetupCullingCamera (  iID );
}

void dbUpdateEx ( void )
{
	UpdateEx ( );
}

void dbStartEx ( void )
{
	StartEx ( );
}

void dbEndEx ( void )
{
	EndEx ( );
}

void dbSetupCameraCollisionEx ( int iID, int iEntityID, float fRadius, int iResponse )
{
	SetupCameraCollisionEx ( iID, iEntityID, fRadius, iResponse );
}

void dbSetupObjectCollisionEx ( int iID, int iEntityID, float fRadius, int iResponse )
{
	SetupObjectCollisionEx ( iID,  iEntityID,  fRadius, iResponse );
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////