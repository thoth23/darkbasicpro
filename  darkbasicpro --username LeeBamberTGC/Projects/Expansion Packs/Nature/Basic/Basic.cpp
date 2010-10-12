
////////////////////////////////////////////////////////////////////
// INFORMATION /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

/*
	TERRAIN COMMANDS
*/

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// DEFINES AND INCLUDES ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#include "core.h"
#include "..\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\dbodata.h"
#include "..\shared\utility.h"
#include "..\shared\cTerrain.h"
#include <time.h>

#include < vector >
using namespace std;

// DIFFUSE makes LOD texturing difficult, so can switch of Diffuse Use here
#undef NODIFFUSEINTERRAIN

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// GLOBALS /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

struct sTerrain
{
	int			iID;
	sObject*	pObject;
	cTerrain*   pData;
	
	char		szHeightMap [ 255 ];

	float*		pHeightData;
	DWORD		dwWidth;
	DWORD		dwHeight;

	float		fXScale;
	float		fYScale;
	float		fZScale;

	int			iSplit;
	int			iTile;

	int			iBase;
	int			iDetail;

	float		fLightDirectionX;
	float		fLightDirectionY;
	float		fLightDirectionZ;
	float		fLightRed;
	float		fLightGreen;
	float		fLightBlue;
	float		fLightScale;
};

#ifdef NODIFFUSEINTERRAIN
struct sVertex
{
	float x, y, z;
	float tu, tv;
	float tu1, tv1;
};
#else
struct sVertex
{
	float x, y, z;
	DWORD dwDiffuse;
	float tu, tv;
	float tu1, tv1;
};
#endif

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////
// FUNCTIONS ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

DARKSDK void  LoadTerrain               ( DWORD dwFilename, int iID );
DARKSDK void  SaveTerrain               ( DWORD dwFilename, int iID );

DARKSDK void  MakeObjectTerrain			( int iID );
DARKSDK void  BuildTerrainEx			( int iID, int iUseSmoothing );
DARKSDK void  BuildTerrain				( int iID );
DARKSDK void  DeleteTerrain				( int iID );

DARKSDK void  SetTerrainScale			( int iID, float fXScale, float fYScale, float fZScale );
DARKSDK void  SetTerrainSplit			( int iID, int iSplit );
DARKSDK void  SetTerrainTiling			( int iID, int iTile );
DARKSDK void  SetTerrainHeightMap		( int iID, DWORD dwHeightMap );
DARKSDK void  SetTerrainTexture			( int iID, int iBase, int iDetail );
DARKSDK void  SetTerrainLight			( int iID, float fX, float fY, float fZ, float fRed, float fGreen, float fBlue, float fScale );

DARKSDK DWORD GetTerrainGroundHeight    ( int iID, float fX, float fZ );
DARKSDK DWORD GetTerrainXSize           ( int iID );
DARKSDK DWORD GetTerrainZSize           ( int iID );
DARKSDK void  UpdateTerrainNoCull		( int iNoCullMode );
DARKSDK void  UpdateTerrain             ( void );

/*
	// export names for string table -

	MAKE OBJECT TERRAIN%L%?MakeObjectTerrain@@YAXH@Z%

	SET TERRAIN SCALE%LFFF%?SetTerrainScale@@YAXHMMM@Z%
	SET TERRAIN SPLIT%LL%?SetTerrainSplit@@YAXHH@Z%
	SET TERRAIN TILING%LL%?SetTerrainTiling@@YAXHH@Z%
	SET TERRAIN HEIGHTMAP%LS%?SetTerrainHeightMap@@YAXHK@Z%
	SET TERRAIN LIGHT%LFFFFFFF%?SetTerrainLight@@YAXHMMMMMMM@Z%
	BUILD TERRAIN%L%?BuildTerrain@@YAXH@Z%

	LOAD TERRAIN%SL%?LoadTerrain@@YAXKH@Z%
	SAVE TERRAIN%SL%?SaveTerrain@@YAXKH@Z%

	GET TERRAIN GROUND HEIGHT[%FLFF%?GetTerrainGroundHeight@@YAKHMM@Z%
	GET TERRAIN X SIZE[%FL%?GetTerrainXSize@@YAMH@Z%
	GET TERRAIN Z SIZE[%FL%?GetTerrainZSize@@YAMH@Z%

	UPDATE TERRAIN%0%?UpdateTerrain@@YAXXZ%

	SET TERRAIN TEXTURE%LLL%?SetTerrainTexture@@YAXHHH@Z%
*/

typedef LPD3DXMESH          ( *LocalMeshToDXMeshPFN           		) ( sMesh*, LPD3DVERTEXELEMENT9, DWORD );
typedef void                ( *UpdateLocalMeshWithDXMeshPFN   		) ( sMesh*, LPD3DXMESH );
typedef sObject*			( *CreateObjectPFN                		) ( int, char* );
typedef bool				( *CreateObjectExPFN              		) ( int, char*, int  );
typedef bool				( *SetupFVFDataPFN                		) ( sMesh*, DWORD, DWORD, DWORD );
typedef void				( *SetNewObjectFinalPropertiesPFN 		) ( int, float );
typedef void				( *SetTexturePFN                  		) ( int, int );
typedef IDirect3DDevice9*	( *GetDirect3DPFN                 		) ( void );
typedef bool				( *CalculateMeshBoundsPFN		  		) ( sMesh* );
typedef void				( *SetTexturePFN                  		) ( int, int );
typedef void				( *SetDetailTexturePFN            		) ( int, int );
typedef void				( *ON_OBJECT_DELETE_CALLBACK      		) ( int Id, int userData );
typedef void				( *SetDeleteCallBackPFN           		) ( int iID, ON_OBJECT_DELETE_CALLBACK pfn, int userData );
typedef D3DXMATRIX			( *GetMatrixPFN					  		) ( int );
typedef void				( *ObjectLoadPFN						) ( DWORD, int );
typedef void				( *ObjectSavePFN						) ( DWORD, int );
typedef char*				( *GetImageNamePFN						) ( int );
typedef void				( *DeleteObjectPFN						) ( int );
typedef void				( *CreateCustomDataArrayForObjectPFN	) ( sObject*, int );

