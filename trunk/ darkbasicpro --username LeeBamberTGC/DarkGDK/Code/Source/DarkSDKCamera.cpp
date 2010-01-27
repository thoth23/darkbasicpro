
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAMERA COMMANDS //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKCamera.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void		CreateEx					( int iID );
void		DestroyEx					( int iID );

void		dbtMoveCamera				( float fStep );
void		dbtMoveCamera				( int iID, float fStep );
void		dbtPositionCamera			( float fX, float fY, float fZ );
void		dbtPositionCamera			( int iID, float fX, float fY, float fZ );
void		dbtPointCamera				( float fX, float fY, float fZ );
void		dbtPointCamera				( int iID, float fX, float fY, float fZ );
void		dbtSetRotateCamera			( float fX, float fY, float fZ );
void		dbtSetRotateCamera			( int iID, float fX, float fY, float fZ ); 
void		SetRotationVector3			( int iID, int iVector );
void		dbtSetXRotateCamera			( float fX );
void		dbtSetXRotateCamera			( int iID, float fX ); 
void		dbtSetYRotateCamera			( float fY );
void		dbtSetYRotateCamera			( int iID, float fY ); 
void		dbtSetZRotateCamera			( float fZ );
void		dbtSetZRotateCamera			( int iID, float fZ ); 

void		TurnLeft					( float fAngle );
void		TurnLeft					( int iID, float fAngle );
void		TurnRight					( float fAngle );
void		TurnRight					( int iID, float fAngle );
void		PitchUp						( float fAngle );
void		PitchUp						( int iID, float fAngle );
void		PitchDown					( float fAngle );
void		PitchDown					( int iID, float fAngle );
void		RollLeft					( float fAngle );
void		RollLeft					( int iID, float fAngle );
void		RollRight					( float fAngle );
void		RollRight					( int iID, float fAngle );

void		SetCurrentCamera			( int iID );
void		ClearView					( DWORD dwColorValue );
void		ClearViewEx					( int iID, DWORD dwColorValue );
void		SetFOV						( float fAngle );
void		SetFOV						( int iID, float fAngle );
void		SetRange					( float fFront, float fBack );
void		SetRange					( int iID, float fFront, float fBack );
void		SetView						( int iLeft, int iTop, int iRight, int iBottom );
void		SetView						( int iID, int iLeft, int iTop, int iRight, int iBottom );
void		SetAspect					( float fAspect );
void		SetAspect					( int iID, float fAspect );
void		dbtSetRotationXYZCamera		( void );
void		dbtSetRotationXYZCamera		( int iID );
void		dbtSetRotationZYXCamera		( void );
void		dbtSetRotationZYXCamera		( int iID );

void		Follow						( float fX, float fY, float fZ, float fAngle, float fDistance, float fHeight, float fSmooth, int iCollision );
void		Follow						( int iID, float fX, float fY, float fZ, float fAngle, float fDistance, float fHeight, float fSmooth, int iCollision );
void		SetCameraToImage			( int iID, int iImage, int iWidth, int iHeight );
void		SetCameraToImageEx			( int iID, int iImage, int iWidth, int iHeight, int iGenAlpha );
void		dbtSetToObjectOrientationCamera		( int iObjectID );
void		dbtSetToObjectOrientationCamera		( int iID, int iObjectID );
void		GetPositionVector3			( int iVector, int iID );
void		GetRotationVector3			( int iVector, int iID );
void		ControlWithArrowKeys		( int iID, float fVelocity, float fTurnSpeed );

void		SetAutoCamOn				( void );
void		SetAutoCamOff				( void );

void		BackdropOn					( void );
void		BackdropOn					( int iID );
void		BackdropOff					( void );
void		BackdropOff					( int iID );
void		BackdropColor				( DWORD dwColor );
void		BackdropColor				( int iID, DWORD dwColor );
void		BackdropTexture				( int iImage );
void		BackdropTexture				( int iID, int iImage );
void		BackdropScroll				( int iU, int iV );
void		BackdropScroll				( int iID, int iU, int iV );

