
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "..\Include\DarkSDK3DMaths.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void  			AddMatrix								( int iResult, int iA, int iB );
void			AddVector2								( int iResult, int iA, int iB );
void  			AddVector3								( int iResult, int iA, int iB );
void  			AddVector4								( int iResult, int iA, int iB );

void  			BuildLookAtLHMatrix						( int iResult, int iVectorEye, int iVectorAt, int iVectorUp );
void  			BuildLookAtRHMatrix						( int iResult, int iVectorEye, int iVectorAt, int iVectorUp );
void  			BuildOrthoLHMatrix						( int iResult, float fWidth, float fHeight, float fZNear, float fZFar );
void  			BuildOrthoRHMatrix						( int iResult, float fWidth, float fHeight, float fZNear, float fZFar );
void  			BuildPerspectiveFovLHMatrix				( int iResult, float fFOV, float fAspect, float fZNear, float fZFar );
void  			BuildPerspectiveFovRHMatrix				( int iResult, float fFOV, float fAspect, float fZNear, float fZFar );
void  			BuildPerspectiveLHMatrix				( int iResult, float fWidth, float fHeight, float fZNear, float fZFar );
void  			BuildPerspectiveRHMatrix				( int iResult, float fWidth, float fHeight, float fZNear, float fZFar );
void  			BuildReflectionMatrix					( int iResult, float a, float b, float c, float d );
void  			BuildRotationAxisMatrix					( int iResult, int iVectorAxis, float fAngle );

void			CatmullRomVector2						( int iResult, int iA, int iB, int iC, int iD, float s );
void  			CatmullRomVector3						( int iResult, int iA, int iB, int iC, int iD, float s );
void  			CatmullRomVector4						( int iResult, int iA, int iB, int iC, int iD, float s );

void  			CopyMatrix								( int iSource, int iDestination );
void			CopyVector2								( int iSource, int iDestination );
void  			CopyVector3								( int iSource, int iDestination );
void  			CopyVector4								( int iSource, int iDestination );
void  			CrossProductVector3						( int iResult, int iA, int iB );

SDK_BOOL		DeleteMatrix							( int iID );
SDK_BOOL		DeleteVector2							( int iID );
SDK_BOOL		DeleteVector3							( int iID );
SDK_BOOL		DeleteVector4							( int iID );

void  			DivideMatrix							( int iID, float fValue );
void			DivideVector2							( int iID, float fValue );
void  			DivideVector3							( int iID, float fValue );
void  			DivideVector4							( int iID, float fValue );
SDK_FLOAT		DotProductVector2						( int iA, int iB );
SDK_FLOAT		DotProductVector3						( int iA, int iB );
void			GetBaryCentricCoordinatesVector2		( int iResult, int iA, int iB, int iC, float f, float g );
void  			GetBaryCentricCoordinatesVector3		( int iResult, int iA, int iB, int iC, float f, float g );
void  			GetBaryCentricCoordinatesVector4		( int iResult, int iA, int iB, int iC, float f, float g );

SDK_FLOAT		GetCCWVector2							( int iA, int iB );
SDK_FLOAT		GetLengthSquaredVector2					( int iID );
SDK_FLOAT		GetLengthSquaredVector3					( int iID );
SDK_FLOAT		GetLengthSquaredVector4					( int iID );
SDK_FLOAT		GetLengthVector2						( int iID );
SDK_FLOAT		GetLengthVector3						( int iID );
SDK_FLOAT		GetLengthVector4						( int iID );

void  			GetProjectionMatrix						( int iID );
void  			GetViewMatrix							( int iID );
SDK_FLOAT		GetWVector4								( int iID );
void  			GetWorldMatrix							( int iID );

SDK_FLOAT		GetXVector2								( int iID );
SDK_FLOAT		GetXVector3								( int iID );
SDK_FLOAT		GetXVector4								( int iID );
SDK_FLOAT		GetYVector2								( int iID );
SDK_FLOAT		GetYVector3								( int iID );
SDK_FLOAT		GetYVector4								( int iID );
SDK_FLOAT		GetZVector3								( int iID );
SDK_FLOAT		GetZVector4								( int iID );