LPDIRECT3DDEVICE9					g_pD3DDevice					= NULL;
CreateObjectPFN 					g_CreateNewObject				= NULL;
CreateObjectExPFN					g_CreateNewObjectEx				= NULL;
SetupFVFDataPFN 					g_SetupMeshFVFData				= NULL;
SetNewObjectFinalPropertiesPFN		g_SetNewObjectFinalProperties	= NULL;
SetTexturePFN						g_SetTexture				    = NULL;
SetDetailTexturePFN					g_SetDetailTexture				= NULL;
GetDirect3DPFN						g_GetDirect3D					= NULL;
CalculateMeshBoundsPFN				g_CalculateMeshBounds			= NULL;
LocalMeshToDXMeshPFN				g_LocalMeshToDXMesh				= NULL;
UpdateLocalMeshWithDXMeshPFN		g_UpdateLocalMeshWithDXMesh		= NULL;
sTerrain*							g_pCurrentTerrain				= NULL;
SetDeleteCallBackPFN				g_DeleteCallBack				= NULL;
GetMatrixPFN						g_GetCameraViewMatrix			= NULL;
GetMatrixPFN						g_GetCameraProjectionMatrix		= NULL;
ObjectLoadPFN						g_LoadObject					= NULL;
ObjectSavePFN						g_SaveObject					= NULL;
GetImageNamePFN						g_GetImageName				    = NULL;
DeleteObjectPFN						g_DeleteObject					= NULL;
CreateCustomDataArrayForObjectPFN	g_CreateCustomDataArrayForObject = NULL;
D3DXPLANE							g_Frustum [ 6 ];
vector < sTerrain >					g_Terrain;

#ifdef DARKSDK_COMPILE
	bool				CreateNewObject					( int iID, LPSTR pName, int iFrame );
	bool				SetupMeshFVFData				( sMesh* pMesh, DWORD dwFVF, DWORD dwSize, DWORD dwIndexCount );
	bool				SetNewObjectFinalProperties		( int iID, float fRadius );
	void				SetTexture						( int iID, int iImageIndex );
	void				SetDetailMap					( int iID, int iImage );
	IDirect3DDevice9*	dbGetDirect3DDevice				( void );
	bool				CalculateMeshBounds				( sMesh* pMesh );
	LPD3DXMESH			LocalMeshToDXMesh				( sMesh* pMesh, CONST LPD3DVERTEXELEMENT9 pDeclarationOverride, DWORD dwFVFOverride );
	void				UpdateLocalMeshWithDXMesh		( sMesh* pMesh, LPD3DXMESH pDXMesh );
	void				SetDeleteCallBack				( int iID, ON_OBJECT_DELETE_CALLBACK pfn, int userData );
	void				CreateCustomDataArrayForObject	( sObject* pObject, int iSize );
	LPSTR 				dbGetImageName					( int iID );
	void				dbPositionCamera				( int iID, float fX, float fY, float fZ );
	float				dbCameraPositionX				( int iID );
	float				dbCameraPositionY 				( int iID );
	float				dbCameraPositionZ	 			( int iID );
	void				dbLoadObject					( char* szFilename, int iID );
	void				dbDeleteObject					( int iID );
#endif

