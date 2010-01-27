
#include "cTerrain.h"
#include <stdio.h>

float Limit255 ( float a )
{
	if ( a < 0.0f )
	{
		return 0.0f;
	}
	else if ( a > 255.0f )
	{
		return 255.0f;
	}
	else
	{
		return a;
	}
}

cTerrain::cTerrain ( )
{
	// terrain class constructor, set all member variables to default values

    m_pD3DDevice        = NULL;
    m_pHeightMap        = NULL;
    m_iHeightMapWidth   = 0;
    m_iHeightMapHeight  = 0;
    m_iBlockWidth       = 0;
    m_iBlockHeight      = 0;
    m_iQuadsWide        = 0;
    m_iQuadsHigh        = 0;
    m_pBlock            = NULL;
    m_iBlockCount       = 0;
	m_vecScale          = D3DXVECTOR3 ( 1.0f, 1.0f, 1.0f );
}

cTerrain::~cTerrain ( )
{
	// terrain destructor, call the destroy function

    Destroy ( );
}

void cTerrain::Destroy ( void )
{
	// destroy everything

	// remove the height map
	if ( m_pHeightMap )
		delete [ ] m_pHeightMap;
    
	// clear the terrain blocks
    if ( m_pBlock )
    {
		// run through all blocks
        for ( int i = 0; i < m_iBlockCount; i++ )
        {
			// delete the block
			if ( m_pBlock [ i ] )
				delete m_pBlock [ i ];
        }

		// delete the block array
        delete [ ] m_pBlock;
    }

	// reset member variables
    m_pD3DDevice        = NULL;
    m_pHeightMap        = NULL;
    m_iHeightMapWidth   = 0;
    m_iHeightMapHeight  = 0;
    m_iBlockWidth       = 0;
    m_iBlockHeight      = 0;
    m_iQuadsWide        = 0;
    m_iQuadsHigh        = 0;
	m_pBlock            = NULL;
    m_iBlockCount       = 0;
}

int cTerrain::GetTerrainWidth ( void )
{
	// get width of the terrain
	return m_iHeightMapWidth;
}

int cTerrain::GetTerrainHeight ( void )
{
	// get height of the terrain
	return m_iHeightMapHeight;
}

D3DXVECTOR3& cTerrain::GetScale ( void )
{
	// get scale of the terrain
	return m_vecScale;
}

LPDIRECT3DDEVICE9 cTerrain::GetD3DDevice ( void )
{
	// return the d3d pointer
	return m_pD3DDevice;
}

bool cTerrain::LockTexture ( LPDIRECT3DTEXTURE9 pTexture, DWORD** ppPixels, DWORD* dwPitch, int* dwWidth, int* dwHeight )
{
	// lock a texture so it can be written to
	D3DLOCKED_RECT	d3dlr;
	D3DSURFACE_DESC desc;

	// check all pointers
	if ( !ppPixels || !dwPitch || !dwWidth || !dwHeight )
		return false;
			
	// get info about the texture
	if ( FAILED ( pTexture->GetLevelDesc ( 0, &desc ) ) )
		return false;

	// lock the texture
	if ( FAILED ( pTexture->LockRect ( 0, &d3dlr, 0, 0 ) ) )
		return false;

	// get data
	*ppPixels = ( DWORD* ) d3dlr.pBits;
	*dwPitch  = d3dlr.Pitch;
	*dwWidth  = desc.Width;
	*dwHeight = desc.Height;

	// all done
	return true;
}

void cTerrain::UnlockTexture ( LPDIRECT3DTEXTURE9 pTexture )
{
	// unlock texture
	pTexture->UnlockRect ( NULL );
}