void			HermiteVector2							( int iResult, int iA, int iB, int iC, int iD, float s );
void  			HermiteVector3							( int iResult, int iA, int iB, int iC, int iD, float s );
void			HermiteVector4							( int iResult, int iA, int iB, int iC, int iD, float s );
SDK_FLOAT		InverseMatrix							( int iResult, int iSource );

SDK_BOOL		IsEqualMatrix							( int iA, int iB );
SDK_BOOL		IsEqualVector2							( int iA, int iB );
SDK_BOOL		IsEqualVector3							( int iA, int iB );
SDK_BOOL		IsEqualVector4							( int iA, int iB );
SDK_BOOL		IsIdentityMatrix						( int iID );

void 			LinearInterpolationVector2				( int iResult, int iA, int iB, float s );
void  			LinearInterpolationVector3				( int iResult, int iA, int iB, float s );
void  			LinearInterpolationVector4				( int iResult, int iA, int iB, float s );

SDK_BOOL		MakeMatrix								( int iID );
SDK_BOOL		MakeVector2								( int iID );
SDK_BOOL		MakeVector3								( int iID );
SDK_BOOL		MakeVector4								( int iID );
void  			MaximizeVector2							( int iResult, int iA, int iB );
void  			MaximizeVector3							( int iResult, int iA, int iB );
void  			MaximizeVector4							( int iResult, int iA, int iB );
void  			MinimizeVector2							( int iResult, int iA, int iB );
void  			MinimizeVector3							( int iResult, int iA, int iB );
void  			MinimizeVector4							( int iResult, int iA, int iB );

void  			MultiplyMatrix							( int iResult, int iA, int iB );
void  			MultiplyMatrix							( int iID, float fValue );
void			MultiplyVector2							( int iID, float fValue );
void  			MultiplyVector3							( int iID, float fValue );
void  			MultiplyVector4							( int iID, float fValue );
void  			NormalizeVector2						( int iResult, int iSource );
void  			NormalizeVector3						( int iResult, int iSource );
void  			NormalizeVector4						( int iResult, int iSource );
void			ProjectVector3							( int iResult, int iSource, int iProjectionMatrix, int iViewMatrix, int iWorldMatrix );

void  			RotateXMatrix							( int iID, float fAngle );
void  			RotateYMatrix							( int iID, float fAngle );
void  			RotateYawPitchRollMatrix				( int iID, float fYaw, float fPitch, float fRoll );
void  			RotateZMatrix							( int iID, float fAngle );
void  			ScaleMatrix								( int iID, float fX, float fY, float fZ );
void  			ScaleVector2							( int iResult, int iSource, float s );
void 			ScaleVector3							( int iResult, int iSource, float s );
void			ScaleVector4							( int iResult, int iSource, float s );

void  			SetIdentityMatrix						( int iID );
void			SetVector2								( int iID, float fX, float fY );
void  			SetVector3								( int iID, float fX, float fY, float fZ );
void  			SetVector4								( int iID, float fX, float fY, float fZ, float fW );
void  			SubtractMatrix							( int iResult, int iA, int iB );
void			SubtractVector2							( int iResult, int iA, int iB );
void  			SubtractVector3							( int iResult, int iA, int iB );
void  			SubtractVector4							( int iResult, int iA, int iB );

void			TransformVector4						( int iResult, int iSource, int iMatrix );
void  			TransformVectorCoordinates2				( int iResult, int iSource, int iMatrix );
void			TransformVectorCoordinates3				( int iResult, int iSource, int iMatrix );
void			TransformVectorNormalCoordinates3		( int iResult, int iSource, int iMatrix );
void  			TranslateMatrix							( int iID, float fX, float fY, float fZ );
void  			TransposeMatrix							( int iResult, int iSource );

void 			Constructor3DMaths						( HINSTANCE hSetup );
void 			Destructor3DMaths						( void );
void 			SetErrorHandler3DMaths					( LPVOID pErrorHandlerPtr );
void			PassCoreData3DMaths						( LPVOID pGlobPtr );
void			RefreshD3D3DMaths						( int iMode );