void SetupTerrain ( void )
{
	// initial terrain setup - allocate function pointers

	// get function pointers
	// lee - 010806 - gdk moved 'g_pGlobForTerrain' code inside define for DLL only
	#ifndef DARKSDK_COMPILE
		// check the global structure pointer is valid
		if ( !g_pGlobForTerrain ) Error ( -1 );
		g_CreateNewObject             		= ( CreateObjectPFN						) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?CreateNewObject@@YA_NHPAD@Z" );
		g_CreateNewObjectEx           		= ( CreateObjectExPFN					) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?CreateNewObject@@YA_NHPADH@Z" );
		g_SetupMeshFVFData            		= ( SetupFVFDataPFN						) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?SetupMeshFVFData@@YA_NPAUsMesh@@KKK@Z" );
		g_SetNewObjectFinalProperties 		= ( SetNewObjectFinalPropertiesPFN		) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?SetNewObjectFinalProperties@@YA_NHM@Z" );
		g_SetTexture                  		= ( SetTexturePFN						) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?SetTexture@@YAXHH@Z" );
		g_SetDetailTexture            		= ( SetDetailTexturePFN					) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?SetDetailMap@@YAXHH@Z" );
		g_GetDirect3D                 		= ( GetDirect3DPFN						) GetProcAddress ( g_pGlobForTerrain->g_GFX,     "?GetDirect3DDevice@@YAPAUIDirect3DDevice9@@XZ" );
		g_CalculateMeshBounds         		= ( CalculateMeshBoundsPFN				) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?CalculateMeshBounds@@YA_NPAUsMesh@@@Z" );
		g_LocalMeshToDXMesh           		= ( LocalMeshToDXMeshPFN				) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?LocalMeshToDXMesh@@YAPAUID3DXMesh@@PAUsMesh@@QAU_D3DVERTEXELEMENT9@@K@Z" );
		g_UpdateLocalMeshWithDXMesh   		= ( UpdateLocalMeshWithDXMeshPFN		) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?UpdateLocalMeshWithDXMesh@@YAXPAUsMesh@@PAUID3DXMesh@@@Z" );
		g_DeleteCallBack			  		= ( SetDeleteCallBackPFN				) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?SetDeleteCallBack@@YAXHP6AXHH@ZH@Z" );
		g_GetCameraViewMatrix		  		= ( GetMatrixPFN						) GetProcAddress ( g_pGlobForTerrain->g_Camera3D, "?GetViewMatrix@@YA?AUD3DXMATRIX@@H@Z" );
		g_GetCameraProjectionMatrix			= ( GetMatrixPFN						) GetProcAddress ( g_pGlobForTerrain->g_Camera3D, "?GetProjectionMatrix@@YA?AUD3DXMATRIX@@H@Z" );
		g_LoadObject						= ( ObjectLoadPFN						) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?Load@@YAXKH@Z" );
		g_SaveObject						= ( ObjectSavePFN						) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?Save@@YAXKH@Z" );
		g_GetImageName						= ( GetImageNamePFN						) GetProcAddress ( g_pGlobForTerrain->g_Image, "?GetName@@YAPADH@Z" );
		g_DeleteObject						= ( DeleteObjectPFN						) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?DeleteEx@@YAXH@Z" );
		g_CreateCustomDataArrayForObject	= ( CreateCustomDataArrayForObjectPFN	) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?CreateCustomDataArrayForObject@@YAXPAUsObject@@H@Z" );
	#else
		g_CreateNewObjectEx					= CreateNewObject;
		g_SetupMeshFVFData					= SetupMeshFVFData;
		g_SetNewObjectFinalProperties		= ( SetNewObjectFinalPropertiesPFN ) SetNewObjectFinalProperties;
		g_SetTexture						= SetTexture;
		g_SetDetailTexture					= SetDetailMap;
		g_GetDirect3D						= dbGetDirect3DDevice;
		g_CalculateMeshBounds				= CalculateMeshBounds;
		g_LocalMeshToDXMesh					= ( LocalMeshToDXMeshPFN )LocalMeshToDXMesh;
		g_UpdateLocalMeshWithDXMesh			= UpdateLocalMeshWithDXMesh;
		g_DeleteCallBack					= SetDeleteCallBack;
		g_GetImageName						= dbGetImageName;
		g_CreateCustomDataArrayForObject	= CreateCustomDataArrayForObject;
		g_LoadObject						= ( ObjectLoadPFN ) dbLoadObject;
        g_DeleteObject						= dbDeleteObject;
	#endif

	// get the direct3D device
	g_pD3DDevice = g_GetDirect3D ( );

	// make sure we have a valid d3d device
	if ( !g_pD3DDevice )
		Error ( 6 );
}

void DestroyTerrain ( void )
{
	for ( int i = 0; i < ( int ) g_Terrain.size ( ); i++ )
	{
		SAFE_DELETE ( g_Terrain [ i ].pData );
		// mike - 020206 - addition for vs8
		//g_Terrain.erase ( &g_Terrain [ i ] );
		g_Terrain.erase ( g_Terrain.begin ( ) + i );
	}
}

bool GetTerrainData ( int iID )
{
	// return the terrain structure for the given ID

	// set the temporary pointer to null
	g_pCurrentTerrain = NULL;

	// go through the terrain list
	for ( int i = 0; i <  ( int ) g_Terrain.size ( ); i++ )
	{
		// see if we have a match
		if ( g_Terrain [ i ].iID == iID )
		{
			// set the temporary pointer and return
			g_pCurrentTerrain = &g_Terrain [ i ];
			return true;
		}
	}

	// fail as terrain does not exist
	return false;
}

void MakeObjectTerrain ( int iID )
{
	// make a new terrain object

	// first see if the terrain already exists
	GetTerrainData ( iID );

	// is the pointer valid
	if ( g_pCurrentTerrain )
	{
		// terrain exists - fail
		Error ( 4 );
	}

	// our new terrain structure
	sTerrain data;

	// clear the structure
	memset ( &data, 0, sizeof ( sTerrain ) );

	// 300305
	strcpy ( data.szHeightMap, "" );

	data.fXScale			= 1.0f;
	data.fYScale			= 1.0f;
	data.fZScale			= 1.0f;
	data.iSplit				= 16;
	data.iTile				= 4;
	data.fLightDirectionX	=  0.0f;
	data.fLightDirectionY	= -1.0f;
	data.fLightDirectionZ	=  0.0f;
	data.fLightRed			=  1.0f;
	data.fLightGreen		=  1.0f;
	data.fLightBlue			=  1.0f;
	data.fLightScale		=  0.5f;

	// store the object ID
	data.iID = iID;

	// send this to the back of the list
	g_Terrain.push_back ( data );
}

void SetTerrainScale ( int iID, float fXScale, float fYScale, float fZScale )
{
	// set the scale of the terrain

	// get the terrain data
	GetTerrainData ( iID );

	// check the pointer is valid
	if ( g_pCurrentTerrain )
	{
		// set scale values
		g_pCurrentTerrain->fXScale = fXScale;
		g_pCurrentTerrain->fYScale = fYScale;
		g_pCurrentTerrain->fZScale = fZScale;
	}
	else
	{
		// fail as terrain does not exist
		Error ( 3 );
	}
}

void SetTerrainSplit ( int iID, int iSplit )
{
	// set the split value of the terrain, this will determine how
	// many meshes end up in the terrain e.g. a split value of 8
	// will create a grid of 8 x 8 meshes resulting in a total of
	// 64 meshes

	// get the terrain data
	GetTerrainData ( iID );

	// check the pointer is valid
	if ( g_pCurrentTerrain )
	{
		// store the split value
		g_pCurrentTerrain->iSplit = iSplit;
	}
	else
	{
		// fail as the terrain does not exist
		Error ( 3 );
	}
}