DWORD		dbtGetXPositionExCamera		( void );
DWORD		dbtGetXPositionExCamera		( int iID );
DWORD		dbtGetYPositionExCamera		( void );
DWORD		dbtGetYPositionExCamera		( int iID );
DWORD		dbtGetZPositionExCamera		( void );
DWORD		dbtGetZPositionExCamera		( int iID );
DWORD		dbtGetXAngleExCamera			( void );
DWORD		dbtGetXAngleExCamera		( int iID );
DWORD		dbtGetYAngleExCamera		( void );
DWORD		dbtGetYAngleExCamera		( int iID );
DWORD		dbtGetZAngleExCamera		( void );
DWORD		dbtGetZAngleExCamera		( int iID );

void		UpdateCamera				( void );
void		ConstructorCamera			( HINSTANCE hSetup, HINSTANCE hImage );
void		DestructorCamera			( void );
void		SetErrorHandlerCamera		( LPVOID pErrorHandlerPtr );
void		PassCoreDataCamera			( LPVOID pGlobPtr );
void		RefreshD3DCamera			( int iMode );
void		StartScene					( void );
int			FinishScene					( void );
int			GetRenderCamera				( void );
void		InternalUpdate				( int iID );
void*		dbtGetInternalDataCamera	( int iID );
D3DXMATRIX	GetMatrixCamera				( int iID );
void		SetCameraMatrix				( int iID, D3DXMATRIX* pMatrix );
void		ReleaseCameraMatrix			( int iID );
//void		Rotate						( int iID, float fX, float fY, float fZ );
void		Create						( int iID );
void		Destroy						( int iID );
void		XRotate						( int iID, float fX );
void		YRotate						( int iID, float fY );
void		ZRotate						( int iID, float fZ );
float		GetXPosition				( int iID );
float		GetYPosition				( int iID );
float		GetZPosition				( int iID );
float		GetXAngle					( int iID );
float		GetYAngle					( int iID );
float		GetZAngle					( int iID );
float		GetLookX					( int iID );
float		GetLookY					( int iID );
float		GetLookZ					( int iID );
void		SetAutoCam					( float fX, float fY, float fZ, float fRadius );
void		Reset						( int iID ); 
void		MoveLeft					( int iID, float fStep ); 
void		MoveRight					( int iID, float fStep ); 
void		MoveUp						( int iID, float fStep );
void		MoveDown					( int iID, float fStep );
void		SetPositionVector3			( int iID, int iVector );
void		ClearView					( int iID, int iRed, int iGreen, int iBlue ); 
DWORD		dbtGetLookXExCamera			( int iID );
DWORD		dbtGetLookYExCamera			( int iID );
DWORD		dbtGetLookZExCamera			( int iID );
*/

