
//
// CObjectManager Functions Implementation
//

//////////////////////////////////////////////////////////////////////////////////
// INCLUDE COMMON ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include "CommonC.h"

//////////////////////////////////////////////////////////////////////////////////
// GLOBAL STENCIL BUFFER /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// Shadow Globals
#define D3DFVF_SHADOWVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )
struct SHADOWVERTEX
{
	D3DXVECTOR4 p;
	D3DCOLOR    color;
};

// Reflection Plane Globals
DBPRO_GLOBAL bool							g_bNormalizeToggle				= false;
DBPRO_GLOBAL D3DXMATRIX						g_matReflectionViewSaved;
DBPRO_GLOBAL D3DXPLANE						g_plnReflectionPlane;
DBPRO_GLOBAL D3DXVECTOR3					g_vecReflectionPoint;
DBPRO_GLOBAL bool							g_bReflectiveClipping			= false;

// 301008 - Saver - New Shadow Clipping feature
int											g_iShadowClipping				= 0;
float										g_fShadowClippingMin			= 0;
float										g_fShadowClippingMax			= 0;

// U71 - 061208 - can prepare scene in two places now, so need flag to keeps tabs on it
bool										g_bScenePrepared				= false;
bool										g_bRenderVeryEarlyObjects		= false;

// U74 - 120409 - during LOD QUAD transition, use ZBIAS to move quad out of way when 3D fades in/out by THIS amount in total
float										g_fZBiasEpsilon					= 0.0005f;

//////////////////////////////////////////////////////////////////////////////////
// MANAGER FUNCTIONS /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// IMPORTANT - COMPILER SWITCH "/EHa" must be in for the "try", "catch" code

namespace
{
	// u74b7
    // Structures for sorting using the STL sort
    // Used for sorting the m_ppSortedObjectList, m_pDepthSortedList
    struct OrderByReverseCameraDistance
    {
        bool operator()(sObject* pObjectA, sObject* pObjectB)
        {
            if (pObjectA->position.fCamDistance > pObjectB->position.fCamDistance)
                return true;
            if (pObjectA->position.fCamDistance == pObjectB->position.fCamDistance)
                return (pObjectA->dwObjectNumber < pObjectB->dwObjectNumber);
            return false;
        }
    };
    // u74b8
    struct OrderByTexture
    {
        bool operator()(sObject* pObjectA, sObject* pObjectB)
        {
            int iImageA = 0;
            int iImageB = 0;

            // Calculate order by object number now,
            // just in case it's needed and to avoid getting incorrect results
            // if instances are involved.
            bool bObjectOrder = pObjectA->dwObjectNumber < pObjectB->dwObjectNumber;

            // Get the image id for object a
		    if ( pObjectA->pInstanceOfObject )
			    pObjectA = pObjectA->pInstanceOfObject;
		    if ( pObjectA->ppMeshList && pObjectA->ppMeshList [ 0 ]->pTextures)
                iImageA = pObjectA->ppMeshList [ 0 ]->pTextures [ 0 ].iImageID;

            // Get the image id for object b
		    if ( pObjectB->pInstanceOfObject )
			    pObjectB = pObjectB->pInstanceOfObject;
		    if ( pObjectB->ppMeshList && pObjectB->ppMeshList [ 0 ]->pTextures)
                iImageB = pObjectB->ppMeshList [ 0 ]->pTextures [ 0 ].iImageID;

            if (iImageA < iImageB)
                return true;
            if (iImageA > iImageB)
                return false;

            // Same images, so order by object id
            return bObjectOrder;
        }
    };
    struct OrderByObject
    {
        bool operator()(sObject* pObjectA, sObject* pObjectB)
        {
            return (pObjectA->dwObjectNumber < pObjectB->dwObjectNumber);
        }
    };
}

// mike - 281106 - move these into cpp file for debugging
CObjectManager::sVertexData::sVertexData ( )
{
	memset ( this, 0, sizeof(sVertexData) );
}

CObjectManager::sVertexData::~sVertexData ( )
{
	SAFE_RELEASE ( pVB );

	// mike - 291106 - deleting this can cause a buffer overrun, is this somehow
	//				 - pointing to an invalid area
	SAFE_DELETE  ( pNext );
}

CObjectManager::sIndexData::sIndexData ( )
{
	memset ( this, 0, sizeof(sIndexData) );
}

CObjectManager::sIndexData::~sIndexData ( )
{
	SAFE_RELEASE ( pIB );

	// mike - 291106 - deleting this can cause a buffer overrun, is this somehow
	//				 - pointing to an invalid area
	SAFE_DELETE  ( pNext );
}

bool CObjectManager::UpdateObjectListSize ( int iSize )
{
	// if list count is larger than size passed in, we can ignore a resize
	if ( iSize < m_iListCount )
		return true;

	// safely delete any of the arrays
	SAFE_DELETE_ARRAY ( m_ppSortedObjectVisibleList );
	SAFE_DELETE_ARRAY ( m_ppSortedObjectList );
	SAFE_DELETE_ARRAY ( m_pbMarkedList              );

	// allocate memory
	m_ppSortedObjectVisibleList = new sObject* [ iSize ];
	m_ppSortedObjectList        = new sObject* [ iSize ];
	m_pbMarkedList              = new bool     [ iSize ];

	// safety checks on new memory
	SAFE_MEMORY ( m_ppSortedObjectVisibleList );
	SAFE_MEMORY ( m_ppSortedObjectList );
	SAFE_MEMORY ( m_pbMarkedList              );

	// set all pointers to null
	for ( int iArrayIndex = 0; iArrayIndex < iSize; iArrayIndex++ )
	{
		m_ppSortedObjectVisibleList [ iArrayIndex ] = NULL;
		m_ppSortedObjectList        [ iArrayIndex ] = NULL;
		m_pbMarkedList              [ iArrayIndex ] = false;
	}

	// store the size of the list
	m_iListCount = iSize;

	// return back
	return true;
}

bool CObjectManager::Setup ( void )
{
	// clear manager members
    // u74b8 - do this the correct & safe way so that embedded classes will work.
    //memset ( this, 0, sizeof ( CObjectManager ) );

	// Render State Global Defaults
    memset( &m_RenderStates, 0, sizeof( m_RenderStates ) );
	m_RenderStates.dwGlobalCullDirection = D3DCULL_CCW;

	// set all pointers to null
	m_ppCurrentVBRef	= NULL;
	m_ppLastVBRef		= NULL;
	m_ppCurrentIBRef	= NULL;
	m_ppLastIBRef		= NULL;

	m_dwCurrentShader = 0;
	m_dwCurrentFVF = 0;
	m_dwLastShader = 0;
	m_dwLastFVF = 0;

	m_iCurrentTexture = 0;
	m_iLastTexture = 0;
	m_dwLastTextureCount = 0;
	m_bUpdateTextureList = 0;
	m_bUpdateVertexDecs = 0;
	m_bUpdateStreams = 0;
	m_iSortedObjectCount = 0;
	m_iLastCount = 0;
	m_iListCount = 0;
	m_iVisibleObjectCount = 0;
	m_pbMarkedList = 0;
	m_ppSortedObjectList = 0;
	m_ppSortedObjectVisibleList = 0;
	m_pVertexDataList = 0;
	m_pIndexDataList = 0;

	// STENCIL State Defaults
    memset( &m_StencilStates, 0, sizeof( m_StencilStates ) );
	m_StencilStates.dwShadowShades = 1;
	m_StencilStates.dwShadowStrength = 64;
	m_StencilStates.dwShadowColor = D3DCOLOR_ARGB ( 0, 0, 0, 0 );
	m_StencilStates.dwReflectionColor = D3DCOLOR_ARGB ( 64, 0, 0, 0 );

    // clear shadow VB ptr
	m_pSquareVB			= NULL;

	// post process shading using QUAD
	m_pQUAD					= NULL;
	m_pQUADDefaultEffect	= NULL;

	// Reset global stadows
	m_bGlobalShadows	= true;

    g_bObjectReplacedUpdateBuffers = false;

    m_pCamera = 0;
	// all okay
	return true;
}

void DeleteEx ( int iID );

bool CObjectManager::Free ( void )
{
	// 291106 - mike - delete all objects manually
	if ( g_ObjectList )
	{
		// lee - 140307 - ensure 'g_ObjectList' as Free is also called by Objectmanager destructor
		for ( int i = 0; i < g_iObjectListCount; i++ )
		{
			DeleteEx ( i );
		}
	}	

	// safely delete any arrays and objects
	SAFE_DELETE ( m_pVertexDataList );
	SAFE_DELETE ( m_pIndexDataList );
	SAFE_DELETE_ARRAY ( m_pbMarkedList );
	SAFE_DELETE_ARRAY ( m_ppSortedObjectList );
	SAFE_DELETE_ARRAY ( m_ppSortedObjectVisibleList );

	// free shadow VB and QUAD mesh
	SAFE_RELEASE ( m_pSquareVB );
	SAFE_DELETE ( m_pQUAD );
	SAFE_DELETE ( m_pQUADDefaultEffect );

	// all okay
	return true;
}

//
// VERTEX AND INDEX BUFFERS
//

CObjectManager::sIndexData* CObjectManager::FindIndexBuffer ( DWORD dwIndexCount, bool bUsesItsOwnBuffers )
{
	// find an index buffer which we wan use

	// check D3D device is valid
	if ( !m_pD3D )
		return NULL;

	// make sure the parameter is valid
	if ( dwIndexCount < 1 )
		return NULL;

	// local variables
	bool		 bMatch = false;
	sIndexData*  pIndexData  = m_pIndexDataList;

	// get device capabilities
	D3DCAPS9 caps;
	m_pD3D->GetDeviceCaps ( &caps );

	// no search if need to use its own buffer
	if ( bUsesItsOwnBuffers )
		pIndexData=NULL;

	// run through all nodes in list
	while ( pIndexData )
	{
		// see if we can fit the data into the buffer
		if ( pIndexData->dwCurrentIndexCount + dwIndexCount < pIndexData->dwMaxIndexCount )
		{
			bMatch = true;
			break;
		}

		// move to next node
		pIndexData = pIndexData->pNext;
	}

	// if we don't have a match then create a new item
	if ( !bMatch )
	{
		if ( !m_pIndexDataList )
		{
			// create new list
			m_pIndexDataList	= new sIndexData;
			pIndexData			= m_pIndexDataList;

			// ensure creation okay
			SAFE_MEMORY ( m_pIndexDataList );
		}
		else
		{
			// find end of list
			pIndexData = m_pIndexDataList;
			while ( pIndexData )
			{
				if ( pIndexData->pNext )
				{
					pIndexData = pIndexData->pNext;
					continue;
				}
				else
					break;
			}

			// add new item to list
			pIndexData->pNext = new sIndexData ( );
			pIndexData        = pIndexData->pNext;
		}

		// create a decent start size of the IB
		DWORD dwIndexBufferSize = caps.MaxVertexIndex;

		// own buffer needs only to be the size of the data
		if ( bUsesItsOwnBuffers )
			dwIndexBufferSize = dwIndexCount;

		// loop until succeed in creating a IB
		bool  bCreate  = true;
		while ( bCreate )
		{
			// attempt to create a IB
			if ( FAILED ( m_pD3D->CreateIndexBuffer ( 
														sizeof ( WORD ) * dwIndexBufferSize,
														D3DUSAGE_WRITEONLY,
														D3DFMT_INDEX16,
														//D3DPOOL_DEFAULT, 250105 - FPSC-EA - large use of objects causes crash!
														//D3DPOOL_MANAGED, 270105 - perhaps managed takes too long to swapin/out
														D3DPOOL_DEFAULT,
														&pIndexData->pIB,
														NULL
													 ) ) )
			{
				// failed, try half the size
				dwIndexBufferSize /= 2;
			}
			else
			{
				// only if IB can hold required vertex data
				if ( dwIndexBufferSize >= dwIndexCount )
				{
					// success, we can use this size
					bCreate = false;
				}
				else
				{
					// IB created, but just too small!
					SAFE_RELEASE(pIndexData->pIB);
					return NULL;
				}
			}

			// if we continue until a ridiculously low value, we must fail
			if ( dwIndexBufferSize <= 0 )
				return NULL;
		}

		// ensure can fit inside the max size of a buffer
		if ( dwIndexCount > dwIndexBufferSize )
		{
			// if not, index data cannot fit inside single IB!
			SAFE_RELEASE ( pIndexData->pIB );
			return NULL;
		}

		// save the format of the buffer and the number allowed
		pIndexData->dwMaxIndexCount = dwIndexBufferSize;
		pIndexData->dwCurrentIndexCount = 0;
	}

	// return the final buffer
	return pIndexData;
}

CObjectManager::sVertexData* CObjectManager::FindVertexBuffer ( DWORD dwFVF, LPDIRECT3DVERTEXDECLARATION9 pVertexDec, DWORD dwSize, DWORD dwVertexCount, DWORD dwIndexCount, bool bUsesItsOwnBuffers, int iType )
{
	// we need to find a buffer which the objects data can be added into
	// this function will go through the list of all buffers and find a
	// match for the FVF

	// check D3D device is valid
	if ( !m_pD3D ) return NULL;

	// make sure we have a FVF mode
	if ( dwFVF==0 && pVertexDec==NULL ) return NULL;

	// make sure the parameters are valid
	if ( dwSize < 1 || dwVertexCount < 1 ) return NULL;

	// local variables
	bool			bMatch			= false;
	sVertexData*	pVertexData		= m_pVertexDataList;

	// get device capabilities
	D3DCAPS9 caps;
	m_pD3D->GetDeviceCaps ( &caps );

	// make sure primitive count can be achieved
	DWORD dwPrimCountMax = caps.MaxPrimitiveCount;
	if ( dwIndexCount>0 )
	{
		if ( dwIndexCount/3 > dwPrimCountMax )
			return NULL;
	}
	else
	{
		if ( dwVertexCount/3 > dwPrimCountMax )
			return NULL;
	}

	// no search if need to use its own buffer
	if ( bUsesItsOwnBuffers )
		pVertexData=NULL;

	// run through all nodes in list
	while ( pVertexData )
	{
		// check if vertex declarations match
		bool bVertDecMatch = false;
		if ( pVertexData->dwFormat==0 )
		{
			UINT numElementsThis;
			D3DVERTEXELEMENT9 VertexDecFromThisBuffer[256];
			if ( pVertexData->pVertexDec )
			{
				HRESULT hr = pVertexData->pVertexDec->GetDeclaration( VertexDecFromThisBuffer, &numElementsThis);
				UINT numElementsRequest;
				D3DVERTEXELEMENT9 VertexDecFromRequest[256];
				if ( pVertexDec )
				{
					hr = pVertexDec->GetDeclaration( VertexDecFromRequest, &numElementsRequest);
					DWORD dwCMPSize = sizeof(D3DVERTEXELEMENT9)*numElementsRequest;
					if ( numElementsRequest == numElementsThis )
						if ( memcmp ( VertexDecFromRequest, VertexDecFromThisBuffer, dwCMPSize )==0 )
							bVertDecMatch = true;
				}
			}
		}
		else
			bVertDecMatch = true;

		// see if we find a match to the FVF
		if ( pVertexData->dwFormat == dwFVF && bVertDecMatch==true )
		{
			// see if we can fit the data into the buffer
			if ( pVertexData->dwCurrentVertexCount + dwVertexCount < pVertexData->dwMaxVertexCount )
			{
				bMatch = true;
				break;
			}
		}

		// move to next node
		pVertexData = pVertexData->pNext;
	}

	// if we don't have a match then create a new VB
	if ( !bMatch )
	{
		if ( !m_pVertexDataList )
		{
			// create new list
			m_pVertexDataList		= new sVertexData;
			pVertexData				= m_pVertexDataList;
			SAFE_MEMORY ( m_pVertexDataList );
		}
		else
		{
			// find end of list
			pVertexData = m_pVertexDataList;
			while ( pVertexData )
			{
				if ( pVertexData->pNext )
				{
					pVertexData = pVertexData->pNext;
					continue;
				}
				else
					break;
			}

			// add new item to list
			pVertexData->pNext = new sVertexData ( );
			pVertexData        = pVertexData->pNext;
		}

		// create a decent start size of the VB
		bool bCreate = true;
		DWORD dwVBSize = caps.MaxVertexIndex;

		// if size exceeds 16bit, make max size 16bit (32bit index supported maybe in future though it shows no speed increase!!)
		if ( dwIndexCount > 0 )
		{
			// mesh uses index buffer so can only have a 16bit vertex buffer
			if ( dwVBSize > 0x0000FFFF ) dwVBSize = 0x0000FFFF;
		}

		// own buffer needs only to be the size of the data
		if ( bUsesItsOwnBuffers )
			dwVBSize = dwVertexCount;

		// loop until succeed in creating a VB
		while ( bCreate )
		{
			DWORD dwUsage = D3DUSAGE_WRITEONLY;

			if ( iType == D3DPT_POINTLIST )
				dwUsage = D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS;

			// attempt to create a VB
			if ( FAILED ( m_pD3D->CreateVertexBuffer ( 
														dwSize * dwVBSize,
														dwUsage,
														dwFVF,
														//D3DPOOL_DEFAULT, 250105 - FPSC-EA - large use of objects causes crash!
														//D3DPOOL_MANAGED, 270105 - perhaps managed takes too long to swapin/out
														D3DPOOL_DEFAULT,
														&pVertexData->pVB,
														NULL
												     ) ) )
			{
				// failed, try half the size
				dwVBSize /= 2;
			}
			else
			{
				// only if VB can hold required vertex data
				if ( dwVBSize >= dwVertexCount )
				{
					// success, we can use this size
					bCreate = false;
				}
				else
				{
					// VB created, but just too small!
					SAFE_RELEASE(pVertexData->pVB);
					return NULL;
				}
			}

			// if we continue until a ridiculously low value, we must fail
			if ( dwVBSize <= 0 )
				return NULL;
		}

		// save the format of the VB and the number of vertices allowed
		pVertexData->dwFormat					= dwFVF;
		pVertexData->pVertexDec					= pVertexDec;
		pVertexData->dwMaxVertexCount			= dwVBSize;
		pVertexData->dwCurrentVertexCount		= 0;
	}

	// return the final buffer
	return pVertexData;
}

bool CObjectManager::AddObjectMeshToBuffers ( sMesh* pMesh, bool bUsesItsOwnBuffers )
{
	// vertex and index buffer set up
	WORD*		 pIndices    = NULL;
	sVertexData* pVertexData = NULL;
	sIndexData*	 pIndexData  = NULL;

	// find a vertex buffer we can use which matches the FVF component
	pVertexData = this->FindVertexBuffer (	pMesh->dwFVF,
											pMesh->pVertexDec,
											pMesh->dwFVFSize,
											pMesh->dwVertexCount,						
											pMesh->dwIndexCount,
											bUsesItsOwnBuffers,
											pMesh->iPrimitiveType
										); // if no indexbuffer, can make larger vertex buffer (16bit index only)

	// check the vertex buffer is valid
	if ( pVertexData==NULL )
	{
		pMesh->pDrawBuffer = NULL;
		return false;
	}

	// find an index buffer (if one is required)
	if ( pMesh->dwIndexCount> 0 )
	{
		// find and check the index buffer is valid
		pIndexData = this->FindIndexBuffer ( pMesh->dwIndexCount, bUsesItsOwnBuffers );
		SAFE_MEMORY ( pIndexData );
	}

	// create a new vertex buffer reference array
	SAFE_DELETE ( pMesh->pDrawBuffer );
	pMesh->pDrawBuffer = new sDrawBuffer;

	// check the reference array is okay
	SAFE_MEMORY ( pMesh->pDrawBuffer );

	// draw primitive type
	pMesh->pDrawBuffer->dwPrimType			= ( D3DPRIMITIVETYPE ) pMesh->iPrimitiveType;

	// store a reference to the vertex buffer
	pMesh->pDrawBuffer->pVertexBufferRef	= pVertexData->pVB;
	pMesh->pDrawBuffer->dwVertexStart		= ( pVertexData->dwPosition * sizeof(float) ) / pMesh->dwFVFSize;
	pMesh->pDrawBuffer->dwVertexCount		= pMesh->iDrawVertexCount;

	// primitive count for drawing
	pMesh->pDrawBuffer->dwPrimitiveCount	= pMesh->iDrawPrimitives;

	// store a reference to the indice buffer
	if( pIndexData )
	{
		pMesh->pDrawBuffer->pIndexBufferRef		= pIndexData->pIB;
		pMesh->pDrawBuffer->dwIndexStart		= pIndexData->dwCurrentIndexCount;
	}
	else
	{
		pMesh->pDrawBuffer->pIndexBufferRef		= NULL;
		pMesh->pDrawBuffer->dwIndexStart		= 0;
	}

	// store a reference to the FVF size
	pMesh->pDrawBuffer->dwFVFSize			= pMesh->dwFVFSize;

	// store references to the VB and IB list items
	pMesh->pDrawBuffer->pVBListEntryRef		= (LPVOID)pVertexData;
	pMesh->pDrawBuffer->pIBListEntryRef		= (LPVOID)pIndexData;

	// copy the indices (if applicable)
	if ( pIndexData )
	{
		// update base vertex index for indices (and adjust vertex start accordingly)
		pMesh->pDrawBuffer->dwBaseVertexIndex	 = ( WORD ) pVertexData->dwCurrentVertexCount;
		pMesh->pDrawBuffer->dwVertexStart		-= ( WORD ) pMesh->pDrawBuffer->dwBaseVertexIndex;

		// copy index data to index buffer (WORD based)
		CopyMeshDataToIndexBuffer ( pMesh, pIndexData->pIB, pIndexData->dwCurrentIndexCount );

		// increment the index count
		pIndexData->dwCurrentIndexCount += pMesh->dwIndexCount;
	}

	// copy vertex data to vertex buffer
	CopyMeshDataToVertexBufferSameFVF ( pMesh, pVertexData->pVB, pVertexData->dwPosition );

	// save the current position
	pVertexData->dwPosition += pMesh->dwVertexCount * ( pMesh->dwFVFSize/sizeof(float) );

	// increment the vertex count in the buffer
	pVertexData->dwCurrentVertexCount += pMesh->dwVertexCount;

	// everything went okay
	return true;
}