void SetTerrainTiling ( int iID, int iTile )
{
	// set the tile value of the terrain, this controls how
	// secondary textures such as a detail map will be applied
	// to the terrain, a value of 1 will apply the second texture
	// to each quad of the terrain, going higher will spread the
	// texture over more quads

	// get the terrain pointer
	GetTerrainData ( iID );

	// check the pointer is valid
	if ( g_pCurrentTerrain )
	{
		// store the tile value
		g_pCurrentTerrain->iTile = iTile;
	}
	else
	{
		// fail as terrain does not exist
		Error ( 3 );
	}
}

void SetTerrainHeightMap ( int iID, DWORD dwHeightMap )
{
	// set the height map to be used for the terrain

	// get the terrain data
	GetTerrainData ( iID );

	// check the pointer is valid
	if ( g_pCurrentTerrain )
	{
		// copy the filename across
		strcpy ( g_pCurrentTerrain->szHeightMap, ( char* ) dwHeightMap );
	}
	else
	{
		// terrain does not exist - fail
		Error ( 3 );
	}
}

void SetTerrainLight ( int iID, float fX, float fY, float fZ, float fRed, float fGreen, float fBlue, float fScale )
{
	// set the light properties of the terrain, this will affect
	// the shadowing - note that lights are not affected by
	// realtime lights - only diffuse

	// get the terrain data
	GetTerrainData ( iID );

	// check the terrain pointer is valid
	if ( g_pCurrentTerrain )
	{
		// store the light values
		g_pCurrentTerrain->fLightDirectionX = fX;
		g_pCurrentTerrain->fLightDirectionY = fY;
		g_pCurrentTerrain->fLightDirectionZ = fZ;
		g_pCurrentTerrain->fLightRed        = fRed;
		g_pCurrentTerrain->fLightGreen      = fGreen;
		g_pCurrentTerrain->fLightBlue       = fBlue;
		g_pCurrentTerrain->fLightScale      = fScale;
	}
	else
	{
		// fail as terrain does not exist
		Error ( 3 );
	}
}

void SetTerrainTexture ( int iID, int iBase, int iDetail )
{
	// sets the textures to be used by the terrain, a detail
	// texture doesn't have to e passed in

	// get the terrain pointer
	GetTerrainData ( iID );

	// check the pointer is valid
	if ( g_pCurrentTerrain )
	{
		// store the base and detail texture IDs
		g_pCurrentTerrain->iBase   = iBase;
		g_pCurrentTerrain->iDetail = iDetail;
	}
	else
	{
		// fail
		Error ( 3 );
	}
}

void DeleteTerrain ( int iID, int iUserData )
{
	for ( int i = 0; i < ( int ) g_Terrain.size ( ); i++ )
	{
		// see if we have a match
		if ( g_Terrain [ i ].iID == iID )
		{
			// remove data
			SAFE_DELETE ( g_Terrain [ i ].pData );
			g_Terrain.erase ( g_Terrain.begin ( ) + i );
		}
	}
}

void DeleteTerrain ( int iID )
{
	// scan all terrains and find our iID
	for ( int i = 0; i < ( int ) g_Terrain.size ( ); i++ )
	{
		// see if we have a match
		if ( g_Terrain [ i ].iID == iID )
		{
			// Delete terrain data, and erase list entry for it
			SAFE_DELETE ( g_Terrain [ i ].pData );
			g_Terrain.erase ( g_Terrain.begin ( ) + i );

			// Also delete the object associated with the terrain
			g_DeleteObject ( iID );
		}
	}
}