D3DXVECTOR2		GetVector2								( int iID );
D3DXVECTOR3		GetVector3								( int iID );
D3DXVECTOR4		GetVector4								( int iID );
D3DXMATRIX		GetMatrix								( int iID );
int				GetExist								( int iID );
bool			CheckTypeIsValid						( int iID, int iType );
void			CheckType								( int iID );

SDK_BOOL		IsNotEqualVector2						( int iA, int iB );
SDK_BOOL		IsNotEqualVector3						( int iA, int iB );
SDK_BOOL		IsNotEqualVector4						( int iA, int iB );
SDK_BOOL		IsNotEqualMatrix						( int iA, int iB );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbAddMatrix4 ( int iResult, int iA, int iB )
{
	AddMatrix (  iResult,  iA,  iB );
}

void dbAddVector2 ( int iResult, int iA, int iB )
{
	AddVector2 ( iResult, iA,  iB );
}

void dbAddVector3 ( int iResult, int iA, int iB )
{
	AddVector3 ( iResult, iA, iB );
}

void dbAddVector4 ( int iResult, int iA, int iB )
{
	AddVector4 ( iResult, iA, iB );
}

void dbBuildLookAtLHMatrix ( int iResult, int iVectorEye, int iVectorAt, int iVectorUp )
{
	BuildLookAtLHMatrix (  iResult,  iVectorEye,  iVectorAt,  iVectorUp );
}

void dbBuildLookAtRHMatrix ( int iResult, int iVectorEye, int iVectorAt, int iVectorUp )
{
	BuildLookAtRHMatrix (  iResult, iVectorEye, iVectorAt, iVectorUp );
}

void dbBuildOrthoLHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildOrthoLHMatrix ( iResult, fWidth, fHeight, fZNear, fZFar );
}

void dbBuildOrthoRHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildOrthoRHMatrix ( iResult,  fWidth,  fHeight, fZNear, fZFar );
}

void dbBuildFovLHMatrix ( int iResult, float fFOV, float fAspect, float fZNear, float fZFar )
{
	BuildPerspectiveFovLHMatrix (  iResult,  fFOV,  fAspect,  fZNear,  fZFar );
}

void dbBuildFovRHMatrix ( int iResult, float fFOV, float fAspect, float fZNear, float fZFar )
{
	BuildPerspectiveFovRHMatrix (  iResult,  fFOV,  fAspect,  fZNear,  fZFar );
}

void dbBuildPerspectiveLHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildPerspectiveLHMatrix (  iResult,  fWidth,  fHeight,  fZNear,  fZFar );
}

void dbBuildPerspectiveRHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildPerspectiveRHMatrix (  iResult,  fWidth,  fHeight,  fZNear,  fZFar );
}

void dbBuildReflectionMatrix ( int iResult, float a, float b, float c, float d )
{
	BuildReflectionMatrix (  iResult,  a,  b,  c,  d );
}

void dbBuildRotationAxisMatrix ( int iResult, int iVectorAxis, float fAngle )
{
	dbBuildRotationAxisMatrix (  iResult,  iVectorAxis,  fAngle );
}

void dbCatmullRomVector2 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	CatmullRomVector2 ( iResult, iA, iB, iC, iD, s );
}

void dbCatmullRomVector3 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	CatmullRomVector3 (  iResult,  iA,  iB,  iC,  iD, s );
}

void dbCatmullRomVector4 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	CatmullRomVector4 (  iResult,  iA,  iB,  iC,  iD,  s );
}

void dbCopyMatrix ( int iSource, int iDestination )
{
	CopyMatrix ( iSource, iDestination );
}

void dbCopyVector2 ( int iSource, int iDestination )
{
	CopyVector2 (  iSource, iDestination );
}

void dbCopyVector3 ( int iSource, int iDestination )
{
	CopyVector3 ( iSource, iDestination );
}

void dbCopyVector4 ( int iSource, int iDestination )
{
	CopyVector4 (  iSource, iDestination );
}

void dbCrossProductVector3 ( int iResult, int iA, int iB )
{
	CrossProductVector3 (  iResult, iA,  iB );
}

SDK_BOOL dbDeleteMatrix4 ( int iID )
{
	return DeleteMatrix ( iID );
}

SDK_BOOL dbDeleteVector2 ( int iID )
{
	return DeleteVector2 ( iID );
}

