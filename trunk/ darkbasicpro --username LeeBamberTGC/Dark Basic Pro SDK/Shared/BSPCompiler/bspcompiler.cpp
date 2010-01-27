#include "bspcompiler.h"

typedef IDirect3DDevice9*		( *GFX_GetDirect3DDevicePFN ) ( void );
GFX_GetDirect3DDevicePFN		g_GFX_GetDirect3DDevice;

void Constructor ( HINSTANCE hSetup )
{
	/*
	if ( !hSetup )
	{
		hSetup = LoadLibrary ( "DBProSetupDebug.dll" );
	}

	// setup function pointers
	g_GFX_GetDirect3DDevice = ( GFX_GetDirect3DDevicePFN ) GetProcAddress ( hSetup, "?GetDirect3DDevice@@YAPAUIDirect3DDevice9@@XZ" );
	m_pD3D                  = g_GFX_GetDirect3DDevice ( );
	*/
}

void Destructor ( void )
{
	/*
		SAFE_DELETE_ARRAY ( NodeArray );
	SAFE_DELETE_ARRAY ( PlaneArray );
	SAFE_DELETE_ARRAY ( LeafArray );

	SAFE_DELETE_ARRAY ( PVSData );
	SAFE_DELETE_ARRAY ( PortalDupArray );

	PVSPORTAL* pTemp = PortalDupArray;

	while ( pTemp )
	{

	}

	DeletePolygonArray ( );
	DeletePortalArray ( );

	SAFE_DELETE ( PolygonList );
	*/

	DeletePolygonArray ( );
	free ( NodeArray );
	free ( LeafArray );
	free ( PlaneArray );
	free ( PVSData );

	DeletePortalArray ( );
	FreeDuplicatePortals ( );

	/*
	for ( int i = 0; i < NumberOfTextures; i++ )
	{
		if ( lpTextureSurface [ i ] )
			lpTextureSurface [ i ]->Release ( );
	}
	*/

	delete [ ] lpTextureSurface;
	delete [ ] TextureLUT;
	delete [ ] pTexturePolygonList;				
}

D3DLVERTEX SV ( float posx, float posy, float posz, float tu, float tv )
{
	D3DLVERTEX vertex;

	vertex.x		= posx;
	vertex.y		= posy;
	vertex.z		= posz;
	vertex.color	= D3DCOLOR_XRGB ( 255, 255, 255 );

	vertex.tu		= tu;
	vertex.tv		= tv;
	
	return vertex;
}

void CreateNewPolygon ( POLYGON** ppPolygon, int iVertexCount = 4, int iWordCount = 6 )
{
	ppPolygon [ 0 ]->Next = new POLYGON;

	*ppPolygon = ppPolygon [ 0 ]->Next;

	ppPolygon [ 0 ]->VertexList         = new D3DLVERTEX [ iVertexCount ];
	ppPolygon [ 0 ]->Indices            = new WORD       [ iWordCount   ];
	ppPolygon [ 0 ]->NumberOfVertices   = iVertexCount;
	ppPolygon [ 0 ]->NumberOfIndices    = iWordCount;
	ppPolygon [ 0 ]->Next               = NULL;
	ppPolygon [ 0 ]->BeenUsedAsSplitter = false;
}