void BuildTerrainEx ( int iID, int iExcludeSmoothingOfTerrain )
{
	// build the final terrain

	g_pD3DDevice = g_GetDirect3D ( );

	// local variables
	sMesh*		pMesh					= NULL;		// temporary mesh pointer
	sFrame*		pFrame					= NULL;		// temporary frame pointer
	DWORD		dwIndexCount			= 0;		// index count
	DWORD		dwVertexCount			= 0;		// vertex count
	sMesh**		ppMeshListFromObject	= NULL;		// mesh list array
	int			iFinalMeshCount			= 0;		// final mesh count
	int			iTerrainBlock			= 0;		// current terrain block
	WORD*		pIndex					= NULL;		// used as pointer to index buffer

	// get the terrain data
	GetTerrainData ( iID );

	// fail if the terrain doest not exist
	if ( !g_pCurrentTerrain )
		Error ( 3 );

	if ( g_pCurrentTerrain->pObject || g_pCurrentTerrain->pData )
	{
		Error ( 6 );
		return;
	}

	// create a new terrain object and store the ID
	g_pCurrentTerrain->pData = new cTerrain;
	g_pCurrentTerrain->iID   = iID;

	// check terrain data is valid
	if ( !g_pCurrentTerrain->pData )
	{
		Error ( 6 );
		return;
	}

	// pass the d3d device into the terrain class
	g_pCurrentTerrain->pData->SetD3DDevice ( g_pD3DDevice );

	// now create the terrain, pass in previously set parameters
	if ( !g_pCurrentTerrain->pData->LoadTerrain  ( 
														g_pCurrentTerrain->szHeightMap,
														g_pCurrentTerrain->fXScale,
														g_pCurrentTerrain->fYScale,
														g_pCurrentTerrain->fZScale,
														g_pCurrentTerrain->iSplit,
														g_pCurrentTerrain->iTile,
														g_pCurrentTerrain->fLightDirectionX,
														g_pCurrentTerrain->fLightDirectionY,
														g_pCurrentTerrain->fLightDirectionZ,
														g_pCurrentTerrain->fLightRed,
														g_pCurrentTerrain->fLightGreen,
														g_pCurrentTerrain->fLightBlue,
														g_pCurrentTerrain->fLightScale,
														iExcludeSmoothingOfTerrain
												 ) )
	{
		Error ( 6 );
		return;
	}

	// store the final mesh count
	iFinalMeshCount = g_pCurrentTerrain->pData->m_iBlocksWide * g_pCurrentTerrain->pData->m_iBlocksHigh;

	// create a new object with the right number of meshes
	g_CreateNewObjectEx ( iID, "terrain", iFinalMeshCount );
	
	// get the object pointer
	g_pCurrentTerrain->pObject = GetObjectData ( iID );

	// check the object pointer is valid
	if ( !g_pCurrentTerrain->pObject )
		Error ( 6 );

	// now get a pointer to the first mesh and first frame, finally
	// create a temporary mesh list so we can access by array and not
	// through the child structure, we can't use the object mesh list
	// at this stage because it hasn't been created yet
	pMesh				 = g_pCurrentTerrain->pObject->pFrame->pMesh;
	pFrame			     = g_pCurrentTerrain->pObject->pFrame;
	ppMeshListFromObject = new sMesh* [ iFinalMeshCount ];

	{
		// width of height map			DWORD
		// height of height map			DWORD
		// x scale						float
		// y scale						float
		// z scale						float
		// height map data				width * height ( float )

		DWORD dwOffset    = 0;
		DWORD dwArraySize = ( g_pCurrentTerrain->pData->m_iHeightMapWidth * g_pCurrentTerrain->pData->m_iHeightMapHeight ) * sizeof ( float );
		DWORD dwSize      = ( sizeof ( DWORD ) * 2 ) + ( sizeof ( float ) * 3 ) + dwArraySize;

		g_pCurrentTerrain->pObject->dwCustomSize = dwSize;
		
		g_CreateCustomDataArrayForObject ( g_pCurrentTerrain->pObject, dwSize );

		// width
		memcpy ( ( BYTE* ) g_pCurrentTerrain->pObject->pCustomData + dwOffset, &g_pCurrentTerrain->pData->m_iHeightMapWidth, sizeof ( DWORD ) );
		dwOffset += sizeof ( DWORD );

		// height
		memcpy ( ( BYTE* ) g_pCurrentTerrain->pObject->pCustomData + dwOffset, &g_pCurrentTerrain->pData->m_iHeightMapHeight, sizeof ( DWORD ) );
		dwOffset += sizeof ( DWORD );

		// x scale
		memcpy ( ( BYTE* ) g_pCurrentTerrain->pObject->pCustomData + dwOffset, &g_pCurrentTerrain->pData->m_vecScale.x, sizeof ( DWORD ) );
		dwOffset += sizeof ( DWORD );

		// y scale
		memcpy ( ( BYTE* ) g_pCurrentTerrain->pObject->pCustomData + dwOffset, &g_pCurrentTerrain->pData->m_vecScale.y, sizeof ( DWORD ) );
		dwOffset += sizeof ( DWORD );

		// z scale
		memcpy ( ( BYTE* ) g_pCurrentTerrain->pObject->pCustomData + dwOffset, &g_pCurrentTerrain->pData->m_vecScale.z, sizeof ( DWORD ) );
		dwOffset += sizeof ( DWORD );
		
		// height map data array
		memcpy ( ( BYTE* ) g_pCurrentTerrain->pObject->pCustomData + dwOffset, g_pCurrentTerrain->pData->m_pHeightMap, dwArraySize );
	}

	// check the mesh list has been created
	if ( !ppMeshListFromObject )
		Error ( 6 );

	// store pointers to meshes in the mesh list
	for ( int i = 0; i < iFinalMeshCount; i++ )
	{
		// store mesh pointer
		ppMeshListFromObject [ i ] = pMesh;

		// move temporary pointers to child frames
		if ( pFrame->pChild )
		{
			pFrame = pFrame->pChild;
			pMesh  = pFrame->pMesh;
		}
	}

	// go through all meshes in list
	for ( int iMesh = 0; iMesh < iFinalMeshCount; iMesh++ )
	{
		// get index and vertex count of the block
		dwIndexCount  = ( g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->m_iQuadsWide  * g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->m_iQuadsHigh ) * 6;
		dwVertexCount =   g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->m_iBlockWidth * g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->m_iBlockHeight;

		// make sure index count is valid
		if ( dwIndexCount > 64000 )
			Error ( 5 );

		// set up the new mesh 
		#ifdef NODIFFUSEINTERRAIN
 		 // leechange - 081205 - shadows are better achived in the paint base texture (diffuse messes landscape SIZE and LOD!)
		 g_SetupMeshFVFData ( ppMeshListFromObject [ iMesh ], D3DFVF_XYZ | D3DFVF_TEX2, dwVertexCount, dwIndexCount );
		#else
		 g_SetupMeshFVFData ( ppMeshListFromObject [ iMesh ], D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2, dwVertexCount, dwIndexCount );
		#endif

		sVertex* pVertex = ( sVertex* ) ppMeshListFromObject [ iMesh ]->pVertexData;

		// copy vertices from block into our new mesh
		for ( int i = 0; i < ( int ) dwVertexCount; i++ )
		{
			pVertex [ i ].x         = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetX ( i );
			pVertex [ i ].y         = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetY ( i );
			pVertex [ i ].z         = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetZ ( i );
  			#ifndef NODIFFUSEINTERRAIN
			 pVertex [ i ].dwDiffuse = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetDiffuse ( i );
			#endif
			pVertex [ i ].tu        = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetTU ( i );
			pVertex [ i ].tv        = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetTV ( i );
			pVertex [ i ].tu1       = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetTU1 ( i );
			pVertex [ i ].tv1       = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->GetTV1 ( i );
		}

		// copy indices from block into mesh
		for ( int i = 0; i < ( int ) dwIndexCount; i++ )
			ppMeshListFromObject [ iMesh ]->pIndices [ i ] = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->m_IndexList [ i ];
			
		// set up mesh properties, no lights as we use diffuse, triangle list for primitive type, store
		// the vertex count and finally the primitive count
		ppMeshListFromObject [ iMesh ]->bLight           = false;
		ppMeshListFromObject [ iMesh ]->iPrimitiveType   = D3DPT_TRIANGLELIST;
		ppMeshListFromObject [ iMesh ]->iDrawVertexCount = ppMeshListFromObject [ iMesh ]->dwVertexCount;
		ppMeshListFromObject [ iMesh ]->iDrawPrimitives  = g_pCurrentTerrain->pData->m_pBlock [ iTerrainBlock ]->m_iPrimitiveCount;
		ppMeshListFromObject [ iMesh ]->iMeshType        = 1;

		// calculate the mesh bounds
		g_CalculateMeshBounds ( ppMeshListFromObject [ iMesh ] );

		// increment the terrain block index
		iTerrainBlock++;
	}

	// set the final properties of the object and apply a blank texture
	g_SetNewObjectFinalProperties ( iID, 0.0f );
	g_SetTexture                  ( iID, 0 );

	if ( g_pCurrentTerrain->iBase > 0 )
	{
		// set the base texture and detail texture from the terrain settings
		g_SetTexture       ( iID, g_pCurrentTerrain->iBase   );
		g_SetDetailTexture ( iID, g_pCurrentTerrain->iDetail );

		char* szBaseName   = g_GetImageName ( g_pCurrentTerrain->iBase );
		char* szDetailName = g_GetImageName ( g_pCurrentTerrain->iDetail );

		// adjust the detail map mode
		for ( int i = 0; i < g_pCurrentTerrain->pObject->iMeshCount; i++ )
		{
			if ( szBaseName && szDetailName )
			{
				strcpy ( g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 0 ].pName, szBaseName );
				strcpy ( g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].pName, szDetailName );
			}

			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwStage        = 1;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwBlendMode    = D3DTOP_ADDSIGNED;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwBlendArg1    = D3DTA_TEXTURE;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwBlendArg2    = D3DTA_CURRENT;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwTexCoordMode = 0;
		}
		
	}

	// safely delete the temporary mesh list
	SAFE_DELETE_ARRAY ( ppMeshListFromObject );
}