SDK_BOOL dbDeleteVector3 ( int iID )
{
	return DeleteVector3 ( iID );
}

SDK_BOOL dbDeleteVector4 ( int iID )
{
	return DeleteVector4 ( iID );
}

void dbDivideMatrix ( int iID, float fValue )
{
	DivideMatrix ( iID, fValue );
}

void dbDivideVector2 ( int iID, float fValue )
{
	DivideVector2 ( iID, fValue );
}

void dbDivideVector3 ( int iID, float fValue )
{
	DivideVector3 ( iID, fValue );
}

void dbDivideVector4 ( int iID, float fValue )
{
	DivideVector4 ( iID, fValue );
}

float dbDotProductVector2 ( int iA, int iB )
{
	DWORD dwReturn = DotProductVector2 (  iA,  iB );
	
	return *( float* ) &dwReturn;
}

float dbDotProductVector3 ( int iA, int iB )
{
	DWORD dwReturn = DotProductVector3 (  iA,  iB );
	
	return *( float* ) &dwReturn;
}

void dbGetBaryCentricCoordinatesVector2 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	GetBaryCentricCoordinatesVector2 ( iResult, iA, iB, iC, f, g );
}

void dbGetBaryCentricCoordinatesVector3 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	GetBaryCentricCoordinatesVector3 ( iResult,  iA, iB, iC, f,  g );
}

void dbGetBaryCentricCoordinatesVector4 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	GetBaryCentricCoordinatesVector4 (  iResult, iA, iB, iC, f, g );
}

float dbGetCCWVector2 ( int iA, int iB )
{
	DWORD dwReturn = GetCCWVector2 (  iA,  iB );
	
	return *( float* ) &dwReturn;
}