bool CObjectManager::AddObjectToBuffers ( sObject* pObject )
{
	// vertex and index buffer set up
	bool			bAllOkay		= true;
	WORD*			pIndices		= NULL;
	sVertexData*	pVertexData		= NULL;
	sIndexData*		pIndexData		= NULL;

	// run through each frame within an object
	for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
	{
		// get frame ptr
		sFrame* pFrame = pObject->ppFrameList [ iFrame ];

		// add each mesh to the buffers
		if ( pFrame->pMesh )		if ( !AddObjectMeshToBuffers ( pFrame->pMesh, pObject->bUsesItsOwnBuffers ) )				bAllOkay=false;
		if ( pFrame->pShadowMesh )	if ( !AddObjectMeshToBuffers ( pFrame->pShadowMesh, pObject->bUsesItsOwnBuffers ) )			bAllOkay=false;
		if ( pFrame->pBoundBox )	if ( !AddObjectMeshToBuffers ( pFrame->pBoundBox, pObject->bUsesItsOwnBuffers ) )			bAllOkay=false;
		if ( pFrame->pBoundSphere )	if ( !AddObjectMeshToBuffers ( pFrame->pBoundSphere, pObject->bUsesItsOwnBuffers ) )		bAllOkay=false;
		if ( pFrame->pLOD[0] )		if ( !AddObjectMeshToBuffers ( pFrame->pLOD[0], pObject->bUsesItsOwnBuffers ) )				bAllOkay=false;
		if ( pFrame->pLOD[1] )		if ( !AddObjectMeshToBuffers ( pFrame->pLOD[1], pObject->bUsesItsOwnBuffers ) )				bAllOkay=false;
		if ( pFrame->pLODForQUAD )	if ( !AddObjectMeshToBuffers ( pFrame->pLODForQUAD, pObject->bUsesItsOwnBuffers ) )				bAllOkay=false;
	}

	// update texture list when introduce new object
	m_bUpdateTextureList=true;

	// everything went..
	return bAllOkay;
}


bool CObjectManager::FlagAllObjectMeshesUsingBuffer ( sVertexData* pVertexData, sIndexData* pIndexData )
{
	// flag any object mesh that uses either of these buffers
	for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
	{
		// get index from shortlist
		int iObjectID = g_ObjectListRef [ iShortList ];

		// see if we have a valid list
		sObject* pObject = g_ObjectList [ iObjectID ];
		if ( !pObject )
			continue;

		// run through each frame within an object
		for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
		{
			// get frame ptr
			sFrame* pFrame = pObject->ppFrameList [ iFrame ];

			// add this object:mesh back in
			if ( pFrame->pMesh )
			{
				if ( pFrame->pMesh->pDrawBuffer )
				{
					if(pFrame->pMesh->pDrawBuffer->pVBListEntryRef==(LPVOID)pVertexData
					|| pFrame->pMesh->pDrawBuffer->pIBListEntryRef==(LPVOID)pIndexData )
					{
						// leefix - 070403 - ensure associated meshes are also removed from buffers to prevent duplication!
						if ( pFrame->pMesh->bAddObjectToBuffers==false )
						{
							pFrame->pMesh->bAddObjectToBuffers=true;
							// mike - 301106 - add flag to stop recursion
							RemoveBuffersUsedByObjectMesh ( pFrame->pMesh, false );
						}
					}
				}
			}
			if ( pFrame->pShadowMesh )
			{
				if ( pFrame->pShadowMesh->pDrawBuffer )
				{
					if(pFrame->pShadowMesh->pDrawBuffer->pVBListEntryRef==(LPVOID)pVertexData
					|| pFrame->pShadowMesh->pDrawBuffer->pIBListEntryRef==(LPVOID)pIndexData )
					{
						// leefix - 070403 - ensure associated meshes are also removed from buffers to prevent duplication!
						if ( pFrame->pShadowMesh->bAddObjectToBuffers==false )
						{
							pFrame->pShadowMesh->bAddObjectToBuffers=true;
							// mike - 301106 - add flag to stop recursion
							RemoveBuffersUsedByObjectMesh ( pFrame->pShadowMesh, false );
						}
					}
				}
			}
			if ( pFrame->pBoundBox )
			{
				if ( pFrame->pBoundBox->pDrawBuffer )
				{
					if(pFrame->pBoundBox->pDrawBuffer->pVBListEntryRef==(LPVOID)pVertexData
					|| pFrame->pBoundBox->pDrawBuffer->pIBListEntryRef==(LPVOID)pIndexData )
					{
						// leefix - 070403 - ensure associated meshes are also removed from buffers to prevent duplication!
						if ( pFrame->pBoundBox->bAddObjectToBuffers==false )
						{
							pFrame->pBoundBox->bAddObjectToBuffers=true;
							// mike - 301106 - add flag to stop recursion
							RemoveBuffersUsedByObjectMesh ( pFrame->pBoundBox, false );
						}
					}
				}
			}	
			if ( pFrame->pBoundSphere )
			{
				if ( pFrame->pBoundSphere->pDrawBuffer )
				{
					if(pFrame->pBoundSphere->pDrawBuffer->pVBListEntryRef==(LPVOID)pVertexData
					|| pFrame->pBoundSphere->pDrawBuffer->pIBListEntryRef==(LPVOID)pIndexData )
					{
						// leefix - 070403 - ensure associated meshes are also removed from buffers to prevent duplication!
						if ( pFrame->pBoundSphere->bAddObjectToBuffers==false )
						{
							pFrame->pBoundSphere->bAddObjectToBuffers=true;
							// mike - 301106 - add flag to stop recursion
							RemoveBuffersUsedByObjectMesh ( pFrame->pBoundSphere, false );
						}
					}
				}
			}	
			for ( int l=0; l<3; l++ )
			{
				sMesh* pLODMesh = NULL;
				if ( l<2 )
					pLODMesh = pFrame->pLOD[l];
				else
					pLODMesh = pFrame->pLODForQUAD;

				if ( pLODMesh )
				{
					if ( pLODMesh->pDrawBuffer )
					{
						if(pLODMesh->pDrawBuffer->pVBListEntryRef==(LPVOID)pVertexData
						|| pLODMesh->pDrawBuffer->pIBListEntryRef==(LPVOID)pIndexData )
						{
							if ( pLODMesh->bAddObjectToBuffers==false )
							{
								pLODMesh->bAddObjectToBuffers=true;
								RemoveBuffersUsedByObjectMesh ( pLODMesh, false );
							}
						}
					}
				}	
			}
		}
	}

	// everything went okay
	return true;
}

// mike - 301106 - add flag to stop recursion
bool CObjectManager::RemoveBuffersUsedByObjectMesh ( sMesh* pMesh, bool bRecurse )
{
	// get reference to drawbuffer
	sDrawBuffer* pDrawBuffer = pMesh->pDrawBuffer;
	if(pDrawBuffer)
	{
		DWORD* pdwAdd = (DWORD*)&pMesh->pDrawBuffer;

		// get reference to VB and IB ptrs
		sVertexData* pVertexData = (sVertexData*)pDrawBuffer->pVBListEntryRef;
		sIndexData*	 pIndexData  = (sIndexData* )pDrawBuffer->pIBListEntryRef;

		// scan for and delete vertex item
		sVertexData* pLastVertexData = NULL;
		sVertexData* pFindVertexData = m_pVertexDataList;
		while ( pFindVertexData )
		{
			// check this item
			sVertexData* pNextVertexData = pFindVertexData->pNext;
			if ( pFindVertexData==pVertexData )
			{
				// sever and delete vertex item
				pFindVertexData->pNext=NULL;

				// free VB
				SAFE_RELEASE( pFindVertexData->pVB );

				// delete vertexdata
				SAFE_DELETE( pFindVertexData );

				// adjust next value to leap deleted item
				if ( pLastVertexData ) pLastVertexData->pNext = pNextVertexData;

				// new start item to replace deleted one
				if ( m_pVertexDataList==pVertexData )
					m_pVertexDataList = pNextVertexData;

				// mike - 281106 - pVertexData points to pFindVertexData, we have just
				//				 - removed pFindVertexData but pVertexData still points
				//				 - to it, later on we call FlagAllObjectMeshesUsingBuffer passing
				//				 - in pVertexData but it's a junk value
				
				
				// done here
				break;
			}

			// next item
			pLastVertexData = pFindVertexData;
			pFindVertexData = pNextVertexData;
		}

		// scan for and delete index item
		sIndexData* pLastIndexData = NULL;
		sIndexData* pFindIndexData = m_pIndexDataList;
		while ( pFindIndexData )
		{
			// check this item
			sIndexData* pNextIndexData = pFindIndexData->pNext;
			if ( pFindIndexData==pIndexData )
			{
				// sever and delete Index item
				pFindIndexData->pNext=NULL;

				// release IB
				SAFE_RELEASE ( pFindIndexData->pIB );

				// delete indexdata
				SAFE_DELETE( pFindIndexData );

				// adjust next value to leap deleted item
				if ( pLastIndexData ) pLastIndexData->pNext = pNextIndexData;

				// new start item to replace deleted one
				if ( m_pIndexDataList==pIndexData )
					m_pIndexDataList = pNextIndexData;

				// done here
				break;
			}

			// next item
			pLastIndexData = pFindIndexData;
			pFindIndexData = pNextIndexData;
		}

		// flag any objects that used either of these buffers
		// mike - 301106 - add flag to stop recursion
		if ( bRecurse == true )
		{
			// useful when we KNOW that the VB IB buffers are not shared
			FlagAllObjectMeshesUsingBuffer ( pVertexData, pIndexData );
		}
	}

	// everything went okay
	return true;
}

// lee - 140307 - added to delete buffers quickly if we know VB/IBs are not shared
bool CObjectManager::RemoveBuffersUsedByObjectMeshDirectly ( sMesh* pMesh )
{
	// delete the buffers from the lit, and release the ptrs, but do NOT assume buffers are shared!
	return RemoveBuffersUsedByObjectMesh ( pMesh, false );
}

bool CObjectManager::RemoveBuffersUsedByObject ( sObject* pObject )
{
	// run through each frame within an object
	for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
	{
		// get frame ptr
		sFrame* pFrame = pObject->ppFrameList [ iFrame ];

		// lee - 140307 - if object 'uses its own buffers' there is no need to use
		// a recursive check for whether the VB/IB buffers are shared, as they cannot be
		// so we can simply delete the buffers with out recursive (buffer remove/flag back in)
		if ( pObject->bUsesItsOwnBuffers==true )
		{
			// delete all vertex/index entries 'directly' used by this mesh (no recursive)
			if(pFrame->pMesh) RemoveBuffersUsedByObjectMeshDirectly ( pFrame->pMesh );
			if(pFrame->pShadowMesh) RemoveBuffersUsedByObjectMeshDirectly ( pFrame->pShadowMesh );
			if(pFrame->pBoundBox) RemoveBuffersUsedByObjectMeshDirectly ( pFrame->pBoundBox );
			if(pFrame->pBoundSphere) RemoveBuffersUsedByObjectMeshDirectly ( pFrame->pBoundSphere );
			if(pFrame->pLOD[0]) RemoveBuffersUsedByObjectMeshDirectly ( pFrame->pLOD[0] );
			if(pFrame->pLOD[1]) RemoveBuffersUsedByObjectMeshDirectly ( pFrame->pLOD[1] );
			if(pFrame->pLODForQUAD) RemoveBuffersUsedByObjectMeshDirectly ( pFrame->pLODForQUAD );
		}
		else
		{
			// delete all vertex/index entries used by this mesh
			if(pFrame->pMesh) RemoveBuffersUsedByObjectMesh ( pFrame->pMesh );
			if(pFrame->pShadowMesh) RemoveBuffersUsedByObjectMesh ( pFrame->pShadowMesh );
			if(pFrame->pBoundBox) RemoveBuffersUsedByObjectMesh ( pFrame->pBoundBox );
			if(pFrame->pBoundSphere) RemoveBuffersUsedByObjectMesh ( pFrame->pBoundSphere );
			if(pFrame->pLOD[0]) RemoveBuffersUsedByObjectMesh ( pFrame->pLOD[0] );
			if(pFrame->pLOD[1]) RemoveBuffersUsedByObjectMesh ( pFrame->pLOD[1] );
			if(pFrame->pLODForQUAD) RemoveBuffersUsedByObjectMesh ( pFrame->pLODForQUAD );
		}
	}

	// everything went okay
	return true;
}

bool CObjectManager::AddFlaggedObjectsBackToBuffers ( void )
{
	// upon buffer removal, some object where flagged for re-creation
	for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
	{
		// get index from shortlist
		int iObjectID = g_ObjectListRef [ iShortList ];

		// see if we have a valid list
		sObject* pObject = g_ObjectList [ iObjectID ];
		if ( !pObject )
			continue;

		// also ensure we skip the removed object (not to be re-added)
		if ( pObject->bReplaceObjectFromBuffers==true )
			continue;

		// run through each frame within an object
		for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
		{
			// refresh the VB data for this mesh (it will auto-lock the VB)
			sFrame* pFrame = pObject->ppFrameList [ iFrame ];

			// add this object:mesh back in
			if ( pFrame->pMesh )
			{
				if ( pFrame->pMesh->bAddObjectToBuffers==true )
				{
					AddObjectMeshToBuffers ( pFrame->pMesh, pObject->bUsesItsOwnBuffers );
					pFrame->pMesh->bAddObjectToBuffers=false;
				}
			}

			// add this object:shadowmesh back in
			if ( pFrame->pShadowMesh )
			{
				if ( pFrame->pShadowMesh->bAddObjectToBuffers==true )
				{
					AddObjectMeshToBuffers ( pFrame->pShadowMesh, pObject->bUsesItsOwnBuffers );
					pFrame->pShadowMesh->bAddObjectToBuffers=false;
				}
			}

			// add this object:boundbox back in
			if ( pFrame->pBoundBox )
			{
				if ( pFrame->pBoundBox->bAddObjectToBuffers==true )
				{
					AddObjectMeshToBuffers ( pFrame->pBoundBox, pObject->bUsesItsOwnBuffers );
					pFrame->pBoundBox->bAddObjectToBuffers=false;
				}
			}	

			// add this object:boundsphere back in
			if ( pFrame->pBoundSphere )
			{
				if ( pFrame->pBoundSphere->bAddObjectToBuffers==true )
				{
					AddObjectMeshToBuffers ( pFrame->pBoundSphere, pObject->bUsesItsOwnBuffers );
					pFrame->pBoundSphere->bAddObjectToBuffers=false;
				}
			}	

			// add this object:mesh back in
			for ( int l=0; l<3; l++ )
			{
				sMesh* pLODMesh = NULL;
				if ( l<2 )
					pLODMesh = pFrame->pLOD[l];
				else
					pLODMesh = pFrame->pLODForQUAD;

				if ( pLODMesh )
				{
					if ( pLODMesh->bAddObjectToBuffers==true )
					{
						AddObjectMeshToBuffers ( pLODMesh, pObject->bUsesItsOwnBuffers );
						pLODMesh->bAddObjectToBuffers=false;
					}
				}
			}
		}
	}

	// everything went okay
	return true;
}

bool CObjectManager::RemoveObjectFromBuffers ( sObject* pRemovedObject )
{
	// delete all buffers that this object resided in
	RemoveBuffersUsedByObject ( pRemovedObject );

	// upon buffer removal, some object where flagged for re-creation
	AddFlaggedObjectsBackToBuffers ();

	// update texture list when introduce new object(s)
	UpdateTextures();

	// everything went okay
	return true;
}

bool CObjectManager::ReplaceAllFlaggedObjectsInBuffers ( void )
{
	// only if global flag switched
	if ( g_bObjectReplacedUpdateBuffers )
	{
		// delete all buffers that these object resided in
		for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
		{
			// get index from shortlist
			int iObjectID = g_ObjectListRef [ iShortList ];
			sObject* pRemovedObject = g_ObjectList [ iObjectID ];
			if ( pRemovedObject )
				if ( pRemovedObject->bReplaceObjectFromBuffers )
					RemoveBuffersUsedByObject ( pRemovedObject );
		}

		// upon buffer removal, some object where flagged for re-creation
		AddFlaggedObjectsBackToBuffers ();

		// when all buffers clear of removed objects, can add new instances of them back in..
		for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
		{
			// get index from shortlist
			int iObjectID = g_ObjectListRef [ iShortList ];
			sObject* pObject = g_ObjectList [ iObjectID ];
			if ( pObject )
			{
				if ( pObject->bReplaceObjectFromBuffers )
				{
					// add object back in
					AddObjectToBuffers ( pObject );

					// and clear flag 
					pObject->bReplaceObjectFromBuffers = false;		
				}
			}
		}

		// update texture list when introduce new object(s)
		UpdateTextures();

		// reset global flag
		g_bObjectReplacedUpdateBuffers = false;
	}

	// everything went okay
	return true;
}

bool CObjectManager::UpdateObjectMeshInBuffer ( sMesh* pMesh )
{
	// only if have a drawbuffer
	if ( !pMesh->pDrawBuffer )
		return false;

	// get the offset map
	sOffsetMap	offsetMap;
	GetFVFOffsetMap ( pMesh, &offsetMap );

	// get vertex list item pointer
	sVertexData* pVertexData = (sVertexData*)pMesh->pDrawBuffer->pVBListEntryRef;
	IDirect3DVertexBuffer9* pVertexBuffer = pVertexData->pVB;

	// lock the vertex buffer (if not already locked)
	if ( pVertexData->bBufferLocked==false )
	{
		pVertexData->pfLockedData = NULL;
		if ( FAILED ( pVertexBuffer->Lock ( 0, 0, ( VOID** ) &pVertexData->pfLockedData, 0 ) ) )
			return false;

		// set the VB flag when locked
		pVertexData->bBufferLocked = true;
	}

	// copy vertex-data-block from object to VB
	DWORD dwPosWithinVB = (pMesh->pDrawBuffer->dwBaseVertexIndex + pMesh->pDrawBuffer->dwVertexStart) * pMesh->dwFVFSize;
	LPVOID pDestPtr = pVertexData->pfLockedData + dwPosWithinVB;
	LPVOID pSourceData = pMesh->pVertexData;
	DWORD dwSizeToCopy = pMesh->dwVertexCount * pMesh->dwFVFSize;
	memcpy ( pDestPtr, pSourceData, dwSizeToCopy );

	// draw quantity can change without having to recreate (like for shadows)
	if ( pMesh->pDrawBuffer )
	{
		pMesh->pDrawBuffer->dwVertexCount		= pMesh->iDrawVertexCount;
		pMesh->pDrawBuffer->dwPrimitiveCount	= pMesh->iDrawPrimitives;
	}

	// leeadd - 230304 - physics changes INDEX DATA TOO,
	// which a VB update above does not do..so add this
	if ( pMesh->pDrawBuffer )
	{
		// only if index buffer exists
		if ( pMesh->pDrawBuffer->pIndexBufferRef )
			CopyMeshDataToIndexBuffer ( pMesh, pMesh->pDrawBuffer->pIndexBufferRef, pMesh->pDrawBuffer->dwIndexStart );
	}

	// everything went okay
	return true;
}

bool CObjectManager::UpdateAllObjectsInBuffers ( void )
{
	// objects that have changed are flagged, and passed to VB updater..

	// run through all objects
	for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
	{
		// get index from shortlist
		int iObjectID = g_ObjectListRef [ iShortList ];

		// see if we have a valid list
		sObject* pObject = g_ObjectList [ iObjectID ];
		if ( !pObject )
			continue;

		if ( iObjectID==2005 )
		{
//	        DXTRACE_ERR( TEXT("Object"), iObjectID );
		}

		// run through each frame within an object
		for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
		{
			// refresh the VB data for this mesh (it will auto-lock the VB)
			sFrame* pFrame = pObject->ppFrameList [ iFrame ];

			// 301007 - new limb excluder
			if ( pFrame==NULL ) continue;
			if ( pFrame->bExcluded==true ) continue;

			// Update regular mesh (if required)
			sMesh* pMesh = pFrame->pMesh;
			if ( pMesh )
			{
				if ( pMesh->bVBRefreshRequired==true )
				{
					UpdateObjectMeshInBuffer ( pMesh );
					pMesh->bVBRefreshRequired=false;
				}
			}

			// Update shadow mesh (if required)
			sMesh* pShadowMesh = pFrame->pShadowMesh;
			if ( pShadowMesh )
			{
				if ( pShadowMesh->bVBRefreshRequired==true )
				{
					UpdateObjectMeshInBuffer ( pShadowMesh );
					pShadowMesh->bVBRefreshRequired=false;
				}
			}

			// Update bound box meshes (if required)
			sMesh* pBoundBox = pFrame->pBoundBox;
			if ( pBoundBox )
			{
				if ( pBoundBox->bVBRefreshRequired==true )
				{
					UpdateObjectMeshInBuffer ( pBoundBox );
					pBoundBox->bVBRefreshRequired=false;
				}
			}				

			// Update bound sphere meshes (if required)
			sMesh* pBoundSphere = pFrame->pBoundSphere;
			if ( pBoundSphere )
			{
				if ( pBoundSphere->bVBRefreshRequired==true )
				{
					UpdateObjectMeshInBuffer ( pBoundSphere );
					pBoundSphere->bVBRefreshRequired=false;
				}
			}

			for ( int l=0; l<3; l++ )
			{
				sMesh* pLOD = NULL;
				if ( l<2 )
					pLOD = pFrame->pLOD[l];
				else
					pLOD = pFrame->pLODForQUAD;

				if ( pLOD )
				{
					if ( pLOD->bVBRefreshRequired==true )
					{
						UpdateObjectMeshInBuffer ( pLOD );
						pLOD->bVBRefreshRequired=false;
					}
				}
			}

		}
	}

	// go through all vertex buffer items (unlock any that have been locked)
	CompleteUpdateInBuffers();

	// okay
	return true;
}