void BuildTerrain ( int iID )
{
	// default - see above
	BuildTerrainEx ( iID, 0 );
}

void LoadTerrain ( DWORD dwFilename, int iID )
{
	// load the terrain

	// custom data structure
	// width of height map			DWORD
	// height of height map			DWORD
	// x scale						float
	// y scale						float
	// z scale						float
	// height map data				width * height ( float )

	// dbo object pointer
	sObject* pObject		= NULL;
	DWORD	 dwArraySize	= 0;

	// load the dbo file through the basic3d dll
	g_LoadObject ( dwFilename, iID );

	// get the pointer to the object
	pObject = GetObjectData ( iID );

	if ( !pObject )
	{
		Error ( 6 );
		return;
	}

	// make the terrain and get the data
	MakeObjectTerrain ( iID );
	GetTerrainData    ( iID );

	// create a new terrain and store the ID
	g_pCurrentTerrain->pData   = new cTerrain;
	g_pCurrentTerrain->iID     = iID;
	g_pCurrentTerrain->pObject = pObject;

	// get the width and height of the terrain
	memcpy ( &g_pCurrentTerrain->pData->m_iHeightMapWidth,  ( BYTE* ) pObject->pCustomData + 0, sizeof ( DWORD ) );
	memcpy ( &g_pCurrentTerrain->pData->m_iHeightMapHeight, ( BYTE* ) pObject->pCustomData + 4, sizeof ( DWORD ) );

	// get scale of the terrain
	memcpy ( &g_pCurrentTerrain->pData->m_vecScale.x, ( BYTE* ) pObject->pCustomData +  8, sizeof ( float ) );
	memcpy ( &g_pCurrentTerrain->pData->m_vecScale.y, ( BYTE* ) pObject->pCustomData + 12, sizeof ( float ) );
	memcpy ( &g_pCurrentTerrain->pData->m_vecScale.z, ( BYTE* ) pObject->pCustomData + 16, sizeof ( float ) );

	// the size of the height array
	dwArraySize = g_pCurrentTerrain->pData->m_iHeightMapWidth * g_pCurrentTerrain->pData->m_iHeightMapHeight;

	// allocate data for the aray
	g_pCurrentTerrain->pData->m_pHeightMap = new float [ dwArraySize ];

	// check memory was safely allocated
	if ( !g_pCurrentTerrain->pData->m_pHeightMap )
		return;

	// copy the data from the object into the height array
	memcpy ( g_pCurrentTerrain->pData->m_pHeightMap, ( BYTE* ) pObject->pCustomData + 20, sizeof ( float ) * dwArraySize );

	// adjust the detail map mode
	for ( int i = 0; i < g_pCurrentTerrain->pObject->iMeshCount; i++ )
	{
		// leefix - 060306 - u60 - only if the texture has a second stage
		if ( g_pCurrentTerrain->pObject->ppMeshList [ i ]->dwTextureCount >= 2 )
		{
			// set blend attributes for detail map
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwStage        = 1;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwBlendMode    = D3DTOP_ADDSIGNED;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwBlendArg1    = D3DTA_TEXTURE;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwBlendArg2    = D3DTA_CURRENT;
			g_pCurrentTerrain->pObject->ppMeshList [ i ]->pTextures [ 1 ].dwTexCoordMode = 0;
		}
	}
}