bool cTerrain::LoadTerrain ( 
								char* szFile, float fXScale, float fYScale, float fZScale, int iSplit, int iTile,
								float fLightXDirection, float fLightYDirection, float fLightZDirection,
								float fLightRed, float fLightGreen, float fLightBlue, float fScale,
								int iExcludeSmoothingOfTerrain
						   )
{
	// load and build the terrain

	// save data to member variables
	m_iTile				= iTile;
	m_iSplit			= iSplit;
	m_fLightXDirection	= fLightXDirection;
	m_fLightYDirection	= fLightYDirection;
	m_fLightZDirection	= fLightZDirection;
	m_fLightRed			= fLightRed;
	m_fLightGreen		= fLightGreen;
	m_fLightBlue		= fLightBlue;
	m_fScale			= fScale;
	m_iTile				= iTile;
	m_vecScale			= D3DXVECTOR3 ( fXScale, fYScale, fZScale );

    // check d3d device is valid at this stage
    if ( !m_pD3DDevice )
		return false;

	// build the height map array
	if ( !BuildHeightMapData ( szFile ) )
		return false;

	// set number of quads
    m_iQuadsWide = m_iBlockWidth  - 1;
    m_iQuadsHigh = m_iBlockHeight - 1;	

	// leechange 101205 - messes up continuation of multiple terrains (as it smooths vital connectivity data)
	// lee - 140306 - u60b3 - added this exclusion on a flag so both ways can be achieved
	if ( iExcludeSmoothingOfTerrain==0 )
	{
		// filter the height map
		FilterHeightMap ( );
	}

	// generate the terrain blocks
	if ( !GenerateTerrainBlocks ( ) )
		return false;

	// all done
    return true;
}

bool cTerrain::BuildHeightMapData ( char* szFile )
{
	// get the height map data from the texture

	// our local variables
	LPDIRECT3DTEXTURE9  lpTexture;		// the texture
	DWORD*				pPixels;		// pixel data
	DWORD				dwPitch;		// pitch info
	int					iOffset = 0;	// offset

	// load the height map image
// leefix - 101205 - can have heightmaps not a power of two
//	if ( FAILED ( D3DXCreateTextureFromFileEx ( 
//													m_pD3DDevice, szFile, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
//													0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
//													0, NULL, NULL, &lpTexture
//											  ) ) )
//		return false;
	// added D3DX_DEFAULT_NONPOW2, D3DX_FILTER_NONE
	if ( FAILED ( D3DXCreateTextureFromFileEx ( 
													m_pD3DDevice, szFile, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,
													0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
													0, NULL, NULL, &lpTexture
											  ) ) )
		return false;

	// lock the texture
	if ( !LockTexture ( lpTexture, &pPixels, &dwPitch, &m_iHeightMapWidth, &m_iHeightMapHeight ) )
		return false;

	// set block width and height and create an array to store height values
	m_iBlockWidth  = m_iHeightMapWidth  / m_iSplit;
	m_iBlockHeight = m_iHeightMapHeight / m_iSplit;
	m_pHeightMap   = new float [ m_iHeightMapWidth * m_iHeightMapHeight ];

	// check array was set up
	if ( !m_pHeightMap )
		return false;

	// loop through the texture data and store the information in the height map array
	// leechange - 101205 - switched J and I around so reading across, THEN down (so heightmap oriented correctly)
	for ( int j = 0; j < ( int ) m_iHeightMapHeight; j++ )
	{
		for ( int i = 0; i < ( int ) m_iHeightMapWidth; i++ )
		{
			// get the pixel and then each component
			DWORD pGet    = pPixels [ i + j * dwPitch / 4 ];
			DWORD dwAlpha = pGet >> 24;
			DWORD dwRed   = ( ( ( pGet ) >> 16 ) & 0xff );
			DWORD dwGreen = ( ( ( pGet ) >>  8 ) & 0xff );
			DWORD dwBlue  = ( ( ( pGet ) )       & 0xff );

//			// store the red part of the pixel in the height map array
//			m_pHeightMap [ iOffset++ ] = ( float ) Limit255 ( ( float ) dwRed );
// place the data in the heightmap oriented same as XZ texture
			iOffset = i + ( ( (m_iHeightMapHeight-1) - j ) * m_iHeightMapWidth ); 
			m_pHeightMap [ iOffset ] = ( float ) Limit255 ( ( float ) dwRed );
		}
	}
	
	// unlock the texture
	UnlockTexture ( lpTexture );

	// all done
	return true;
}