bool CObjectManager::CompleteUpdateInBuffers ( void )
{
	// go through all vertex buffer items (unlock any that have been locked)
	sVertexData* pVertexData = m_pVertexDataList;
	while ( pVertexData )
	{
		// if buffer has been locked
		if ( pVertexData->bBufferLocked==true )
		{
			// unlock and restore flag
			pVertexData->pVB->Unlock ( );
			pVertexData->bBufferLocked=false;
		}

		// move to next node
		pVertexData = pVertexData->pNext;
	}

	// okay
	return true;
}

bool CObjectManager::QuicklyUpdateObjectMeshInBuffer ( sMesh* pMesh, DWORD dwVertexFrom, DWORD dwVertexTo )
{
	// only if have a drawbuffer
	if ( !pMesh->pDrawBuffer )
		return false;

	// get the offset map
	sOffsetMap	offsetMap;
	GetFVFOffsetMap ( pMesh, &offsetMap );

	// get vertex list item pointer
	sVertexData* pVertexData = (sVertexData*)pMesh->pDrawBuffer->pVBListEntryRef;
	IDirect3DVertexBuffer9* pVertexBuffer = pVertexData->pVB;

	// lock the vertex buffer (if not already locked)
	if ( pVertexData->bBufferLocked==false )
	{
		pVertexData->pfLockedData = NULL;
		if ( FAILED ( pVertexBuffer->Lock ( 0, 0, ( VOID** ) &pVertexData->pfLockedData, 0 ) ) )
			return false;

		// set the VB flag when locked
		pVertexData->bBufferLocked = true;
	}

	// copy only vertex data changed as described in params passed in
	DWORD dwPosWithinVB = (pMesh->pDrawBuffer->dwBaseVertexIndex + pMesh->pDrawBuffer->dwVertexStart + dwVertexFrom) * pMesh->dwFVFSize;
	LPVOID pDestPtr = pVertexData->pfLockedData + dwPosWithinVB;
	DWORD dwPosWithinMesh = dwVertexFrom * pMesh->dwFVFSize;
	LPVOID pSourceData = pMesh->pVertexData + dwPosWithinMesh;
	DWORD dwSizeToCopy = (dwVertexTo-dwVertexFrom) * pMesh->dwFVFSize;
	memcpy ( pDestPtr, pSourceData, dwSizeToCopy );

	// draw quantity can change without having to recreate (like for shadows)
	pMesh->pDrawBuffer->dwVertexCount		= pMesh->iDrawVertexCount;
	pMesh->pDrawBuffer->dwPrimitiveCount	= pMesh->iDrawPrimitives;

	// everything went okay
	return true;
}

bool CObjectManager::RenewReplacedMeshes ( sObject* pObject )
{
	// run through each mesh within an object
	bool bReplaceObjectOwningMesh=false;
	for ( int iMesh = 0; iMesh < pObject->iMeshCount; iMesh++ )
	{
		// replace any meshes in VB/IB that have been changed
		sMesh* pMesh = pObject->ppMeshList [ iMesh ];
		if ( pMesh->bMeshHasBeenReplaced==true )
		{
			pMesh->bMeshHasBeenReplaced=false;
			bReplaceObjectOwningMesh=true;
			break;
		}
	}

	// replace object
	if ( bReplaceObjectOwningMesh )
	{
		pObject->bReplaceObjectFromBuffers = true;
		g_bObjectReplacedUpdateBuffers = true;
		bReplaceObjectOwningMesh = false;
	}

	// okay
	return true;
}

bool CObjectManager::RefreshObjectInBuffer ( sObject* pObject )
{
	pObject->bReplaceObjectFromBuffers = true;
	g_bObjectReplacedUpdateBuffers = true;
	return true;
}

void CObjectManager::RemoveTextureRefFromAllObjects ( LPDIRECT3DTEXTURE9 pTextureRef )
{
	// run through all objects
	for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
	{
		// get index from shortlist
		int iObjectID = g_ObjectListRef [ iShortList ];

		// see if we have a valid list
		sObject* pObject = g_ObjectList [ iObjectID ];
		if ( !pObject )
			continue;

		// run through each frame within an object
		for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
		{
			// get frame
			sFrame* pFrame = pObject->ppFrameList [ iFrame ];
			sMesh* pMesh = pFrame->pMesh;
			if ( pMesh )
			{
				// go through all textures in mesh
				RemoveTextureRefFromMesh ( pMesh, pTextureRef );
			}
		}
	}
}

//
// SORTING
//

bool CObjectManager::SortTextureList ( void )
{
    // If the object list size hasn't changed and no textures have changed, then nothing to do here.
    if ( m_iLastCount == g_iObjectListRefCount && !m_bUpdateTextureList )
        return true;

    // Reset ready for next time
    m_iLastCount         = g_iObjectListRefCount;
    m_bUpdateTextureList = false;

    // make sure the lists we're using are valid
    SAFE_MEMORY ( m_ppSortedObjectList );
    SAFE_MEMORY ( m_ppSortedObjectVisibleList );

    // reset all data so we can build the list from scratch
    m_iSortedObjectCount = 0;
    g_bRenderVeryEarlyObjects = false;

    // run through all known items and put them into render list ready for sorting
    for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
    {
	    // Get an object id from shortlist
	    int iObjectID = g_ObjectListRef [ iShortList ];

	    // Actual object or instance of object
	    sObject* pOriginalObject = g_ObjectList [ iObjectID ];
        if ( ! pOriginalObject )
            continue;

        sObject* pRenderObject = pOriginalObject;
        if ( pRenderObject->pInstanceOfObject )
		    pRenderObject = pRenderObject->pInstanceOfObject;

	    // See if we have enough information to render this object
        // A (possibly instanced) object with a mesh list and with mesh 0 having a texture.
        if ( pRenderObject && pRenderObject->ppMeshList && pRenderObject->ppMeshList[0]->pTextures)
	    {
            // Add the original object into the render list
            m_ppSortedObjectList [ m_iSortedObjectCount++ ] = pOriginalObject;

            // If we are sorting by distance, calculate the distance ready for sorting
            if ( g_eGlobalSortOrder == E_SORT_BY_DEPTH )
		    {
			    if ( pOriginalObject->bVeryEarlyObject == true )
			    {
                    // very early objects are placed at extreme distance
				    pOriginalObject->position.fCamDistance = 9999999.9f;
			    }
			    else
			    {
				    pOriginalObject->position.fCamDistance = CalculateObjectDistanceFromCamera ( pOriginalObject );
			    }
            }
            
            // Check to see if there is an early draw object
            if ( pOriginalObject->bVeryEarlyObject == true )
		    {
			    // If this object is an early draw, set global flag to show we have one in the scene
                g_bRenderVeryEarlyObjects = true;
            }
	    }
    }

    // Now sort the list appropriately
    switch(g_eGlobalSortOrder)
    {
    case E_SORT_BY_TEXTURE:
        std::sort(m_ppSortedObjectList, m_ppSortedObjectList + m_iSortedObjectCount, OrderByTexture() );
        break;
    case E_SORT_BY_OBJECT:
        std::sort(m_ppSortedObjectList, m_ppSortedObjectList + m_iSortedObjectCount, OrderByObject() );
        break;
    case E_SORT_BY_DEPTH:
        // Delay sort until the visibility list is generated
        // This increases the accuracy of the sorting for depth.
        break;
    case E_SORT_BY_NONE:
        // No sort needed
        break;
    default:
        break;
	}

	// return back to caller
	return true;
}

bool CObjectManager::SortVisibilityList ( void )
{
	// run through the sorted texture list and find out which objects
	// are visible, then create a new list which stores the visible objects

	// reset the number of visible objects to 0
	m_iVisibleObjectCount = 0;

    m_vVisibleObjectList.clear();
    m_vVisibleObjectEarly.clear();
    m_vVisibleObjectTransparent.clear();
    m_vVisibleObjectNoZDepth.clear();
    m_vVisibleObjectStandard.clear();

    // when the textures have been sorted we have a counter which stores
	// the number of sorted objects, this is m_iSortedObjectCount, we now
	// run through the sorted texture list and find which objects are visible
	for ( int iSort = 0; iSort < m_iSortedObjectCount; iSort++ )
	{
		// make sure we have a valid object
		if ( !m_ppSortedObjectList [ iSort ] )
			return false;

		// get a pointer to the object from the sorted draw list
		sObject* pObject = m_ppSortedObjectList [ iSort ];

		
		// mike - 130405 - skip objects if they are excluded, I changed the way
		//				 - exclusion works because in driving test by ravey it highlighted
		//				 - a few problems
		if ( pObject->bExcluded )
			continue;

		// actual object or instance of object
		sObject* pActualObject = pObject;
		if ( pActualObject->pInstanceOfObject )
			pActualObject=pActualObject->pInstanceOfObject;

		// VISIBILITY CULLING CHECK PROCESS
		bool bIsVisible=false;

		// locked objects are always visible
		// glued objects are always visible (deferred to parent visibility)
		int iGluedToObj = pObject->position.iGluedToObj;
		if ( pObject->bLockedObject || iGluedToObj!=0 )
		{
			// leefix -040803- maintenance check, if glued to object that has been deleted, deal with it
			if ( iGluedToObj!=0 )
			{
				sObject* pParentObject = g_ObjectList [ iGluedToObj ];
				if ( pParentObject==NULL )
				{
					// wipe out glue assignment
					pObject->position.bGlued		= false;
					pObject->position.iGluedToObj	= 0;
					pObject->position.iGluedToMesh	= 0;
				}
			}

			// locked objects and glued are visible
			bIsVisible=true;
		}
		else
		{
			// send the position of the object and it's radius to the "CheckSphere" function, if this returns true the object will be visible
			float fScaledRadius = pActualObject->collision.fScaledLargestRadius;
			if ( fScaledRadius<=0.0f )
			{
				// objects with no mesh scope are visible
				bIsVisible=true;
			}
			else
			{
				// ensure have latest object center
				UpdateColCenter ( pObject );

				// get center of the object
				D3DXVECTOR3 vecRealCenter = pObject->position.vecPosition + pObject->collision.vecColCenter;

				// leeadd - 100805 - add in offset from first frame (limb zero), as this moves whole object render)
				if ( pObject->ppFrameList )
				{
					sFrame* pRootFrame = pObject->ppFrameList [ 0 ];
					if ( pRootFrame )
					{
						// leeadd - 211008 - u71 - added flag to NOT shift object bounds by frame zero matrix
						if ( (pRootFrame->dwStatusBits && 1)==0 ) 
						{
							// offset center to account for movement of the object by limb zero (root frame)
							vecRealCenter.x += pRootFrame->matUserMatrix._41;
							vecRealCenter.y += pRootFrame->matUserMatrix._42;
							vecRealCenter.z += pRootFrame->matUserMatrix._43;
						}
					}
				}

				// objects within frustrum are visible
				if ( CheckSphere ( vecRealCenter.x, vecRealCenter.y, vecRealCenter.z, fScaledRadius ) )
					bIsVisible=true;

				// lerfix - 221008 - u71 - if reflection shading in effect, frustrum cull is not required
				if ( g_pGlob->dwStencilMode==2 )
					bIsVisible=true;
			}
		}

		// determine visiblity

		// MIKE - 021203 - added in second part of if statement for external objects, physics DLL
		if ( bIsVisible || pObject->bDisableTransform == true )
		{
			// save a pointer to the object and place it in the new drawlist
			m_ppSortedObjectVisibleList [ m_iVisibleObjectCount++ ] = pObject;

            // Build individual draw lists for each layer
            if (pObject->bVeryEarlyObject == true)
            {
                m_vVisibleObjectEarly.push_back( pObject );
            }
            else if ( pObject->bNewZLayerObject || pObject->bLockedObject )
            {
				m_vVisibleObjectNoZDepth.push_back( pObject );
            }
            else if ( pObject->bGhostedObject || pObject->bTransparentObject )
            {
                m_vVisibleObjectTransparent.push_back( pObject );
            }
            else
            {
                m_vVisibleObjectStandard.push_back( pObject );
            }

            // u74b8 - If sort order is by distance, update the object distance
            if (g_eGlobalSortOrder == E_SORT_BY_DEPTH)
            {
			    if ( pObject->bVeryEarlyObject == true )
			    {
                    // very early objects are placed at extreme distance
				    pObject->position.fCamDistance = 9999999.9f;
			    }
			    else
			    {
				    pObject->position.fCamDistance = CalculateObjectDistanceFromCamera ( pObject );
			    }
            }
		}
	}

    // u74b8 - If sort order is by distance, sort the list into the correct order
    //         as it varies by camera.
    if (g_eGlobalSortOrder == E_SORT_BY_DEPTH)
    {
        // No ghost/transparent sort just yet - still need to take into account water -
        //but do need to sort everything else.
        std::sort( m_vVisibleObjectEarly.begin(), m_vVisibleObjectEarly.end(), OrderByReverseCameraDistance() );
        std::sort( m_vVisibleObjectNoZDepth.begin(), m_vVisibleObjectNoZDepth.end(), OrderByReverseCameraDistance() );
        std::sort( m_vVisibleObjectStandard.begin(), m_vVisibleObjectStandard.end(), OrderByReverseCameraDistance() );
    }

	// all went okay
	return true;
}

bool CObjectManager::UpdateTextures ( void )
{
	// triggers texture list update
	m_bUpdateTextureList=true;
	return true;
}

bool CObjectManager::UpdateAnimationCycle ( void )
{
	// run through all objects (animate code)
	for ( int iShortList = 0; iShortList < g_iObjectListRefCount; iShortList++ )
	{
		// get index from shortlist
		int iObjectID = g_ObjectListRef [ iShortList ];

		// see if we have a valid list
		sObject* pObject = g_ObjectList [ iObjectID ];
		if ( !pObject )
			continue;

		// leeopt - 080908 - if object is excluded, skip rest - no engine interaction!
		if ( pObject->bExcluded==true )
			continue;

		// simply control animation frame
		if ( pObject->bAnimPlaying )
		{
			// advance frame
			pObject->fAnimFrame += pObject->fAnimSpeed;

			// if reach end
			if ( pObject->fAnimFrame >= pObject->fAnimFrameEnd )
			{
				// if animation loops
//				pObject->fAnimFrame = pObject->fAnimLoopStart; (190303-see below)
				if ( pObject->bAnimLooping==false )
				{
					pObject->bAnimPlaying = false;
				}
				else
				{
					// leefix - 190303 - beta 4.7 - so play anim stays on last frame (is this DBV1 friendly?)
					pObject->fAnimFrame = pObject->fAnimLoopStart;
				}
			}

			// leeadd - 300605 - support looping frames backwards - speed can be minus!
			if ( pObject->fAnimSpeed<0 && pObject->fAnimFrame < pObject->fAnimLoopStart )
			{
				if ( pObject->bAnimLooping==false )
					pObject->bAnimPlaying = false;
				else
					pObject->fAnimFrame = pObject->fAnimFrameEnd;
			}
		}
		else
		{
			// control manual slerp
			if ( pObject->bAnimManualSlerp )
			{
				pObject->fAnimSlerpTime += pObject->fAnimInterp;
				if ( pObject->fAnimSlerpTime >= 1.0f )
				{
					pObject->bAnimManualSlerp = false;
					pObject->fAnimFrame = pObject->fAnimSlerpEndFrame;
				}
			}
		}

		// leeadd - 080305 - copy animation bound boxes to collision boundbox
		if ( pObject->pAnimationSet )
		{
			// leefix - 310305 - ONLY if not using the fixed box check from (make object collision box)
			// lee - 140306 - u60b3 - added bUseBoxCollision as this was not accounted with first flag
			if ( pObject->collision.bFixedBoxCheck==false && pObject->collision.bUseBoxCollision==false )
			{
				if ( pObject->pAnimationSet->pvecBoundMin )
				{
					pObject->collision.vecMin = pObject->pAnimationSet->pvecBoundMin [ (int)pObject->fAnimFrame ];
					pObject->collision.vecMax = pObject->pAnimationSet->pvecBoundMax [ (int)pObject->fAnimFrame ];
					pObject->collision.vecCentre = pObject->pAnimationSet->pvecBoundCenter [ (int)pObject->fAnimFrame ];
					pObject->collision.fRadius = pObject->pAnimationSet->pfBoundRadius [ (int)pObject->fAnimFrame ];
					pObject->ppMeshList [ 0 ]->Collision.vecMin = pObject->collision.vecMin;
					pObject->ppMeshList [ 0 ]->Collision.vecMax = pObject->collision.vecMax;
					pObject->ppMeshList [ 0 ]->Collision.vecCentre = pObject->collision.vecCentre;
					pObject->ppMeshList [ 0 ]->Collision.fRadius = pObject->collision.fRadius;
				}
			}
		}
	}

	// okay
	return true;
}

bool CObjectManager::UpdateOnlyVisible ( void )
{
	// run through all objects
	for ( int iObjectID = 0; iObjectID < m_iVisibleObjectCount; iObjectID++ )
	{
		// see if we have a valid list
		sObject* pObject = m_ppSortedObjectVisibleList [ iObjectID ];
		if ( !pObject ) continue;

		// U72 - 100109 - allow parents of instances
		if ( pObject->position.bParentOfInstance==false )
		{
			// leespeed - 240705 - only need to calc matrix data and update anim data if VISIBLE!
			if ( pObject->bVisible==false || pObject->bUniverseVisible==false )
				continue;
		}

		// leefix - 140604 - moved from DBOFormat (needs to be done before a VB update)
		// calculate all frame/slerp/animation data
		UpdateAllFrameData ( pObject, pObject->fAnimFrame );

		// reset hierarchy and calculate combined frame matrix data
		D3DXMATRIX matrix;
		D3DXMatrixIdentity ( &matrix );

		// leeadd - 120606 - u62 - special 'bone matrix' override (ragdoll systems, etc)
		if ( pObject->position.bCustomBoneMatrix==false ) UpdateFrame ( pObject->pFrame, &matrix );

		// leemove - 040104 - moved this code to DBOFormat.cpp - handle vertex level animation (even if not animating)
		// leeadd - 120204 - instances that use animating objects must animate them indirectly
		sObject* pActualObject = pObject;
		if ( pActualObject->pInstanceOfObject )
		{
			// animate actual object of the instance indirectly
			UpdateObjectCamDistance ( pActualObject );

			// U72 - 100109 - each instance calls same function, parent object should animate!
			//pActualObject=pActualObject->pInstanceOfObject;
			//UpdateObjectAnimation ( pActualObject );
		}
		else
		{
			// animate object directly
			if ( pObject->bVisible && pObject->bUniverseVisible )
			{
				UpdateObjectCamDistance ( pObject );
				UpdateObjectAnimation ( pObject );
			}
			else
			{
				// U72 - 100109 - moved instance animation here as we only want to call it once
				if ( pObject->position.bParentOfInstance )
					UpdateObjectAnimation ( pObject );
			}
		}
	}

	// okay
	return true;
}

//
// RENDERING
//

