
//////////////////////////////////////////////////////////////////////////////////
// INCLUDES / LIBS ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#include "cVectorC.h"
#include ".\..\error\cerror.h"
#include ".\..\core\globstruct.h"
#include ".\..\camera\ccameradatac.h"

#ifdef DARKSDK_COMPILE
	#include ".\..\..\..\DarkGDK\Code\Include\DarkSDKDisplay.h"
	#include ".\..\..\..\DarkGDK\Code\Include\DarkSDKCamera.h"
#endif

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////
// DEFINES ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#define TYPE_VECTOR2	1
#define TYPE_VECTOR3	2
#define TYPE_VECTOR4	3
#define TYPE_MATRIX		4

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////
// GLOBALS ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

typedef tagCameraData*		( *CAMERA3D_GetInternalDataPFN )	( int );
typedef IDirect3DDevice9*	( *GFX_GetDirect3DDevicePFN )		( void );

DBPRO_GLOBAL GlobStruct*					g_pGlob						= NULL;
DBPRO_GLOBAL bool							g_bRTE						= false;
DBPRO_GLOBAL CAMERA3D_GetInternalDataPFN	g_Camera3D_GetInternalData	= NULL;
DBPRO_GLOBAL cDataManager					m_DataManager;
DBPRO_GLOBAL cDataManager					m_DataTypeManager;
DBPRO_GLOBAL LPDIRECT3DDEVICE9				m_pD3D						= NULL;
DBPRO_GLOBAL HINSTANCE						g_GFX;
DBPRO_GLOBAL GFX_GetDirect3DDevicePFN		g_GFX_GetDirect3DDevice;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////
// FUNCTION LISTINGS /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

DARKSDK void Constructor ( HINSTANCE hSetup )
{
	#ifndef DARKSDK_COMPILE
	if ( !hSetup )
	{
		hSetup = LoadLibrary ( "DBProSetupDebug.dll" );		// load the setup library
	}
	#endif

	#ifndef DARKSDK_COMPILE
		g_GFX_GetDirect3DDevice = ( GFX_GetDirect3DDevicePFN ) GetProcAddress ( hSetup, "?GetDirect3DDevice@@YAPAUIDirect3DDevice9@@XZ" );
	#else
		g_GFX_GetDirect3DDevice = dbGetDirect3DDevice;
	#endif
	
	m_pD3D = g_GFX_GetDirect3DDevice ( );
}

DARKSDK void Destructor ( void )
{
	
}

DARKSDK void SetErrorHandler ( LPVOID pErrorHandlerPtr )
{
	// Update error handler pointer
	g_pErrorHandler = (CRuntimeErrorHandler*)pErrorHandlerPtr;
}

DARKSDK void PassCoreData( LPVOID pGlobPtr )
{
	// Held in Core, used here..
	g_pGlob = (GlobStruct*)pGlobPtr;
	g_bRTE = true;

	// Get camera data function ptr (for view and projection matrix commands)
	#ifndef DARKSDK_COMPILE
		g_Camera3D_GetInternalData = ( CAMERA3D_GetInternalDataPFN )	GetProcAddress ( g_pGlob->g_Camera3D, "?GetInternalData@@YAPAXH@Z" );
	#else
		g_Camera3D_GetInternalData = ( CAMERA3D_GetInternalDataPFN ) dbGetCameraInternalData;
	#endif
}

DARKSDK void RefreshD3D ( int iMode )
{
	if(iMode==0)
	{
		// Remove all traces of old D3D usage
		Destructor();
	}
	if(iMode==1)
	{
		// Get new D3D and recreate everything D3D related
		Constructor ( g_pGlob->g_GFX );
		PassCoreData ( g_pGlob );
	}
}

DARKSDK D3DXVECTOR2 GetVector2 ( int iID )
{
	// returns a vector

	// setup a vector pointer
	D3DXVECTOR2* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return D3DXVECTOR2 ( 0.0f, 0.0f );
	}

	// see if the vector exists, if it doesn't return a null vector
	if ( ! ( pVector = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetVector2 - vector does not exist" );
		return D3DXVECTOR2 ( 0.0f, 0.0f );
	}

	// return the value of the vector
	return *pVector;
}

DARKSDK D3DXVECTOR3 GetVector3 ( int iID )
{
	// returns a vector

	// setup a vector pointer
	D3DXVECTOR3* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
	}

	// see if the vector exists, if it doesn't return a null vector
	if ( ! ( pVector = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetVector3 - vector does not exist" );
		return D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
	}

	// return the value of the vector
	return *pVector;
}

DARKSDK D3DXVECTOR4 GetVector4 ( int iID )
{
	// returns a vector

	// setup a vector pointer
	D3DXVECTOR4* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return D3DXVECTOR4 ( 0.0f, 0.0f, 0.0f, 0.0f );
	}

	// see if the vector exists, if it doesn't return a null vector
	if ( ! ( pVector = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetVector4 - vector does not exist" );
		return D3DXVECTOR4 ( 0.0f, 0.0f, 0.0f, 0.0f );
	}

	// return the value of the vector
	return *pVector;
}

DARKSDK D3DXMATRIX GetMatrix ( int iID )
{
	// returns a matrix

	D3DXMATRIX	matNull;			// null matrix ( as in contains nothing )
	D3DXMATRIX* pMatrix = NULL;		// pointer to matrix data

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return matNull;
	}

	// clear out the null matrix
	memset ( &matNull, 0, sizeof ( matNull ) );

	// check if the matrix exists, if it doesn't return the null matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetMatrix - vector does not exist" );
		return matNull;
	}

	// return the data for the matrix
	return *pMatrix;
}

DARKSDK int GetExist ( int iID )
{
	if ( m_DataManager.GetData ( iID )==NULL )
		return 0;
	else
		return 1;
}

DARKSDK bool CheckTypeIsValid ( int iID, int iType )
{
	// mike - 220406 - new function to determine if the type
	//				 - matches the operation e.g. you cannot
	//				 - do a vector3 operation on a vector2 type

	int* piType = NULL;
	piType = ( int* ) m_DataTypeManager.GetData ( iID );

	if ( piType )
	{
		if ( *piType != iType )
		{
			return false;
		}
	}

	return true;
}

DARKSDK void CheckType ( int iID )
{
	// mike - 220406 - checks the vector and removes it
	//				 - if it exists

	void* pVoid = m_DataManager.GetData ( iID );

	if ( pVoid )
	{
		m_DataTypeManager.Delete ( iID );
		m_DataManager.Delete ( iID );
	}
}

DARKSDK void AddVector ( int iID, int iType )
{
	int* piType = new int;
	*piType = iType;	
	m_DataTypeManager.Add ( piType, iID );
}

DARKSDK SDK_BOOL MakeVector2 ( int iID )
{
	// create a vector

	// pointer to vector data
	D3DXVECTOR2* pVector = NULL;
	
	// mike - 220406 - remove vector if needed
	CheckType ( iID );

	// create a vector and set all components to 0
	if ( ! ( pVector = new D3DXVECTOR2 ( 0.0f, 0.0f ) ) )
	{
		// return an error if it fails
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "Failed to create vector" );
		return false;
	}
	
	// mike - 220406 - add vector2 type into list
	AddVector ( iID, TYPE_VECTOR2 );

	// add the vector to the linked list
	m_DataManager.Add ( pVector, iID );

	// return ok
	return true;
}

DARKSDK SDK_BOOL DeleteVector2 ( int iID )
{
	// deletes a given vector

	// mike - 190107 - must also delete from data type manager list
	m_DataTypeManager.Delete ( iID );

	m_DataManager.Delete ( iID );

	// return ok
	return true;
}