void SaveTerrain ( DWORD dwFilename, int iID )
{
	sObject* pObject = GetObjectData ( iID );
	if ( !pObject )
		return;

	// lee - 130206 - switched to using glob struct as the old function tried to get the DBO function directly!
//	SaveDBOEx ( ( char* ) dwFilename, pObject );
	g_SaveObject ( dwFilename, iID );
}

bool SetupFrustumEx ( float fZDistance )
{
	D3DXMATRIX	Matrix,
				matView,
				matProj;

	if ( !g_pD3DDevice )
		return false;

	g_pD3DDevice->GetTransform ( D3DTS_PROJECTION, &matProj );
	g_pD3DDevice->GetTransform ( D3DTS_VIEW, &matView );

	D3DXMATRIX m = matView * matProj;

	  // Left clipping plane
    g_Frustum[0].a = -(m._14 + m._11);
    g_Frustum[0].b = -(m._24 + m._21);
    g_Frustum[0].c = -(m._34 + m._31);
    g_Frustum[0].d = -(m._44 + m._41);

    // Right clipping plane
    g_Frustum[1].a = -(m._14 - m._11);
    g_Frustum[1].b = -(m._24 - m._21);
    g_Frustum[1].c = -(m._34 - m._31);
    g_Frustum[1].d = -(m._44 - m._41);

    // Top clipping plane
    g_Frustum[2].a = -(m._14 - m._12);
    g_Frustum[2].b = -(m._24 - m._22);
    g_Frustum[2].c = -(m._34 - m._32);
    g_Frustum[2].d = -(m._44 - m._42);

    // Bottom clipping plane
    g_Frustum[3].a = -(m._14 + m._12);
    g_Frustum[3].b = -(m._24 + m._22);
    g_Frustum[3].c = -(m._34 + m._32);
    g_Frustum[3].d = -(m._44 + m._42);

    // Near clipping plane
    g_Frustum[4].a = -(m._13);
    g_Frustum[4].b = -(m._23);
    g_Frustum[4].c = -(m._33);
    g_Frustum[4].d = -(m._43);

    // Far clipping plane
    g_Frustum[5].a = -(m._14 - m._13);
    g_Frustum[5].b = -(m._24 - m._23);
    g_Frustum[5].c = -(m._34 - m._33);
    g_Frustum[5].d = -(m._44 - m._43);

    // Normalize the g_Frustum
    for ( ULONG i = 0; i < 6; i++ ) D3DXPlaneNormalize( &g_Frustum[i], &g_Frustum[i] );

	return true;
}

bool BoundsInFrustum ( const D3DXVECTOR3 & Min, const D3DXVECTOR3 & Max )
{
    ULONG i;
    D3DXVECTOR3 NearPoint, FarPoint, Normal;

    // Loop through all the planes
    for ( i = 0; i < 6; i++ )
    {
        // Store the plane normal
        Normal = D3DXVECTOR3( g_Frustum[i].a, g_Frustum[i].b, g_Frustum[i].c );

        if ( Normal.x > 0.0f )
        {
            if ( Normal.y > 0.0f )
            {
                if ( Normal.z > 0.0f ) 
                {
                    NearPoint.x = Min.x; NearPoint.y = Min.y; NearPoint.z = Min.z;
                
                } // End if Normal.z > 0
                else 
                {
                    NearPoint.x = Min.x; NearPoint.y = Min.y; NearPoint.z = Max.z; 
                
                } // End if Normal.z <= 0
            
            } // End if Normal.y > 0
            else
            {
                if ( Normal.z > 0.0f ) 
                {
                    NearPoint.x = Min.x; NearPoint.y = Max.y; NearPoint.z = Min.z;
                
                } // End if Normal.z > 0
                else 
                {
                    NearPoint.x = Min.x; NearPoint.y = Max.y; NearPoint.z = Max.z; 
                
                } // End if Normal.z <= 0

            } // End if Normal.y <= 0
        
        } // End if Normal.x > 0
        else
        {
            if ( Normal.y > 0.0f )
            {
                if ( Normal.z > 0.0f ) 
                {
                    NearPoint.x = Max.x; NearPoint.y = Min.y; NearPoint.z = Min.z;
                
                } // End if Normal.z > 0
                else 
                {
                    NearPoint.x = Max.x; NearPoint.y = Min.y; NearPoint.z = Max.z; 
                
                } // End if Normal.z <= 0
            
            } // End if Normal.y > 0
            else
            {
                if ( Normal.z > 0.0f ) 
                {
                    NearPoint.x = Max.x; NearPoint.y = Max.y; NearPoint.z = Min.z;
                
                } // End if Normal.z > 0
                else 
                {
                    NearPoint.x = Max.x; NearPoint.y = Max.y; NearPoint.z = Max.z; 
                
                } // End if Normal.z <= 0

            } // End if Normal.y <= 0

        } // End if Normal.x <= 0
                
        // Near extreme point is outside, and thus the
        // AABB is totally outside the frustum ?
        if ( D3DXVec3Dot( &Normal, &NearPoint ) + g_Frustum[i].d > 0.0f ) return false;

    } // Next Plane

    // Is within the frustum
    return true;
}