bool CObjectManager::SetVertexShader ( sMesh* pMesh )
{
	// set the vertex shader for a mesh - only change if the FVF is different

	// if VertDec different in any way
	bool bRefresh = false;
	if ( m_bUpdateVertexDecs==true )
	{
		m_bUpdateVertexDecs = false;
		bRefresh = true;
	}

	// check the mesh is okay
	SAFE_MEMORY ( pMesh );

	// regular or custom shader
	if ( pMesh->bUseVertexShader )
		m_dwCurrentShader = (DWORD)pMesh->pVertexShader;
	else
		m_dwCurrentShader = 0;

	// store the current FVF as regular
	m_dwCurrentFVF = pMesh->dwFVF;

	// is the shader different to the previously set shader
	if ( m_dwCurrentShader != m_dwLastShader || bRefresh==true )
	{
		// custom shader or Fixed-Function\FX-Effect 
		if ( pMesh->pVertexShader )
		{
			// set the new vertex shader
			if ( FAILED ( m_pD3D->SetVertexShader ( pMesh->pVertexShader ) ) )
				return false;
		}
		else
		{
			// set no vertex shader
			if ( FAILED ( m_pD3D->SetVertexShader ( NULL ) ) )
				return false;
		}

		// store the current shader
		m_dwLastShader = m_dwCurrentShader;
	}

	// is the FVF different to the previously set FVF
	if ( (m_dwCurrentFVF != m_dwLastFVF) || m_dwCurrentFVF==0 || bRefresh==true )
	{
		// custom low level shader
		if ( pMesh->pVertexShader )
		{
			// vertex dec - usually from low level assembly shader
			if ( FAILED ( m_pD3D->SetVertexDeclaration ( pMesh->pVertexDec ) ) )
				return false;
		}
		else
		{
			// custom FVF or Regular
			if ( m_dwCurrentFVF==0 )
			{
				// custom vertex dec - usually from FX effect
				if ( FAILED ( m_pD3D->SetVertexDeclaration ( pMesh->pVertexDec ) ) )
					return false;

				// regular vertex FVF - standard usage
				if ( FAILED ( m_pD3D->SetFVF ( 0 ) ) )
					return false;
			}
			else
			{
				// regular vertex FVF - standard usage
				if ( FAILED ( m_pD3D->SetFVF ( m_dwCurrentFVF ) ) )
					return false;
			}
		}

		// store the current shader
		m_dwLastFVF = m_dwCurrentFVF;
	}

	// return okay
	return true;
}

bool CObjectManager::SetInputStreams ( sMesh* pMesh )
{
	// set the input streams for drawing - only change if different

	// make sure the mesh is valid
	SAFE_MEMORY ( pMesh );

	// store a pointer to the current VB and IB
	m_ppCurrentVBRef = pMesh->pDrawBuffer->pVertexBufferRef;
	m_ppCurrentIBRef = pMesh->pDrawBuffer->pIndexBufferRef;

	// see the difference flag to false
	bool bDifferent = false;
	if ( m_ppCurrentVBRef != m_ppLastVBRef )
		bDifferent = true;

	// when a new frame starts we need to reset the streams
	if ( m_bUpdateStreams ) bDifferent = true;

	// update VB only when necessary
	if ( bDifferent )
	{
		// store the current VB
		m_ppLastVBRef = m_ppCurrentVBRef;

		// set the stream source
		if ( FAILED ( m_pD3D->SetStreamSource ( 0,
												pMesh->pDrawBuffer->pVertexBufferRef,
												0, 
												pMesh->pDrawBuffer->dwFVFSize				 ) ) )
			return false;

	}

	// leefix - 230604 - u54 - removed IF which skipped NULL index buffers (messed with last-current state check)

	// see the difference flag to false
	bDifferent = false;
	if ( m_ppCurrentIBRef != m_ppLastIBRef )
		bDifferent = true;

	// when a new frame starts we need to reset the streams
	if ( m_bUpdateStreams ) bDifferent = true;

	// update VB only when necessary
	if ( bDifferent )
	{
		// store the current VB
		m_ppLastIBRef = m_ppCurrentIBRef;

		// set the indices (if any)
		if ( m_ppCurrentIBRef )
			if ( FAILED ( m_pD3D->SetIndices ( pMesh->pDrawBuffer->pIndexBufferRef ) ) )//, pMesh->pDrawBuffer->dwBaseVertexIndex ) ) )
				return false;
	}

	// update refresh used (resets at start of cycle)
	m_bUpdateStreams = false;

	return true;
}

bool CObjectManager::PreSceneSettings ( void )
{
	// cullmode
	m_RenderStates.dwCullDirection				= m_RenderStates.dwGlobalCullDirection;
	m_pD3D->SetRenderState ( D3DRS_CULLMODE,	m_RenderStates.dwCullDirection );
	m_RenderStates.bCull						= true;
	m_RenderStates.iCullMode					= 0;	

	// okay
	return true;
}

bool CObjectManager::PreDrawSettings ( void )
{
	// obtain external render states not tracked by manager
	m_pD3D->GetRenderState ( D3DRS_AMBIENT, &m_RenderStates.dwAmbientColor );
	m_pD3D->GetRenderState ( D3DRS_FOGCOLOR, &m_RenderStates.dwFogColor );

	// wireframe
	m_pD3D->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
	m_RenderStates.bWireframe							= false;

	// lighting
	m_pD3D->SetRenderState ( D3DRS_LIGHTING, TRUE );
	m_RenderStates.bLight								= true;

	// fog override starts off disabled
	m_RenderStates.bFogOverride=false;

	// fogenable
	m_pD3D->SetRenderState ( D3DRS_FOGENABLE, FALSE );
	m_RenderStates.bFog									= false;

	// ambient
	m_pD3D->SetRenderState ( D3DRS_AMBIENT, m_RenderStates.dwAmbientColor );
	m_RenderStates.iAmbient								= 1;

	// transparency
	m_pD3D->SetRenderState ( D3DRS_ALPHATESTENABLE,		false );
	m_pD3D->SetRenderState ( D3DRS_ALPHAFUNC,			D3DCMP_ALWAYS );
	m_pD3D->SetRenderState ( D3DRS_DEPTHBIAS,			0 );
	m_RenderStates.bTransparency						= false;
	m_RenderStates.dwAlphaTestValue						= 0;

	// ghost
	m_pD3D->SetRenderState ( D3DRS_ZENABLE,				TRUE );
	m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,		TRUE );
	m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE,	FALSE );
	m_RenderStates.bZRead								= true;
	m_RenderStates.bZWrite								= true;
	m_RenderStates.bGhost								= false;
	
	// zbias handling
	m_pD3D->SetRenderState ( D3DRS_DEPTHBIAS,			0 );
	m_pD3D->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	0 );
	m_RenderStates.bZBiasActive							= false;
	m_RenderStates.fZBiasSlopeScale						= 0.0f;
	m_RenderStates.fZBiasDepth							= 0.0f;

	// default material render states
	m_pD3D->SetRenderState ( D3DRS_COLORVERTEX,					TRUE );
	m_pD3D->SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE,		D3DMCS_COLOR1 );
	m_pD3D->SetRenderState ( D3DRS_SPECULARMATERIALSOURCE,		D3DMCS_MATERIAL );
	m_pD3D->SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE,		D3DMCS_MATERIAL );
	m_pD3D->SetRenderState ( D3DRS_EMISSIVEMATERIALSOURCE,		D3DMCS_MATERIAL );
	m_pD3D->SetRenderState ( D3DRS_SPECULARENABLE,				TRUE );

	// white default material 'set during init'
	m_RenderStates.gWhiteDefaultMaterial.Diffuse.r		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Diffuse.g		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Diffuse.b		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Diffuse.a		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Ambient.r		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Ambient.g		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Ambient.b		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Ambient.a		= 1.0f;
	m_RenderStates.gWhiteDefaultMaterial.Specular.r		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Specular.g		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Specular.b		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Specular.a		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Emissive.r		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Emissive.g		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Emissive.b		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Emissive.a		= 0.0f;
	m_RenderStates.gWhiteDefaultMaterial.Power			= 10.0f;

	// set default white material (for diffuse, ambience, etc)
	if ( FAILED ( m_pD3D->SetMaterial ( &m_RenderStates.gWhiteDefaultMaterial ) ) )
		return false;

	// fixed function blending stage defaults
	DWORD dwMaxTextureStage = MAXTEXTURECOUNT;
	for ( DWORD dwTextureStage = 0; dwTextureStage < dwMaxTextureStage; dwTextureStage++ )
	{
		// leefix - 180204 - set defaults at start of render phase
		m_RenderStates.dwAddressU[dwTextureStage] = D3DTADDRESS_WRAP;
		m_RenderStates.dwAddressV[dwTextureStage] = D3DTADDRESS_WRAP;
		m_RenderStates.dwMagState[dwTextureStage] = D3DTEXF_LINEAR;
		m_RenderStates.dwMinState[dwTextureStage] = D3DTEXF_LINEAR;
		m_RenderStates.dwMipState[dwTextureStage] = D3DTEXF_LINEAR;

		// texture filter modes
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_ADDRESSU, m_RenderStates.dwAddressU[dwTextureStage] );
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_ADDRESSV, m_RenderStates.dwAddressV[dwTextureStage] );
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MAGFILTER, m_RenderStates.dwMagState[dwTextureStage] );
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MINFILTER, m_RenderStates.dwMinState[dwTextureStage] );
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MIPFILTER, m_RenderStates.dwMipState[dwTextureStage] );

		// texture blending modes
		if(dwTextureStage==0)
		{
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLOROP, D3DTOP_MODULATE );
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		}
		else
		{
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLOROP, D3DTOP_DISABLE );
		}

		// texture coordinate data
		m_pD3D->SetTextureStageState( dwTextureStage, D3DTSS_TEXCOORDINDEX, dwTextureStage );
		m_pD3D->SetTextureStageState( dwTextureStage, D3DTSS_TEXTURETRANSFORMFLAGS, 0 );
	}

	// leeadd - 140304 - Set default FOV from camera (zero does not change camera FOV!)
	tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData( 0 );
	m_RenderStates.fStoreCameraFOV = m_Camera_Ptr->fFOV;
	m_RenderStates.fObjectFOV = 0.0f;

	// success
	return true;
}

bool CObjectManager::SetMeshMaterial ( sMesh* pMesh, D3DMATERIAL9* pMaterial )
{
	if ( pMesh->bUsesMaterial )
	{
		// use diffuse from material (if present)
		m_pD3D->SetRenderState ( D3DRS_COLORVERTEX,					FALSE );
		m_pD3D->SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE,		D3DMCS_MATERIAL );

		// set the material from the mesh
		if ( FAILED ( m_pD3D->SetMaterial ( pMaterial ) ) )
			return false;
	}
	else
	{
		// use diffuse from mesh vertex (if any)
		m_pD3D->SetRenderState ( D3DRS_COLORVERTEX,					TRUE );
		m_pD3D->SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE,		D3DMCS_COLOR1 );

		// set no material
		if ( FAILED ( m_pD3D->SetMaterial ( &m_RenderStates.gWhiteDefaultMaterial ) ) )
			return false;
	}

	// success
	return true;
}

bool CObjectManager::SetMeshRenderStates( sMesh* pMesh )
{
	// wireframe
	if ( pMesh->bWireframe != m_RenderStates.bWireframe )
	{
		if ( pMesh->bWireframe )
			m_pD3D->SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		else
			m_pD3D->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );

		m_RenderStates.bWireframe = pMesh->bWireframe;
	}

	// lighting
	if ( pMesh->bLight != m_RenderStates.bLight )
	{
		if ( pMesh->bLight )
			m_pD3D->SetRenderState ( D3DRS_LIGHTING, TRUE );
		else
			m_pD3D->SetRenderState ( D3DRS_LIGHTING, FALSE );

		m_RenderStates.bLight = pMesh->bLight;
	}

	// cullmode
	if ( pMesh->bCull != m_RenderStates.bCull || pMesh->iCullMode != m_RenderStates.iCullMode )
	{
		// lee - 040306 -u6rc5 - cull mode (direction override)
		if ( pMesh->iCullMode==2 )
		{
			m_pD3D->SetRenderState ( D3DRS_CULLMODE, D3DCULL_CCW );
		}
		else
		{
			// lee - 121006 - u63 - cull mode CW (for manual reflection cull toggle)
			if ( pMesh->iCullMode==3 )
			{
				m_pD3D->SetRenderState ( D3DRS_CULLMODE, D3DCULL_CW );
			}
			else
			{
				// on/off
				if ( pMesh->bCull )
					m_pD3D->SetRenderState ( D3DRS_CULLMODE, m_RenderStates.dwCullDirection );
				else	
					m_pD3D->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
			}
		}
		m_RenderStates.bCull = pMesh->bCull;
		m_RenderStates.iCullMode = pMesh->iCullMode;
	}

	// fog system (from light DLL)
	if(g_pGlob)
	{
		if(g_pGlob->iFogState==1)
		{
			// fogenable
			if ( pMesh->bFog != m_RenderStates.bFog )
			{
				if ( pMesh->bFog )
					m_pD3D->SetRenderState ( D3DRS_FOGENABLE, TRUE );
				else	
					m_pD3D->SetRenderState ( D3DRS_FOGENABLE, FALSE );
	
				m_RenderStates.bFog = pMesh->bFog;
			}

			// ghosts in fog must override fog color part (fog override)
			if ( pMesh->bFog && pMesh->bGhost )
			{
				if ( m_RenderStates.bFogOverride==false )
				{
					m_pD3D->SetRenderState ( D3DRS_FOGCOLOR, D3DCOLOR_RGBA ( 0, 0, 0, 0 ) );
					m_RenderStates.bFogOverride=true;
				}
			}
			else
			{
//				leefix - 030205 - seems universe getting no fog color because this condition fails!?!
//				if ( m_RenderStates.bFogOverride==true )
//				{
				m_pD3D->SetRenderState ( D3DRS_FOGCOLOR, m_RenderStates.dwFogColor );
				m_RenderStates.bFogOverride=false;
//				}
			}
		}
	}

	// ambient - leefix - 230604 - u54 - no ambience can now be 255 or 0 (so need to do code in those cases)
	if ( pMesh->bAmbient==false && m_RenderStates.iAmbient==1
	||	 m_RenderStates.iAmbient==0 || m_RenderStates.iAmbient==2 )
	{
		if ( pMesh->bAmbient )
		{
			m_pD3D->SetRenderState ( D3DRS_AMBIENT, m_RenderStates.dwAmbientColor );
			m_RenderStates.iAmbient = 1;
		}
		else
		{
			// leefix - 210303 - diffuse colour must be maintained over any ambience
			bool bWhite=false;
			if ( pMesh->pTextures )
				if ( pMesh->pTextures[0].iImageID!=0 )
					bWhite=true;

			// leefix - 210303 - white used for no ambient on a texture
			if ( bWhite )
			{
				m_pD3D->SetRenderState ( D3DRS_AMBIENT, D3DCOLOR_ARGB(255,255,255,255) );
				m_RenderStates.iAmbient = 2;
			}
			else
			{
				m_pD3D->SetRenderState ( D3DRS_AMBIENT, D3DCOLOR_ARGB(0,0,0,0) );
				m_RenderStates.iAmbient = 0;
			}
		}
	}

	// leefix - 070204 - introduced for better Zwrite control
	bool bCorrectZWriteState = pMesh->bZWrite;

	// transparency (leefix - 190303 - added second condition where transparency is reimposed after a ghosted object)
	bool bDoGhostAgain = false;
	if ( pMesh->bTransparency != m_RenderStates.bTransparency
	||	 pMesh->dwAlphaTestValue != m_RenderStates.dwAlphaTestValue
	|| ( pMesh->bTransparency==true && m_RenderStates.bGhost==true	) )
	{
		if ( pMesh->bTransparency )
		{
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE,	true );
			m_pD3D->SetRenderState ( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA );
			m_pD3D->SetRenderState ( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA );
			m_pD3D->SetRenderState ( D3DRS_ALPHATESTENABLE,		true );
			
			// mike - 020904 - use this for alpha testing - do not get edges anymore
			// lee - 240903 - need full range of alpha rendered, not just the upper band
			DWORD dwuseAlphaTestValue = pMesh->dwAlphaTestValue;

			if ( dwuseAlphaTestValue==0 )
			{
				m_pD3D->SetRenderState ( D3DRS_ALPHAFUNC,	D3DCMP_GREATER );
				m_pD3D->SetRenderState ( D3DRS_ALPHAREF,	(DWORD)0x00000000 );
			}
			else
			{
				// leeadd - 131205 - let SetAlphaMappingOn command scale the alpha-test to let semi-transparent pixel through
				if ( pMesh->bAlphaOverride==true )
				{
					// alpha mapping percentage vased alpha test
					DWORD dwPercAlpha = ( (pMesh->dwAlphaOverride & 0xFF000000) >> 24 ) ;
					float perc = (float)dwPercAlpha / 255.0f;
					// leefix - 071208 - alpha test transition not perfect as go from override to 0xCF based alpha, so cap it
					// DWORD dwAlphaLevelToDraw = (DWORD)(128.0f * perc);
					DWORD dwAlphaLevelToDraw = (DWORD)(255 * perc);
					dwuseAlphaTestValue = dwAlphaLevelToDraw;
					if ( dwuseAlphaTestValue > (DWORD)0x000000CF ) dwuseAlphaTestValue=(DWORD)0x000000CF;
					m_pD3D->SetRenderState ( D3DRS_ALPHAFUNC,	D3DCMP_GREATEREQUAL );
				}
				else
				{
					// regular alpha test
					dwuseAlphaTestValue=(DWORD)0x000000CF;
					m_pD3D->SetRenderState ( D3DRS_ALPHAFUNC,	D3DCMP_GREATEREQUAL );
				}
				m_pD3D->SetRenderState ( D3DRS_ALPHAREF,	dwuseAlphaTestValue );
			}
			m_RenderStates.dwAlphaTestValue = dwuseAlphaTestValue;
		}
		else
		{
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE,	false );
			m_pD3D->SetRenderState ( D3DRS_ALPHATESTENABLE,		false );
			m_pD3D->SetRenderState ( D3DRS_ALPHAFUNC,			D3DCMP_ALWAYS );
		}
		m_RenderStates.bTransparency = pMesh->bTransparency;

		// now must do ghost again - to combine with blend settings
		bDoGhostAgain = true;
	}

	// ghost
	if ( bDoGhostAgain==true
	||	pMesh->bGhost != m_RenderStates.bGhost
	||  pMesh->iGhostMode != m_RenderStates.iGhostMode )
	{
		if ( pMesh->bGhost )
		{
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
			switch ( pMesh->iGhostMode )
			{
				case 0:
				{
					m_pD3D->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
					m_pD3D->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
				}
				break;

				case 1:
				{
					// lee - 220306 - u6b4 - direct from best of DBC (darkghostmode7)
					DWORD dwDarkAlphaSourceBlend = D3DBLEND_ZERO;
					DWORD dwDarkAlphaDestinationBlend = D3DBLEND_SRCCOLOR;
					m_pD3D->SetRenderState( D3DRS_SRCBLEND,  dwDarkAlphaSourceBlend );
					m_pD3D->SetRenderState( D3DRS_DESTBLEND, dwDarkAlphaDestinationBlend );
				}
				break;

				case 2:
				{
					m_pD3D->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR );
					m_pD3D->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
				}
				break;

				case 3:
				{
					m_pD3D->SetRenderState ( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
					m_pD3D->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );
				}
				break;

				case 4:
				{
					m_pD3D->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR );
					m_pD3D->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR );
				}
				break;

				case 5:
				{
					// leeadd - 210806 - replace OLD-MODE-1 (used in FPSC) for Scorch Texture Multiply
					m_pD3D->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR );
					m_pD3D->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
				}
				break;
			}
			bCorrectZWriteState = false;
		}
		else
		{
			// no ghost and no transparency, end alpha blend effect
			if ( pMesh->bTransparency==false)
			{
				m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );
			}
		}

		m_RenderStates.bGhost = pMesh->bGhost;
		m_RenderStates.iGhostMode = pMesh->iGhostMode;
	}

	// leefix - 070204 - simplified - set zwrite state
	if ( pMesh->bZWrite != m_RenderStates.bZWrite )
	{
		if ( pMesh->bZWrite )
			m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,		TRUE );
		else
			m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,		FALSE );

		m_RenderStates.bZWrite = pMesh->bZWrite;
	}

	// leeadd - 080604 - ZBIAS handling - always set unless not active, then unset once
	if ( pMesh->bZBiasActive )
	{
		m_pD3D->SetRenderState ( D3DRS_DEPTHBIAS,			*(DWORD*)&pMesh->fZBiasDepth );
		m_pD3D->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*(DWORD*)&pMesh->fZBiasSlopeScale );
		m_RenderStates.bZBiasActive = true;
	}
	else
	{
		if ( m_RenderStates.bZBiasActive )
		{
			m_pD3D->SetRenderState ( D3DRS_DEPTHBIAS,			0 );
			m_pD3D->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	0 );
			m_RenderStates.bZBiasActive = false;
		}
	}

	// set zread state
	if ( pMesh->bZRead != m_RenderStates.bZRead )
	{
		if ( pMesh->bZRead )
			m_pD3D->SetRenderState ( D3DRS_ZENABLE,		TRUE );
		else
			m_pD3D->SetRenderState ( D3DRS_ZENABLE,		FALSE );

		m_RenderStates.bZRead = pMesh->bZRead;
	}

	// set the new material and render state
	SetMeshMaterial ( pMesh, &pMesh->mMaterial );

	// mike - 230505 - need to be able to set mip map LOD bias on a per mesh basis
	m_pD3D->SetSamplerState ( 0, D3DSAMP_MIPMAPLODBIAS, *( ( LPDWORD ) ( &pMesh->fMipMapLODBias ) ) );

	// success
	return true;
}