DARKSDK void SetVector2 ( int iID, float fX, float fY )
{
	// sets the values of a vector

	// pointer to vector data
	D3DXVECTOR2* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the vector data
	if ( ! ( pVector = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SetVector2 - matrix does not exist" );
		return;
	}

	// now set the values
	*pVector = D3DXVECTOR2 ( fX, fY );
}

DARKSDK void CopyVector2 ( int iDestination, int iSource  )
{
	// copy the source vector to the destination vector

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iDestination, TYPE_VECTOR2 ) || !CheckTypeIsValid ( iSource, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// pointers to vector data
	D3DXVECTOR2* pVecSource      = NULL;
	D3DXVECTOR2* pVecDestination = NULL;

	// get the source vector
	if ( ! ( pVecSource = ( D3DXVECTOR2* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CopyVector2 - source vector does not exist" );
		return;
	}

	// get the destination vector
	if ( ! ( pVecDestination = ( D3DXVECTOR2* ) m_DataManager.GetData ( iDestination ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CopyVector2 - destination vector does not exist" );
		return;
	}

	// mike - 220406
	pVecDestination->x = pVecSource->x;
	pVecDestination->y = pVecSource->y;
}

DARKSDK void AddVector2 ( int iResult, int iA, int iB )
{
	// adds 2 vectors and stores the result in the first vector

	// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// pointers to vector data
	D3DXVECTOR2* pVecResult = NULL;
	D3DXVECTOR2* pVecA      = NULL;
	D3DXVECTOR2* pVecB      = NULL;

	// get the resulting vector
	if ( ! ( pVecResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pVecA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pVecB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector2 - vector b does not exist" );
		return;
	}

	// finally add vectors together
	*pVecResult = *pVecA + *pVecB;
}

DARKSDK void SubtractVector2 ( int iResult, int iA, int iB )
{
	// adds 2 vectors and stores the result in the first vector

	// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// pointers to vector data
	D3DXVECTOR2* pVecResult = NULL;
	D3DXVECTOR2* pVecA      = NULL;
	D3DXVECTOR2* pVecB      = NULL;

	// get the resulting vector
	if ( ! ( pVecResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pVecA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pVecB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector2 - vector b does not exist" );
		return;
	}

	// subtract the vectors
	*pVecResult = *pVecA - *pVecB;
}

DARKSDK void MultiplyVector2 ( int iID, float fValue )
{
	// multiples a vector by a float

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// pointer to vector data
	D3DXVECTOR2* pID = NULL;
	
	// get the vector
	if ( ! ( pID = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MultiplyVector2 - vector does not exist" );
		return;
	}

	// multiply the vector
	*pID *= fValue;
}

DARKSDK void DivideVector2 ( int iID, float fValue )
{
	// divides a vector by a float

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// pointer to vector data
	D3DXVECTOR2* pID = NULL;
	
	// get the vector
	if ( ! ( pID = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "DivideVector2 - vector does not exist" );
		return;
	}

	// divide the vector
	*pID /= fValue;
}

DARKSDK SDK_BOOL IsEqualVector2 ( int iA, int iB )
{
	// returns true if the vectors are the same
	
	// pointers to vector data
	D3DXVECTOR2* pA = NULL;
	D3DXVECTOR2* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR2 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsEqualVector2 - vector a does not exist" );
		return false;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsEqualVector2 - vector b does not exist" );
		return false;
	}

	// if both vectors are the same then return true
	if ( *pA == *pB )
		return true;

	// otherwise return false
	return false;
}

DARKSDK SDK_BOOL IsNotEqualVector2 ( int iA, int iB )
{
	// returns true if the vectors are different

	// pointers to vector data
	D3DXVECTOR2* pA = NULL;
	D3DXVECTOR2* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR2 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsNotEqualVector2 - vector a does not exist" );
		return false;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsNotEqualVector2 - vector b does not exist" );
		return false;
	}

	// if both vectors are different then return true
	if ( *pA != *pB )
		return true;

	// otherwise return false
	return false;
}

DARKSDK SDK_FLOAT GetXVector2 ( int iID )
{
	// returns the x component of a vector

	// mike - 220406 - check operation is valid on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError(RUNTIMEERROR_VECTORNOTEXIST);
		return (DWORD)0.0f;
	}

	// pointer to vector data
	D3DXVECTOR2* pVector = NULL;

	// get the data, if it doesn't exist return 0
	if ( ! ( pVector = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetXVector2 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the x value
	return SDK_RETFLOAT(pVector->x);
}

DARKSDK SDK_FLOAT GetYVector2 ( int iID )
{
	// returns the x component of a vector

	// mike - 220406 - check operation is valid on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError(RUNTIMEERROR_VECTORNOTEXIST);
		return (DWORD)0.0f;
	}

	// pointer to vector data
	D3DXVECTOR2* pVector = NULL;

	// get the data, if it doesn't exist return 0
	if ( ! ( pVector = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetYVector2 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the y value
	return SDK_RETFLOAT(pVector->y);
}

DARKSDK void GetBaryCentricCoordinatesVector2 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	// returns a point in barycentric coordinates

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pA      = NULL;
	D3DXVECTOR2* pB      = NULL;
	D3DXVECTOR2* pC      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iC,      TYPE_VECTOR2 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector2 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR2* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector2 - vector c does not exist" );
		return;
	}

	// now perform the operation
	D3DXVec2BaryCentric ( pResult, pA, pB, pC, f, g ); 
}

DARKSDK void CatmullRomVector2 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	// performs a catmull rom interpolation on a vector

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pA      = NULL;
	D3DXVECTOR2* pB      = NULL;
	D3DXVECTOR2* pC      = NULL;
	D3DXVECTOR2* pD      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iC,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iD,      TYPE_VECTOR2 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector2 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR2* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector2 - vector c does not exist" );
		return;
	}

	// get vector d
	if ( ! ( pD = ( D3DXVECTOR2* ) m_DataManager.GetData ( iD ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector2 - vector d does not exist" );
		return;
	}

	// perform the operation
	D3DXVec2CatmullRom ( pResult, pA, pB, pC, pD, s ); 
}

DARKSDK SDK_FLOAT GetCCWVector2 ( int iA, int iB )
{
	// returns the z component by taking the cross product of both vectors

	// pointers to vector data
	D3DXVECTOR2* pA = NULL;
	D3DXVECTOR2* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR2 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetCCWVector2 - vector a does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetCCWVector2 - vector b does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	float result = D3DXVec2CCW ( pA, pB );
	return SDK_RETFLOAT(result);
}

DARKSDK SDK_FLOAT DotProductVector2 ( int iA, int iB )
{
	// returns the dot product of 2 vectors

	// pointers to vector data
	D3DXVECTOR2* pA = NULL;
	D3DXVECTOR2* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR2 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "DotProductVector2 - vector a does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "DotProductVector2 - vector b does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// get the dot product
	float result = D3DXVec2Dot ( pA, pB );
	return SDK_RETFLOAT(result);
}

DARKSDK void HermiteVector2 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	// performs a hermite spline interpolation

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pA      = NULL;
	D3DXVECTOR2* pB      = NULL;
	D3DXVECTOR2* pC      = NULL;
	D3DXVECTOR2* pD      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iC,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iD,      TYPE_VECTOR2 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector2 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR2* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector2 - vector c does not exist" );
		return;
	}

	// get vector d
	if ( ! ( pD = ( D3DXVECTOR2* ) m_DataManager.GetData ( iD ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector2 - vector d does not exist" );
		return;
	}

	// get the hermite vector
	D3DXVec2Hermite ( pResult, pA, pB, pC, pD, s ); 
}

DARKSDK SDK_FLOAT GetLengthVector2 ( int iID )
{
	// get the length of a vector

	// pointer to vector data
	D3DXVECTOR2* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)0.0f;
	}
	
	// get the vector data
	if ( ! ( pID = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetLengthVector2 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}
	
	// return the length
	float result = D3DXVec2Length ( pID );
	return SDK_RETFLOAT(result);
}

DARKSDK SDK_FLOAT GetLengthSquaredVector2 ( int iID )
{
	// get the squared length of a vector

	// pointer to the vector data
	D3DXVECTOR2* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}
	
	// get the vector data
	if ( ! ( pID = ( D3DXVECTOR2* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetLengthSquaredVector2 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}
	
	// return the squared length
	float result = D3DXVec2LengthSq ( pID );
	return SDK_RETFLOAT(result);
}

DARKSDK void LinearInterpolationVector2 ( int iResult, int iA, int iB, float s )
{
	// perform a linear interpolation between 2 vectors

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pA      = NULL;
	D3DXVECTOR2* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector2 - vector b does not exist" );
		return;
	}

	// get the linear interpolation
	D3DXVec2Lerp ( pResult, pA, pB, s ); 
}

DARKSDK void MaximizeVector2 ( int iResult, int iA, int iB )
{
	// create a vector that is made up of the largest components of 2 other vectors

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pA      = NULL;
	D3DXVECTOR2* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector2 - vector b does not exist" );
		return;
	}

	// perform the operation
	D3DXVec2Maximize ( pResult, pA, pB ); 
}

DARKSDK void MinimizeVector2 ( int iResult, int iA, int iB )
{
	// create a vector that is made up of the smallest components of 2 other vectors

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pA      = NULL;
	D3DXVECTOR2* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iA,      TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iB,      TYPE_VECTOR2 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector2 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR2* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector2 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR2* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector2 - vector b does not exist" );
		return;
	}

	// perform the operation
	D3DXVec2Minimize ( pResult, pA, pB ); 
}

DARKSDK void NormalizeVector2 ( int iResult, int iSource )
{
	// get the normalized vector

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pSource = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) || !CheckTypeIsValid ( iSource, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "NormalizeVector2 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR2* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "NormalizeVector2 - source vector does not exist" );
		return;
	}

	// perform the operation
	D3DXVec2Normalize ( pResult, pSource ); 
}

DARKSDK void ScaleVector2 ( int iResult, int iSource, float s )
{
	// scales a vector

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pSource = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) || !CheckTypeIsValid ( iSource, TYPE_VECTOR2 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ScaleVector2 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR2* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ScaleVector2 - source vector does not exist" );
		return;
	}

	// perform the operation
	D3DXVec2Scale ( pResult, pSource, s ); 
}

DARKSDK void TransformVectorCoordinates2 ( int iResult, int iSource, int iMatrix )
{
	// transforms a vector

	// pointers to vector data
	D3DXVECTOR2* pResult = NULL;
	D3DXVECTOR2* pSource = NULL;
	D3DXMATRIX*  pMatrix = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR2 ) ||
			!CheckTypeIsValid ( iMatrix, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR2* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates2 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR2* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates2 - source vector does not exist" );
		return;
	}

	// get the matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iMatrix ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates2 - matrix does not exist" );
		return;
	}

	// transform the vector by the matrix
	D3DXVec2TransformCoord ( pResult, pSource, pMatrix );
}