void CalculateNormal ( POLYGON* pPolygon, bool bFlip = false )
{
	D3DXVECTOR3* vec0;
	D3DXVECTOR3* vec1;
	D3DXVECTOR3* vec2;

	vec0 = ( D3DXVECTOR3* ) &pPolygon->VertexList [ pPolygon->Indices [ 0 ] ];
	vec1 = ( D3DXVECTOR3* ) &pPolygon->VertexList [ pPolygon->Indices [ 1 ] ];
	vec2 = ( D3DXVECTOR3* ) &pPolygon->VertexList [ pPolygon->Indices [ pPolygon->NumberOfIndices - 1 ] ];

	if ( bFlip )
	{
		vec0 = ( D3DXVECTOR3* ) &pPolygon->VertexList [ pPolygon->Indices [ 0 ] ];
		vec2 = ( D3DXVECTOR3* ) &pPolygon->VertexList [ pPolygon->Indices [ 1 ] ];
		vec1 = ( D3DXVECTOR3* ) &pPolygon->VertexList [ pPolygon->Indices [ pPolygon->NumberOfIndices - 1 ] ];
	}

	D3DXVECTOR3 edge1 = ( *vec1 ) - ( *vec0 );
	D3DXVECTOR3 edge2 = ( *vec2 ) - ( *vec0 );

	D3DXVec3Cross ( &pPolygon->Normal, &edge1, &edge2 );

	D3DXVec3Normalize ( &pPolygon->Normal, &pPolygon->Normal );
}

int* GetProgress ( void )
{
	return &g_iProgress;
}

extern BOOL	m_FastPVSCompile;