bool CObjectManager::SetMeshTextureStates ( sMesh* pMesh )
{
	// close off any stages from previous runs
	if ( m_dwLastTextureCount > pMesh->dwTextureCount )
	{
		for ( DWORD dwTextureStage = pMesh->dwTextureCount; dwTextureStage < m_dwLastTextureCount; dwTextureStage++ )
		{
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLOROP, D3DTOP_DISABLE );
		}
	}

	// texture filtering and blending
	DWORD dwTextureCountMax = pMesh->dwTextureCount;
	if ( dwTextureCountMax>=MAXTEXTURECOUNT ) dwTextureCountMax=MAXTEXTURECOUNT;
	for ( DWORD dwTextureIndex = 0; dwTextureIndex < pMesh->dwTextureCount; dwTextureIndex++ )
	{
		// Determine texture stage to write to
		DWORD dwTextureStage = pMesh->pTextures [ dwTextureIndex ].dwStage;

		// Determine texture data ptr
		sTexture* pTexture = &pMesh->pTextures [ dwTextureIndex ];

		// texture wrap and filter modes
		// leefix - 180204 - filter states after first stage where ignored as renderstate only holds the state of one stage at a time
		// lee - 040306 - u6rc5 - commented out renderstate assigns (tetxure counts over 7 wrote into other areas of the data structure!
//		// so address/mip/mag/min states now arrays to the size of the texture count
//		if ( pTexture->dwAddressU != m_RenderStates.dwAddressU[dwTextureIndex] )
//		{
			m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_ADDRESSU, pTexture->dwAddressU );
//			m_RenderStates.dwAddressU[dwTextureIndex] = pTexture->dwAddressU;
//		}
//		if ( pTexture->dwAddressV != m_RenderStates.dwAddressV[dwTextureIndex] )
//		{
			m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_ADDRESSV, pTexture->dwAddressV );
//			m_RenderStates.dwAddressV[dwTextureIndex] = pTexture->dwAddressV;
//		}
//		if ( pTexture->dwMagState != m_RenderStates.dwMagState[dwTextureIndex] )
//		{
			m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MAGFILTER, pTexture->dwMagState );
//			m_RenderStates.dwMagState[dwTextureIndex] = pTexture->dwMagState;
//		}
//		if ( pTexture->dwMinState != m_RenderStates.dwMinState[dwTextureIndex] )
//		{
			m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MINFILTER, pTexture->dwMinState );
//			m_RenderStates.dwMinState[dwTextureIndex] = pTexture->dwMinState;
//		}
//		if ( pTexture->dwMipState != m_RenderStates.dwMipState[dwTextureIndex] )
//		{
			m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MIPFILTER, pTexture->dwMipState );
//			m_RenderStates.dwMipState[dwTextureIndex] = pTexture->dwMipState;
//		}

//		// leeadd - 131205 - new sampler state to make better mipmap results ?!??!
//		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MIPMAPLODBIAS, 1000000 );

		// texture blending modes
		if ( pMesh->bOverridePixelShader )
		{
			// use custom pixel shader to replace blending stages
			m_pD3D->SetPixelShader ( pMesh->pPixelShader );
		}
		else
		{
			// fixed function does not use pixel shaders
			m_pD3D->SetPixelShader ( 0 );

			// fixed function blending (leefix-210703-fixed now at source)
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLOROP, pTexture->dwBlendMode );
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG1, pTexture->dwBlendArg1 );

			// lee - 240206 - u60 - will use TFACTOR (diffuse replacement) instead of regular DIFFUSE
			if ( dwTextureStage==0 && m_RenderStates.bIgnoreDiffuse==true )
			{
				// TFACTOR set in previous call from the INSTANCE drawmesh call (instance diffuse changes)
				m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG2, D3DTA_TFACTOR );
			}
			else
				m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG2, pTexture->dwBlendArg2 );

			// U73 - 210309 - apply extra ARG values
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG0, pTexture->dwBlendArg0 );
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_ALPHAARG0, pTexture->dwBlendArg0 );
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_RESULTARG, pTexture->dwBlendArgR );

			// last texture stage can override alpha with tfactor
			if ( m_RenderStates.bNoMeshAlphaFactor==false )
			{
				if ( pMesh->bAlphaOverride==true )
				{
					if ( dwTextureStage==pMesh->dwTextureCount-1 )
					{
						// instance overrides alpha value using TFACTOR
						m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_ALPHAOP, pTexture->dwBlendMode );
						m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_ALPHAARG2, pTexture->dwBlendArg1 );
						m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
						m_pD3D->SetRenderState( D3DRS_TEXTUREFACTOR, pMesh->dwAlphaOverride );
					}
				}
				else
				{
					// regular alpha operations
					m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_ALPHAOP, pTexture->dwBlendMode );
					m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_ALPHAARG1, pTexture->dwBlendArg1 );
					m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_ALPHAARG2, pTexture->dwBlendArg2 );
				}
			}

			// texture coordinate data
			switch ( pTexture->dwTexCoordMode )
			{
				case 0 :{	// Regular UV Stage Match
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXCOORDINDEX, dwTextureStage );
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXTURETRANSFORMFLAGS, 0 );
						}
						break;

				case 1 :{	// Sphere Mapping
							D3DXMATRIX mat;
							mat._11 = 0.5f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f; 
							mat._21 = 0.0f; mat._22 =-0.5f; mat._23 = 0.0f; mat._24 = 0.0f; 
							mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 1.0f; mat._34 = 0.0f; 
							mat._41 = 0.5f; mat._42 = 0.5f; mat._43 = 0.0f; mat._44 = 1.0f; 
							D3DTRANSFORMSTATETYPE dwTexTS = D3DTS_TEXTURE1;
							if( dwTextureStage==2 ) dwTexTS=D3DTS_TEXTURE2;
							m_pD3D->SetTransform ( dwTexTS, &mat );
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL );
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
						}
						break;

				case 2 :{	// Cube Mapping (leefix - 190303 - works now as camera moves around)
							D3DXMATRIX mat;
							D3DXMATRIX matview;
							m_pD3D->GetTransform ( D3DTS_VIEW, &matview );
							D3DXMatrixInverse ( &mat, NULL, &matview );
							mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 1.0f; 
							D3DTRANSFORMSTATETYPE dwTexTS = D3DTS_TEXTURE0;
							if( dwTextureStage==1 ) dwTexTS=D3DTS_TEXTURE1;
							if( dwTextureStage==2 ) dwTexTS=D3DTS_TEXTURE2;
							m_pD3D->SetTransform ( dwTexTS, &mat );
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR );
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3 );
						}
						break;

				case 3 :{	// Steal UV Stage From Zero
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXCOORDINDEX, 0 );
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXTURETRANSFORMFLAGS, 0 );
						}
						break;

				case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17:
						{	// Set alternate texture bank for UV data
							int iGetFrom = pTexture->dwTexCoordMode-10;
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXCOORDINDEX, iGetFrom );
							m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXTURETRANSFORMFLAGS, 0 );
						}
						break;
			}
		}
	}

	// store number of textures handled for this mesh
	m_dwLastTextureCount = pMesh->dwTextureCount;

	// okay
	return true;
}

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

bool CObjectManager::DrawMesh ( sMesh* pMesh )
{
	// draw a mesh
	
	// get pointer to drawbuffers
	sDrawBuffer* pDrawBuffer = pMesh->pDrawBuffer;
	if ( pDrawBuffer==NULL )
		return true;

	// skip if mesh is invisible
	if ( pMesh->iMeshType==0 )
	{
		// regular mesh mode
		if ( pMesh->bVisible==false )
			return true;
	}
	if ( pMesh->iMeshType==1 )
	{
		// terrain mesh mode (scene culling system)
		if ( g_pGlob->dwRenderCameraID==0 )
			if ( pMesh->bVisible==false )
				return true;
	}

	// call the renderstate function
	if ( !SetMeshRenderStates ( pMesh ) )
		return false;

	// set the input streams
	if ( !SetInputStreams ( pMesh ) )
		return false;

	// set the vertex shader
	if ( !SetVertexShader ( pMesh ) )
		return false;

	// This POINTLIST code is not used by CORE PARTICLES (uses own renderer)
	// nor is it used by CLOTH&PARTICLES (uses quad based rendered meshes)
	// this is actuall used by the PHYSX PLUGIN FOR FLUID PARTICLES
	if ( pMesh->iPrimitiveType == D3DPT_POINTLIST )
	{
		// set a default - mike needs to do this in ANYTHING that creates a pointlist object
		if ( pMesh->Collision.fRadius==0.0f ) pMesh->Collision.fRadius = 50.0f;

		// handle point sprite for distance scaling and default mesh point sprite size
		m_pD3D->SetRenderState( D3DRS_POINTSCALEENABLE, TRUE );
		m_pD3D->SetRenderState( D3DRS_POINTSIZE,		FtoDW(pMesh->Collision.fRadius/100.0f) );
		m_pD3D->SetRenderState( D3DRS_POINTSIZE_MIN,	FtoDW(0.0f) );
		m_pD3D->SetRenderState( D3DRS_POINTSIZE_MAX,	FtoDW(pMesh->Collision.fRadius) );
		m_pD3D->SetRenderState( D3DRS_POINTSCALE_A,		FtoDW(0.0f) );
		m_pD3D->SetRenderState( D3DRS_POINTSCALE_B,		FtoDW(0.0f) );
		m_pD3D->SetRenderState( D3DRS_POINTSCALE_C,		FtoDW(2.0f) );

		// force a basic texture render state
		m_pD3D->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
	}

	// set effect shader (if any)
	UINT uPasses = 1;
	bool bEffectRendering=false;
	//u64 bool bEffectWillUseFFTexturing=true;
	if ( pMesh->pVertexShaderEffect )
	{
		// use an effect
		D3DXMATRIX matWorld;
		m_pD3D->GetTransform ( D3DTS_WORLD, &matWorld );
		uPasses = pMesh->pVertexShaderEffect->Start ( pMesh, matWorld );

		// if FX effect, flag effect code
		if ( pMesh->pVertexShaderEffect->m_pEffect )
		{
			// effect shall be used
			bEffectRendering=true;

			// U74 - BETA9 - 100709 - set states prior to shader begin pass
			if ( pMesh->pTextures )
			{
				for ( DWORD dwTextureIndex = 0; dwTextureIndex < pMesh->dwTextureCount; dwTextureIndex++ )
				{
					DWORD dwTextureStage = pMesh->pTextures [ dwTextureIndex ].dwStage;
					if ( dwTextureStage < 16 )
					{
						// so the object states to do overwrite any shader states that begin pass will set
						sTexture* pTexture = &pMesh->pTextures [ dwTextureIndex ];
						if ( pTexture )
						{
							m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_ADDRESSU, pTexture->dwAddressU );
							m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_ADDRESSV, pTexture->dwAddressV );
							m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MAGFILTER, pTexture->dwMagState );
							m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MINFILTER, pTexture->dwMinState );
							m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MIPFILTER, pTexture->dwMipState );
						}
					}
				}
			}

			// and does NOT require old FF texturing
			// u64 if ( pMesh->pVertexShaderEffect->m_bUseShaderTextures )
			// u64 	bEffectWillUseFFTexturing=false;
		}
	}

	// each mesh can have several render passes
    for(UINT uPass = 0; uPass < uPasses; uPass++)
    {
		// FX Effect or Regular
		if ( bEffectRendering )
		{
			// mike - 300905 - disable fog for shaders
			m_pD3D->SetRenderState ( D3DRS_FOGENABLE, FALSE );
			// m_RenderStates.bFog						= false;

			// begin effect rendernig
	        pMesh->pVertexShaderEffect->m_pEffect->BeginPass ( uPass );
		}

		// old FF texturing code (some effects do not do any texturing stuff)
		// leefixback - 010405 - this allowed non PS shader to use DBP textures
		//                       but it killed shader ability to use DBP textures that HAD PS code!
		// if ( bEffectWillUseFFTexturing==true && uPass==0 )
		if ( bEffectRendering )
		{
			// SHADER EFFECT
			// u64 - 180107 - effects CAN use 'texture object' textures if the effect 
			// did not assign a specfic texture to them (paul request for DarkSHADER)
			if ( pMesh->pTextures )
			{
				for ( DWORD dwTextureIndex = 0; dwTextureIndex < pMesh->dwTextureCount; dwTextureIndex++ )
				{
					DWORD dwTextureStage = pMesh->pTextures [ dwTextureIndex ].dwStage;
					if ( dwTextureStage < 16 )
					{
						// get texture ptr
						sTexture* pTexture = &pMesh->pTextures [ dwTextureIndex ];

						// m_dwUseDynamicTextureMask holds a mask of 32 bits, 1=use dynamic texture form texture object command
						int iUseDyntex = ( ( pMesh->pVertexShaderEffect->m_dwUseDynamicTextureMask >> dwTextureStage ) & 1 );
						if ( iUseDyntex==1 )
						{
							// when in effect, only if texture in effect is NULL should this be allowed
							if ( pTexture->pTexturesRef )
							{
								m_pD3D->SetTexture ( dwTextureStage, pTexture->pTexturesRef );
							}
							else
							{
								if ( pMesh->pTextures [ dwTextureIndex ].pCubeTexture )
									m_pD3D->SetTexture ( dwTextureStage, pTexture->pCubeTexture );
								else
									m_pD3D->SetTexture ( dwTextureStage, NULL);
							}
						}
					}
				}
			}
		}
		else
		{
			// FIXED FUNCTION TEXTURING

			// call the texturestate function
			if ( !SetMeshTextureStates ( pMesh ) )
				return false;

			// is there a texture
			if ( pMesh->pTextures )
			{
				// store the current texture
				m_iCurrentTexture = pMesh->pTextures [ 0 ].iImageID;

				// is it different to the last texture we set (leefix-040803-and only if single texture otherwise lightmaps might be used)
				if ( m_iCurrentTexture != m_iLastTexture || pMesh->dwTextureCount>1 )
				{
					// set the new texture - along with related stage textures
					for ( DWORD dwTextureIndex = 0; dwTextureIndex < pMesh->dwTextureCount; dwTextureIndex++ )
					{
						// Determine texture stage to write to
						DWORD dwTextureStage = pMesh->pTextures [ dwTextureIndex ].dwStage;

						// Determine texture data ptr
						sTexture* pTexture = &pMesh->pTextures [ dwTextureIndex ];

						if ( pTexture->pTexturesRef )
						{
							// set regular texture
							if ( FAILED ( m_pD3D->SetTexture ( dwTextureStage, pTexture->pTexturesRef ) ) )
								break;
						}
						else
						{
							if ( pMesh->pTextures [ dwTextureIndex ].pCubeTexture )
							{
								// set cube texture
								if ( FAILED ( m_pD3D->SetTexture ( dwTextureStage, pTexture->pCubeTexture ) ) )
									break;
							}
							else
							{
								// set no texture
								if ( FAILED ( m_pD3D->SetTexture ( dwTextureStage, NULL) ) )
									break;
							}
						}
					}

					// now store the current texture
					m_iLastTexture = m_iCurrentTexture;
				}
			}
			else
			{
				// default zero texture
				m_pD3D->SetTexture ( 0, NULL );
				m_iLastTexture = 0;
			}
		}

		// see if we have an index buffer
		if ( pMesh->pIndices )
		{
			// if multimaterial mesh
			if ( pMesh->bUseMultiMaterial )
			{
				// draw several indexed primitives (one for each material)
				sMultiMaterial* pMultiMaterial = pMesh->pMultiMaterial;
				for ( DWORD dwMaterialIndex=0; dwMaterialIndex<pMesh->dwMultiMaterialCount; dwMaterialIndex++ )
				{
					// set mesh-part texture
					m_pD3D->SetTexture ( 0, pMultiMaterial [ dwMaterialIndex ].pTexturesRef );

					// set mesh-part material and render state
					SetMeshMaterial ( pMesh, &pMultiMaterial [ dwMaterialIndex ].mMaterial );

					// draw mesh-part
					if ( pMultiMaterial [ dwMaterialIndex ].dwPolyCount > 0 )
					{
						if ( FAILED ( m_pD3D->DrawIndexedPrimitive (	pDrawBuffer->dwPrimType,
																		pDrawBuffer->dwBaseVertexIndex,
																		pDrawBuffer->dwVertexStart,
																		pDrawBuffer->dwVertexCount,
																		pDrawBuffer->dwIndexStart + pMultiMaterial [ dwMaterialIndex ].dwIndexStart,
																		pMultiMaterial [ dwMaterialIndex ].dwPolyCount	) ) )
						{
							uPass=uPasses;
							break;
						}
					}

					// add to polycount
					if ( g_pGlob ) g_pGlob->dwNumberOfPolygonsDrawn += pMultiMaterial [ dwMaterialIndex ].dwPolyCount;
					if ( g_pGlob ) g_pGlob->dwNumberOfPrimCalls++;
				}

				// restore texture settings next cycle
				m_iLastTexture=-9999999;
			}
			else
			{
				// draw an indexed primitive
				if ( FAILED ( m_pD3D->DrawIndexedPrimitive (	pDrawBuffer->dwPrimType,
																pDrawBuffer->dwBaseVertexIndex,
																pDrawBuffer->dwVertexStart,
																pDrawBuffer->dwVertexCount,
																pDrawBuffer->dwIndexStart,
																pDrawBuffer->dwPrimitiveCount		) ) )
					break;

				// add to polycount
				if ( g_pGlob ) g_pGlob->dwNumberOfPolygonsDrawn += pDrawBuffer->dwPrimitiveCount;
				if ( g_pGlob ) g_pGlob->dwNumberOfPrimCalls++;
			}
		}
		else
		{
			// draw a standard primitive
			if ( FAILED ( m_pD3D->DrawPrimitive (	pDrawBuffer->dwPrimType,
													pDrawBuffer->dwVertexStart,
													pDrawBuffer->dwPrimitiveCount				) ) )
			{
				// leehack-040803-if fail to render, try smaller batches of primitives until we figure it out!
				DWORD dwHowManyLeft = pDrawBuffer->dwPrimitiveCount;
				DWORD dwVertexBeginData = pDrawBuffer->dwVertexStart;
				DWORD dwPrimCountBatch=65535/3;//leehack-040803-seems some cards report 65535 but can only handle a third of that?!?
				for ( DWORD dwI=0; dwI<=(pDrawBuffer->dwPrimitiveCount/dwPrimCountBatch); dwI++ )
				{
					DWORD dwHowManyToRender = dwPrimCountBatch;
					if ( dwHowManyLeft < dwPrimCountBatch )
						dwHowManyToRender = dwHowManyLeft;

					if ( FAILED ( m_pD3D->DrawPrimitive (	pDrawBuffer->dwPrimType,
															dwVertexBeginData,
															dwHowManyToRender		) ) )
						break;

					// next batch of vertex data
					dwVertexBeginData+=dwHowManyToRender*3;
					dwHowManyLeft -= dwPrimCountBatch;
				}
			}

			// add to polycount
			if ( g_pGlob ) g_pGlob->dwNumberOfPolygonsDrawn += pDrawBuffer->dwPrimitiveCount;
			if ( g_pGlob ) g_pGlob->dwNumberOfPrimCalls++;
		}

		// End FX Effect
		if ( bEffectRendering )	pMesh->pVertexShaderEffect->m_pEffect->EndPass();
	}

	// mike - 300905 - enable fog after being disabled for shaders
	if ( m_RenderStates.bFog )
		m_pD3D->SetRenderState ( D3DRS_FOGENABLE, TRUE );
	

	// Run any end code for any effect used
	if ( pMesh->pVertexShaderEffect )
	    pMesh->pVertexShaderEffect->End();

	// leeadd - 310506 - u62 - end pointlist session (used by PhysX plugin HW fluids)
	if ( pMesh->iPrimitiveType == D3DPT_POINTLIST )
	{
		// end pointlist states
		m_pD3D->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
	}
		
	// okay
	return true;
}

bool CObjectManager::DrawObject ( sObject* pObject )
{
	// draw an object

	// check the object and array index value
	SAFE_MEMORY ( pObject );
	
	// leefix - some TPC can call FASTSYNC after object removed
	// if resource destroyed, quit now
	if ( pObject->dwObjectNumber > 0 )
		if ( g_ObjectList [ pObject->dwObjectNumber ]==NULL )
			return true;

	// setup the world matrix for the object
	CalcObjectWorld ( pObject );

	// skip if object is invisible
	if ( pObject->bVisible==false
	||   pObject->bNoMeshesInObject==true
	||   pObject->bUniverseVisible==false  )
		return true;

	///////////////////////////////////////////////
	///////////////////////////////////////////////
		
	// MIKE : add 05/02/03
	// for linked objects
	// need to neaten this up lee at some point..

	D3DXMATRIX matSavedWorld;
	D3DXMATRIX matNewWorld;

	// mike - 050803 - external DLLs can disable transform it they want
	if ( !pObject->bDisableTransform )
	{
		m_pD3D->SetTransform ( D3DTS_WORLD, &pObject->position.matWorld );
	}
	else
	{
		D3DXMATRIX	matTranslation,
				matScale,
				matObject;


		// World Transform
		D3DXMatrixTranslation ( &matTranslation, 0.0f, 0.0f, 0.0f );
		D3DXMatrixScaling     ( &matScale,       1.0f, 1.0f, 1.0f );
		matObject = matScale * matTranslation;
		m_pD3D->SetTransform ( D3DTS_WORLD, &matObject );
	}

	///////////////////////////////////////////////
	///////////////////////////////////////////////

	// leeadd - 140304 - an object can control its own FOV for rendering
	if ( pObject->bLockedObject==false )
	{
		if ( pObject->fFOV != m_RenderStates.fObjectFOV )
		{
			if ( pObject->fFOV == 0.0f )
				g_Camera3D_SetFOV ( m_RenderStates.fStoreCameraFOV );
			else
				g_Camera3D_SetFOV ( pObject->fFOV );

			m_RenderStates.fObjectFOV = pObject->fFOV;
		}
	}

	// get LOD flag from core object
	int iUsingWhichLOD = pObject->iUsingWhichLOD;

	// actual object or instance of object
	sObject* pActualObject = pObject;
	if ( pActualObject->pInstanceOfObject )
	{
		// get actual object via instance ptr
		pActualObject=pActualObject->pInstanceOfObject;

//		/* U72 - 100109 - whole lot moved until AFTER the LOD alpha changes so it can be per-instance too!
		// if instance uses alpha factor, apply to object
		if ( pObject->bInstanceAlphaOverride )
		{
			// mike - 011005 - change render settings so instanced objects can have their own alpha
			//               - settings and keep transparency
			
			/* U72 - 100109 - not sure if this was my DBP code or Mikes GDK DTSP code..but will leave it commented out as it was in U71
			// set alpha factor and render states here
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE,	true );
			m_pD3D->SetRenderState ( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA );
			m_pD3D->SetRenderState ( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA );
			m_pD3D->SetRenderState ( D3DRS_ALPHATESTENABLE,		true );
			m_pD3D->SetRenderState ( D3DRS_ALPHAFUNC,			D3DCMP_GREATER );
			m_pD3D->SetRenderState ( D3DRS_ALPHAREF,			0x00000000 );

			// set alpha factor and texture stage
			m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
			m_pD3D->SetRenderState( D3DRS_TEXTUREFACTOR, pObject->dwInstanceAlphaOverride );
			*/

			// if mesh exists with blending and argument mode, set the individual instance alpha value
			if ( pActualObject->iMeshCount > 0 )
			{
				sMesh* pMesh = pActualObject->ppMeshList [ 0 ];
				if ( pMesh->dwTextureCount > 0 )
				{
					m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAOP, pMesh->pTextures [ 0 ].dwBlendMode );
					m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAARG2, pMesh->pTextures [ 0 ].dwBlendArg1 );
					m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
					m_pD3D->SetRenderState( D3DRS_TEXTUREFACTOR, pObject->dwInstanceAlphaOverride );
				}
			}
 			m_RenderStates.bNoMeshAlphaFactor = true;
		}