DARKSDK SDK_FLOAT GetXVector3 ( int iID )
{
	// returns the x component of a vector

	// pointer to vector data
	D3DXVECTOR3* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get the data, if it doesn't exist return 0
	if ( ! ( pVector = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetXVector3 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the x value
	return SDK_RETFLOAT(pVector->x);
}

DARKSDK SDK_FLOAT GetYVector3 ( int iID )
{
	// returns the y component of a vector

	// pointer to vector data
	D3DXVECTOR3* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get the data, if it doesn't exist then return 0
	if ( ! ( pVector = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetYVector3 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// return the y value
	return SDK_RETFLOAT(pVector->y);
}

DARKSDK SDK_FLOAT GetZVector3 ( int iID )
{
	// returns the z component of a vector

	// pointer to vector data
	D3DXVECTOR3* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get the data, if it doesn't exist then return 0
	if ( ! ( pVector = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetZVector3 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the z value
	return SDK_RETFLOAT(pVector->z);
}

DARKSDK void GetWorldMatrix ( int iID )
{
	// copy the world matrix into the specified matrix

	// pointer to matrix data
	D3DXMATRIX* pMatrix = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetWorldMatrix - matrix does not exist" );
		return;
	}

	// world starts with identity
	D3DXMatrixIdentity ( pMatrix );
}

DARKSDK void GetViewMatrix ( int iID )
{
	// copy the view matrix into the specified matrix

	// pointer to matrix data
	D3DXMATRIX* pMatrix = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetViewMatrix - matrix does not exist" );
		return;
	}

	// get the view transformation matrix from current camera
	if ( g_Camera3D_GetInternalData )
	{
		// get camera pointer
		// leefix - 020308 - use current camera not just camera zero
		// tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData ( 0 );
		tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData ( g_pGlob->dwCurrentSetCameraID );
		*pMatrix = m_Camera_Ptr->matView;
	}
}

DARKSDK void GetProjectionMatrix ( int iID )
{
	// copy the projection matrix into the specified matrix

	// pointer to matrix data
	D3DXMATRIX* pMatrix = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetProjectionMatrix - matrix does not exist" );
		return;
	}

	// get the projection matrix from current camera
	if ( g_Camera3D_GetInternalData )
	{
		// get camera pointer
		// leefix - 020308 - use current camera not just camera zero
		// tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData ( 0 );
		tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData ( g_pGlob->dwCurrentSetCameraID );
		*pMatrix = m_Camera_Ptr->matProjection;
	}
}

DARKSDK SDK_BOOL MakeVector3 ( int iID )
{
	// create a vector

	// pointer to vector data
	D3DXVECTOR3* pVector = NULL;

	// mike - 220406 - remove vector if needed
	CheckType ( iID );

	// create a vector and set all components to 0
	if ( ! ( pVector = new D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f ) ) )
	{
		// return an error if it fails
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "Failed to create vector" );
		return false;
	}

	// mike - 220406 - add vector3 type into list
	AddVector ( iID, TYPE_VECTOR3 );

	// add the vector to the linked list
	m_DataManager.Add ( pVector, iID );

	// return ok
	return true;
}

DARKSDK SDK_BOOL DeleteVector3 ( int iID )
{
	// deletes a given vector

	// mike - 190107 - must also delete from data type manager list
	m_DataTypeManager.Delete ( iID );

	m_DataManager.Delete ( iID );

	// return ok
	return true;
}

DARKSDK void SetVector3 ( int iID, float fX, float fY, float fZ )
{
	// sets the values of a vector

	// pointer to vector data
	D3DXVECTOR3* pVector = NULL;
	
	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the vector data
	if ( ! ( pVector = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SetVector3 - matrix does not exist" );
		return;
	}

	// now set the values
	*pVector = D3DXVECTOR3 ( fX, fY, fZ );
}

DARKSDK void CopyVector3 ( int iDestination, int iSource )
{
	// mike - 220406 - parameters need to be swapped around

	// copy the source vector to the destination vector

	// pointers to vector data
	D3DXVECTOR3* pVecSource      = NULL;
	D3DXVECTOR3* pVecDestination = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iDestination, TYPE_VECTOR3 ) || !CheckTypeIsValid ( iSource, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the source vector
	if ( ! ( pVecSource = ( D3DXVECTOR3* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CopyVector3 - source vector does not exist" );
		return;
	}

	// get the destination vector
	// mike - 220406 - changed to destination ID
	if ( ! ( pVecDestination = ( D3DXVECTOR3* ) m_DataManager.GetData ( iDestination ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CopyVector3 - destination vector does not exist" );
		return;
	}

	// final assignment
	//*pVecDestination = *pVecSource;

	// mike - 220406
	pVecDestination->x = pVecSource->x;
	pVecDestination->y = pVecSource->y;
	pVecDestination->z = pVecSource->z;
}

DARKSDK void AddVector3 ( int iResult, int iA, int iB )
{
	// adds 2 vectors and stores the result in the first vector

	// pointers to vector data
	D3DXVECTOR3* pVecResult = NULL;
	D3DXVECTOR3* pVecA      = NULL;
	D3DXVECTOR3* pVecB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the resulting vector
	if ( ! ( pVecResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pVecA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pVecB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector3 - vector b does not exist" );
		return;
	}

	// finally add vectors together
	*pVecResult = *pVecA + *pVecB;
}

DARKSDK void SubtractVector3 ( int iResult, int iA, int iB )
{
	// adds 2 vectors and stores the result in the first vector

	// pointers to vector data
	D3DXVECTOR3* pVecResult = NULL;
	D3DXVECTOR3* pVecA      = NULL;
	D3DXVECTOR3* pVecB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the resulting vector
	if ( ! ( pVecResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pVecA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pVecB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector3 - vector b does not exist" );
		return;
	}

	// subtract the vectors
	*pVecResult = *pVecA - *pVecB;
}

DARKSDK void MultiplyVector3 ( int iID, float fValue )
{
	// multiples a vector by a float

	// pointer to vector data
	D3DXVECTOR3* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the vector
	if ( ! ( pID = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MultiplyVector3 - vector does not exist" );
		return;
	}

	// multiply the vector
	*pID *= fValue;
}

DARKSDK void DivideVector3 ( int iID, float fValue )
{
	// divides a vector by a float

	// pointer to vector data
	D3DXVECTOR3* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the vector
	if ( ! ( pID = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "DivideVector3 - vector does not exist" );
		return;
	}

	// divide the vector
	*pID /= fValue;
}

DARKSDK SDK_BOOL IsEqualVector3 ( int iA, int iB )
{
	// returns true if the vectors are the same
	
	// pointers to vector data
	D3DXVECTOR3* pA = NULL;
	D3DXVECTOR3* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR3 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsEqualVector3 - vector a does not exist" );
		return false;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsEqualVector3 - vector b does not exist" );
		return false;
	}

	// if both vectors are the same then return true
	if ( *pA == *pB )
		return true;

	// otherwise return false
	return false;
}

DARKSDK SDK_BOOL IsNotEqualVector3 ( int iA, int iB )
{
	// returns true if the vectors are different

	// pointers to vector data
	D3DXVECTOR3* pA = NULL;
	D3DXVECTOR3* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR3 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsNotEqualVector3 - vector a does not exist" );
		return false;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsNotEqualVector3 - vector b does not exist" );
		return false;
	}

	// if both vectors are different then return true
	if ( *pA != *pB )
		return true;

	// otherwise return false
	return false;
}

DARKSDK void GetBaryCentricCoordinatesVector3 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	// returns a point in barycentric coordinates

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pA      = NULL;
	D3DXVECTOR3* pB      = NULL;
	D3DXVECTOR3* pC      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iC, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector3 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR3* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector3 - vector c does not exist" );
		return;
	}

	// now perform the operation
	D3DXVec3BaryCentric ( pResult, pA, pB, pC, f, g ); 
}

DARKSDK void ProjectVector3 ( int iResult, int iSource, int iProjectionMatrix, int iViewMatrix, int iWorldMatrix )
{
	// projects a given vector from object space into screen space

	// pointers to vector data
	D3DXVECTOR3*	pResult		 = NULL;
	D3DXVECTOR3*	pSource      = NULL;
	D3DXMATRIX*		pProjection  = NULL;
	D3DXMATRIX*		pView        = NULL;
	D3DXMATRIX*		pWorld       = NULL;
	D3DVIEWPORT9	viewport;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iProjectionMatrix, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iViewMatrix, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iWorldMatrix, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ProjectVector3 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR3* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ProjectVector3 - source vector does not exist" );
		return;
	}

	// get the projection matrix
	if ( ! ( pProjection = ( D3DXMATRIX* ) m_DataManager.GetData ( iProjectionMatrix ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ProjectVector3 - projection matrix does not exist" );
		return;
	}

	// get the view matrix
	if ( ! ( pView = ( D3DXMATRIX* ) m_DataManager.GetData ( iViewMatrix ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ProjectVector3 - view matrix does not exist" );
		return;
	}

	// get the world matrix
	if ( ! ( pWorld = ( D3DXMATRIX* ) m_DataManager.GetData ( iWorldMatrix ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ProjectVector3 - world matrix does not exist" );
		return;
	}

	// now we can get the viewport
	m_pD3D->GetViewport ( &viewport );

	// and finally perform the operation
	D3DXVec3Project ( pResult, pSource, &viewport, pProjection, pView, pWorld ); 
}

DARKSDK void TransformVectorCoordinates3 ( int iResult, int iSource, int iMatrix )
{
	/*
	{
		D3DXMATRIX matA;
		D3DXMATRIX matB;
		D3DXMATRIX matC;

		D3DXMatrixRotationYawPitchRoll ( &matA, 180.0f, 180.0f, 180.0f );
		D3DXMatrixRotationYawPitchRoll ( &matB, 90.0f, 90.0f, 90.0f );

		matC = matA + matB;

		D3DXVECTOR3 vecA = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
		D3DXVECTOR3 vecB = D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );

		D3DXVec3TransformCoord ( &vecA, &vecB, &matC );

		int c = 0;
	}
	*/

	// transforms a vector

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pSource = NULL;
	D3DXMATRIX*  pMatrix = NULL;

	// mike - 220405 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iMatrix, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates3 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR3* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates3 - source vector does not exist" );
		return;
	}

	// get the matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iMatrix ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates3 - matrix does not exist" );
		return;
	}

	// transform the vector by the matrix
	D3DXVec3TransformCoord ( pResult, pSource, pMatrix );
}

DARKSDK void TransformVectorNormalCoordinates3 ( int iResult, int iSource, int iMatrix )
{
	// transforms a normal vector

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pSource = NULL;
	D3DXMATRIX*  pMatrix = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iMatrix, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorNormalCoordinates3 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR3* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorNormalCoordinates3 - source vector does not exist" );
		return;
	}

	// get the matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iMatrix ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorNormalCoordinates3 - matrix does not exist" );
		return;
	}

	// transform the normal
	D3DXVec3TransformNormal ( pResult, pSource, pMatrix );
}

DARKSDK void CatmullRomVector3 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	// performs a catmull rom interpolation on a vector

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pA      = NULL;
	D3DXVECTOR3* pB      = NULL;
	D3DXVECTOR3* pC      = NULL;
	D3DXVECTOR3* pD      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iC, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iD, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector3 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR3* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector3 - vector c does not exist" );
		return;
	}

	// get vector d
	if ( ! ( pD = ( D3DXVECTOR3* ) m_DataManager.GetData ( iD ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector3 - vector d does not exist" );
		return;
	}

	// perform the operation
	D3DXVec3CatmullRom ( pResult, pA, pB, pC, pD, s ); 
}

DARKSDK void CrossProductVector3 ( int iResult, int iA, int iB )
{
	// gets the cross product of 2 vectors

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pA      = NULL;
	D3DXVECTOR3* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 )
	)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CrossProductVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CrossProductVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CrossProductVector3 - vector b does not exist" );
		return;
	}

	// cross product on 2 vectors
	D3DXVec3Cross ( pResult, pA, pB );
}

DARKSDK SDK_FLOAT DotProductVector3 ( int iA, int iB )
{
	// returns the dot product of 2 vectors

	// pointers to vector data
	D3DXVECTOR3* pA = NULL;
	D3DXVECTOR3* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 )
	)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "DotProductVector3 - vector a does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "DotProductVector3 - vector b does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// get the dot product
	float result = D3DXVec3Dot ( pA, pB );
	return SDK_RETFLOAT(result);
}

DARKSDK void HermiteVector3 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	// performs a hermite spline interpolation

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pA      = NULL;
	D3DXVECTOR3* pB      = NULL;
	D3DXVECTOR3* pC      = NULL;
	D3DXVECTOR3* pD      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iC, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iD, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector3 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR3* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector3 - vector c does not exist" );
		return;
	}

	// get vector d
	if ( ! ( pD = ( D3DXVECTOR3* ) m_DataManager.GetData ( iD ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector3 - vector d does not exist" );
		return;
	}

	// get the hermite vector
	D3DXVec3Hermite ( pResult, pA, pB, pC, pD, s ); 
}

DARKSDK SDK_FLOAT GetLengthVector3 ( int iID )
{
	// get the length of a vector

	// pointer to vector data
	D3DXVECTOR3* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}
	
	// get the vector data
	if ( ! ( pID = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetLengthVector3 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}
	
	// return the length
	float result = D3DXVec3Length ( pID );
	return SDK_RETFLOAT(result);
}

DARKSDK SDK_FLOAT GetLengthSquaredVector3 ( int iID )
{
	// get the squared length of a vector

	// pointer to the vector data
	D3DXVECTOR3* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR3 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}
	
	// get the vector data
	if ( ! ( pID = ( D3DXVECTOR3* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetLengthSquaredVector3 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}
	
	// return the squared length
	float result = D3DXVec3LengthSq ( pID );
	return SDK_RETFLOAT(result);
}

DARKSDK void LinearInterpolationVector3 ( int iResult, int iA, int iB, float s )
{
	// perform a linear interpolation between 2 vectors

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pA      = NULL;
	D3DXVECTOR3* pB      = NULL;
	
	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector3 - vector b does not exist" );
		return;
	}

	// get the linear interpolation
	D3DXVec3Lerp ( pResult, pA, pB, s ); 
}

DARKSDK void MaximizeVector3 ( int iResult, int iA, int iB )
{
	// create a vector that is made up of the largest components of 2 other vectors

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pA      = NULL;
	D3DXVECTOR3* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector3 - vector b does not exist" );
		return;
	}

	// perform the operation
	D3DXVec3Maximize ( pResult, pA, pB ); 
}