/*
void		ConstructorCamera			( HINSTANCE hSetup, HINSTANCE hImage );
void		DestructorCamera			( void );
void		SetErrorHandlerCamera		( LPVOID pErrorHandlerPtr );
void		PassCoreDataCamera			( LPVOID pGlobPtr );
void		RefreshD3DCamera			( int iMode );

void		StartScene				( void );
int			FinishScene				( void );
int			GetRenderCamera			( void );

void		CameraRotate			( int iID, float fX, float fY, float fZ );				// rotate on all axis
void		CameraXRotate			( int iID, float fX );									// x rotate
void		CameraYRotate			( int iID, float fY );									// y rotate
void		CameraZRotate			( int iID, float fZ );									// z rotate

float		CameraGetXPosition		( int iID );											// get x pos
float		CameraGetYPosition		( int iID );											// get y pos
float		CameraGetZPosition		( int iID );											// get z pos
float		CameraGetXAngle			( int iID );											// get x angle
float		CameraGetYAngle			( int iID );											// get y angle
float		CameraGetZAngle			( int iID );											// get z angle
float		CameraGetLookX			( int iID );											// get look x
float		CameraGetLookY			( int iID );											// get look y
float		CameraGetLookZ			( int iID );											// get look z

void*		CameraGetInternalData	( int iID );											// used to retrieve camera data
D3DXMATRIX	CameraGetMatrix			( int iID );											// used to get the camera matrix

void		SetAutoCam				( float fX, float fY, float fZ, float fRadius );
void		CameraCreate			( int iID );
void		CameraDestroy			( int iID );

// mike - 250903 - override camera
void		SetCameraMatrix			( int iID, D3DXMATRIX* pMatrix );
void		ReleaseCameraMatrix		( int iID );

//////////////////////////////////////////////////////////////////////////////////
// COMMAND FUNCTIONS //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// DBV1 Commands
void	PositionCamera					( float fX, float fY, float fZ );
void	RotateCamera					( float fX, float fY, float fZ );
void	XRotateCamera					( float fX );
void	YRotateCamera					( float fY );
void	ZRotateCamera					( float fZ );
void	PointCamera						( float fX, float fY, float fZ );
void	MoveCamera						( float fStep );
void	SetCameraRange					( float fFront, float fBack );
void	SetCameraView					( int iLeft, int iTop, int iRight, int iBottom );
void	ClearCameraView					( DWORD dwColorValue );
void	SetCameraRotationXYZ			( void );
void	SetCameraRotationZYX			( void );
void	SetCameraFOV					( float fAngle );
void	SetCameraAspect					( float fAspect );
void	SetCameraToFollow				( float fX, float fY, float fZ, float fAngle, float fDistance, float fHeight, float fSmooth, int iCollision );
void	SetAutoCamOn					( void );
void	SetAutoCamOff					( void );
void	TurnCameraLeft					( float fAngle );
void	TurnCameraRight					( float fAngle );
void	PitchCameraUp					( float fAngle );
void	PitchCameraDown					( float fAngle );
void	RollCameraLeft					( float fAngle );
void	RollCameraRight					( float fAngle );
void	SetCameraToObjectOrientation	( int iObjectID );

void	BackdropOn						( void ); 
void	BackdropOff						( void ); 
void	BackdropColor					( DWORD dwColor ); 
void	BackdropTexture					( int iImage ); 
void	BackdropScroll					( int iU, int iV ); 

// DBV1 Expressions
DWORD	CameraPositionX					( void );
DWORD	CameraPositionY					( void );
DWORD	CameraPositionZ					( void );
DWORD	CameraAngleX					( void );
DWORD	CameraAngleY					( void );
DWORD	CameraAngleZ					( void );

// DBPro Commands (new commands)
void	MakeCamera						( int iID );
void	DeleteCamera					( int iID );
void	SetCurrentCamera				( int iID );														// set the current camera
void	SetCameraToImage				( int iID, int iImage, int iWidth, int iHeight );
void	SetCameraToImage				( int iID, int iImage, int iWidth, int iHeight, int iGenAlpha );
void	ResetCamera						( int iID );														// sets all default values for camera
void	MoveCameraLeft					( int iID, float fStep );											// move in current direction
void	MoveCameraRight					( int iID, float fStep );											// move in current direction
void	MoveCameraUp					( int iID, float fStep );
void	MoveCameraDown					( int iID, float fStep );
void	ControlCameraWithArrowKeys		( int iID, float fVelocity, float fTurnSpeed );
void	PositionCamera					( int iID, int iVector );
void	SetVector3ToCameraPosition		( int iVector, int iID );
void	RotateCamera					( int iID, int iVector );
void	SetVector3ToCameraRotation		( int iVector, int iID );

// DBPro Commands (old commands with camera ID parameter)
void	PositionCamera					( int iID, float fX, float fY, float fZ );					// setup position
void	RotateCamera					( int iID, float fX, float fY, float fZ );					// rotate on all axis
void	XRotateCamera					( int iID, float fX );										// x rotate
void	YRotateCamera					( int iID, float fY );										// y rotate
void	ZRotateCamera					( int iID, float fZ );										// z rotate
void	PointCamera						( int iID, float fX, float fY, float fZ );					// point to position
void	MoveCamera						( int iID, float fStep );									// move in current direction
void	SetCameraRange					( int iID, float fFront, float fBack );						// set front and back viewing distances
void	SetCameraView					( int iID, int iLeft, int iTop, int iRight, int iBottom );	// set where display is drawn on screen
void	ClearCameraView					( int iID, DWORD dwColorValue );
void	ClearCameraView					( int iID, int iRed, int iGreen, int iBlue );				// clear camera to specified colour
void	SetCameraRotationXYZ			( int iID );												// set order of rotation to xyz
void	SetCameraRotationZYX			( int iID );												// set order of rotation to zyx
void	SetCameraFOV					( int iID, float fAngle );									// set field of view
void	SetCameraAspect					( int iID, float fAspect );									// set field of view
void	SetCameraToFollow				( int iID, float fX, float fY, float fZ, float fAngle, float fDistance, float fHeight, float fSmooth, int iCollision );	// follow object
void	TurnCameraLeft					( int iID, float fAngle );									// turn left
void	TurnCameraRight					( int iID, float fAngle );									// turn right
void	PitchCameraUp					( int iID, float fAngle );									// pitch up
void	PitchCameraDown					( int iID, float fAngle );									// pitch down
void	RollCameraLeft					( int iID, float fAngle );									// roll left
void	RollCameraRight					( int iID, float fAngle );									// roll right
void	SetCameraToObjectOrientation	( int iID, int iObjectID );									// set to same orientation as specified object

void	BackdropOn						( int iID );
void	BackdropOff						( int iID );
void	BackdropColor					( int iID, DWORD dwColor );
void	BackdropTexture					( int iID, int iImage );
void	BackdropScroll					( int iID, int iU, int iV );

// DBPro Expressions
DWORD	CameraPositionX					( int iID );												// get x pos
DWORD	CameraPositionY					( int iID );												// get y pos
DWORD	CameraPositionZ					( int iID );												// get z pos
DWORD	CameraAngleX					( int iID );												// get x angle
DWORD	CameraAngleY					( int iID );												// get y angle
DWORD	CameraAngleZ					( int iID );												// get z angle
DWORD	CameraLookX						( int iID );
DWORD	CameraLookY						( int iID );
DWORD	CameraLookZ						( int iID );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbMakeCamera ( int iID )
{
	MakeCamera ( iID );
}

void dbDeleteCamera ( int iID )
{
	DeleteCamera ( iID );
}

void dbMoveCamera ( float fStep )
{
	MoveCamera ( fStep );
}

void dbMoveCamera ( int iID, float fStep )
{
	MoveCamera ( iID, fStep );
}

void dbPositionCamera ( float fX, float fY, float fZ )
{
	PositionCamera ( fX, fY, fZ );
}

void dbPositionCamera ( int iID, float fX, float fY, float fZ )
{
	PositionCamera ( iID, fX, fY, fZ );
}

void dbPointCamera ( float fX, float fY, float fZ )
{
	PointCamera ( fX, fY, fZ );
}

void dbPointCamera ( int iID, float fX, float fY, float fZ )
{
	PointCamera ( iID, fX, fY, fZ );
}

void dbRotateCamera ( float fX, float fY, float fZ )
{
	RotateCamera ( fX, fY, fZ );
}

void dbRotateCamera ( int iID, float fX, float fY, float fZ )
{
	RotateCamera ( iID, fX, fY, fZ );
}

void dbRotateCamera ( int iID, int iVector )
{
	RotateCamera ( iID, iVector );
}

void dbXRotateCamera ( float fX )
{
	XRotateCamera ( fX );
}

void dbYRotateCamera ( int iID, float fX )
{
	YRotateCamera ( iID, fX );
}

void dbZRotateCamera ( float fY )
{
	ZRotateCamera ( fY );
}

void dbXRotateCamera ( int iID, float fY )
{
	XRotateCamera ( iID, fY );
}

void dbYRotateCamera ( float fZ )
{
	YRotateCamera ( fZ );
}

void dbZRotateCamera ( int iID, float fZ )
{
	ZRotateCamera ( iID,  fZ );
}

void dbTurnCameraLeft ( float fAngle )
{
	TurnCameraLeft ( fAngle );
}

void dbTurnCameraLeft ( int iID, float fAngle )
{
	TurnCameraLeft ( iID, fAngle );
}

void dbTurnCameraRight ( float fAngle )
{
	TurnCameraRight ( fAngle );
}

void dbTurnCameraRight ( int iID, float fAngle )
{
	TurnCameraRight ( iID, fAngle );
}

void dbPitchCameraUp ( float fAngle )
{
	PitchCameraUp ( fAngle );
}

void dbPitchCameraUp ( int iID, float fAngle )
{
	PitchCameraUp ( iID, fAngle );
}

void dbPitchCameraDown ( float fAngle )
{
	PitchCameraDown ( fAngle );
}

void dbPitchCameraDown ( int iID, float fAngle )
{
	PitchCameraDown ( iID, fAngle );
}

void dbRollCameraLeft ( float fAngle )
{
	RollCameraLeft ( fAngle );
}

void dbRollCameraLeft ( int iID, float fAngle )
{
	RollCameraLeft ( iID, fAngle );
}

void dbRollCameraRight ( float fAngle )
{
	RollCameraRight ( fAngle );
}
 
void dbRollCameraRight ( int iID, float fAngle )
{
	RollCameraRight ( iID, fAngle );
}

void dbSetCurrentCamera ( int iID )
{
	SetCurrentCamera ( iID );
}

void dbClearCameraView ( DWORD dwColorValue )
{
	ClearCameraView ( dwColorValue );
}

void dbClearCameraView ( int iID, DWORD dwColorValue )
{
	ClearCameraView ( iID, dwColorValue );
}

void dbSetCameraFOV ( float fAngle )
{
	SetCameraFOV ( fAngle );
}
 
void dbSetCameraFOV ( int iID, float fAngle )
{
	SetCameraFOV ( iID, fAngle );
}

void dbSetCameraRange ( float fFront, float fBack )
{
	SetCameraRange ( fFront, fBack );
}
 
void dbSetCameraRange ( int iID, float fFront, float fBack )
{
	SetCameraRange ( iID, fFront, fBack );
}

void dbSetCameraView ( int iLeft, int iTop, int iRight, int iBottom )
{
	SetCameraView (  iLeft,  iTop,  iRight,  iBottom );
}

void dbSetCameraView ( int iID, int iLeft, int iTop, int iRight, int iBottom )
{
	SetCameraView ( iID, iLeft, iTop, iRight, iBottom );
}

void dbSetCameraAspect ( float fAspect )
{
	SetCameraAspect ( fAspect );
}
 
void dbSetCameraAspect ( int iID, float fAspect )
{
	SetCameraAspect ( iID, fAspect );
}

void dbSetCameraRotationXYZ ( void )
{
	SetCameraRotationXYZ ( );
}

void dbSetCameraRotationXYZ ( int iID )
{
	SetCameraRotationXYZ ( iID );
}

void dbSetCameraRotationZYX ( void )
{
	SetCameraRotationZYX ( );
}
 
void dbSetCameraRotationZYX ( int iID )
{
	SetCameraRotationZYX( iID );
}

void dbSetCameraToFollow ( float fX, float fY, float fZ, float fAngle, float fDistance, float fHeight, float fSmooth, int iCollision )
{
	SetCameraToFollow ( fX, fY, fZ, fAngle, fDistance, fHeight, fSmooth, iCollision );
}

void dbSetCameraToFollow ( int iID, float fX, float fY, float fZ, float fAngle, float fDistance, float fHeight, float fSmooth, int iCollision )
{
	SetCameraToFollow ( iID, fX, fY, fZ, fAngle, fDistance, fHeight, fSmooth, iCollision );
}

void dbSetCameraToImage ( int iID, int iImage, int iWidth, int iHeight )
{
	SetCameraToImage ( iID, iImage, iWidth, iHeight );
}

void dbSetCameraToImage ( int iID, int iImage, int iWidth, int iHeight, int iGenAlpha )
{
	SetCameraToImage ( iID, iImage, iWidth, iHeight, iGenAlpha );
}

void dbSetCameraToObjectOrientation ( int iObjectID )
{
	SetCameraToObjectOrientation ( iObjectID );
}
 
void dbSetCameraToObjectOrientation ( int iID, int iObjectID )
{
	SetCameraToObjectOrientation ( iID, iObjectID );
}

void dbSetVector3ToCameraPosition ( int iVector, int iID )
{
	SetVector3ToCameraPosition ( iVector, iID );
}

void dbSetVector3ToCameraRotation ( int iVector, int iID )
{
	SetVector3ToCameraRotation ( iVector, iID );
}

void dbControlCameraUsingArrowKeys ( int iID, float fVelocity, float fTurnSpeed )
{
	ControlCameraWithArrowKeys ( iID, fVelocity, fTurnSpeed );
}

void dbAutoCamOn ( void )
{
	SetAutoCamOn ( );
}

void dbAutoCamOff ( void )
{
	SetAutoCamOff ( );
}

void dbBackdropOn ( void )
{
	BackdropOn ( );
}

void dbBackdropOn ( int iID ) 
{
	BackdropOn ( iID );
}
 
void dbBackdropOff ( void )
{
	BackdropOff ( );
}

void dbBackdropOff ( int iID ) 
{
	BackdropOff ( iID );
}
 
void dbColorBackdrop ( DWORD dwColor )
{
	BackdropColor ( dwColor );
}

void dbColorBackdrop ( int iID, DWORD dwColor ) 
{
	BackdropColor ( iID, dwColor );
}
 
void dbTextureBackdrop ( int iImage )
{
	BackdropTexture ( iImage );
}

void dbTextureBackdrop ( int iID, int iImage ) 
{
	BackdropTexture ( iID, iImage );
}
 
void dbScrollBackdrop ( int iU, int iV )
{
	BackdropScroll ( iU, iV );
}

void dbScrollBackdrop ( int iID, int iU, int iV ) 
{
	BackdropScroll ( iID, iU, iV ) ;
}

float dbCameraPositionX ( void )
{
	DWORD dwReturn = CameraPositionX ( );
	
	return *( float* ) &dwReturn;
}

float dbCameraPositionX ( int iID )
{
	DWORD dwReturn = CameraPositionX ( iID );
	
	return *( float* ) &dwReturn;
}

float dbCameraPositionY ( void )
{
	DWORD dwReturn = CameraPositionY ( );
	
	return *( float* ) &dwReturn;
}

float dbCameraPositionY ( int iID )
{
	DWORD dwReturn = CameraPositionY ( iID );
	
	return *( float* ) &dwReturn;
}

float dbCameraPositionZ ( void )
{
	DWORD dwReturn = CameraPositionZ ( );
	
	return *( float* ) &dwReturn;
}

float dbCameraPositionZ ( int iID )
{
	DWORD dwReturn = CameraPositionZ ( iID );
	
	return *( float* ) &dwReturn;
}

float dbCameraAngleX ( void )
{
	DWORD dwReturn = CameraAngleX ( );
	
	return *( float* ) &dwReturn;
}

float dbCameraAngleX ( int iID )
{
	DWORD dwReturn = CameraAngleX ( iID );
	
	return *( float* ) &dwReturn;
}

float dbCameraAngleY ( void )
{
	DWORD dwReturn = CameraAngleY ( );
	
	return *( float* ) &dwReturn;
}

float dbCameraAngleY ( int iID )
{
	DWORD dwReturn = CameraAngleY ( iID );
	
	return *( float* ) &dwReturn;
}

float dbCameraAngleZ ( void )
{
	DWORD dwReturn = CameraAngleZ ( );
	
	return *( float* ) &dwReturn;
}

float dbCameraAngleZ ( int iID )
{
	DWORD dwReturn = CameraAngleZ ( iID );
	
	return *( float* ) &dwReturn;
}

//////////////////////////

void dbUpdateCamera ( void )
{

}

void dbConstructorCamera ( HINSTANCE hSetup, HINSTANCE hImage )
{
	ConstructorCamera ( hSetup, hImage );
}

void dbDestructorCamera ( void )
{
	DestructorCamera ( );
}

void dbSetErrorHandlerCamera ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerCamera ( pErrorHandlerPtr );
}

void dbPassCoreDataCamera( LPVOID pGlobPtr )
{
	PassCoreDataCamera( pGlobPtr );
}

void dbRefreshD3DCamera ( int iMode )
{
	RefreshD3DCamera ( iMode );
}

void dbSetAutoCam ( float fX, float fY, float fZ, float fRadius )
{
	SetAutoCam ( fX, fY, fZ, fRadius );
}

void dbCameraInternalUpdate ( int iID )
{

}

void* dbCameraGetInternalData ( int iID )
{
	return CameraGetInternalData ( iID );
}


void dbStartScene ( void )
{
	StartScene ( );
}

int dbFinishScene ( void )
{
	return FinishScene ( );
}

  */