//		U72 - 100109 - commented this out as it was probably from GDK for DTSP (too hidden for my liking)
//		// if instance has a diffuse other than zero, it can override current ARGUMENT2 arrangment
//		if ( pObject->pCustomData )
//		{
//			// use diffuse from material (if present)
//			m_pD3D->SetRenderState ( D3DRS_TEXTUREFACTOR, *(DWORD*)pObject->pCustomData );
//			m_RenderStates.bIgnoreDiffuse = true;
//		}
	}

	// run through all of the frames within the object
	for ( int iFrame = 0; iFrame < pActualObject->iFrameCount; iFrame++ )
	{
		// if instance limb visibility hidden, skip now
		if ( pObject->pInstanceMeshVisible )
		{
			// if limb in instance hidden, skip
			if ( pObject->pInstanceMeshVisible [ iFrame ]==false )
				continue;
		}

		// get a pointer to the frame
		sFrame* pFrame = pActualObject->ppFrameList [ iFrame ];

		// 301007 - new limb excluder
		if ( pFrame==NULL ) continue;
		if ( pFrame->bExcluded==true ) continue;

		// get mesh from frame
		sMesh* pMesh = pFrame->pMesh;

		// calculate correct absolute world matrix
		CalculateAbsoluteWorldMatrix ( pObject, pFrame, pMesh );

		// draw mesh
		if ( pMesh )
		{
			// apply the final transform
			if ( !pMesh->bLinked )
			{
				// mike 170505 - new matrix for completely custom, physics needs this for implementing it's own matrix
				if ( pFrame->bOverride )
				{
					if ( FAILED ( m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &pFrame->matOverride ) ) )
						return false;
				}
				else if ( !pObject->bDisableTransform )
				{
					// mike - 140803
					if ( FAILED ( m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &pFrame->matAbsoluteWorld ) ) )
						return false;
				}
			}

			// update world transform for LOD quad (always faces camera)
			D3DXMATRIX matQuadRotation;

            // u74b7 - moved to UpdateLayer
			// tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData ( g_pGlob->dwRenderCameraID );
			if ( (pObject->iUsingOldLOD==3 || pObject->iUsingWhichLOD==3) && m_pCamera )
			{
				float dx = pObject->position.vecPosition.x - m_pCamera->vecPosition.x;
				float dz = pObject->position.vecPosition.z - m_pCamera->vecPosition.z;
				float theangle = atan2 ( dx, dz );
				D3DXMatrixRotationY(&matQuadRotation, theangle );
				matQuadRotation._41 = pFrame->matAbsoluteWorld._41;
				matQuadRotation._42 = pFrame->matAbsoluteWorld._42;
				matQuadRotation._43 = pFrame->matAbsoluteWorld._43;
			}

			// leeadd - U71 - determine LOD meshes (current and old (transition) if applicable)
			sMesh* pCurrentLOD = pMesh;
			if ( iUsingWhichLOD==1 && pFrame->pLOD[0] ) pCurrentLOD = pFrame->pLOD[0];
			if ( iUsingWhichLOD==2 && pFrame->pLOD[1] ) pCurrentLOD = pFrame->pLOD[1];
			if ( iUsingWhichLOD==3 && pFrame->pLODForQUAD ) pCurrentLOD = pFrame->pLODForQUAD;
			sMesh* pOldLOD = NULL;
			if ( pObject->iUsingOldLOD != -1 )
			{
				// the old lod mesh
				if ( pObject->iUsingOldLOD==0 ) pOldLOD = pMesh;
				if ( pObject->iUsingOldLOD==1 && pFrame->pLOD[0] ) pOldLOD = pFrame->pLOD[0];
				if ( pObject->iUsingOldLOD==2 && pFrame->pLOD[1] ) pOldLOD = pFrame->pLOD[1];
				if ( pObject->iUsingOldLOD==3 && pFrame->pLODForQUAD ) pOldLOD = pFrame->pLODForQUAD;

				// transition in progress from OLD to CURRENT
				pObject->fLODTransition += 0.03f;
				if ( pObject->fLODTransition >= 2.0f )
				{
					// end transition and restore alpha states
					pObject->fLODTransition = 0.0f;
					pObject->iUsingOldLOD = -1;
					D3DCOLOR dwAlphaValueOnly = D3DCOLOR_ARGB ( 255, 0, 0, 0 );
					pCurrentLOD->dwAlphaOverride = dwAlphaValueOnly;
					pCurrentLOD->bAlphaOverride = false;
					pCurrentLOD->bZWrite = true;
					pCurrentLOD->bZBiasActive = false; // U74 - 120409 - refresh each cycle for each instance
					pCurrentLOD->fZBiasDepth = 0.0f; // U74 - 120409 - refresh each cycle for each instance
					pOldLOD->dwAlphaOverride = dwAlphaValueOnly;
					pOldLOD->bAlphaOverride = false;
					pOldLOD->bZWrite = true;
					pOldLOD->bZBiasActive = false;
					pOldLOD->fZBiasDepth = 0.0f;
					pOldLOD = NULL;

					// U72 - 100109 - record alpha state of this mesh (for when instance is not being calculated, i.e. updated in actual mesh for render state change)
					if ( pObject->pInstanceOfObject ) pObject->dwInstanceAlphaOverride = dwAlphaValueOnly;
				}
				else
				{
					// change alpha level of meshes involved in transition
					if ( pObject->fLODTransition<=1.0f )
					{
						// FIRST fade in current LOD mesh
						DWORD dwAlpha = (DWORD)(pObject->fLODTransition*255);
						D3DCOLOR dwAlphaValueOnly = D3DCOLOR_ARGB ( dwAlpha, 0, 0, 0 );
						pCurrentLOD->dwAlphaOverride = dwAlphaValueOnly;
						pCurrentLOD->bAlphaOverride = true;
						pCurrentLOD->bTransparency = true;

						// AND first bit of fade in switch off Zwrite so the 'appearing' image goes not clip the
						// new current mesh and other objects in the area (causing the background to come through)
						if ( pObject->iUsingWhichLOD!=3 )
						{
							if ( pObject->fLODTransition < 0.5f )
								pCurrentLOD->bZWrite = false;
							else
								pCurrentLOD->bZWrite = true;

							pCurrentLOD->bZBiasActive = false;
							pCurrentLOD->fZBiasDepth = 0.0f;
						}
						else
						{
							// last LODQUAD mesh is a plane, so can adjust bias ahead to ensure it is rendered ABOVE everything
							pCurrentLOD->bZWrite = false;
							pCurrentLOD->fZBiasDepth = -g_fZBiasEpsilon;
							pCurrentLOD->bZBiasActive = true;
						}

						// and OLD LOD must stay as reset
						if ( pOldLOD )
						{
							pOldLOD->dwAlphaOverride = D3DCOLOR_ARGB ( 255, 0, 0, 0 );
							pOldLOD->bAlphaOverride = false;
							pOldLOD->bZWrite = true;
							pOldLOD->bZBiasActive = false;
							pOldLOD->fZBiasDepth = 0.0f;
						}
					}
					else
					{
						// Ensure current LOD mesh is default (writing Z and no bias)
						pCurrentLOD->dwAlphaOverride = D3DCOLOR_ARGB ( 255, 0, 0, 0 );
						pCurrentLOD->bAlphaOverride = false;
						pCurrentLOD->bZWrite = true;
						pCurrentLOD->bZBiasActive = false;
						pCurrentLOD->fZBiasDepth = 0.0f;

						// AND now as OLD one fades away, push zbias so NEW/CURRENT mesh has all of Z buffer opportunity
						if ( pObject->iUsingOldLOD!=3 )
						{
							// except the last LODQUAD, which needs zbias as is to do proper fade out
							pOldLOD->fZBiasDepth = g_fZBiasEpsilon * (pObject->fLODTransition-1.0f);
							pOldLOD->bZBiasActive = true;

							// AND last bit of fade out switch off Zwrite so the 'almost gone' image goes not clip the
							// new current mesh and other objects in the area (causing the background to come through)
							if ( pObject->fLODTransition > 1.5f )
								pOldLOD->bZWrite = false;
							else
								pOldLOD->bZWrite = true; // U74 - 120409 - refresh each cycle for each instance
						}
						else
						{
							// For the last LOD QUAD, make the decal fade out slower (to avoid the flick against the sky)
							pObject->fLODTransition -= 0.01f;

							// U74 - 120409 - no zbias effect
							pOldLOD->bZBiasActive = false;
							pOldLOD->fZBiasDepth = 0.0f;

							// also disable ALL zwrites from LODQUAD to avoid artefacts
							pOldLOD->bZWrite = false;
						}

						// THEN fade out old LOD mesh
						DWORD dwAlpha = (DWORD)((2.0f-pObject->fLODTransition)*255);
						D3DCOLOR dwAlphaValueOnly = D3DCOLOR_ARGB ( dwAlpha, 0, 0, 0 );
						pOldLOD->dwAlphaOverride = dwAlphaValueOnly;
						pOldLOD->bAlphaOverride = true;
						pOldLOD->bTransparency = true;
					}
				}
			}
			else
			{
				// U72 - 100109 - mesh not in transition, but still need the alpha state if this is an instanced object
				if ( pObject->pInstanceOfObject && pCurrentLOD )
				{
					pCurrentLOD->dwAlphaOverride = pObject->dwInstanceAlphaOverride;
					pCurrentLOD->bAlphaOverride = false;
					pCurrentLOD->bZWrite = true;
					pCurrentLOD->bZBiasActive = false;
					pCurrentLOD->fZBiasDepth = 0.0f;
				}
			}

			/* U72 - 110109 - use only ONE quad image (rotating them creates jumps and adds to performance cost)
			// also update QUAD UV image to reflect current angle between camera and this object
			// DUPLICATED BELOW FOR OLD MESH AS WELL! This needs improving!!
			if ( pCurrentLOD )
			{
				if ( pObject->iUsingWhichLOD==3 && m_Camera_Ptr )
				{
					// angle between this object and camera view
					float dx = m_Camera_Ptr->vecPosition.x - pObject->position.vecPosition.x;
					float dz = pObject->position.vecPosition.z - m_Camera_Ptr->vecPosition.z;
					float theangle = D3DXToDegree(atan2(dx,dz)) - 90.0f + 12.5f;
					theangle=theangle/(360/16);
					if ( theangle > 15 ) theangle -= 16;
					if ( theangle < 00 ) theangle += 16;

					// try same decal (reduce popping?)
					theangle = 0;

					int tv=theangle/4;
					int tu=theangle-(tv*4);
					float fS=1.0/4.0;
					float fU=fS*tu;
					float fV=fS*tv;

					// current LOD mesh needs UV editing to get right image-direction
					SetUVData ( pCurrentLOD, 0, fU+fS, fV );
					SetUVData ( pCurrentLOD, 1, fU, fV );
					SetUVData ( pCurrentLOD, 2, fU+fS, fV+fS );
					SetUVData ( pCurrentLOD, 3, fU, fV );
					SetUVData ( pCurrentLOD, 4, fU, fV+fS );
					SetUVData ( pCurrentLOD, 5, fU+fS, fV+fS );

					// instruct engine to update this mesh in the buffer IMMEDIATELY
					// pCurrentLOD->bVBRefreshRequired = true;
					UpdateObjectMeshInBuffer ( pCurrentLOD );
					pCurrentLOD->bVBRefreshRequired=false;

					// slow way, should find another way WITHOUT HAVING TO LOCK VERTEX DATA IN RENDER LOOP!!!
					sVertexData* pVertexData = (sVertexData*)pCurrentLOD->pDrawBuffer->pVBListEntryRef;
					if ( pVertexData->bBufferLocked==true )
					{
						// unlock and restore flag
						pVertexData->pVB->Unlock ( );
						pVertexData->bBufferLocked=false;
					}
				}
			}
			// also update QUAD UV image to reflect current angle between camera and this object
			if ( pOldLOD )
			{
				if ( pObject->iUsingOldLOD==3 && m_Camera_Ptr )
				{
					// angle between this object and camera view
					float dx = m_Camera_Ptr->vecPosition.x - pObject->position.vecPosition.x;
					float dz = pObject->position.vecPosition.z - m_Camera_Ptr->vecPosition.z;
					float theangle = D3DXToDegree(atan2(dx,dz)) - 90.0f + 12.5f;
					theangle=theangle/(360/16);
					if ( theangle > 15 ) theangle -= 16;
					if ( theangle < 00 ) theangle += 16;

					// try same decal (reduce popping?)
					theangle = 0;

					int tv=theangle/4;
					int tu=theangle-(tv*4);
					float fS=1.0/4.0;
					float fU=fS*tu;
					float fV=fS*tv;

					// current LOD mesh needs UV editing to get right image-direction
					SetUVData ( pOldLOD, 0, fU+fS, fV );
					SetUVData ( pOldLOD, 1, fU, fV );
					SetUVData ( pOldLOD, 2, fU+fS, fV+fS );
					SetUVData ( pOldLOD, 3, fU, fV );
					SetUVData ( pOldLOD, 4, fU, fV+fS );
					SetUVData ( pOldLOD, 5, fU+fS, fV+fS );

					// instruct engine to update this mesh in the buffer IMMEDIATELY
					// pCurrentLOD->bVBRefreshRequired = true;
					UpdateObjectMeshInBuffer ( pOldLOD );
					pOldLOD->bVBRefreshRequired=false;

					// slow way, should find another way WITHOUT HAVING TO LOCK VERTEX DATA IN RENDER LOOP!!!
					sVertexData* pVertexData = (sVertexData*)pOldLOD->pDrawBuffer->pVBListEntryRef;
					if ( pVertexData->bBufferLocked==true )
					{
						// unlock and restore flag
						pVertexData->pVB->Unlock ( );
						pVertexData->bBufferLocked=false;
					}
				}
			}
			*/

			// leeadd - U71 - if in transition, draw OLD first
			if ( pOldLOD )
			{
// shader work
//				pObject->pFrame->matAbsoluteWorld._43 += 450.0f;

				if ( pObject->iUsingOldLOD==3 )
					m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &matQuadRotation );
				else
					m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &pFrame->matAbsoluteWorld );

// shader work
//				pObject->pFrame->matAbsoluteWorld._43 -= 450.0f;
				// ensure this is drawn behind the current one by shifting projection Z range (alternative to DEPTHBIAS which does not work on shaders)
// shader work
//				D3DXMATRIX matBiasProj = m_Camera_Ptr->matProjection;
//				float fNear = 0.0f;
//				float fFar = 4900.0f;
//				D3DXMatrixPerspectiveFovLH ( &matBiasProj, D3DXToRadian(m_Camera_Ptr->fFOV), m_Camera_Ptr->fAspect, fNear, fFar );
//				m_pD3D->SetTransform ( D3DTS_PROJECTION, &matBiasProj );

				// draw old LOD mesh
				if ( !DrawMesh ( pOldLOD ) )
					return false;

				// restore projection matrix
// shader work
//				m_pD3D->SetTransform ( D3DTS_PROJECTION, &m_Camera_Ptr->matProjection );

				if ( pObject->iUsingWhichLOD!=3 )
					m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &pFrame->matAbsoluteWorld );
			}

			// update world transform for LOD quad (always faces camera)
			if ( pObject->iUsingWhichLOD==3 )
				m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &matQuadRotation );

			// draw the current mesh
			if ( !DrawMesh ( pCurrentLOD ) )
				return false;

			// render any bound meshes
			// mike 160505 - changed this line so a mesh can draw it's bounds
			if ( pObject->bDrawBounds || pMesh->bDrawBounds )
			{
				if ( pFrame->pBoundBox ) DrawMesh ( pFrame->pBoundBox );
				if ( pFrame->pBoundSphere ) DrawMesh ( pFrame->pBoundSphere );
			}

			// for linked objects
			if ( pMesh->bLinked )
			{
				// mike - 140803
				if ( !pObject->bDisableTransform )
				{
					m_pD3D->GetTransform ( D3DTS_WORLD, &matSavedWorld );
					matNewWorld = pFrame->matOriginal * matSavedWorld;
					m_pD3D->SetTransform ( D3DTS_WORLD, &matNewWorld );
				}
			}
		}
	}

	// if instance uses alpha factor, apply to object
	// if ( pObject->bInstanceAlphaOverride )
	// lee - 240206 - u60 - ALWAYS back to normal - use mesh based alpha factor after override
	m_RenderStates.bNoMeshAlphaFactor = false;
	m_RenderStates.bIgnoreDiffuse = false;

	// sorted, return back
	return true;
}

bool CObjectManager::PostDrawRestores ( void )
{
	// cleanup render states before leave draw process
	m_pD3D->SetPixelShader ( 0 );

	// fixed function blending restores
	DWORD dwMaxTextureStage = 7;
	for ( DWORD dwTextureStage = 0; dwTextureStage < dwMaxTextureStage; dwTextureStage++ )
	{
		// texture filter modes
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		m_pD3D->SetSamplerState ( dwTextureStage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

		// texture coordinate data
		m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_TEXCOORDINDEX, dwTextureStage );

		// texture blending modes
		if ( dwTextureStage==0 )
		{
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLOROP, D3DTOP_MODULATE );
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		}
		else
		{
			m_pD3D->SetTextureStageState ( dwTextureStage, D3DTSS_COLOROP, D3DTOP_DISABLE );
		}
	}

	// must always restore ambient level (for layers)
	m_pD3D->SetRenderState ( D3DRS_AMBIENT, m_RenderStates.dwAmbientColor );
	m_pD3D->SetRenderState ( D3DRS_FOGCOLOR, m_RenderStates.dwFogColor );

	// leeadd - 140304 - ensure FOV is restored
	if ( m_RenderStates.fObjectFOV != 0.0f )
	{
		g_Camera3D_SetFOV ( m_RenderStates.fStoreCameraFOV );
		m_RenderStates.fObjectFOV = 0.0f;
	}

	// okay
	return true;
}

bool CObjectManager::Reset ( void )
{
	// set values to default
	m_iCurrentTexture = -22000000;		// current texture being used
	m_iLastTexture    = -22000000;		// last texture being used
	m_dwLastTextureCount = 0;
	m_dwCurrentShader    = 0;
	m_dwCurrentFVF       = 0;				// current FVF
	m_dwLastShader       = 0;
	m_dwLastFVF          = 0;				// previous FVF
	m_bUpdateVertexDecs  = true;			// reset update vertex settings 
	m_bUpdateStreams     = true;			// reset update stream sources every frame

	// leefix - 200303 - reset cullmode from various stencil effects
	m_RenderStates.dwCullDirection				= m_RenderStates.dwGlobalCullDirection;
	m_pD3D->SetRenderState ( D3DRS_CULLMODE,	m_RenderStates.dwCullDirection );
	m_RenderStates.bCull						= true;
	m_RenderStates.iCullMode					= 0;	

	return true;
}