//void CompileBSP ( char* szInput, char* szOutput, bool bFastVis, bool bFlipNormals, bool bRemoveFloat, int iSplits )
void CompileBSP ( char* szInput, char* szOutput, bool bFastVis, float fEpsilon, int iSplits )
{
	if ( !szInput || !szOutput )
		return;


	ReserveInitialMemoryForArrays ( );

	m_FastPVSCompile = bFastVis;

	g_EPSILON = fEpsilon;

	/*
	LPD3DXMESH				pNewMesh;
	D3DXATTRIBUTERANGE*		pAttributeTable;

	struct sMeshData
	{
		float x, y, z;
		float tu, tv;
	};

	LPD3DXBUFFER			pD3DXMtrlBuffer;
	DWORD					dwNumMaterials;
	LPD3DXMESH				pTempMesh;
	DWORD					dwAttributeTableSize;
	LPDIRECT3DVERTEXBUFFER8 pMeshVertexBuffer;
	LPDIRECT3DINDEXBUFFER8  pMeshIndexBuffer;

	sMeshData*				pMeshVertices;
	WORD*					pMeshIndices;

	/////////
	NumberOfTextures = 1;
	TextureLUT = new char [ NumberOfTextures ] [ 21 ];
	strcpy ( TextureLUT [ 0 ], "brick.bmp" );
	/////////

	D3DXLoadMeshFromX ( 
							szInput,
							D3DXMESH_SYSTEMMEM,
							m_pD3D,
							NULL,
							&pD3DXMtrlBuffer,
							&dwNumMaterials, 
							&pTempMesh
					  );

	pTempMesh->CloneMeshFVF ( 0, D3DFVF_XYZ | D3DFVF_TEX1, m_pD3D, &pNewMesh );
		
	pNewMesh->Optimize ( D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pNewMesh );
	pNewMesh->GetAttributeTable ( NULL, &dwAttributeTableSize );

	pAttributeTable = new D3DXATTRIBUTERANGE [ dwAttributeTableSize ];
	pNewMesh->GetAttributeTable ( pAttributeTable, NULL );

	DWORD dwNumVertices = pNewMesh->GetNumVertices ( );
	DWORD dwNumFaces    = pNewMesh->GetNumFaces    ( );

	pNewMesh->GetVertexBuffer ( &pMeshVertexBuffer );
	pNewMesh->GetIndexBuffer  ( &pMeshIndexBuffer );

	pMeshVertexBuffer->Lock ( 0, pNewMesh->GetNumVertices  ( ) * sizeof ( sMeshData ), ( BYTE** ) &pMeshVertices, 0 );
	pMeshIndexBuffer->Lock  ( 0, 3 * pNewMesh->GetNumFaces ( ) * sizeof ( WORD ),      ( BYTE** ) &pMeshIndices,  0 );
	
	POLYGON* pTemp = new POLYGON;

	int iInd = 0;

	sMeshData* pVert = ( sMeshData* ) pMeshVertices;
	WORD*      pInd  = ( WORD* ) pMeshIndices;

	for ( int iTemp = 0; iTemp < dwNumFaces; iTemp++ )
	{
		int iA = pInd [ iInd + 0 ];
		int iB = pInd [ iInd + 1 ];
		int iC = pInd [ iInd + 2 ];

		CreateNewPolygon ( &pTemp, 3, 3 );
		
		pTemp->VertexList [ 0 ] = SV ( pVert [ iA ].x, pVert [ iA ].y, pVert [ iA ].z, pVert [ iA ].tu, pVert [ iA ].tv );
		pTemp->VertexList [ 1 ] = SV ( pVert [ iB ].x, pVert [ iB ].y, pVert [ iB ].z, pVert [ iB ].tu, pVert [ iB ].tv );
		pTemp->VertexList [ 2 ] = SV ( pVert [ iC ].x, pVert [ iC ].y, pVert [ iC ].z, pVert [ iC ].tu, pVert [ iC ].tv );

		char szX [ 256 ];
		char szY [ 256 ];
		char szZ [ 256 ];

		sprintf ( szX, "%.1f", pTemp->VertexList [ 0 ].x );
		sprintf ( szY, "%.1f", pTemp->VertexList [ 0 ].y );
		sprintf ( szZ, "%.1f", pTemp->VertexList [ 0 ].z );

		pTemp->VertexList [ 0 ].x = atof ( szX );
		pTemp->VertexList [ 0 ].y = atof ( szY );
		pTemp->VertexList [ 0 ].z = atof ( szZ );

		sprintf ( szX, "%.1f", pTemp->VertexList [ 1 ].x );
		sprintf ( szY, "%.1f", pTemp->VertexList [ 1 ].y );
		sprintf ( szZ, "%.1f", pTemp->VertexList [ 1 ].z );

		pTemp->VertexList [ 1 ].x = atof ( szX );
		pTemp->VertexList [ 1 ].y = atof ( szY );
		pTemp->VertexList [ 1 ].z = atof ( szZ );

		sprintf ( szX, "%.1f", pTemp->VertexList [ 2 ].x );
		sprintf ( szY, "%.1f", pTemp->VertexList [ 2 ].y );
		sprintf ( szZ, "%.1f", pTemp->VertexList [ 2 ].z );

		pTemp->VertexList [ 2 ].x = atof ( szX );
		pTemp->VertexList [ 2 ].y = atof ( szY );
		pTemp->VertexList [ 2 ].z = atof ( szZ );

		if ( bRemoveFloat )
		{
			pTemp->VertexList [ 0 ].x = ( int ) pTemp->VertexList [ 0 ].x;
			pTemp->VertexList [ 0 ].y = ( int ) pTemp->VertexList [ 0 ].y;
			pTemp->VertexList [ 0 ].z = ( int ) pTemp->VertexList [ 0 ].z;

			pTemp->VertexList [ 1 ].x = ( int ) pTemp->VertexList [ 1 ].x;
			pTemp->VertexList [ 1 ].y = ( int ) pTemp->VertexList [ 1 ].y;
			pTemp->VertexList [ 1 ].z = ( int ) pTemp->VertexList [ 1 ].z;

			pTemp->VertexList [ 2 ].x = ( int ) pTemp->VertexList [ 2 ].x;
			pTemp->VertexList [ 2 ].y = ( int ) pTemp->VertexList [ 2 ].y;
			pTemp->VertexList [ 2 ].z = ( int ) pTemp->VertexList [ 2 ].z;
			
			
		}

		pTemp->Indices [ 0 ] = 0;
		pTemp->Indices [ 1 ] = 1;
		pTemp->Indices [ 2 ] = 2;
		
		CalculateNormal ( pTemp, bFlipNormals );

		pTemp->TextureIndex = 0;
		pTemp->Next         = NULL;

		if ( iTemp == 0 )
			PolygonList = pTemp;

		iInd += 3;
	}
	
	pMeshVertexBuffer->Unlock ( );
	pMeshIndexBuffer->Unlock  ( );
	*/

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FILE* fp = fopen ( "level.dat", "rb" );

	while ( !fp )
	{
		fp = fopen ( "level.dat", "rb" );
	}
	
	int iTemp = 0;
	int iLoop = 0;
	int iNumPolygons = 0;
	int iNumTextures = 0;

	char	( *szTextures ) [ 256 ];
	
	fread ( &iNumTextures, sizeof ( int ), 1, fp );

	szTextures = new char [ iNumTextures ] [ 256 ];

	NumberOfTextures = iNumTextures;

	TextureLUT = new char [ NumberOfTextures ] [ 21 ];

	for ( iLoop = 0; iLoop < iNumTextures; iLoop++ )
	{
		fread ( szTextures [ iLoop ], sizeof ( char ) * 256, 1, fp );

		strcpy ( TextureLUT [ iLoop ], szTextures [ iLoop ] );
	}
	
	fread ( &iNumPolygons, sizeof ( int ), 1, fp );

	POLYGON* pTemp = new POLYGON;

	for ( iLoop = 0; iLoop < iNumPolygons; iLoop++ )
	{
		WORD iVertexCount;
		WORD iIndexCount;
		int iTextureIndex;

		fread ( &iVertexCount,  sizeof ( WORD ), 1, fp );
		fread ( &iIndexCount,   sizeof ( WORD ), 1, fp );
		fread ( &iTextureIndex, sizeof ( int ),  1, fp );

		float nx, ny, nz;
		fread ( &nx,  sizeof ( float ), 1, fp );
		fread ( &ny,  sizeof ( float ), 1, fp );
		fread ( &nz,  sizeof ( float ), 1, fp );
		
		CreateNewPolygon ( &pTemp, iVertexCount, iIndexCount );

		if ( iTemp == 0 )
			PolygonList = pTemp;

		
		
		pTemp->Normal.x = nx;
		pTemp->Normal.y = ny;
		pTemp->Normal.z = nz;

			//pTemp->VertexList [ 0 ] = SetupVertex ( pVertex [ iV ].x,  pVertex [ iV ].y, pVertex [ iV ].z, D3DCOLOR_XRGB  ( 255, 255, 255 ),  1.22301,  0.0 );
			//pTemp->VertexList [ 1 ] = SetupVertex ( pVertex [ iV ].x,  pVertex [ iV ].y, pVertex [ iV ].z, D3DCOLOR_XRGB  ( 255, 255, 255 ), -0.741755, 0.0 );
			//pTemp->VertexList [ 2 ] = SetupVertex ( pVertex [ iV ].x,  pVertex [ iV ].y, pVertex [ iV ].z, D3DCOLOR_XRGB  ( 255, 255, 255 ), -0.741755, 1.08281 );

		for ( iTemp = 0; iTemp < iVertexCount; iTemp++ )
		{
			float x, y, z, tu, tv;

			fread ( &x,  sizeof ( float ), 1, fp );
			fread ( &y,  sizeof ( float ), 1, fp );
			fread ( &z,  sizeof ( float ), 1, fp );
			fread ( &tu, sizeof ( float ), 1, fp );
			fread ( &tv, sizeof ( float ), 1, fp );

			pTemp->VertexList [ iTemp ] = SV ( x, y, z, tu, tv );
			//pTemp->TextureIndex = 0;
			pTemp->TextureIndex = iTextureIndex;
		}

		for ( iTemp = 0; iTemp < iIndexCount; iTemp++ )
		{
			WORD w;

			fread ( &w, sizeof ( WORD ), 1, fp );

			pTemp->Indices [ iTemp ] = w;
		}

		//CalculateNormal ( pTemp );
	}

	fclose ( fp );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	NumberOfTextures = 1;
	//TextureLUT = new char [ NumberOfTextures ] [ 21 ];
	//strcpy ( TextureLUT [ 0 ], "brick.bmp" );

	BuildBspTree ( 0, PolygonList );
	BuildPortals ( );
	CalculatePVS ( );

	//NumberOfTextures = 1;
	//delete [ ] TextureLUT;
	//TextureLUT = new char [ NumberOfTextures ] [ 21 ];

	//strcpy ( TextureLUT [ 0 ], "brick.bmp" );

	SaveBSPTree ( szOutput );

	SAFE_DELETE_ARRAY ( szTextures );
	SAFE_DELETE_ARRAY ( TextureLUT );
}