float dbGetLengthSquaredVector2 ( int iID )
{
	DWORD dwReturn = GetLengthSquaredVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetLengthSquaredVector3 ( int iID )
{
	DWORD dwReturn = GetLengthSquaredVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetLengthSquaredVector4 ( int iID )
{
	DWORD dwReturn = GetLengthSquaredVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetLengthVector2 ( int iID )
{
	DWORD dwReturn = GetLengthVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetLengthVector3 ( int iID )
{
	DWORD dwReturn = GetLengthVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetLengthVector4 ( int iID )
{
	DWORD dwReturn = GetLengthVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbGetProjectionMatrix ( int iID )
{
	GetProjectionMatrix (  iID );
}

void dbGetViewMatrix ( int iID )
{
	GetViewMatrix (  iID );
}

float dbGetWVector4 ( int iID )
{
	DWORD dwReturn = GetWVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbGetWorldMatrix ( int iID )
{
	GetWorldMatrix (  iID );
}

float dbGetXVector2 ( int iID )
{
	DWORD dwReturn = GetXVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetXVector3 ( int iID )
{
	DWORD dwReturn = GetXVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetXVector4 ( int iID )
{
	DWORD dwReturn = GetXVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetYVector2 ( int iID )
{
	DWORD dwReturn = GetYVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetYVector3 ( int iID )
{
	DWORD dwReturn = GetYVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetYVector4 ( int iID )
{
	DWORD dwReturn = GetYVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetZVector3 ( int iID )
{
	DWORD dwReturn = GetZVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbGetZVector4 ( int iID )
{
	DWORD dwReturn = GetZVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbHermiteVector2 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	HermiteVector2 ( iResult, iA, iB, iC, iD, s );
}

void dbHermiteVector3 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	HermiteVector3 ( iResult, iA, iB, iC, iD, s );
}

void dbHermiteVector4 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	HermiteVector4 ( iResult,  iA, iB, iC,  iD, s );
}

float dbInverseMatrix ( int iResult, int iSource )
{
	DWORD dwReturn = InverseMatrix ( iResult, iSource );
	
	return *( float* ) &dwReturn;
}

SDK_BOOL dbIsEqualMatrix ( int iA, int iB )
{
	return IsEqualMatrix ( iA, iB );
}

SDK_BOOL dbIsEqualVector2 ( int iA, int iB )
{
	return IsEqualVector2 ( iA, iB );
}

SDK_BOOL dbIsEqualVector3 ( int iA, int iB )
{
	return IsEqualVector3 ( iA, iB );
}

SDK_BOOL dbIsEqualVector4 ( int iA, int iB )
{
	return IsEqualVector4 ( iA, iB );
}

SDK_BOOL dbIsIdentityMatrix ( int iID )
{
	return IsIdentityMatrix ( iID );
}

void dbLinearInterpolationVector2 ( int iResult, int iA, int iB, float s )
{
	LinearInterpolationVector2 ( iResult, iA, iB, s );
}

void dbLinearInterpolationVector3 ( int iResult, int iA, int iB, float s )
{
	LinearInterpolationVector3 ( iResult,  iA,  iB, s );
}

void dbLinearInterpolationVector4 ( int iResult, int iA, int iB, float s )
{
	LinearInterpolationVector4 (  iResult,  iA,  iB,  s );
}

SDK_BOOL dbMakeMatrix ( int iID )
{
	return MakeMatrix ( iID );
}

SDK_BOOL dbMakeVector2 ( int iID )
{
	return MakeVector2 ( iID );
}

SDK_BOOL dbMakeVector3 ( int iID )
{
	return MakeVector3 ( iID );
}

SDK_BOOL dbMakeVector4 ( int iID )
{
	return MakeVector4 ( iID );
}

void dbMaximizeVector2 ( int iResult, int iA, int iB )
{
	MaximizeVector2 ( iResult, iA, iB );
}

void dbMaximizeVector3 ( int iResult, int iA, int iB )
{
	MaximizeVector3 ( iResult, iA, iB );
}

void dbMaximizeVector4 ( int iResult, int iA, int iB )
{
	MaximizeVector4 (  iResult,  iA, iB );
}

void dbMinimizeVector2 ( int iResult, int iA, int iB )
{
	MinimizeVector2 ( iResult, iA, iB );
}

void dbMinimizeVector3 ( int iResult, int iA, int iB )
{
	MinimizeVector3 ( iResult, iA, iB );
}

void dbMinimizeVector4 ( int iResult, int iA, int iB )
{
	MinimizeVector4 (  iResult, iA,  iB );
}

void dbMultiplyMatrix ( int iResult, int iA, int iB )
{
	MultiplyMatrix (  iResult,  iA,  iB );
}

void dbMultiplyMatrix ( int iID, float fValue )
{
	MultiplyMatrix (  iID,  fValue );
}

void dbMultiplyVector2 ( int iID, float fValue )
{
	MultiplyVector2 (  iID, fValue );
}

void dbMultiplyVector3 ( int iID, float fValue )
{
	MultiplyVector3 ( iID, fValue );
}

void dbMultiplyVector4 ( int iID, float fValue )
{
	MultiplyVector4 (  iID, fValue );
}

void dbNormalizeVector2 ( int iResult, int iSource )
{
	NormalizeVector2 ( iResult, iSource );
}

void dbNormalizeVector3 ( int iResult, int iSource )
{
	NormalizeVector3 ( iResult, iSource );
}

void dbNormalizeVector4 ( int iResult, int iSource )
{
	NormalizeVector4 ( iResult, iSource );
}

void dbProjectVector3 ( int iResult, int iSource, int iProjectionMatrix, int iViewMatrix, int iWorldMatrix )
{
	ProjectVector3 ( iResult,  iSource,  iProjectionMatrix, iViewMatrix, iWorldMatrix );
}

void dbRotateXMatrix ( int iID, float fAngle )
{
	dbRotateXMatrix (  iID,  fAngle );
}

void dbRotateYMatrix ( int iID, float fAngle )
{
	dbRotateYMatrix (  iID,  fAngle );
}

void dbRotateYawPitchRollMatrix ( int iID, float fYaw, float fPitch, float fRoll )
{
	RotateYawPitchRollMatrix (  iID,  fYaw,  fPitch,  fRoll );
}

void dbRotateZMatrix ( int iID, float fAngle )
{
	dbRotateZMatrix (  iID,  fAngle );
}

void dbScaleMatrix ( int iID, float fX, float fY, float fZ )
{
	ScaleMatrix (  iID,  fX,  fY,  fZ );
}

void dbScaleVector2 ( int iResult, int iSource, float s )
{
	ScaleVector2 ( iResult, iSource, s );
}

void dbScaleVector3 ( int iResult, int iSource, float s )
{
	ScaleVector3 ( iResult, iSource, s );
}

void dbScaleVector4 ( int iResult, int iSource, float s )
{
	ScaleVector4 (  iResult,  iSource,  s );
}

void dbSetIdentityMatrix ( int iID )
{
	SetIdentityMatrix ( iID );
}

void dbSetVector2 ( int iID, float fX, float fY )
{
	SetVector2 (  iID,  fX, fY );
}

void dbSetVector3 ( int iID, float fX, float fY, float fZ )
{
	SetVector3 ( iID, fX, fY, fZ );
}

void dbSetVector4 ( int iID, float fX, float fY, float fZ, float fW )
{
	SetVector4 (  iID, fX, fY, fZ, fW );
}

void dbSubtractMatrix ( int iResult, int iA, int iB )
{
	SubtractMatrix (  iResult,  iA, iB );
}

void dbSubtractVector2 ( int iResult, int iA, int iB )
{
	SubtractVector2 (  iResult,  iA, iB );
}

void dbSubtractVector3 ( int iResult, int iA, int iB )
{
	SubtractVector3 ( iResult, iA,  iB );
}

void dbSubtractVector4 ( int iResult, int iA, int iB )
{
	SubtractVector4 ( iResult,  iA,  iB );
}

void dbTransformVector4 ( int iResult, int iSource, int iMatrix )
{
	TransformVector4 ( iResult, iSource, iMatrix );
}

void dbTransformVectorCoordinates2 ( int iResult, int iSource, int iMatrix )
{
	TransformVectorCoordinates2 ( iResult, iSource, iMatrix );
}

void dbTransformVectorCoordinates3 ( int iResult, int iSource, int iMatrix )
{
	TransformVectorCoordinates3 ( iResult, iSource,  iMatrix );
}

void dbTransformVectorNormalCoordinates3 ( int iResult, int iSource, int iMatrix )
{
	TransformVectorNormalCoordinates3 ( iResult, iSource, iMatrix );
}

void dbTranslateMatrix ( int iID, float fX, float fY, float fZ )
{
	TranslateMatrix (  iID,  fX,  fY,  fZ );
}

void dbTransposeMatrix ( int iResult, int iSource )
{
	TransposeMatrix (  iResult,  iSource );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbConstructor3DMaths ( HINSTANCE hSetup )
{
	Constructor3DMaths ( hSetup );
}

void dbDestructor3DMaths ( void )
{
	Destructor3DMaths ( );
}

void dbSetErrorHandler3DMaths ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandler3DMaths ( pErrorHandlerPtr );
}

void dbPassCoreData3DMaths ( LPVOID pGlobPtr )
{
	PassCoreData3DMaths ( pGlobPtr );
}

void dbRefreshD3D3DMaths ( int iMode )
{
	RefreshD3D3DMaths ( iMode );
}

D3DXVECTOR2 dbGetVector2 ( int iID )
{
	return GetVector2 ( iID );
}

D3DXVECTOR3 dbGetVector3 ( int iID )
{
	return GetVector3 (  iID );
}

D3DXVECTOR4 dbGetVector4 ( int iID )
{
	return GetVector4 ( iID );
}

D3DXMATRIX dbGetMatrix ( int iID )
{
	return GetMatrix ( iID );
}

int dbGetExist ( int iID )
{
	return GetExist ( iID );
}

bool dbCheckTypeIsValid ( int iID, int iType )
{
	return CheckTypeIsValid ( iID, iType );
}

void dbCheckType ( int iID )
{
	CheckType ( iID );
}

SDK_BOOL dbIsNotEqualVector2 ( int iA, int iB )
{
	return IsNotEqualVector2 ( iA, iB );
}

SDK_BOOL dbIsNotEqualVector3 ( int iA, int iB )
{
	return IsNotEqualVector3 ( iA, iB );
}

SDK_BOOL dbIsNotEqualVector4 ( int iA, int iB )
{
	return IsNotEqualVector4 ( iA, iB );
}

SDK_BOOL dbIsNotEqualMatrix ( int iA, int iB )
{
	return IsNotEqualMatrix ( iA, iB );
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////