void CObjectManager::UpdateInitOnce ( void )
{
	// V111 - 110608 - called only once (form the main SYNC call), not from FASTSYNCS

	// ensure that the D3D device is valid
	if ( !m_pD3D )
		return;

	// replace any buffers if object modified (from mesh size change or limb mods)
	if ( !m_ObjectManager.ReplaceAllFlaggedObjectsInBuffers() )
		return;
	
	// update all animation data (advance AnimFame only)
	if ( !m_ObjectManager.UpdateAnimationCycle() )
		return;

	// sort texture list if needed
	if ( !SortTextureList ( ) )
		return;

	// lee - 190307 - u66 - ensure current set camera controls the culling
	// lee - 200307 - u66 - craps up the camera content visibility
	// lee - 300808 - u70 - because we needed to use 'camera being rendered now' not 'g_pGlob->dwCurrentSetCameraID'
	// lee - 300808 - u70 - and moved from UpdateInit because visibility culling happens in UpdateInitOnce!
    // u74b8 - get camera data into member variable
	m_pCamera = (tagCameraData*)g_Camera3D_GetInternalData ( g_pGlob->dwRenderCameraID );
	if ( m_pCamera )
	{
		// projection matrix
		D3DXMATRIX matProj = m_pCamera->matProjection;
		m_pD3D->SetTransform ( D3DTS_PROJECTION, &matProj );

		// lee - 211008 - u71 - regular or reflected (clip mode has reflection modes at 2 and 4)
		if ( m_pCamera->iClipPlaneOn==2 || m_pCamera->iClipPlaneOn==4 )
		{
			// Reflect camera view in clip plane (mirror)
			D3DXMATRIX matView, matReflect;
			D3DXMatrixReflect ( &matReflect, &m_pCamera->planeClip );
			D3DXMatrixMultiply ( &matView, &matReflect, &m_pCamera->matView );
			m_pD3D->SetTransform ( D3DTS_VIEW, &matView );
		}
		else
		{
			// Regular camera view
			D3DXMATRIX matView = m_pCamera->matView;
			m_pD3D->SetTransform ( D3DTS_VIEW, &matView );
		}
	}

	// setup the viewing frustum data
	if ( !SetupFrustum ( 0.0f ) )
		return;

	// setup the visibility list
	if ( !SortVisibilityList ( ) )
		return;

	// update only those that are visible
	if ( !m_ObjectManager.UpdateOnlyVisible() )
		return;

	// leefix - 140604 - moved here to capture latest animation data change
	// refresh all data in VB (from any vertex changes in objects)
	if ( !m_ObjectManager.UpdateAllObjectsInBuffers() )
		return;

	// leeadd - U71 - can render even earlier in pipeline, so this can be flagged to happen earlier in UpdateOnce
	g_bScenePrepared = false;

	// leeadd - U71 - some objects need to be rendered before ANYTHING (camera, light, matrix, terrain even stencilstart)
	// ideal for things like sky boxes that do not interfere with the Z buffer for scene fidelity
	if ( g_bRenderVeryEarlyObjects==true )
	{
		// Prepare main render
		UpdateInit();

		// prepare initial scene states
		if ( !PreSceneSettings ( ) )
			return;

		// render VERY EARLY objects (such as sky)
		UpdateLayer ( -1 );

		// scene prepared
		g_bScenePrepared = true;
	}

	// okay
	return;
}

bool CObjectManager::UpdateInit ( void )
{
	// ensure that the D3D device is valid
	if ( !m_pD3D )
		return false;

	// reset values
	if ( !Reset ( ) )
		return false;
	
	// okay
	return true;
}

// calculate distance from object to camera
float CObjectManager::CalculateObjectDistanceFromCamera ( sObject* pObject )
{
    // u74b8 - If the camera isn't selected, just use main camera
    if (!m_pCamera)
        m_pCamera = (tagCameraData*) g_Camera3D_GetInternalData(0);

    // u74b8 - Use current camera position as the start point, or 0,0,0 for a locked object
    D3DXVECTOR3 vecCamPos = (pObject->bLockedObject) ? D3DXVECTOR3(0,0,0) : m_pCamera->vecPosition;

    // u74b8 - Follow glued objects until you reach the end of the glue-chain, or a
    // glued object that no longer exists.
    while ( pObject->position.iGluedToObj != 0 )
    {
        if (g_ObjectList [ pObject->position.iGluedToObj ] == NULL)
        {
            // Glued to an object that does not exist, so break the chain
            pObject->position.iGluedToObj = 0;
            break;
        }

        pObject = g_ObjectList [ pObject->position.iGluedToObj ];
    }

    float fdx = pObject->position.vecPosition.x - vecCamPos.x;
    float fdy = pObject->position.vecPosition.y - vecCamPos.y;
    float fdz = pObject->position.vecPosition.z - vecCamPos.z;

	return sqrt ( (fdx * fdx) + (fdy * fdy) + (fdz * fdz) );
}

bool CObjectManager::UpdateLayer ( int iLayer )
{
	// if resources destroyed, quit now
	if ( GetSortedObjectVisibleList()==NULL )
		return true;

	// work vars
	int iObject = 0;
	bool bUseStencilWrite=false;
	D3DLIGHT9 lightzero;
	D3DXVECTOR3 vecShadowPos;

	// prepare render states to draw
	if ( !PreDrawSettings ( ) )
		return false;

    // u74b7 - Get camera information for LOD and distance calculation
    DWORD dwCurrentCameraBit = 1 << g_pGlob->dwRenderCameraID;

    // u74b8 - General iterator for vectors
//    std::vector< sObject* >::iterator Current;
//    std::vector< sObject* >::iterator Last;

	// run through all visible objects and draw them (unrolled for performance)
	switch ( iLayer )
	{
    case -1 : // leeadd - U71 - Very Early Objects (rendered even before StencilStart)

        if ( ! m_vVisibleObjectEarly.empty() )
        {
            for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectEarly.size(); ++iIndex )
            {
                sObject* pObject = m_vVisibleObjectEarly [ iIndex ];
                if ( !pObject ) continue;

			    // leeadd - 211006 - u63 - ignore objects whose masks reject the current camera
			    if ( (pObject->dwCameraMaskBits & dwCurrentCameraBit)==0 )
				    continue;

			    // leeadd - 240106 - if any LOD activity
                // u74b8 - avoid recalculation of distance if already sorted by distance
                if ( pObject->bHadLODNeedCamDistance && g_eGlobalSortOrder != E_SORT_BY_DEPTH)
				    pObject->position.fCamDistance = CalculateObjectDistanceFromCamera ( pObject );

			    // call the draw function
			    if ( !DrawObject ( pObject ) )
				    return false;

			    // update any shadow meshes
			    if ( m_bGlobalShadows )
				    if ( pObject->bCastsAShadow && pObject->bVisible )
					    if ( g_pGlob->dwStencilMode==1 )
						    for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
							    if ( pObject->ppFrameList [ iFrame ]->bExcluded==false ) // 301007 - new limb excluder
								    UpdateShadowFrame ( pObject->ppFrameList [ iFrame ], &pObject->position.matWorld, g_iShadowPositionFixed, g_vecShadowPosition );
		    }
        }
		break;

	case 0 : // Main Layer

        if ( ! m_vVisibleObjectStandard.empty() )
        {
            for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectStandard.size(); ++iIndex )
            {
                sObject* pObject = m_vVisibleObjectStandard [ iIndex ];
				if ( !pObject ) continue;

				// leeadd - 211006 - u63 - ignore objects whose masks reject the current camera
				if ( (pObject->dwCameraMaskBits & dwCurrentCameraBit)==0 )
					continue;

				/* Replace with a nice debug visual of shadow volume soon
				// DEBUG SHOW SHADOW
				if ( 1 )
				{
					// set world matrix to base object (should be limb world)
					int tokay=0;
					D3DXMATRIX matDef;
					D3DXMatrixIdentity ( &matDef );
					if ( FAILED ( m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &pObject->position.matWorld ) ) ) 	return false;
					for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
					{
						if ( pObject->ppFrameList [ iFrame ]->pShadowMesh )
						{
							tokay=1;
							DrawMesh ( pObject->ppFrameList [ iFrame ]->pShadowMesh );
						}
					}
				}
				*/

				// do not render static objects
				if ( pObject->bStatic )
					continue;

				// or stencil objects
				if ( pObject->bReflectiveObject )
					continue;

				// leeadd - 240106 - if any LOD activity
                // u74b8 - avoid recalculation of distance if already sorted by distance
                if ( pObject->bHadLODNeedCamDistance && g_eGlobalSortOrder != E_SORT_BY_DEPTH)
					pObject->position.fCamDistance = CalculateObjectDistanceFromCamera ( pObject );

				// call the draw function
				if ( !DrawObject ( pObject ) )
					return false;

				// update any shadow meshes
				if ( m_bGlobalShadows )
					if ( pObject->bCastsAShadow && pObject->bVisible )
						if ( g_pGlob->dwStencilMode==1 )
							for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
								if ( pObject->ppFrameList [ iFrame ]->bExcluded==false ) // 301007 - new limb excluder
									UpdateShadowFrame ( pObject->ppFrameList [ iFrame ], &pObject->position.matWorld, g_iShadowPositionFixed, g_vecShadowPosition );
			}
        }
        break;

	case 1 : // Reflection Stencil Layer

        if ( ! m_vVisibleObjectStandard.empty() )
        {
			// STENCIL DRAW STAGE
			bUseStencilWrite=false;

            for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectStandard.size(); ++iIndex )
            {
                sObject* pObject = m_vVisibleObjectStandard [ iIndex ];
				if ( !pObject ) continue;

				// leeadd - 211006 - u63 - ignore objects whose masks reject the current camera
				if ( (pObject->dwCameraMaskBits & dwCurrentCameraBit)==0 )
					continue;

				// only render Reflective objects
				if ( pObject->bReflectiveObject )
				{
					// prepare for stencil write
					if ( bUseStencilWrite==false )
					{
						// set position and direction of reflection plane
						D3DXPlaneFromPointNormal ( &g_plnReflectionPlane, &pObject->position.vecPosition, &pObject->position.vecLook );
						g_vecReflectionPoint = pObject->position.vecPosition;

						// set the user clipping flag for reflection
						g_bReflectiveClipping = pObject->bReflectiveClipping;

						// turn depth buffer off, and stencil buffer on
						m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,    TRUE );
						m_pD3D->SetRenderState ( D3DRS_STENCILFUNC,      D3DCMP_ALWAYS );
						m_pD3D->SetRenderState ( D3DRS_STENCILREF,       0x1 );
						m_pD3D->SetRenderState ( D3DRS_STENCILMASK,      0xffffffff );
						m_pD3D->SetRenderState ( D3DRS_STENCILWRITEMASK, 0xffffffff );
// leefix - 050303 - fixes garbage in stencil buffer found in GF3 cards
//							m_pD3D->SetRenderState ( D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP );
//							m_pD3D->SetRenderState ( D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP );
// leeifx - 150503 - several objects in overlap would cause stencil to fill with zero when it should stay one
//							m_pD3D->SetRenderState ( D3DRS_STENCILZFAIL,     D3DSTENCILOP_ZERO );
						m_pD3D->SetRenderState ( D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP );
						m_pD3D->SetRenderState ( D3DRS_STENCILFAIL,      D3DSTENCILOP_ZERO );
						m_pD3D->SetRenderState ( D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE );

						// make sure no pixels are written to the z buffer or frame buffer
						m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,     FALSE );
						m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
						m_pD3D->SetRenderState ( D3DRS_SRCBLEND,         D3DBLEND_ZERO );
						m_pD3D->SetRenderState ( D3DRS_DESTBLEND,        D3DBLEND_ONE );

						// clear the stencil buffer
						m_pD3D->Clear ( 0L, NULL, D3DCLEAR_STENCIL, 0L, 1.0f, 0L );

						// flag that the stencil is engaged
						bUseStencilWrite=true;
					}

					// poke holes in stencil
					if ( !DrawObject ( pObject ) )
						return false;
				}
			}
			if ( bUseStencilWrite )
			{
				// Complete stencil write by restoring render mode
				m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,	FALSE );
				m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,	 TRUE );
				m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );
				bUseStencilWrite=false;
			}
        }
		break;

	case 2 : // Shadow Stencil Layer

        if ( ! m_vVisibleObjectStandard.empty() )
        {
			// STENCIL DRAW STAGE
			bUseStencilWrite=false;

            for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectStandard.size(); ++iIndex )
            {
                sObject* pObject = m_vVisibleObjectStandard [ iIndex ];
                if ( !pObject ) continue;

				// leeadd - 211006 - u63 - ignore objects whose masks reject the current camera
				if ( (pObject->dwCameraMaskBits & dwCurrentCameraBit)==0 )
					continue;

				sObject* pActualObject = pObject;
				if ( pActualObject->pInstanceOfObject )
					pActualObject=pActualObject->pInstanceOfObject;

				// also draw any shadows cast by this object
				if ( pActualObject->bCastsAShadow==true && pObject->bVisible==true && pObject->bHideShadow==false )
				{
					// prepare for stencil write
					if ( bUseStencilWrite==false )
					{
						// render the shadow
						StartStencilMeshWrite();
						bUseStencilWrite=true;

						// shadow shader takes light zero, so hack it in
						m_pD3D->GetLight( 0, &lightzero );
						if ( g_iShadowPositionFixed==-1 )
							vecShadowPos = g_vecShadowPosition;
						else
						{
							D3DLIGHT9 lightfrom;
							m_pD3D->GetLight( g_iShadowPositionFixed, &lightfrom );
							vecShadowPos = lightfrom.Position;
						}
						lightzero.Position = vecShadowPos;
						lightzero.Type=D3DLIGHT_POINT;
						m_pD3D->SetLight( 0, &lightzero );
					}

					// all frames of this object
					bool buseCPUNoShaderShadow=false;
					for ( int iFrame = 0; iFrame < pActualObject->iFrameCount; iFrame++ )
					{
						// get a pointer to the frame
						sFrame* pActualFrame = pActualObject->ppFrameList [ iFrame ];
						if ( pActualFrame )
						{
							if ( pActualFrame->bExcluded==true ) // 301007 - new limb excluder
								continue;

							// get mesh from frame
							sMesh* pShadowMesh = pActualFrame->pShadowMesh;
							CalculateAbsoluteWorldMatrix ( pObject, pActualFrame, pShadowMesh );
							if ( pShadowMesh )
							{
								if ( pShadowMesh->Collision.fLargestRadius != -1 )
								{
									// leeadd - shadows can be clipped if outside range
									if ( g_iShadowClipping > 0 )
									{
										if ( g_iShadowClipping==1 )
										{
											if ( pActualFrame->matAbsoluteWorld._43 < g_fShadowClippingMin ) continue;
											if ( pActualFrame->matAbsoluteWorld._43 > g_fShadowClippingMax ) continue;
										}
									}

									// draw shadow at correct world position
									m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &pActualFrame->matAbsoluteWorld );
									DrawMesh ( pShadowMesh );
								}
								else
								{
									// use CPU shadow technique
									buseCPUNoShaderShadow=true;
									continue;
								}
							}
						}
					}
					if ( buseCPUNoShaderShadow==true )
					{
						// set world matrix to base object (should be limb world)
						D3DXMATRIX matDef;
						D3DXMatrixIdentity ( &matDef );
						if ( FAILED ( m_pD3D->SetTransform ( D3DTS_WORLD, ( D3DXMATRIX* ) &pObject->position.matWorld ) ) )
							return false;

						// prepare for stencil write
						if ( bUseStencilWrite==false )
						{
							// render the shadow
							StartStencilMeshWrite();
							bUseStencilWrite=true;
						}

						// old way of drawing shadow
						DrawStencilMesh ( pObject->iFrameCount, pObject->ppFrameList, NULL );
					}
				}
			}
			if ( bUseStencilWrite )
			{
				// Complete stencil write by restoring render mode
				FinishStencilMeshWrite();
				bUseStencilWrite=false;

				// restore light zero now shader finished with light zero position
				m_pD3D->SetLight( 0, &lightzero );
			}
        }
		break;
			
	case 3 : // Overlay Ghost Layer (in stages)

        if ( ! m_vVisibleObjectTransparent.empty() )
        {
			// leeadd - 021205 - new feature which can divide transparent depth-sorted objects by a water
			// line so everything below is rendered, then the water, then everything at normal surface order
			bool bWaterPlaneDivision = false;
			float fWaterPlaneDivisionY = 99999.99f;

			// get list of ghosted objects for depth sort

            // u74b8 - use the current camera, not camera 0
            // tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData( 0 );

            for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectTransparent.size(); ++iIndex )
            {
                sObject* pObject = m_vVisibleObjectTransparent [ iIndex ];
				if ( !pObject ) continue;

				// leeadd - 211006 - u63 - ignore objects whose masks reject the current camera
				if ( (pObject->dwCameraMaskBits & dwCurrentCameraBit)==0 )
					continue;

				// calculate distance from object to camera (fills fCamDistance)
				if ( pObject->bTransparencyWaterLine==true )
				{
					// leeadd - 021205 - transparent object water line, using HEIGHY (Y) as ordering (great for water planes)
					if ( pObject->position.vecPosition.y < fWaterPlaneDivisionY )
						fWaterPlaneDivisionY = pObject->position.vecPosition.y;

					// set as furthest surface distance object (and first to be drawn after underwater objs)
					// u74b8 - use the current camera
					pObject->position.fCamDistance = m_pCamera->fZFar;
					bWaterPlaneDivision = true;
				}
				else
				{
					// regular object vs camera distance
                    // u74b8 - If already sorted by distance, then we've also already
                    //         calculated the camera distance and there's no need to do it again.
                    if (g_eGlobalSortOrder != E_SORT_BY_DEPTH)
                    {
						pObject->position.fCamDistance = CalculateObjectDistanceFromCamera ( pObject );
                    }
				}
			}

			// if some objs underwater division, increase their cam distances so they ALL are drawn first (in same order)
			if ( bWaterPlaneDivision==true )
			{
                for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectTransparent.size(); ++iIndex )
                {
                    sObject* pObject = m_vVisibleObjectTransparent [ iIndex ];
					if ( pObject->bTransparencyWaterLine==false )
					{
						float fBaseOfObj = (pObject->collision.vecMax.y - pObject->collision.vecMin.y);
						fBaseOfObj *= pObject->position.vecScale.y;
						fBaseOfObj = pObject->position.vecPosition.y - fBaseOfObj;
						if ( fBaseOfObj < fWaterPlaneDivisionY )
                        {
                            // u74b8 - use the current camera
							// pObjectA->position.fCamDistance += m_Camera_Ptr->fZFar;
							pObject->position.fCamDistance += m_pCamera->fZFar;
                        }
					}
				}
			}

			// u74b7 - sort objects by distance, replaced bubblesort with STL sort
            std::sort(m_vVisibleObjectTransparent.begin(), m_vVisibleObjectTransparent.end(), OrderByReverseCameraDistance() );

            // draw in correct back to front order
            for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectTransparent.size(); ++iIndex )
            {
                sObject* pObject = m_vVisibleObjectTransparent [ iIndex ];

				// u75b9 - fixes Transparency and Camera Mask problem
                if (( pObject->dwCameraMaskBits & dwCurrentCameraBit ) == 0)
                    continue;

                if ( !DrawObject ( pObject ) )
                {
                    return false;
                }
            }

	    }
		// end ghost layer
		break;

	case 4 : // Overlay Locked/NoZ Layer (added 050406 - u6rc6)
	    
        if ( ! m_vVisibleObjectNoZDepth.empty() )
        {
			// NOZDEPTH LOOP (locked and nozdepth)
			// ( Pass A-ZDepth : Pass B-NoZDepth )
			bool bClearZBuffer = false;
			for ( int iPass=0; iPass<2; iPass++ )
			{
				// LOCKED STAGE
				float fCurrentFOV = 0.0f;
				bool bResetCamera = false;
				D3DXMATRIX matCurrentCameraView;

                for ( DWORD iIndex = 0; iIndex < m_vVisibleObjectNoZDepth.size(); ++iIndex )
                {
                    sObject* pObject = m_vVisibleObjectNoZDepth [ iIndex ];
					if ( !pObject ) continue;

					// leeadd - 211006 - u63 - ignore objects whose masks reject the current camera
					if ( (pObject->dwCameraMaskBits & dwCurrentCameraBit)==0 )
						continue;

					// only render nozdepth objects on second pass
					bool bRenderObject = false;
					if ( iPass==0 && pObject->bNewZLayerObject==false )
					{
						// object has zdepth pass 1
						bRenderObject=true;
					}
					if ( iPass==1 && pObject->bNewZLayerObject==true )
					{
						// object has no zdepth pass 2
						bRenderObject=true;
						if ( bClearZBuffer==false )
						{
							// clear zbuffer
							m_pD3D->Clear ( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
							bClearZBuffer=true;
						}
					}

					// only if object should be rendered
					if ( !bRenderObject )
						continue;

					// locked objects
					if ( pObject->bLockedObject )
					{
						// reset camera
						if ( bResetCamera==false )
						{
							// Store current camera
							m_pD3D->GetTransform ( D3DTS_VIEW, &matCurrentCameraView );

							// LEEFIX - 191102 - record current FOV, and set default FOV
							tagCameraData* m_Camera_Ptr = (tagCameraData*)g_Camera3D_GetInternalData( 0 );
							fCurrentFOV = m_Camera_Ptr->fFOV;
							g_Camera3D_SetFOV ( D3DXToDegree(3.14159265358979323846f/2.905f) );

							// Use Default unmodified camera
							D3DXMATRIX matDefaultCameraView;
							D3DXMatrixIdentity ( &matDefaultCameraView );
							matDefaultCameraView._11 = 1; 
							matDefaultCameraView._12 = 0; 
							matDefaultCameraView._13 = 0;
							matDefaultCameraView._21 = 0; 
							matDefaultCameraView._22 = 1; 
							matDefaultCameraView._23 = 0;
							matDefaultCameraView._31 = 0;
							matDefaultCameraView._32 = 0; 
							matDefaultCameraView._33 = 1;

							// Assign new default camera
							m_pD3D->SetTransform ( D3DTS_VIEW, &matDefaultCameraView );

							// clear zbuffer
							m_pD3D->Clear ( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );

							// New camera established
							bResetCamera=true;
						}
					}
					else
					{
						if ( bResetCamera==true )
						{
							// Restore camera view
							m_pD3D->SetTransform ( D3DTS_VIEW, &matCurrentCameraView );
							g_Camera3D_SetFOV ( fCurrentFOV );
							bResetCamera=false;
						}
					}

					// draw
					if ( !DrawObject ( pObject ) )
						return false;
				}
				if ( bResetCamera )
				{
					// Restore camera view
					m_pD3D->SetTransform ( D3DTS_VIEW, &matCurrentCameraView );
					bResetCamera=false;

					// LEEFIX - 191102 - restore FOV if Locked Object set it (replaced in DBO system wioth better method)
					g_Camera3D_SetFOV ( fCurrentFOV );
				}
			}
		}
		break;
	}

	// restore render states after draw
	if ( !PostDrawRestores ( ) )
		return false;
	
	// okay
	return true;
}