void CompileBSPEx ( char* szInput, char* szOutput, int iFastVis, float fEpsilon, int iSplits )
{
	if ( !szInput || !szOutput )
		return;

	bool bFastVis=false;;
	if(iFastVis==1) bFastVis=true;
	CompileBSP ( szInput, szOutput, bFastVis, fEpsilon, iSplits );
}

void SaveBSPTree ( char* szFilename )
{
	long	a;
	FILE*	stream = fopen ( szFilename, "w+b" );

	NumberOfNodes++;
	fwrite ( &NumberOfNodes, sizeof ( long ), 1, stream );
	
	NODE* n = NodeArray;

	for ( a = 0; a < NumberOfNodes; a++ )
	{
		fwrite ( &n->IsLeaf,sizeof ( unsigned char ), 1, stream );
		fwrite ( &n->Plane, sizeof ( unsigned long ), 1, stream );
		fwrite ( &n->Front, sizeof ( unsigned long ), 1, stream );
		fwrite ( &n->Back,  sizeof ( signed long   ), 1, stream );
		n++;
	}

	fwrite ( &NumberOfPlanes, sizeof ( long  ), 1,              stream );	
	fwrite ( PlaneArray,      sizeof ( PLANE ), NumberOfPlanes, stream );

	fwrite ( &NumberOfLeafs, sizeof ( long ), 1, stream );
	
	LEAF* l = LeafArray;

	for  ( a = 0; a < NumberOfLeafs; a++ )
	{
		fwrite ( &l->StartPolygon, sizeof ( long        ), 1, stream );
		fwrite ( &l->EndPolygon,   sizeof ( long        ), 1, stream );
		fwrite ( &l->PVSIndex,     sizeof ( long        ), 1, stream );
		fwrite ( &l->BoundingBox,  sizeof ( BOUNDINGBOX ), 1, stream );
		l++;
	}

	fwrite ( &NumberOfPolygons, sizeof ( long ), 1, stream );
	
	POLYGON* p = PolygonArray;

	for ( a = 0; a < NumberOfPolygons; a++ )
	{
		fwrite ( &p->NumberOfVertices, sizeof ( WORD        ), 1,                   stream );
		fwrite (  p->VertexList,       sizeof ( D3DLVERTEX  ), p->NumberOfVertices, stream );
		fwrite ( &p->NumberOfIndices,  sizeof ( WORD        ), 1,                   stream );
		fwrite (  p->Indices,          sizeof ( WORD        ), p->NumberOfIndices,  stream );
		fwrite ( &p->Normal,           sizeof ( D3DXVECTOR3 ), 1,                   stream );
		fwrite ( &p->TextureIndex,     sizeof ( WORD        ), 1,                   stream );
		p++;
	}

	fwrite ( &PVSCompressedSize, sizeof ( long ), 1,                 stream );
	fwrite ( PVSData,            sizeof ( BYTE ), PVSCompressedSize, stream );

	
	fwrite( &NumberOfTextures, sizeof ( WORD ),      1,                stream );
	fwrite( TextureLUT,        sizeof ( char ) * 21, NumberOfTextures, stream );

	fclose ( stream );
}