bool cTerrain::GenerateTerrainBlocks ( void )
{
    // generate a mesh for each part of the terrain

	// determine the number of blocks we need
	m_iBlocksWide = ( m_iHeightMapWidth  - 1 ) / m_iQuadsWide;
	m_iBlocksHigh = ( m_iHeightMapHeight - 1 ) / m_iQuadsHigh;

	// create an array to hold the blocks
	if ( AddTerrainBlock ( m_iBlocksWide * m_iBlocksHigh ) < 0 )
		return false;

	for ( int z = 0; z < m_iBlocksHigh; z++ )
    {
        for ( int x = 0; x < m_iBlocksWide; x++ )
        {
            cTerrainBlock* pBlock = m_pBlock [ x + z * m_iBlocksWide ];
			// leenote - from a 128 wide terrain, 16 splits, it starts at X 0,7,14,21, not 0,8,16,24...
            if ( !pBlock->GenerateBlock ( this, x * m_iQuadsWide, z * m_iQuadsHigh, m_iBlockWidth, m_iBlockHeight ) )
				return false;
		}
    }

    return true;
}

void cTerrain::FilterHeightMap ( void )
{
	// apply a filter to the height map to smooth it out

	int x, z;
    float Value;

    if ( !m_pHeightMap )
		return;
    
    float* pResult = new float [ m_iHeightMapWidth * m_iHeightMapHeight ];

    if ( !pResult )
		return;

    memcpy ( pResult, m_pHeightMap, m_iHeightMapWidth * m_iHeightMapHeight * sizeof ( float ) );

    for ( z = 1; z < m_iHeightMapHeight - 1; ++z )
    {
        for ( x = 1; x < m_iHeightMapWidth - 1; ++x )
        {
            Value  = m_pHeightMap [ ( x - 1 ) + ( z - 1 ) * m_iHeightMapWidth ]; 
            Value += m_pHeightMap [ ( x     ) + ( z - 1 ) * m_iHeightMapWidth ]; 
            Value += m_pHeightMap [ ( x + 1 ) + ( z - 1 ) * m_iHeightMapWidth ]; 

            Value += m_pHeightMap [ ( x - 1 ) + ( z     ) * m_iHeightMapWidth ]; 
            Value += m_pHeightMap [ ( x     ) + ( z     ) * m_iHeightMapWidth ]; 
            Value += m_pHeightMap [ ( x + 1 ) + ( z     ) * m_iHeightMapWidth ]; 

            Value += m_pHeightMap [ ( x - 1 ) + ( z + 1 ) * m_iHeightMapWidth ]; 
            Value += m_pHeightMap [ ( x     ) + ( z + 1 ) * m_iHeightMapWidth ]; 
            Value += m_pHeightMap [ ( x + 1 ) + ( z + 1 ) * m_iHeightMapWidth ]; 

            pResult [ x + z * m_iHeightMapWidth ] = Value / 9.0f;
		}
	}

    delete [ ] m_pHeightMap;

    m_pHeightMap = pResult;
}

D3DXVECTOR3 cTerrain::GetHeightMapNormal ( int x, int z )
{
	D3DXVECTOR3 Normal, Edge1, Edge2;
	int         HMIndex, HMAddX, HMAddZ;
    float       y1, y2, y3;

	if ( x < 0.0f || z < 0.0f || x >= m_iHeightMapWidth || z >= m_iHeightMapHeight )
		return D3DXVECTOR3 ( 0.0f, 1.0f, 0.0f );

    HMIndex = x + z * m_iHeightMapWidth;
    
    if ( x < ( m_iHeightMapWidth - 1 ) )
		HMAddX = 1;
	else
		HMAddX = -1;

	if ( z < ( m_iHeightMapHeight - 1 ) )
		HMAddZ = m_iHeightMapWidth;
	else
		HMAddZ = -( signed ) m_iHeightMapWidth;
	
    y1 = m_pHeightMap [ HMIndex          ] * m_vecScale.y;
	y2 = m_pHeightMap [ HMIndex + HMAddX ] * m_vecScale.y; 
	y3 = m_pHeightMap [ HMIndex + HMAddZ ] * m_vecScale.y;
			
	Edge1 = D3DXVECTOR3 ( 0.0f,         y3 - y1, m_vecScale.z );
	Edge2 = D3DXVECTOR3 ( m_vecScale.x, y2 - y1, 0.0f         );
			
	D3DXVec3Cross     ( &Normal, &Edge1, &Edge2 );
	D3DXVec3Normalize ( &Normal, &Normal );
	
    return Normal;
}