bool CObjectManager::Update ( void )
{
	// leeadd - U71 - can render even earlier in pipeline, so this can be flagged to happen earlier in UpdateOnce
	if ( g_bScenePrepared==false )
	{
		// Prepare main render
		UpdateInit();

		// prepare initial scene states
		if ( !PreSceneSettings ( ) )
			return false;

		// scene prepared
		g_bScenePrepared = true;
	}
	
	// Render Scene from Universe
	if ( g_pUniverse )
		g_pUniverse->Render ( );

	// Main layer render
	UpdateLayer ( 0 );

	// Etch any reflection stencils
	if(g_pGlob->dwStencilMode==2)
		if(g_pGlob->dwRedrawPhase==0)
			UpdateLayer ( 1 );

	// Shadow rendering
	if ( m_bGlobalShadows )
	{
		// Render Shadows from Casting Objects
		if(g_pGlob->dwStencilMode==1)
			if(g_pGlob->dwRedrawPhase==0)
				UpdateLayer ( 2 );

		// Render Shadows from Universe
		if(g_pGlob->dwStencilMode==1)
			if(g_pGlob->dwRedrawPhase==0)
				if ( g_pUniverse )
					g_pUniverse->RenderShadows ( );
	}

	// okay
	return true;
}

bool CObjectManager::UpdateGhostLayer ( void )
{
	// lee - 050406 - u6rc6 - overlay render layer (ghost used to be part of UpdateNoZLayer)
	Reset();
	UpdateLayer ( 3 );
	return true;
}

bool CObjectManager::UpdateNoZLayer ( void )
{
	// Must reset when return to manager
	Reset();

	// Overlay render layer (lock, nozdepth)
	UpdateLayer ( 4 );

	// okay
	return true;
}

//
// Stencil Buffer Management
//

void CObjectManager::SetGlobalShadowsOn ( void )
{
	m_bGlobalShadows = true;
}

void CObjectManager::SetGlobalShadowsOff ( void )
{
	m_bGlobalShadows = false;
}

void CObjectManager::DeleteStencilEffect ( sObject* pObject )
{
	// Stencil effect mode
	DWORD dwMode=g_pGlob->dwStencilMode;

	// decrement shadow count
	if( pObject->bCastsAShadow )
	{
		// remove shadow meshes from all frames
		for ( int iFrame = 0; iFrame < pObject->iFrameCount; iFrame++ )
		{
			// get fframe ptr
			sFrame* pFrame = pObject->ppFrameList [ iFrame ];

			// if frame has shadow
			if ( pFrame->pShadowMesh )
			{
				// remove shadow mesh from drawrender list
				m_ObjectManager.RemoveBuffersUsedByObjectMesh ( pFrame->pShadowMesh );

				// and ensure any objects removed are replaced before the render
				g_bObjectReplacedUpdateBuffers = true;

				// delete shadow mesh
				SAFE_DELETE( pFrame->pShadowMesh );
			}
		}

		pObject->bCastsAShadow = false;
		g_pGlob->dwStencilShadowCount--;
		if(g_pGlob->dwStencilShadowCount==0) dwMode=0;
	}

	// decrement reflection count
	if( pObject->bReflectiveObject )
	{
		pObject->bReflectiveObject = false;
		g_pGlob->dwStencilReflectionCount--;
		if(g_pGlob->dwStencilReflectionCount==0) dwMode=0;
	}

	// defer stencil effect mode
	if(dwMode==0)
	{
		// u74b7 - reorder stencilmode checking to allow the removal of last reflections to reduce the draw count
		if(g_pGlob->dwStencilReflectionCount>0)
		{
			g_pGlob->dwStencilMode=2;
			g_pGlob->dwRedrawCount=2;
		}
		else
		{
			if(g_pGlob->dwStencilShadowCount>0)
				g_pGlob->dwStencilMode=1;
			else
			    g_pGlob->dwStencilMode=0;
			g_pGlob->dwRedrawCount=1;
		}
	}
}

void CObjectManager::DoPrepareFillRender ( DWORD dwFillColor )
{
	// create filler initailly
	if(m_pSquareVB==NULL)
	{
		m_pD3D->CreateVertexBuffer ( 4 * sizeof ( SHADOWVERTEX ), D3DUSAGE_WRITEONLY, D3DFVF_SHADOWVERTEX, D3DPOOL_MANAGED, &m_pSquareVB, NULL );
   	}
	if(m_pSquareVB)
	{
		SHADOWVERTEX*	v  = NULL;
		float			sx = 5000.0f;
		float			sy = 5000.0f;

		m_pSquareVB->Lock ( 0, 0, ( VOID** ) &v, 0 );
		v [ 0 ].p = D3DXVECTOR4 (  0, sy, 0.0f, 1.0f );
		v [ 1 ].p = D3DXVECTOR4 (  0,  0, 0.0f, 1.0f );
		v [ 2 ].p = D3DXVECTOR4 ( sx, sy, 0.0f, 1.0f );
		v [ 3 ].p = D3DXVECTOR4 ( sx,  0, 0.0f, 1.0f );
		v [ 0 ].color = dwFillColor;
		v [ 1 ].color = dwFillColor;
		v [ 2 ].color = dwFillColor;
		v [ 3 ].color = dwFillColor;			
		m_pSquareVB->Unlock ( );
	}
}

void CObjectManager::DoStencilRenderStart ( void )
{
	if(g_pGlob->dwStencilMode==1) //shadow
	{
		if ( m_bGlobalShadows )
		{
			// clear stencil buffer before cast shadow data
			m_pD3D->Clear ( 0L, NULL, D3DCLEAR_STENCIL, 0L, 1.0f, 0L );
		}
	}
	if(g_pGlob->dwStencilMode==2) // reflection
	{
		if(g_pGlob->dwRedrawPhase==1)
		{
			// clear the zbuffer ( leave frame and stencil buffer intact )
			m_pD3D->Clear ( 0L, NULL, D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L );

			// Save the view matrix
			m_pD3D->GetTransform ( D3DTS_VIEW, &g_matReflectionViewSaved );

			// Reflect camera in X-Z plane mirror
			D3DXMATRIX matView, matReflect;
			D3DXMatrixReflect ( &matReflect, &g_plnReflectionPlane );
			D3DXMatrixMultiply ( &matView, &matReflect, &g_matReflectionViewSaved );
			m_pD3D->SetTransform ( D3DTS_VIEW, &matView );

			// For BSP culling camera location within a reflection, record plane position
			g_pGlob->fReflectionPlaneX = g_vecReflectionPoint.x;
			g_pGlob->fReflectionPlaneY = g_vecReflectionPoint.y;
			g_pGlob->fReflectionPlaneZ = g_vecReflectionPoint.z;

			// Set clipping plane to stop objs behind mirror being drawn
			if ( g_bReflectiveClipping )
			{
				m_pD3D->SetClipPlane ( 0, (float*)g_plnReflectionPlane );
				m_pD3D->SetRenderState ( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 );
			}

			// Setup render states to a blended render scene against mask in stencil
			// buffer. An important step here is to reverse the cull-order of the
			// polygons, since the view matrix is being relected.
			m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,	TRUE );
			m_pD3D->SetRenderState ( D3DRS_STENCILREF,		0x1 );
			m_pD3D->SetRenderState ( D3DRS_STENCILFUNC,		D3DCMP_EQUAL );
			m_pD3D->SetRenderState ( D3DRS_STENCILPASS,		D3DSTENCILOP_KEEP );

			// Before render stencil view
			DoPrepareFillRender ( 0xFF000088 );

			// do general fill to show back and reverse of reflection area
			m_pD3D->SetRenderState  ( D3DRS_ZWRITEENABLE,		FALSE );
			m_pD3D->SetRenderState  ( D3DRS_FOGENABLE,			FALSE );
			m_pD3D->SetRenderState  ( D3DRS_CULLMODE,			D3DCULL_NONE );
			m_pD3D->SetRenderState  ( D3DRS_SRCBLEND,			D3DBLEND_ONE );
			m_pD3D->SetRenderState  ( D3DRS_DESTBLEND,			D3DBLEND_ZERO );
			m_pD3D->SetVertexShader ( NULL );
			m_pD3D->SetFVF ( D3DFVF_SHADOWVERTEX );
			m_pD3D->SetStreamSource ( 0, m_pSquareVB, 0, sizeof ( SHADOWVERTEX ) );
			m_pD3D->SetRenderState  ( D3DRS_CULLMODE,			D3DCULL_NONE );
			m_pD3D->DrawPrimitive   ( D3DPT_TRIANGLESTRIP,		0, 2 );
			m_pD3D->SetRenderState  ( D3DRS_FOGENABLE,			TRUE );

			// Settings for regular drawing
			m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,	TRUE );
			m_pD3D->SetRenderState ( D3DRS_SRCBLEND,		D3DBLEND_DESTCOLOR );
			m_pD3D->SetRenderState ( D3DRS_DESTBLEND,		D3DBLEND_ZERO );

			// Need to render everything in reverse
			m_pD3D->SetRenderState ( D3DRS_CULLMODE,		D3DCULL_CW );
			m_RenderStates.dwGlobalCullDirection		  = D3DCULL_CW;
		}
	}
}

void CObjectManager::DoStencilRenderEnd ( void )
{
	if(g_pGlob->dwStencilMode==1) //shadow
	{
		if ( m_bGlobalShadows )
		{
			// Apply all Shadows determined by stencil buffer
			m_pD3D->SetRenderState ( D3DRS_ZENABLE,          FALSE );
			m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,    TRUE );
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pD3D->SetRenderState ( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
			m_pD3D->SetRenderState ( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
			m_pD3D->SetTextureStageState ( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );

			// render shades for shadow overlap
			m_pD3D->SetVertexShader	( NULL );
			m_pD3D->SetFVF			( D3DFVF_SHADOWVERTEX );
			m_pD3D->SetStreamSource	( 0, m_pSquareVB, 0,	sizeof ( SHADOWVERTEX ) );

			// full shade range supported
			int iNumberOfShades = m_StencilStates.dwShadowShades;
			DWORD dwBaseColor = (m_StencilStates.dwShadowStrength)/iNumberOfShades;
			DWORD dwColor = dwBaseColor;
			if ( iNumberOfShades > 1 ) 
			{
				m_pD3D->SetRenderState	( D3DRS_STENCILPASS,	D3DSTENCILOP_ZERO );
				m_pD3D->SetRenderState	( D3DRS_STENCILFUNC,	D3DCMP_EQUAL );
				for ( int iShades=iNumberOfShades*-1; iShades<0; iShades++ )
				{
					dwColor = dwBaseColor * (iShades*-1);
					DoPrepareFillRender( m_StencilStates.dwShadowColor + (dwColor<<24) );
					m_pD3D->SetRenderState ( D3DRS_STENCILREF, *(DWORD*)&iShades );
					m_pD3D->DrawPrimitive ( D3DPT_TRIANGLESTRIP, 0, 2 );
				}
				dwColor = dwBaseColor * iNumberOfShades;
			}
			DoPrepareFillRender( m_StencilStates.dwShadowColor + (dwColor<<24) );
			m_pD3D->SetRenderState	( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
			m_pD3D->SetRenderState	( D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL );
			m_pD3D->SetRenderState ( D3DRS_STENCILREF, 0x00 );
			m_pD3D->DrawPrimitive ( D3DPT_TRIANGLESTRIP, 0, 2 );

			// restore render states
			m_pD3D->SetRenderState ( D3DRS_ZENABLE,          TRUE );
			m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,    FALSE );
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );
		}
	}
	if(g_pGlob->dwStencilMode==2) //reflection
	{
		// Reflection render complete
		if(g_pGlob->dwRedrawPhase==1)
		{
			// fill render for distortion effect on reflection
			DoPrepareFillRender( m_StencilStates.dwReflectionColor );
			m_pD3D->SetRenderState ( D3DRS_ZENABLE,          FALSE );
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pD3D->SetRenderState ( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
			m_pD3D->SetRenderState ( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
			m_pD3D->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			m_pD3D->SetRenderState ( D3DRS_STENCILREF,  0x1 );
			m_pD3D->SetRenderState ( D3DRS_STENCILFUNC, D3DCMP_EQUAL );
			m_pD3D->SetRenderState ( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
			m_pD3D->SetVertexShader ( NULL );
			m_pD3D->SetFVF ( D3DFVF_SHADOWVERTEX );
			m_pD3D->SetStreamSource ( 0, m_pSquareVB, 0, sizeof ( SHADOWVERTEX ) );
			m_pD3D->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
			m_pD3D->DrawPrimitive   ( D3DPT_TRIANGLESTRIP, 0, 2 );
			m_pD3D->SetRenderState ( D3DRS_FOGENABLE,        TRUE );

			// clipping plane off
			if ( g_bReflectiveClipping )
				m_pD3D->SetRenderState ( D3DRS_CLIPPLANEENABLE,		0x00 );

			// restore for continued normal rendering
			m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,		FALSE );
			m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,		TRUE );
			m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE,	FALSE );

			// cull back to normal LEEFIX - 171002 - CW to CCW
			m_pD3D->SetRenderState ( D3DRS_CULLMODE,			D3DCULL_CCW );
			m_RenderStates.dwGlobalCullDirection			  = D3DCULL_CCW;

			// restore view
			m_pD3D->SetTransform ( D3DTS_VIEW, &g_matReflectionViewSaved );
		}
	}
}

void CObjectManager::StartStencilMeshWrite ( void )
{
	// render the shadow (leefix - 260604-u54-reduces selfshadow artifacts)
	float fZBiasSlopeScale = 0.1f;
	m_pD3D->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	*(DWORD*)&fZBiasSlopeScale );
	m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,		FALSE );
	m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,		TRUE );
	m_pD3D->SetRenderState ( D3DRS_STENCILFUNC,			D3DCMP_ALWAYS );
	m_pD3D->SetRenderState ( D3DRS_STENCILREF,			0x1 );
	m_pD3D->SetRenderState ( D3DRS_STENCILMASK,			0xffffffff );
	m_pD3D->SetRenderState ( D3DRS_STENCILWRITEMASK,	0xffffffff );
	m_pD3D->SetRenderState ( D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP );
	m_pD3D->SetRenderState ( D3DRS_STENCILFAIL,			D3DSTENCILOP_KEEP );
	m_pD3D->SetRenderState ( D3DRS_STENCILPASS,			D3DSTENCILOP_INCR );
	m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE,	TRUE );
	m_pD3D->SetRenderState ( D3DRS_SRCBLEND,			D3DBLEND_ZERO );
	m_pD3D->SetRenderState ( D3DRS_DESTBLEND,			D3DBLEND_ONE );

	// lee - 220306 - u6b4 - shadow cannot use the fog effect
	m_pD3D->SetRenderState ( D3DRS_FOGENABLE, FALSE );
	m_RenderStates.bFog	= false;
}

void CObjectManager::DrawFrameOrMesh ( int iFrameCount, sFrame** ppFrameList, sMesh* pOrShadowMesh )
{
	if ( ppFrameList )
	{
		// frame hierarchy
		for ( int iFrame = 0; iFrame < iFrameCount; iFrame++ )
			if ( ppFrameList [ iFrame ]->pShadowMesh )
				DrawMesh ( ppFrameList [ iFrame ]->pShadowMesh );
	}
	else
	{
		// single mesh
		DrawMesh ( pOrShadowMesh );
	}
}

void CObjectManager::DrawStencilMesh ( int iFrameCount, sFrame** ppFrameList, sMesh* pOrShadowMesh )
{
	// DEPTH-FAIL Approach

	// set stencil write operation and CULL
	m_pD3D->SetRenderState ( D3DRS_STENCILZFAIL,		D3DSTENCILOP_INCR );
	m_pD3D->SetRenderState ( D3DRS_STENCILPASS,			D3DSTENCILOP_KEEP );
	m_RenderStates.dwCullDirection = D3DCULL_CCW;
	m_RenderStates.bCull = false;
	m_RenderStates.iCullMode = 0;	

	// BACK of shadow
	DrawFrameOrMesh ( iFrameCount, ppFrameList, pOrShadowMesh );

	// reverse stencil write operation and reverse CULL
	m_pD3D->SetRenderState ( D3DRS_STENCILZFAIL,		D3DSTENCILOP_DECR );
	m_pD3D->SetRenderState ( D3DRS_STENCILPASS,			D3DSTENCILOP_KEEP );
	m_RenderStates.dwCullDirection = D3DCULL_CW;
	m_RenderStates.bCull = false;
	m_RenderStates.iCullMode = 0;	

	// FRONT of shadow
	DrawFrameOrMesh ( iFrameCount, ppFrameList, pOrShadowMesh );

	// CULL direction restore
	m_RenderStates.bCull = false;
	m_RenderStates.dwCullDirection = D3DCULL_CCW;
	m_RenderStates.iCullMode = 0;	
}

void CObjectManager::FinishStencilMeshWrite ( void )
{
	// Complete stencil write by restoring render mode
	m_pD3D->SetRenderState ( D3DRS_SHADEMODE,			D3DSHADE_GOURAUD );
	m_pD3D->SetRenderState ( D3DRS_CULLMODE,			D3DCULL_CCW );
	m_pD3D->SetRenderState ( D3DRS_STENCILENABLE,		FALSE );
	m_pD3D->SetRenderState ( D3DRS_ALPHABLENDENABLE,	FALSE );
	m_pD3D->SetRenderState ( D3DRS_ZWRITEENABLE,		TRUE );
	m_pD3D->SetRenderState ( D3DRS_DEPTHBIAS,			0 );
	m_pD3D->SetRenderState ( D3DRS_SLOPESCALEDEPTHBIAS,	0 );
}

// Post process shaders using QUAD

void CObjectManager::CreateScreenQUAD ( int iInitialCameraZeroImage )
{
	// create a plane quad
	if ( m_pQUAD==NULL )
	{
		// create mesh for quad
		m_pQUAD = new sMesh;
		DWORD dwColor = D3DXCOLOR(1,1,1,1);
		MakeMeshPlain ( true, m_pQUAD, 1, 1, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, dwColor );
		SetCullCWCCW ( m_pQUAD, 0 );

		// also apply default shader
		LPSTR pQuadFX = "quad.fx";
		m_pQUADDefaultEffect = new cExternalEffect;
		m_pQUADDefaultEffect->Load ( pQuadFX, false, false );
		if ( SetSpecialEffect ( m_pQUAD, m_pQUADDefaultEffect ) )
		{
			AddObjectMeshToBuffers ( m_pQUAD, true );
			m_pQUAD->bVertexShaderEffectRefOnly = true;
			m_pQUAD->bVBRefreshRequired = true;
		}
		else
			SAFE_DELETE(m_pQUADDefaultEffect);

		// also apply default texture (camera zero image)
		SetBaseTextureStage ( m_pQUAD, 0, iInitialCameraZeroImage );
	}
}

void CObjectManager::SetScreenTextureQUAD ( int iStage, int iImage )
{
	// only if got QUAD
	if ( !m_pQUAD )
		return;

	// texture mesh with image
	SetBaseTextureStage ( m_pQUAD, iStage, iImage );
}

void CObjectManager::SetScreenEffectQUAD ( int iEffectID )
{
	// only if got QUAD
	if ( !m_pQUAD )
		return;

	// check if effect numbe valid
	if ( iEffectID<=0 )
		return;

	// check the effect exists
	if ( !ConfirmEffect ( iEffectID ) )
		return;

	// get effect ptr
	sEffectItem* pEffectItem = m_EffectList [ iEffectID ];

	// assign vertex shader from effect to QUAD
	SetSpecialEffect ( m_pQUAD, NULL );
	ResetVertexDataInMeshPerMesh ( m_pQUAD );
	SetSpecialEffect ( m_pQUAD, pEffectItem->pEffectObj );
	AddObjectMeshToBuffers ( m_pQUAD, true );
	m_pQUAD->bVertexShaderEffectRefOnly = true;
	m_pQUAD->bVBRefreshRequired = true;
}

void CObjectManager::RenderScreenQUAD ( void )
{
	if ( m_pQUAD )
	{
		DrawMesh ( m_pQUAD );
	}
}

int CObjectManager::GetVisibleObjectCount ( void )
{
	// 301007 - new function
	return m_iVisibleObjectCount;
}

sObject** CObjectManager::GetSortedObjectVisibleList ( void )
{
	// 301007 - new function
	return m_ppSortedObjectVisibleList;
}