DARKSDK void MinimizeVector3 ( int iResult, int iA, int iB )
{
	// create a vector that is made up of the smallest components of 2 other vectors

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pA      = NULL;
	D3DXVECTOR3* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector3 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR3* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector3 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR3* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector3 - vector b does not exist" );
		return;
	}

	// perform the operation
	D3DXVec3Minimize ( pResult, pA, pB ); 
}

DARKSDK void NormalizeVector3 ( int iResult, int iSource )
{
	// get the normalized vector

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pSource = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "NormalizeVector3 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR3* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "NormalizeVector3 - source vector does not exist" );
		return;
	}

	// perform the operation
	D3DXVec3Normalize ( pResult, pSource ); 
}

DARKSDK void ScaleVector3 ( int iResult, int iSource, float s )
{
	// scales a vector

	// pointers to vector data
	D3DXVECTOR3* pResult = NULL;
	D3DXVECTOR3* pSource = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR3* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ScaleVector3 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR3* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ScaleVector3 - source vector does not exist" );
		return;
	}

	// perform the operation
	D3DXVec3Scale ( pResult, pSource, s ); 
}

DARKSDK SDK_BOOL MakeMatrix ( int iID )
{
	// creates a matrix

	// pointer to matrix data
	D3DXMATRIX* pMatrix = NULL;
	
	//// delete any existing item
	//pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iID );
	//if(pMatrix) DeleteMatrix ( iID );

	// mike - 220406 - remove vector if needed
	CheckType ( iID );

	// create a matrix
	if ( ! ( pMatrix = new D3DXMATRIX ) )
	{
		// return an error if it fails
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "Failed to create matrix" );
		return false;
	}

	// clear the memory out
	memset ( pMatrix, 0, sizeof ( D3DXMATRIX ) );

	// mike - 220406 - add vector2 type into list
	AddVector ( iID, TYPE_MATRIX );

	// add the matrix to the linked list
	m_DataManager.Add ( pMatrix, iID );

	return true;
}

DARKSDK SDK_BOOL DeleteMatrix ( int iID )
{
	// mike - 190107 - must also delete from data type manager list
	m_DataTypeManager.Delete ( iID );

	// deletes a matrix
	m_DataManager.Delete ( iID );

	return SDK_TRUE;
}

DARKSDK void CopyMatrix ( int iSource, int iDestination )
{
	// copies 1 matrix to another

	// pointers to matrix data
	D3DXMATRIX* pSource      = NULL;
	D3DXMATRIX* pDestination = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iSource, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iDestination, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pSource = ( D3DXMATRIX* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "CopyMatrix - source matrix does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pDestination = ( D3DXMATRIX* ) m_DataManager.GetData ( iDestination ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "CopyMatrix - destination matrix does not exist" );
		return;
	}

	// assign matrices
	*pDestination = *pSource;
}

DARKSDK void AddMatrix ( int iResult, int iA, int iB )
{
	// add 2 matrices to another

	// pointers to matrix data
	D3DXMATRIX* pResult = NULL;
	D3DXMATRIX* pA      = NULL;
	D3DXMATRIX* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iA, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iB, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "AddMatrix - result matrix does not exist" );
		return;
	}

	// get matrix a
	if ( ! ( pA = ( D3DXMATRIX* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "AddMatrix - matrix a does not exist" );
		return;
	}

	// get matrix b
	if ( ! ( pB = ( D3DXMATRIX* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "AddMatrix - matrix b does not exist" );
		return;
	}

	// add them together
	*pResult = *pA + *pB;
}

DARKSDK void SubtractMatrix ( int iResult, int iA, int iB )
{
	// subtract 2 matrices

	// pointers to matrix data
	D3DXMATRIX* pResult = NULL;
	D3DXMATRIX* pA      = NULL;
	D3DXMATRIX* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iA, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iB, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "SubtractMatrix - result matrix does not exist" );
		return;
	}

	// get matrix a
	if ( ! ( pA = ( D3DXMATRIX* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "SubtractMatrix - matrix a does not exist" );
		return;
	}

	// get matrix b
	if ( ! ( pB = ( D3DXMATRIX* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "SubtractMatrix - matrix b does not exist" );
		return;
	}

	// subtract matrices
	*pResult = *pA - *pB;
}

DARKSDK void MultiplyMatrix ( int iResult, int iA, int iB )
{
	// multiply 2 matrices

	// pointers to matrix data
	D3DXMATRIX* pResult = NULL;
	D3DXMATRIX* pA      = NULL;
	D3DXMATRIX* pB      = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iA, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iB, TYPE_MATRIX )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "MultiplyMatrix - result matrix does not exist" );
		return;
	}

	// get matrix a
	if ( ! ( pA = ( D3DXMATRIX* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "MultiplyMatrix - matrix a does not exist" );
		return;
	}

	// get matrix b
	if ( ! ( pB = ( D3DXMATRIX* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "MultiplyMatrix - matrix b does not exist" );
		return;
	}

	// multiply matrices
	*pResult = *pA * *pB;
}

DARKSDK void MultiplyMatrix ( int iID, float fValue )
{
	// multiply a matrix by a float

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "MultiplyMatrix - matrix does not exist" );
		return;
	}

	// multiply the matrix
	*pID *= fValue;
}

DARKSDK void DivideMatrix ( int iID, float fValue )
{
	// divide a matrix by a float

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;
	
	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "DivideMatrix - matrix does not exist" );
		return;
	}

	// divide the matrix
	*pID /= fValue;
}

DARKSDK SDK_BOOL IsEqualMatrix ( int iA, int iB )
{
	// returns true if both matrices are the same

	// pointers to matrix data
	D3DXMATRIX* pA = NULL;
	D3DXMATRIX* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_MATRIX ) || !CheckTypeIsValid ( iB, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}
	

	// get matrix a
	if ( ! ( pA = ( D3DXMATRIX* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "IsEqualMatrix - matrix a does not exist" );
		return false;
	}

	// get matrix b
	if ( ! ( pB = ( D3DXMATRIX* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "IsEqualMatrix - matrix b does not exist" );
		return false;
	}

	// compare them and return true if they are equal
	if ( *pA == *pB )
		return true;

	// otherwise return false;
	return false;
}

DARKSDK SDK_BOOL IsNotEqualMatrix ( int iA, int iB )
{
	// returns true if both matrices are different

	// pointers to matrix data
	D3DXMATRIX* pA = NULL;
	D3DXMATRIX* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_MATRIX ) || !CheckTypeIsValid ( iB, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}

	// get matrix a
	if ( ! ( pA = ( D3DXMATRIX* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "IsNotEqualMatrix - matrix a does not exist" );
		return false;
	}

	// get matrix b
	if ( ! ( pB = ( D3DXMATRIX* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "IsNotEqualMatrix - matrix b does not exist" );
		return false;
	}

	// compare them and return true if they are different
	if ( *pA != *pB )
		return true;

	// otherwise return false;
	return false;
}

DARKSDK void SetIdentityMatrix ( int iID )
{
	// sets an identity matrix

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX )  )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "SetIdentityMatrix - matrix does not exist" );
		return;
	}

	// setup the matrix
	D3DXMatrixIdentity ( pID );
}