float cTerrain::GetHeight ( float x, float z, bool ReverseQuad )
{
	// retrieves the height at the given world space location
	// pass in true to the 'ReverseQuad' parameter to reverse the direction
	// in which the quads dividing edge is based ( normally top right to
	// bottom left )

    float fTopLeft, fTopRight, fBottomLeft, fBottomRight;

    x = x / m_vecScale.x;
    z = z / m_vecScale.z;

    if ( x < 0.0f || z < 0.0f || x >= m_iHeightMapWidth || z >= m_iHeightMapHeight )
		return 0.0f;	

    int ix = ( int ) x;
    int iz = ( int ) z;
	
    float fPercentX = x - ( ( float ) ix );
    float fPercentZ = z - ( ( float ) iz );

    if ( ReverseQuad )
    {
        fTopLeft     = m_pHeightMap [   ix       +   iz       * m_iHeightMapWidth ] * m_vecScale.y;
        fBottomRight = m_pHeightMap [ ( ix + 1 ) + ( iz + 1 ) * m_iHeightMapWidth ] * m_vecScale.y;

        if ( fPercentX < fPercentZ )
        {
            fBottomLeft = m_pHeightMap [ ix + ( iz + 1 ) * m_iHeightMapWidth ] * m_vecScale.y;
		    fTopRight   = fTopLeft + ( fBottomRight - fBottomLeft );
        }
        else
        {
            fTopRight   = m_pHeightMap [ ( ix + 1 ) + iz * m_iHeightMapWidth ] * m_vecScale.y;
		    fBottomLeft = fTopLeft + ( fBottomRight - fTopRight );
		}
    }
    else
    {
        fTopRight   = m_pHeightMap [ ( ix + 1 ) +   iz       * m_iHeightMapWidth ] * m_vecScale.y;
        fBottomLeft = m_pHeightMap [   ix       + ( iz + 1 ) * m_iHeightMapWidth ] * m_vecScale.y;

        if ( fPercentX < ( 1.0f - fPercentZ ) )
        {
            fTopLeft     = m_pHeightMap [ ix + iz * m_iHeightMapWidth ] * m_vecScale.y;
            fBottomRight = fBottomLeft + ( fTopRight - fTopLeft );
        }
        else
        {
            fBottomRight = m_pHeightMap [ ( ix + 1 ) + ( iz + 1 ) * m_iHeightMapWidth ] * m_vecScale.y;
            fTopLeft     = fTopRight + ( fBottomLeft - fBottomRight );
		}
    }
  
    float fTopHeight    = fTopLeft    + ( ( fTopRight    - fTopLeft    ) * fPercentX );
    float fBottomHeight = fBottomLeft + ( ( fBottomRight - fBottomLeft ) * fPercentX );

    return fTopHeight + ( ( fBottomHeight - fTopHeight ) * fPercentZ );
}

int cTerrain::AddTerrainBlock ( int Count )
{
	cTerrainBlock** pBlockBuffer = NULL;
    
    if ( !( pBlockBuffer = new cTerrainBlock* [ m_iBlockCount + Count ] ) )
		return -1;

    ZeroMemory ( &pBlockBuffer [ m_iBlockCount ], Count * sizeof ( cTerrainBlock* ) );

    if ( m_pBlock )
    {
        memcpy ( pBlockBuffer, m_pBlock, m_iBlockCount * sizeof ( cTerrainBlock* ) );

        delete [ ] m_pBlock;
	}
    
    m_pBlock = pBlockBuffer;

    for ( int i = 0; i < Count; i++ )
    {
        if ( ! ( m_pBlock [ m_iBlockCount ] = new cTerrainBlock ( ) ) )
			return -1;

        m_iBlockCount++;
	}
    
    return m_iBlockCount - Count;
}

void cTerrain::SetD3DDevice ( LPDIRECT3DDEVICE9 pD3DDevice )
{
    if ( !pD3DDevice )
		return;

    m_pD3DDevice = pD3DDevice;
}

float* cTerrain::GetHeightMap ( void )
{ 
	return m_pHeightMap;
}