void UpdateTerrainNoCull ( int iNoCullMode )
{
	typedef float ( *GetPosition ) ( int );
	typedef void  ( *Position    ) ( int, float, float, float );
	typedef float ( *GetPosition ) ( int );
	typedef void  ( *Position    ) ( int, float, float, float );

	GetPosition GetXPosition;
	GetPosition GetYPosition;
	GetPosition GetZPosition;
	Position	SetPosition;

	// lee - 010806 - gdk moved 'g_pGlobForTerrain' code inside define for DLL only
	#ifndef DARKSDK_COMPILE
		GetXPosition = ( GetPosition ) GetProcAddress ( g_pGlobForTerrain->g_Camera3D, "?GetXPosition@@YAMH@Z" );
		GetYPosition = ( GetPosition ) GetProcAddress ( g_pGlobForTerrain->g_Camera3D, "?GetYPosition@@YAMH@Z" );
		GetZPosition = ( GetPosition ) GetProcAddress ( g_pGlobForTerrain->g_Camera3D, "?GetZPosition@@YAMH@Z" );
		SetPosition  = ( Position    ) GetProcAddress ( g_pGlobForTerrain->g_Camera3D, "?Position@@YAXHMMM@Z" );
	#else
		GetXPosition = dbCameraPositionX;
		GetYPosition = dbCameraPositionY;
		GetZPosition = dbCameraPositionZ;
		SetPosition  = dbPositionCamera;
	#endif

	SetupFrustumEx ( 0.0f );
	
	float fX = GetXPosition ( 0 );
	float fY = GetYPosition ( 0 );
	float fZ = GetZPosition ( 0 );

	for ( int i = 0; i < ( int ) g_Terrain.size ( ); i++ )
	{
		sObject* pObject = GetObjectData ( g_Terrain [ i ].iID );
		GetTerrainData ( g_Terrain [ i ].iID );
//		sMesh** pMeshList = g_pCurrentTerrain->pObject->ppMeshList;
		sMesh** pMeshList = pObject->ppMeshList;
		if ( pObject && g_pCurrentTerrain )
		{
			for ( int k = 0; k < pObject->iMeshCount; k++ )
			{
				D3DXVECTOR3 vecMin = pMeshList [ k ]->Collision.vecMin;
				D3DXVECTOR3 vecMax = pMeshList [ k ]->Collision.vecMax;

				vecMin += pObject->position.vecPosition;
				vecMax += pObject->position.vecPosition;

				// vis cull..
				if ( iNoCullMode==1 )
				{
					// lee - 270206 - u60 - with multiple cameras 'Culling Is Bad'
					pObject->ppMeshList [ k ]->bVisible = true;
				}
				else
				{
					if ( BoundsInFrustum ( vecMin, vecMax ) )
						pObject->ppMeshList [ k ]->bVisible = true;
					else
						pObject->ppMeshList [ k ]->bVisible = false;
				}
			}
		}
	}
}

void UpdateTerrain ( void )
{
	// cull by default
	UpdateTerrainNoCull ( 0 );
}

DWORD GetTerrainGroundHeight ( int iID, float fX, float fZ )
{
	GetTerrainData ( iID );

	// get ptr
	sObject* pObject = GetObjectData ( iID );

	// lee - 220306 - u6b4 - validate ptr can call this before terrain exists for it!
	if ( pObject==NULL )
		return 0;

	float fNewX = pObject->position.vecPosition.x * -1;
	float fNewY = pObject->position.vecPosition.y * -1;
	float fNewZ = pObject->position.vecPosition.z * -1;

	if ( g_pCurrentTerrain )
	{
		fX += fNewX;
		fZ += fNewZ;

		float fHeight = g_pCurrentTerrain->pData->GetHeight ( fX, fZ, true );

		fHeight -= fNewY;

		return *( DWORD* ) &fHeight;
	}

	return 0;
}

DWORD GetTerrainXSize ( int iID )
{
	// lee - 310306 - u6rc4 - because the whole advanced terrain grid system
	// uses coordinate steps (in a 16split of 128pixels) of 0,7,14,21 instead of 0,8,16,32, width 
	// will always report as shorter than a proper implementation (shows 126 instead of 127)
	// so for now report the basic terrain dimension
	GetTerrainData ( iID );
	sObject* pObject = GetObjectData ( iID );
	float fValue = 0.0f;
	if ( pObject )
		if ( g_pCurrentTerrain )
			fValue = (float)g_pCurrentTerrain->pData->GetTerrainWidth();
	return *( DWORD* ) &fValue;
	/*
	GetTerrainData ( iID );
	sObject* pObject = GetObjectData ( iID );

	float fValue = 0.0f;
	if ( pObject )
	{
		fValue = (pObject->collision.vecMax.x - pObject->collision.vecMin.x);
		fValue *= pObject->position.vecScale.x;
	}

// lee - 200306 - u6b4 - amazingly never worked! (see above now)
//	return 0.0f;
	return *( DWORD* ) &fValue;
	*/
}

DWORD GetTerrainZSize ( int iID )
{
	// lee - 310306 - u6rc4 - because the whole advanced terrain grid system
	// uses coordinate steps (in a 16split of 128pixels) of 0,7,14,21 instead of 0,8,16,32, width 
	// will always report as shorter than a proper implementation (shows 126 instead of 127)
	// so for now report the basic terrain dimension
	GetTerrainData ( iID );
	sObject* pObject = GetObjectData ( iID );
	float fValue = 0.0f;
	if ( pObject )
		if ( g_pCurrentTerrain )
			fValue = (float)g_pCurrentTerrain->pData->GetTerrainHeight();
	return *( DWORD* ) &fValue;
	/*
	GetTerrainData ( iID );
	sObject* pObject = GetObjectData ( iID );

	float fValue = 0.0f;
	if ( pObject )
	{
		fValue = (pObject->collision.vecMax.z - pObject->collision.vecMin.z);
		fValue *= pObject->position.vecScale.z;
	}

// lee - 200306 - u6b4 - amazingly never worked!
//	return 0.0f;
	return *( DWORD* ) &fValue;
	*/
}

// correct GDK link references
#ifdef DARKSDK_COMPILE

DARKSDK float GetTerrainXSizeEx ( int iID )
{
	DWORD dwValue = GetTerrainXSize ( iID );
	return *(float*)&dwValue;
}

DARKSDK float GetTerrainZSizeEx ( int iID )
{
	DWORD dwValue = GetTerrainZSize ( iID );
	return *(float*)&dwValue;
}

#endif