DARKSDK SDK_FLOAT InverseMatrix ( int iResult, int iSource )
{
	// get the inverse of a matrix

	// pointers to matrix data and determinant info
	D3DXMATRIX* pResult = NULL;
	D3DXMATRIX* pSource = NULL;
	float		fDeterminant;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iResult, TYPE_MATRIX ) || !CheckTypeIsValid ( iSource, TYPE_MATRIX ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}
	
	// get result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "InverseMatrix - result matrix does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// get source matrix
	if ( ! ( pSource = ( D3DXMATRIX* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "InverseMatrix - source matrix does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// get the inverse
	D3DXMatrixInverse ( pResult, &fDeterminant, pSource ); 

	// return the determinant
	return SDK_RETFLOAT(fDeterminant);
}

DARKSDK SDK_BOOL IsIdentityMatrix ( int iID )
{
	// returns true if the matrix is an identity matrix

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_MATRIX )  )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}
	
	// get matrix data
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "IsIdentityMatrix - matrix does not exist" );
		return false;
	}

	// check the matrix
	if ( D3DXMatrixIsIdentity ( pID ) )
		return true;

	// otherwise return false
	return false;
}

DARKSDK void BuildLookAtRHMatrix ( int iResult, int iVectorEye, int iVectorAt, int iVectorUp )
{
	// builds a look at matrix using right handed rule

	// pointers to matrix and vector data
	D3DXMATRIX*	 pResult = NULL;
	D3DXVECTOR3* pEye    = NULL;
	D3DXVECTOR3* pAt     = NULL;
	D3DXVECTOR3* pUp     = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iVectorEye, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iVectorAt, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iVectorUp, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildLookAtRHMatrix - result matrix does not exist" );
		return;
	}

	// get eye vector
	if ( ! ( pEye = ( D3DXVECTOR3* ) m_DataManager.GetData ( iVectorEye ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "BuildLookAtRHMatrix - eye vector does not exist" );
		return;
	}

	// get at vector
	if ( ! ( pAt = ( D3DXVECTOR3* ) m_DataManager.GetData ( iVectorAt ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "BuildLookAtRHMatrix - at vector does not exist" );
		return;
	}

	// get up vector
	if ( ! ( pUp = ( D3DXVECTOR3* ) m_DataManager.GetData ( iVectorUp ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "BuildLookAtRHMatrix - up vector does not exist" );
		return;
	}

	// finally build matrix
	D3DXMatrixLookAtRH ( pResult, pEye, pAt, pUp ); 
}

DARKSDK void BuildLookAtLHMatrix ( int iResult, int iVectorEye, int iVectorAt, int iVectorUp )
{
	// builds a look at matrix using left handed rule

	// pointers to matrix and vector data
	D3DXMATRIX*	 pResult = NULL;
	D3DXVECTOR3* pEye    = NULL;
	D3DXVECTOR3* pAt     = NULL;
	D3DXVECTOR3* pUp     = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( 
			!CheckTypeIsValid ( iResult, TYPE_MATRIX ) ||
			!CheckTypeIsValid ( iVectorEye, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iVectorAt, TYPE_VECTOR3 ) ||
			!CheckTypeIsValid ( iVectorUp, TYPE_VECTOR3 )
	   )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildLookAtLHMatrix - result matrix does not exist" );
		return;
	}

	// get eye vector
	if ( ! ( pEye = ( D3DXVECTOR3* ) m_DataManager.GetData ( iVectorEye ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "BuildLookAtLHMatrix - eye vector does not exist" );
		return;
	}

	// get at vector
	if ( ! ( pAt = ( D3DXVECTOR3* ) m_DataManager.GetData ( iVectorAt ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "BuildLookAtLHMatrix - at vector does not exist" );
		return;
	}

	// get up vector
	if ( ! ( pUp = ( D3DXVECTOR3* ) m_DataManager.GetData ( iVectorUp ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "BuildLookAtLHMatrix - up vector does not exist" );
		return;
	}

	// finally build matrix
	D3DXMatrixLookAtLH ( pResult, pEye, pAt, pUp ); 
}

DARKSDK void BuildOrthoRHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	// build an orthoganal matrix using right handed rule

	// pointer to matrix data
	D3DXMATRIX* pResult = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildOrthoRHMatrix - matrix does not exist" );
		return;
	}

	// build matrix
	D3DXMatrixOrthoRH ( pResult, fWidth, fHeight, fZNear, fZFar ); 
}

DARKSDK void BuildOrthoLHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	// build an orthogonal matrix using left handed rule

	// pointer to matrix data
	D3DXMATRIX* pResult = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildOrthoLHMatrix - matrix does not exist" );
		return;
	}

	// build matrix
	D3DXMatrixOrthoLH ( pResult, fWidth, fHeight, fZNear, fZFar ); 
}

DARKSDK void BuildPerspectiveRHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	// build a perspective matrix using right handed rule

	// pointer to matrix data
	D3DXMATRIX* pResult = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildPerspectiveRHMatrix - matrix does not exist" );
		return;
	}

	// build the matrix
	D3DXMatrixPerspectiveRH ( pResult, fWidth, fHeight, fZNear, fZFar ); 
}

DARKSDK void BuildPerspectiveLHMatrix ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	// build a perspective matrix using left handed rule

	// pointer to matrix data
	D3DXMATRIX* pResult = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildPerspectiveLHMatrix - matrix does not exist" );
		return;
	}

	// build the matrix
	D3DXMatrixPerspectiveLH ( pResult, fWidth, fHeight, fZNear, fZFar ); 
}

DARKSDK void BuildPerspectiveFovRHMatrix ( int iResult, float fFOV, float fAspect, float fZNear, float fZFar )
{
	// builds a perspective field of view right hand matrix

	// pointer to matrix data
	D3DXMATRIX* pResult = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildPerspectiveFovRHMatrix - matrix does not exist" );
		return;
	}

	// build the matrix
	D3DXMatrixPerspectiveFovRH ( pResult, fFOV, fAspect, fZNear, fZFar ); 
}

DARKSDK void BuildPerspectiveFovLHMatrix ( int iResult, float fFOV, float fAspect, float fZNear, float fZFar )
{
	// builds a perspective field of view left hand matrix

	// pointer to matrix data
	D3DXMATRIX* pResult = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildPerspectiveFovLHMatrix - matrix does not exist" );
		return;
	}

	// build the matrix
	D3DXMatrixPerspectiveFovLH ( pResult, fFOV, fAspect, fZNear, fZFar ); 
}

DARKSDK void BuildReflectionMatrix ( int iResult, float a, float b, float c, float d )
{
	// builds a reflection matrix

	// pointer to matrix data
	D3DXMATRIX* pResult = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildReflectionMatrix - matrix does not exist" );
		return;
	}

	D3DXMatrixReflect ( pResult, &D3DXPLANE ( a, b, c, d ) ); 
}

DARKSDK void BuildRotationAxisMatrix ( int iResult, int iVectorAxis, float fAngle )
{
	// build a matrix that rotates around an axis

	// pointer to matrix data
	D3DXMATRIX*  pResult = NULL;
	D3DXVECTOR3* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) || !CheckTypeIsValid ( iVectorAxis, TYPE_VECTOR3 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "BuildRotationAxisMatrix - result matrix does not exist" );
		return;
	}

	// get the rotation axis vector
	if ( ! ( pVector = ( D3DXVECTOR3* ) m_DataManager.GetData ( iVectorAxis ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "BuildRotationAxisMatrix - rotation axis vector does not exist" );
		return;
	}

	// finally create the matrix
	D3DXMatrixRotationAxis ( pResult, pVector, fAngle );
}

DARKSDK void RotateXMatrix ( int iID, float fAngle )
{
	// rotate the matrix on it's x axis (WRONG)
	// Builds a matrix that rotates around the x-axis.

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "RotateXMatrix - matrix does not exist" );
		return;
	}
	
	// now rotate the matrix
	D3DXMatrixRotationX ( pID, fAngle );
}

DARKSDK void RotateYMatrix ( int iID, float fAngle )
{
	// rotate the matrix on it's y axis (WRONG)
	// Builds a matrix that rotates around the y-axis.

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "RotateYMatrix - matrix does not exist" );
		return;
	}
	
	// now rotate the matrix
	D3DXMatrixRotationY ( pID, fAngle );
}

DARKSDK void RotateZMatrix ( int iID, float fAngle )
{
	// rotate the matrix on it's z axis (WRONG)
	// Builds a matrix that rotates around the z-axis.

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "RotateZMatrix - matrix does not exist" );
		return;
	}
	
	// now rotate the matrix
	D3DXMatrixRotationZ ( pID, fAngle );
}

DARKSDK void RotateYawPitchRollMatrix ( int iID, float fYaw, float fPitch, float fRoll )
{
	// rotate a matrix using yaw, pitch and roll

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "RotateYawPitchRollMatrix - matrix does not exist" );
		return;
	}
	
	// rotate the matrix
	D3DXMatrixRotationYawPitchRoll ( pID, fYaw, fPitch, fRoll );
}

DARKSDK void ScaleMatrix ( int iID, float fX, float fY, float fZ )
{
	// scale a matrix

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "ScaleMatrix - matrix does not exist" );
		return;
	}

	// now perform the scaling
	D3DXMatrixScaling ( pID, fX, fY, fZ );
}