cTerrainBlock::cTerrainBlock ( )
{
	m_iStartX       = 0;
    m_iStartZ       = 0;
    m_pParent       = NULL;
}

cTerrainBlock::~cTerrainBlock ( )
{

}

bool cTerrainBlock::GenerateBlock ( cTerrain* pParent, int StartX, int StartZ, int BlockWidth, int BlockHeight )
{
    if ( !pParent || !pParent->GetD3DDevice ( ) || !pParent->GetHeightMap ( ) )
		return false;

    m_pParent      = pParent;
    m_iStartX      = StartX;
    m_iStartZ      = StartZ;
    m_iBlockWidth  = BlockWidth;
    m_iBlockHeight = BlockHeight;
    m_iQuadsHigh   = BlockHeight - 1;
    m_iQuadsWide   = BlockWidth  - 1;

	if ( !GenerateVertices ( ) )
		return false;

   if ( !GenerateIndices ( ) )
		return false;

    return true;
}

bool cTerrainBlock::GenerateVertices ( void )
{
	ULONG				x, z;
    ULONG				Usage      = D3DUSAGE_WRITEONLY;
    USHORT*				pIndex     = NULL;
    float*				pHeightMap = NULL;
    D3DXVECTOR3			VertexPos, LightDir = D3DXVECTOR3 ( 0.650945f, -0.390567f, 0.650945f );

	LightDir.x = m_pParent->m_fLightXDirection;
	LightDir.y = m_pParent->m_fLightYDirection;
	LightDir.z = m_pParent->m_fLightZDirection;

	pHeightMap     = m_pParent->GetHeightMap ( );
    
    m_BoundsMin = D3DXVECTOR3 (  999999.0f,  999999.0f,  999999.0f );
    m_BoundsMax = D3DXVECTOR3 ( -999999.0f, -999999.0f, -999999.0f );

	// m-proto - work out UV dimension of 1 pixel (texture is 514 for this to work)
//	float fTrimU = 1.0f / 514.0f;
//	float fTrimV = 1.0f / 514.0f;
	float fTrimU = 0.0f; // no trim in official plugin until requested (need texture dimension from terrain image)
	float fTrimV = 0.0f;
	float fDeductTwoPixelsU = 1.0f - (fTrimU*2);
	float fDeductTwoPixelsV = 1.0f - (fTrimV*2);

    for ( z = m_iStartZ; z < (ULONG)m_iStartZ + m_iBlockHeight; z++ )
	{
		for ( x = m_iStartX; x < (ULONG)m_iStartX + m_iBlockWidth; x++ )
		{
            VertexPos.x = ( float ) x * m_pParent->GetScale ( ).x;
            VertexPos.y = pHeightMap [ x + z * m_pParent->GetTerrainWidth ( ) ] * m_pParent->GetScale ( ).y;
			VertexPos.z = ( float ) z * m_pParent->GetScale ( ).z;

            float fRed = 1.0f, fGreen = 1.0f, fBlue = 1.0f, fScale = 0.25f;

			fRed   = m_pParent->m_fLightRed;
			fGreen = m_pParent->m_fLightGreen;
			fBlue  = m_pParent->m_fLightBlue;
            
            fScale  = D3DXVec3Dot ( &m_pParent->GetHeightMapNormal ( x,     z     ), & ( -LightDir ) );
            fScale += D3DXVec3Dot ( &m_pParent->GetHeightMapNormal ( x + 1, z     ), & ( -LightDir ) );
            fScale += D3DXVec3Dot ( &m_pParent->GetHeightMapNormal ( x + 1, z + 1 ), & ( -LightDir ) );
            fScale += D3DXVec3Dot ( &m_pParent->GetHeightMapNormal ( x,     z + 1 ), & ( -LightDir ) );
            fScale /= 4.0f;

            fScale += m_pParent->m_fScale;
			
			if ( fScale > 1.0f ) fScale = 1.0f;
            if ( fScale < 0.4f ) fScale = 0.4f;

			sVertex vertex;

			vertex.x         = VertexPos.x;
            vertex.y         = VertexPos.y;
            vertex.z         = VertexPos.z;
  			#ifndef NODIFFUSEINTERRAIN
			 vertex.dwDiffuse = D3DCOLOR_COLORVALUE ( fRed * fScale, fGreen * fScale, fBlue * fScale, 1.0f );
		    #endif
		    // leechange - 101205 - old version missed out last texel of data from Stage Zero Paint Texture
			vertex.tu        =   ( float ) x / m_pParent->m_iHeightMapWidth;
			vertex.tv        = - ( float ) z / m_pParent->m_iHeightMapHeight;
			vertex.tu1       =   ( float ) x / m_pParent->m_iTile;
			vertex.tv1       =   ( float ) z / m_pParent->m_iTile;
			// vertex.tu        = 0.0f + ( fTrimU + ( ( float ) x / (m_pParent->m_iHeightMapWidth-1) ) * fDeductTwoPixelsU );
			// vertex.tv        = 1.0f - ( fTrimV + ( ( float ) z / (m_pParent->m_iHeightMapHeight-1) ) * fDeductTwoPixelsV );
			// vertex.tu1       =   ( float ) x / m_pParent->m_iTile;
			// vertex.tv1       =   ( float ) z / m_pParent->m_iTile;

			m_VertexList.push_back ( vertex );

			if ( VertexPos.x < m_BoundsMin.x ) m_BoundsMin.x = VertexPos.x;
            if ( VertexPos.y < m_BoundsMin.y ) m_BoundsMin.y = VertexPos.y;
            if ( VertexPos.z < m_BoundsMin.z ) m_BoundsMin.z = VertexPos.z;
            if ( VertexPos.x > m_BoundsMax.x ) m_BoundsMax.x = VertexPos.x;
            if ( VertexPos.y > m_BoundsMax.y ) m_BoundsMax.y = VertexPos.y;
            if ( VertexPos.z > m_BoundsMax.z ) m_BoundsMax.z = VertexPos.z;
        }
    }

	return true;
}