/*
int dbGetRenderCamera ( void )
{
	return GetRenderCamera ( );
}

void dbRotate ( int iID, float fX, float fY, float fZ )
{
	RotateCamera ( iID, fX, fY, fZ );
}
 
void dbXRotate ( int iID, float fX )
{
	dbXRotate ( iID, fX );
}
 
void dbYRotate ( int iID, float fY )
{
	dbYRotate ( iID, fY );
}
 
void dbZRotate ( int iID, float fZ )
{
	dbZRotate ( iID, fZ );
}

float dbGetXPosition ( int iID )
{
	return GetXPosition ( iID );
}
 
float dbGetYPosition ( int iID )
{
	return GetYPosition ( iID );
}
 
float dbGetZPosition ( int iID )
{
	return GetZPosition ( iID );
}

float dbGetXAngle ( int iID )
{
	return GetXAngle ( iID );
}
 
float dbGetYAngle ( int iID )
{
	return GetYAngle ( iID );
}
 
float dbGetZAngle ( int iID )
{
	return GetZAngle ( iID );
}
 
float dbGetLookX ( int iID )
{
	return dbtGetLookXExCamera ( iID );
}
 
float dbGetLookY ( int iID )
{
	return dbtGetLookYExCamera ( iID );
}
 
float dbGetLookZ ( int iID )
{
	return dbtGetLookZExCamera ( iID );
}


 
D3DXMATRIX dbGetMatrixCamera ( int iID )
{
	return GetMatrixCamera ( iID );
}



void dbCreate ( int iID )
{
	Create ( iID );
}

void dbDestroy ( int iID )
{
	Destroy ( iID );
}

void dbSetCameraMatrix ( int iID, D3DXMATRIX* pMatrix )
{
	SetCameraMatrix ( iID, pMatrix );
}

void dbReleaseCameraMatrix ( int iID )
{
	ReleaseCameraMatrix ( iID );
}

void dbReset ( int iID )
{
	Reset ( iID );
}
 
void dbMoveLeft ( int iID, float fStep )
{
	MoveLeft ( iID, fStep );
}
 
void dbMoveRight ( int iID, float fStep )
{
	MoveRight ( iID, fStep );
}
 
void dbMoveUp ( int iID, float fStep )
{
	MoveUp ( iID, fStep );
}

void dbMoveDown ( int iID, float fStep )
{
	MoveDown ( iID, fStep );
}

void dbSetPositionVector3 ( int iID, int iVector )
{
	SetPositionVector3 ( iID, iVector );
}

void dbClearView ( int iID, int iRed, int iGreen, int iBlue )
{
	ClearView (  iID, iRed, iGreen, iBlue );
}
 
DWORD dbGetLookXEx ( int iID )
{
	return dbtGetLookXExCamera ( iID );
}

DWORD dbGetLookYEx ( int iID )
{
	return dbtGetLookYExCamera ( iID );
}

DWORD dbGetLookZEx ( int iID )
{
	return dbtGetLookZExCamera ( iID );
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////