DARKSDK void TranslateMatrix ( int iID, float fX, float fY, float fZ )
{
	// translate a matrix ( set it's position )

	// pointer to matrix data
	D3DXMATRIX* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iID, TYPE_MATRIX ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get matrix
	if ( ! ( pID = ( D3DXMATRIX* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "TranslateMatrix - matrix does not exist" );
		return;
	}

	// perform the translation
	D3DXMatrixTranslation ( pID, fX, fY, fZ );
}

DARKSDK void TransposeMatrix ( int iResult, int iSource )
{
	// get the transpose of a matrix

	// pointers to matrix data
	D3DXMATRIX* pResult = NULL;
	D3DXMATRIX* pSource = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iResult, TYPE_MATRIX ) ||!CheckTypeIsValid ( iSource, TYPE_MATRIX ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get result matrix
	if ( ! ( pResult = ( D3DXMATRIX* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "TransposeMatrix - result matrix does not exist" );
		return;
	}

	// get source matrix
	if ( ! ( pSource = ( D3DXMATRIX* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST); }  Error ( "TransposeMatrix - source matrix does not exist" );
		return;
	}

	// perform the operation
	D3DXMatrixTranspose ( pResult, pSource );
}

DARKSDK SDK_BOOL MakeVector4 ( int iID )
{
	// create a vector

	// pointer to vector data
	D3DXVECTOR4* pVector = NULL;

	// mike - 220406 - remove vector if needed
	CheckType ( iID );
	
	// delete any existing item
	//pVector = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID );
	//if(pVector) DeleteVector4 ( iID );

	// create a vector and set all components to 0
	if ( ! ( pVector = new D3DXVECTOR4 ( 0.0f, 0.0f, 0.0f, 0.0f ) ) )
	{
		// return an error if it fails
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "Failed to create vector" );
		return false;
	}

	// mike - 220406 - add vector4 type into list
	AddVector ( iID, TYPE_VECTOR4 );

	// add the vector to the linked list
	m_DataManager.Add ( pVector, iID );

	// return ok
	return true;
}

DARKSDK SDK_BOOL DeleteVector4 ( int iID )
{
	// deletes a given vector

	// mike - 190107 - must also delete from data type manager list
	m_DataTypeManager.Delete ( iID );

	m_DataManager.Delete ( iID );

	// return ok
	return true;
}

DARKSDK void SetVector4 ( int iID, float fX, float fY, float fZ, float fW )
{
	// sets the values of a vector

	// pointer to vector data
	D3DXVECTOR4* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iID, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the vector data
	if ( ! ( pVector = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SetVector4 - matrix does not exist" );
		return;
	}

	// now set the values
	*pVector = D3DXVECTOR4 ( fX, fY, fZ, fW );
}

DARKSDK void CopyVector4 ( int iSource, int iDestination )
{
	// copy the source vector to the destination vector

	// pointers to vector data
	D3DXVECTOR4* pVecSource      = NULL;
	D3DXVECTOR4* pVecDestination = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (!CheckTypeIsValid ( iSource, TYPE_VECTOR4 ) || !CheckTypeIsValid ( iDestination, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the source vector
	if ( ! ( pVecSource = ( D3DXVECTOR4* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CopyVector4 - source vector does not exist" );
		return;
	}

	// get the destination vector
	if ( ! ( pVecDestination = ( D3DXVECTOR4* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CopyVector4 - destination vector does not exist" );
		return;
	}

	// final assignment
	*pVecDestination = *pVecSource;
}

DARKSDK void AddVector4 ( int iResult, int iA, int iB )
{
	// adds 2 vectors and stores the result in the first vector

	// pointers to vector data
	D3DXVECTOR4* pVecResult = NULL;
	D3DXVECTOR4* pVecA      = NULL;
	D3DXVECTOR4* pVecB      = NULL;

		// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the resulting vector
	if ( ! ( pVecResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pVecA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pVecB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "AddVector4 - vector b does not exist" );
		return;
	}

	// finally add vectors together
	*pVecResult = *pVecA + *pVecB;
}

DARKSDK void SubtractVector4 ( int iResult, int iA, int iB )
{
	// adds 2 vectors and stores the result in the first vector

	// pointers to vector data
	D3DXVECTOR4* pVecResult = NULL;
	D3DXVECTOR4* pVecA      = NULL;
	D3DXVECTOR4* pVecB      = NULL;

		// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the resulting vector
	if ( ! ( pVecResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pVecA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pVecB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "SubtractVector4 - vector b does not exist" );
		return;
	}

	// subtract the vectors
	*pVecResult = *pVecA - *pVecB;
}

DARKSDK void MultiplyVector4 ( int iID, float fValue )
{
	// multiples a vector by a float

	// pointer to vector data
	D3DXVECTOR4* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the vector
	if ( ! ( pID = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MultiplyVector4 - vector does not exist" );
		return;
	}

	// multiply the vector
	*pID *= fValue;
}

DARKSDK void DivideVector4 ( int iID, float fValue )
{
	// divides a vector by a float

	// pointer to vector data
	D3DXVECTOR4* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the vector
	if ( ! ( pID = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "DivideVector4 - vector does not exist" );
		return;
	}

	// divide the vector
	*pID /= fValue;
}

DARKSDK SDK_BOOL IsEqualVector4 ( int iA, int iB )
{
	// returns true if the vectors are the same
	
	// pointers to vector data
	D3DXVECTOR4* pA = NULL;
	D3DXVECTOR4* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR4 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsEqualVector4 - vector a does not exist" );
		return false;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsEqualVector4 - vector b does not exist" );
		return false;
	}

	// if both vectors are the same then return true
	if ( *pA == *pB )
		return true;

	// otherwise return false
	return false;
}

DARKSDK SDK_BOOL IsNotEqualVector4 ( int iA, int iB )
{
	// returns true if the vectors are different

	// pointers to vector data
	D3DXVECTOR4* pA = NULL;
	D3DXVECTOR4* pB = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iA, TYPE_VECTOR4 ) || !CheckTypeIsValid ( iB, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return false;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsNotEqualVector4 - vector a does not exist" );
		return false;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "IsNotEqualVector4 - vector b does not exist" );
		return false;
	}

	// if both vectors are different then return true
	if ( *pA != *pB )
		return true;

	// otherwise return false
	return false;
}

DARKSDK SDK_FLOAT GetXVector4 ( int iID )
{
	// returns the x component of a vector

	// pointer to vector data
	D3DXVECTOR4* pVector = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get the data, if it doesn't exist return 0
	if ( ! ( pVector = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetXVector4 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the x value
	return SDK_RETFLOAT(pVector->x);
}

DARKSDK SDK_FLOAT GetYVector4 ( int iID )
{
	// returns the y component of a vector

	// pointer to vector data
	D3DXVECTOR4* pVector = NULL;

		// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get the data, if it doesn't exist return 0
	if ( ! ( pVector = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetYVector4 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the y value
	return SDK_RETFLOAT(pVector->y);
}

DARKSDK SDK_FLOAT GetZVector4 ( int iID )
{
	// returns the z component of a vector

	// pointer to vector data
	D3DXVECTOR4* pVector = NULL;

		// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get the data, if it doesn't exist return 0
	if ( ! ( pVector = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetYVector4 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the z value
	return SDK_RETFLOAT(pVector->z);
}

DARKSDK SDK_FLOAT GetWVector4 ( int iID )
{
	// returns the w component of a vector

	// pointer to vector data
	D3DXVECTOR4* pVector = NULL;

		// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ))
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}

	// get the data, if it doesn't exist return 0
	if ( ! ( pVector = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetYVector4 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}

	// now return the w value
	return SDK_RETFLOAT(pVector->w);
}

DARKSDK void GetBaryCentricCoordinatesVector4 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	// returns a point in barycentric coordinates

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pA      = NULL;
	D3DXVECTOR4* pB      = NULL;
	D3DXVECTOR4* pC      = NULL;

		// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iC, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector4 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR4* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetBaryCentricCoordinatesVector4 - vector c does not exist" );
		return;
	}

	// now perform the operation
	D3DXVec4BaryCentric ( pResult, pA, pB, pC, f, g ); 
}

DARKSDK void CatmullRomVector4 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	// performs a catmull rom interpolation on a vector

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pA      = NULL;
	D3DXVECTOR4* pB      = NULL;
	D3DXVECTOR4* pC      = NULL;
	D3DXVECTOR4* pD      = NULL;

		// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iC, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iD, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector4 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR4* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector4 - vector c does not exist" );
		return;
	}

	// get vector d
	if ( ! ( pD = ( D3DXVECTOR4* ) m_DataManager.GetData ( iD ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "CatmullRomVector4 - vector d does not exist" );
		return;
	}

	// perform the operation
	D3DXVec4CatmullRom ( pResult, pA, pB, pC, pD, s ); 
}

DARKSDK void HermiteVector4 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	// performs a hermite spline interpolation

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pA      = NULL;
	D3DXVECTOR4* pB      = NULL;
	D3DXVECTOR4* pC      = NULL;
	D3DXVECTOR4* pD      = NULL;

			// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iC, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iD, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}

	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector4 - vector b does not exist" );
		return;
	}

	// get vector c
	if ( ! ( pC = ( D3DXVECTOR4* ) m_DataManager.GetData ( iC ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector4 - vector c does not exist" );
		return;
	}

	// get vector d
	if ( ! ( pD = ( D3DXVECTOR4* ) m_DataManager.GetData ( iD ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "HermiteVector4 - vector d does not exist" );
		return;
	}

	// get the hermite vector
	D3DXVec4Hermite ( pResult, pA, pB, pC, pD, s ); 
}

DARKSDK SDK_FLOAT GetLengthVector4 ( int iID )
{
	// get the length of a vector

	// pointer to vector data
	D3DXVECTOR4* pID = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}
	
	// get the vector data
	if ( ! ( pID = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetLengthVector4 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}
	
	// return the length
	float result = D3DXVec4Length ( pID );
	return SDK_RETFLOAT(result);
}

DARKSDK SDK_FLOAT GetLengthSquaredVector4 ( int iID )
{
	// get the squared length of a vector

	// pointer to the vector data
	D3DXVECTOR4* pID = NULL;

			// mike - 220406 - check the operation can be carried out on this type
	if ( !CheckTypeIsValid ( iID, TYPE_VECTOR4 ) )
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return (DWORD)(0.0f);
	}
	
	// get the vector data
	if ( ! ( pID = ( D3DXVECTOR4* ) m_DataManager.GetData ( iID ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "GetLengthSquaredVector4 - vector does not exist" );
		float result=0.0f; return SDK_RETFLOAT(result);
	}
	
	// return the squared length
	float result = D3DXVec4LengthSq ( pID );
	return SDK_RETFLOAT(result); 
}

DARKSDK void LinearInterpolationVector4 ( int iResult, int iA, int iB, float s )
{
	// perform a linear interpolation between 2 vectors

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pA      = NULL;
	D3DXVECTOR4* pB      = NULL;

			// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "LinearInterpolationVector4 - vector b does not exist" );
		return;
	}

	// get the linear interpolation
	D3DXVec4Lerp ( pResult, pA, pB, s );
}

DARKSDK void MaximizeVector4 ( int iResult, int iA, int iB )
{
	// create a vector that is made up of the largest components of 2 other vectors

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pA      = NULL;
	D3DXVECTOR4* pB      = NULL;

			// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MaximizeVector4 - vector b does not exist" );
		return;
	}

	// perform the operation
	D3DXVec4Maximize ( pResult, pA, pB ); 
}

DARKSDK void MinimizeVector4 ( int iResult, int iA, int iB )
{
	// create a vector that is made up of the smallest components of 2 other vectors

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pA      = NULL;
	D3DXVECTOR4* pB      = NULL;

			// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iA, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iB, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector4 - result vector does not exist" );
		return;
	}

	// get vector a
	if ( ! ( pA = ( D3DXVECTOR4* ) m_DataManager.GetData ( iA ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector4 - vector a does not exist" );
		return;
	}

	// get vector b
	if ( ! ( pB = ( D3DXVECTOR4* ) m_DataManager.GetData ( iB ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "MinimizeVector4 - vector b does not exist" );
		return;
	}

	// perform the operation
	D3DXVec4Minimize ( pResult, pA, pB ); 
}

DARKSDK void NormalizeVector4 ( int iResult, int iSource )
{
	// get the normalized vector

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pSource = NULL;

			// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "NormalizeVector4 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR4* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "NormalizeVector4 - source vector does not exist" );
		return;
	}

	// perform the operation
	D3DXVec4Normalize ( pResult, pSource ); 
}

DARKSDK void ScaleVector4 ( int iResult, int iSource, float s )
{
	// scales a vector

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pSource = NULL;

				// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR4 )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ScaleVector4 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR4* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "ScaleVector4 - source vector does not exist" );
		return;
	}

	// perform the operation
	D3DXVec4Scale ( pResult, pSource, s ); 
}

DARKSDK void TransformVector4 ( int iResult, int iSource, int iMatrix )
{
	// transforms a vector

	// pointers to vector data
	D3DXVECTOR4* pResult = NULL;
	D3DXVECTOR4* pSource = NULL;
	D3DXMATRIX*  pMatrix = NULL;

	// mike - 220406 - check the operation can be carried out on this type
	if (
			!CheckTypeIsValid ( iResult, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iSource, TYPE_VECTOR4 ) ||
			!CheckTypeIsValid ( iMatrix, TYPE_MATRIX )
		)
	{
		RunTimeError ( RUNTIMEERROR_VECTORNOTEXIST );
		return;
	}
	
	// get the result vector
	if ( ! ( pResult = ( D3DXVECTOR4* ) m_DataManager.GetData ( iResult ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates4 - result vector does not exist" );
		return;
	}

	// get the source vector
	if ( ! ( pSource = ( D3DXVECTOR4* ) m_DataManager.GetData ( iSource ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates4 - source vector does not exist" );
		return;
	}

	// get the matrix
	if ( ! ( pMatrix = ( D3DXMATRIX* ) m_DataManager.GetData ( iMatrix ) ) )
	{
		if(g_bRTE) { RunTimeError(RUNTIMEERROR_VECTORNOTEXIST); }  Error ( "TransformVectorCoordinates4 - matrix does not exist" );
		return;
	}

	// transform the vector by the matrix
	D3DXVec4Transform ( pResult, pSource, pMatrix );
}

// U65/6.. new command for users

DARKSDK SDK_FLOAT GetMatrixElement ( int iMatrix, int iElement )
{
	// request provided by Joey Barfield [barfieldjoey@mapmasterz.com]
	if ( !CheckTypeIsValid ( iMatrix, TYPE_MATRIX ) )
	{
		RunTimeError(RUNTIMEERROR_MATRIX4NOTEXIST);
		return 0;
	}
	float res = 0.0;
	D3DXMATRIX mat = GetMatrix(iMatrix);
	if (iElement>=0 && iElement<=15) res = mat[iElement];
	return SDK_RETFLOAT(res);
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////
// DARK SDK SECTION //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifdef DARKSDK_COMPILE

void Constructor3DMaths ( HINSTANCE hSetup )
{
	Constructor ( hSetup );
}

void Destructor3DMaths  ( void )
{
	Destructor ( );
}

void SetErrorHandler3DMaths ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandler ( pErrorHandlerPtr );
}

void PassCoreData3DMaths ( LPVOID pGlobPtr )
{
	PassCoreData ( pGlobPtr );
}

void RefreshD3D3DMaths ( int iMode )
{
	RefreshD3D ( iMode );
}

bool dbMakeVector2 ( int iID )
{
	return 0 != MakeVector2 ( iID );
}

bool dbDeleteVector2 ( int iID )
{
	return 0 != DeleteVector2 ( iID );
}

void dbSetVector2 ( int iID, float fX, float fY )
{
	SetVector2 ( iID, fX, fY );
}

void dbCopyVector2 ( int iSource, int iDestination )
{
	CopyVector2 ( iSource, iDestination );
}

void dbAddVector2 ( int iResult, int iA, int iB )
{
	AddVector2 ( iResult, iA, iB );
}

void dbSubtractVector2 ( int iResult, int iA, int iB )
{
	// mike - 190107 - made sure call is correct
	SubtractVector2 ( iResult, iA, iB );
}

void dbMultiplyVector2 ( int iID, float fValue )
{
	MultiplyVector2 ( iID, fValue );
}

void dbDivideVector2 ( int iID, float fValue )
{
	DivideVector2 ( iID, fValue );
}

bool dbIsEqualVector2 ( int iA, int iB )
{
	return 0 != IsEqualVector2 ( iA, iB );
}

bool dbIsNotEqualVector2 ( int iA, int iB )
{
	return 0 != IsNotEqualVector2 ( iA, iB );
}

float dbXVector2 ( int iID )
{
	DWORD dwReturn = GetXVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbYVector2 ( int iID )
{
	DWORD dwReturn = GetYVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbBCCVector2 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	GetBaryCentricCoordinatesVector2 ( iResult, iA, iB, iC, f, g );
}

void dbCatmullRomVector2 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	CatmullRomVector2 ( iResult, iA, iB, iC, iD, s );
}

float dbCCWVector2 ( int iA, int iB )
{
	DWORD dwReturn = GetCCWVector2 ( iA, iB );
	
	return *( float* ) &dwReturn;
}

float dbDotProductVector2 ( int iA, int iB )
{
	DWORD dwReturn = DotProductVector2 ( iA, iB );
	
	return *( float* ) &dwReturn;
}

void dbHermiteVector2 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	HermiteVector2 ( iResult, iA, iB, iC, iD, s );
}

float dbLengthVector2 ( int iID )
{
	DWORD dwReturn = GetLengthVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbSquaredLengthVector2 ( int iID )
{
	DWORD dwReturn = GetLengthSquaredVector2 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbLinearInterpolateVector2 ( int iResult, int iA, int iB, float s )
{
	LinearInterpolationVector2 ( iResult, iA, iB, s );
}

void dbMaximizeVector2 ( int iResult, int iA, int iB )
{
	MaximizeVector2 ( iResult, iA, iB );
}

void dbMinimizeVector2 ( int iResult, int iA, int iB )
{
	MinimizeVector2 ( iResult, iA, iB );
}

void dbNormalizeVector2 ( int iResult, int iSource )
{
	NormalizeVector2 ( iResult, iSource );
}

void dbScaleVector2 ( int iResult, int iSource, float s )
{
	ScaleVector2 ( iResult, iSource, s );
}

void dbTransformCoordsVector2 ( int iResult, int iSource, int iMatrix )
{
	TransformVectorCoordinates2 ( iResult, iSource, iMatrix );
}

bool dbMakeVector3 ( int iID )
{
	return 0 != MakeVector3 ( iID );
}

bool dbDeleteVector3 ( int iID )
{
	return 0 != DeleteVector3 ( iID );
}

void dbSetVector3 ( int iID, float fX, float fY, float fZ )
{
	SetVector3 ( iID, fX, fY, fZ );
}

void dbCopyVector3 ( int iSource, int iDestination )
{
	CopyVector3 ( iSource, iDestination );
}

void dbAddVector3 ( int iResult, int iA, int iB )
{
	AddVector3 ( iResult, iA, iB );
}

void dbSubtractVector3 ( int iResult, int iA, int iB )
{
	SubtractVector3 ( iResult, iA, iB );
}

void dbMultiplyVector3 ( int iID, float fValue )
{
	MultiplyVector3 ( iID, fValue );
}

void dbDivideVector3 ( int iID, float fValue )
{
	DivideVector3 ( iID, fValue );
}

bool dbIsEqualVector3 ( int iA, int iB )
{
	return 0 != IsEqualVector3 ( iA, iB );
}

bool dbIsNotEqualVector3 ( int iA, int iB )
{
	return 0 != IsNotEqualVector3 ( iA, iB );
}

void dbBCCVector3 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	GetBaryCentricCoordinatesVector3 ( iResult, iA, iB, iC, f, g );
}

void dbCatmullRomVector3 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	CatmullRomVector3 ( iResult, iA, iB, iC, iD, s );
}

void dbCrossProductVector3 ( int iResult, int iA, int iB )
{
	CrossProductVector3 ( iResult, iA, iB );
}

void dbHermiteVector3 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	HermiteVector3 ( iResult, iA, iB, iC, iD, s );
}

void dbLinearInterpolateVector3 ( int iResult, int iA, int iB, float s )
{
	LinearInterpolationVector3 ( iResult, iA, iB, s );
}

void dbMaximizeVector3 ( int iResult, int iA, int iB )
{
	MaximizeVector3 ( iResult, iA, iB );
}

void dbMinimizeVector3 ( int iResult, int iA, int iB )
{
	MinimizeVector3 ( iResult, iA, iB );
}

void dbNormalizeVector3 ( int iResult, int iSource )
{
	NormalizeVector3 ( iResult, iSource );
}

void dbScaleVector3 ( int iResult, int iSource, float s )
{
	ScaleVector3 ( iResult, iSource, s );
}

float dbDotProductVector3 ( int iA, int iB )
{
	DWORD dwReturn = DotProductVector3 ( iA, iB );
	
	return *( float* ) &dwReturn;
}

float dbLengthVector3 ( int iID )
{
	DWORD dwReturn = GetLengthVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbSquaredLengthVector3 ( int iID )
{
	DWORD dwReturn = GetLengthSquaredVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbProjectVector3 ( int iResult, int iSource, int iProjectionMatrix, int iViewMatrix, int iWorldMatrix )
{
	ProjectVector3 ( iResult, iSource, iProjectionMatrix, iViewMatrix, iWorldMatrix );
}

void dbTransformCoordsVector3 ( int iResult, int iSource, int iMatrix )
{
	TransformVectorCoordinates3 ( iResult, iSource, iMatrix );
}

void dbTransformNormalsVector3 ( int iResult, int iSource, int iMatrix )
{
	TransformVectorNormalCoordinates3 ( iResult, iSource, iMatrix );
}

float dbXVector3 ( int iID )
{
	DWORD dwReturn = GetXVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbYVector3 ( int iID )
{
	DWORD dwReturn = GetYVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbZVector3 ( int iID )
{
	DWORD dwReturn = GetZVector3 ( iID );
	
	return *( float* ) &dwReturn;
}

bool dbMakeVector4 ( int iID )
{
	return 0 != MakeVector4 ( iID );
}

bool dbDeleteVector4 ( int iID )
{
	return 0 != DeleteVector4 ( iID );
}

void dbSetVector4 ( int iID, float fX, float fY, float fZ, float fW )
{
	SetVector4 ( iID, fX, fY, fZ, fW );
}

void dbCopyVector4 ( int iSource, int iDestination )
{
	CopyVector4 ( iSource, iDestination );
}

void dbAddVector4 ( int iResult, int iA, int iB )
{
	AddVector4 ( iResult, iA, iB );
}

void dbSubtractVector4 ( int iResult, int iA, int iB )
{
	SubtractVector4 ( iResult, iA, iB );
}

void dbMultiplyVector4 ( int iID, float fValue )
{
	MultiplyVector4 ( iID, fValue );
}

void dbDivideVector4 ( int iID, float fValue )
{
	DivideVector4 ( iID, fValue );
}

bool dbIsEqualVector4 ( int iA, int iB )
{
	return 0 != IsEqualVector4 ( iA, iB );
}

bool dbIsNotEqualVector4 ( int iA, int iB )
{
	return 0 != IsNotEqualVector4 ( iA, iB );
}

float dbXVector4 ( int iID )
{
	DWORD dwReturn = GetXVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbYVector4 ( int iID )
{
	DWORD dwReturn = GetYVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbZVector4 ( int iID )
{
	DWORD dwReturn = GetZVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbWVector4 ( int iID )
{
	DWORD dwReturn = GetWVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbBCCVector4 ( int iResult, int iA, int iB, int iC, float f, float g )
{
	GetBaryCentricCoordinatesVector4 ( iResult, iA, iB, iC, f, g );
}

void dbCatmullRomVector4 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	CatmullRomVector4 ( iResult, iA, iB, iC, iD, s );
}

void dbHermiteVector4 ( int iResult, int iA, int iB, int iC, int iD, float s )
{
	HermiteVector4 ( iResult, iA, iB, iC, iD, s );
}

float dbLengthVector4 ( int iID )
{
	DWORD dwReturn = GetLengthVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

float dbSquaredLengthVector4 ( int iID )
{
	DWORD dwReturn = GetLengthSquaredVector4 ( iID );
	
	return *( float* ) &dwReturn;
}

void dbLinearInterpolateVector4 ( int iResult, int iA, int iB, float s )
{
	LinearInterpolationVector4 ( iResult, iA, iB, s );
}

void dbMaximizeVector4 ( int iResult, int iA, int iB )
{
	MaximizeVector4 ( iResult, iA, iB );
}

void dbMinimizeVector4 ( int iResult, int iA, int iB )
{
	MinimizeVector4 ( iResult, iA, iB );
}

void dbNormalizeVector4 ( int iResult, int iSource )
{
	NormalizeVector4 ( iResult, iSource );
}

void dbScaleVector4 ( int iResult, int iSource, float s )
{
	ScaleVector4 ( iResult, iSource, s );
}

void dbTransformVector4 ( int iResult, int iSource, int iMatrix )
{
	TransformVector4 ( iResult, iSource, iMatrix );
}

bool dbMakeMatrix4 ( int iID )
{
	return 0 != MakeMatrix ( iID );
}

bool dbDeleteMatrix4 ( int iID )
{
	return 0 != DeleteMatrix ( iID );
}

void dbCopyMatrix4 ( int iSource, int iDestination )
{
	CopyMatrix ( iSource, iDestination );
}

void dbAddMatrix4 ( int iResult, int iA, int iB )
{
	AddMatrix ( iResult, iA, iB );
}

void dbSubtractMatrix4 ( int iResult, int iA, int iB )
{
	SubtractMatrix ( iResult, iA, iB );
}

void dbMultiplyMatrix4 ( int iResult, int iA, int iB )
{
	MultiplyMatrix ( iResult, iA, iB );
}

void dbMultiplyMatrix4 ( int iID, float fValue )
{
	MultiplyMatrix ( iID, fValue );
}

void dbDivideMatrix4 ( int iID, float fValue )
{
	DivideMatrix ( iID, fValue );
}

bool dbIsEqualMatrix4 ( int iA, int iB )
{
	return 0 != IsEqualMatrix ( iA, iB );
}

bool dbIsNotEqualMatrix4 ( int iA, int iB )
{
	return 0 != IsNotEqualMatrix ( iA, iB );
}

void dbSetIdentityMatrix4 ( int iID )
{
	SetIdentityMatrix ( iID );
}

float dbInverseMatrix4 ( int iResult, int iSource )
{
	DWORD dwReturn = InverseMatrix ( iResult, iSource );
	
	return *( float* ) &dwReturn;
}

bool dbIsIdentityMatrix4 ( int iID )
{
	return 0 != IsIdentityMatrix ( iID );
}

void dbBuildLookAtRHMatrix4 ( int iResult, int iVectorEye, int iVectorAt, int iVectorUp )
{
	BuildLookAtRHMatrix ( iResult, iVectorEye, iVectorAt, iVectorUp );
}

void dbBuildLookAtLHMatrix4 ( int iResult, int iVectorEye, int iVectorAt, int iVectorUp )
{
	BuildLookAtLHMatrix ( iResult, iVectorEye, iVectorAt, iVectorUp );
}

void dbBuildOrthoRHMatrix4 ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildOrthoRHMatrix ( iResult, fWidth, fHeight, fZNear, fZFar );
}

void dbBuildOrthoLHMatrix4 ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildOrthoLHMatrix ( iResult, fWidth, fHeight, fZNear, fZFar );
}

void dbBuildPerspectiveRHMatrix4 ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildPerspectiveRHMatrix ( iResult, fWidth, fHeight, fZNear, fZFar );
}

void dbBuildPerspectiveLHMatrix4 ( int iResult, float fWidth, float fHeight, float fZNear, float fZFar )
{
	BuildPerspectiveLHMatrix ( iResult, fWidth, fHeight, fZNear, fZFar );
}

void dbBuildFovRHMatrix4 ( int iResult, float fFOV, float fAspect, float fZNear, float fZFar )
{
	BuildPerspectiveFovRHMatrix ( iResult, fFOV, fAspect, fZNear, fZFar );
}

void dbBuildFovLHMatrix4 ( int iResult, float fFOV, float fAspect, float fZNear, float fZFar )
{
	BuildPerspectiveFovLHMatrix ( iResult, fFOV, fAspect, fZNear, fZFar );
}

void dbBuildReflectionMatrix4 ( int iResult, float a, float b, float c, float d )
{
	BuildReflectionMatrix ( iResult, a, b, c, d );
}

void dbBuildRotationAxisMatrix4 ( int iResult, int iVectorAxis, float fAngle )
{
	BuildRotationAxisMatrix ( iResult, iVectorAxis, fAngle );
}

void dbRotateXMatrix4 ( int iID, float fAngle )
{
	RotateXMatrix ( iID, fAngle );
}

void dbRotateYMatrix4 ( int iID, float fAngle )
{
	RotateYMatrix ( iID, fAngle );
}

void dbRotateZMatrix4 ( int iID, float fAngle )
{
	RotateZMatrix ( iID, fAngle );
}

void dbRotateYPRMatrix4 ( int iID, float fYaw, float fPitch, float fRoll )
{
	RotateYawPitchRollMatrix ( iID, fYaw, fPitch, fRoll );
}

void dbScaleMatrix4 ( int iID, float fX, float fY, float fZ )
{
	ScaleMatrix ( iID, fX, fY, fZ );
}

void dbTranslateMatrix4 ( int iID, float fX, float fY, float fZ )
{
	TranslateMatrix ( iID, fX, fY, fZ );
}

void dbTransposeMatrix4 ( int iResult, int iSource )
{
	TransposeMatrix ( iResult, iSource );
}

void dbWorldMatrix4 ( int iID )
{
	GetWorldMatrix ( iID );
}

void dbViewMatrix4 ( int iID )
{
	GetViewMatrix ( iID );
}

void dbProjectionMatrix4 ( int iID )
{
	GetProjectionMatrix ( iID );
}

D3DXVECTOR2	dbGetVector2 ( int iID )
{
	return GetVector2 ( iID );
}

D3DXVECTOR3	dbGetVector3 ( int iID )
{
	return GetVector3 ( iID );
}

D3DXVECTOR4	dbGetVector4 ( int iID )
{
	return GetVector4 ( iID );
}

D3DXMATRIX dbGetMatrix ( int iID )
{
	return GetMatrix ( iID );
}

int	dbGet3DMathsExist ( int iID )
{
	return GetExist ( iID );
}

#endif

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////