float cTerrainBlock::GetX ( int iID )
{
	return m_VertexList [ iID ].x;
}

float cTerrainBlock::GetY ( int iID )
{
	return m_VertexList [ iID ].y;
}

float cTerrainBlock::GetZ ( int iID )
{
	return m_VertexList [ iID ].z;
}

DWORD cTerrainBlock::GetDiffuse ( int iID )
{
	return m_VertexList [ iID ].dwDiffuse;
}

float cTerrainBlock::GetTU ( int iID )
{
	return m_VertexList [ iID ].tu;
}

float cTerrainBlock::GetTV ( int iID )
{
	return m_VertexList [ iID ].tv;
}

float cTerrainBlock::GetTU1 ( int iID )
{
	return m_VertexList [ iID ].tu1;
}

float cTerrainBlock::GetTV1 ( int iID )
{
	return m_VertexList [ iID ].tv1;
}

bool cTerrainBlock::GenerateIndices ( void )
{
    int*	pIndex = NULL;
    int		x, z;

    m_iIndexCount       = 0;
    m_iPrimitiveCount   = 0;
    m_iLayerIndex       = 0;

	for ( z = 0; z < m_iQuadsHigh; z++ )
    {
        WORD LoopStartZ = ( WORD ) ( z + m_iStartZ ) * 1;
        WORD LoopEndZ   = ( WORD ) LoopStartZ + 1;

        for ( x = 0; x < m_iQuadsWide; x++ )
        {
            WORD LoopStartX = ( WORD ) ( x + m_iStartX );
            WORD LoopEndX   = ( WORD ) LoopStartX + 1;

			m_IndexList.push_back (   x +         z       * m_iBlockWidth );
			m_IndexList.push_back (   x +       ( z + 1 ) * m_iBlockWidth );
			m_IndexList.push_back ( ( x + 1 ) + ( z + 1 ) * m_iBlockWidth );
			m_IndexList.push_back (   x +         z       * m_iBlockWidth );
			m_IndexList.push_back ( ( x + 1 ) + ( z + 1 ) * m_iBlockWidth );
			m_IndexList.push_back ( ( x + 1 ) +   z       * m_iBlockWidth );

            m_iIndexCount     += 6;
            m_iPrimitiveCount += 2;
		}
    }

    return true;
}