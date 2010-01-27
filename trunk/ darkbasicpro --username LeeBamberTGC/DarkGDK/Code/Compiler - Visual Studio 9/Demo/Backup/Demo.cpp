
#include "DarkSDK.h"
#include "cBitmap.h"

// GENERATE ONE BASE TEXTURE FOR TERRAIN FROM A COLLECTION OF INPUT TEXTURES

class cArray
{
	// useful class for 2 dimensional floating point array

	public:
		cArray  ( int iDimensionA, int iDimensionB );
		~cArray ( );

		float** m_Array;
		int		m_iDimensionA;
		int		m_iDimensionB;
};

struct sLayer
{
	// layer structure - contains information about each texture layer

	char				szName [ 255 ];
	int					iWidth;
	int					iHeight;
	LPDIRECT3DTEXTURE9	lpTexture;
};

struct sTerrain
{
	// the terrain structure

	char		szHeightMap;
	int			iWidth;
	sLayer		layers [ 16 ];
	int			iLayerCount;
};

struct sImage
{
	// information about the image

	int				iWidth;
	int				iHeight;
	unsigned char*	pData;
};

struct sHeightData
{
	unsigned char*	m_ucpData;
	int				m_iSize;
};

// terrain and direct 3D device
sTerrain			g_Terrain [ 64 ];
LPDIRECT3DDEVICE9   m_pD3DDevice;

// functions we will use
void  SetTerrainHeightMap			( int iID, char* szFile );
void  SetTerrainLayerCount			( int iID, int iCount );
void  SetTerrainLayerTexture		( int iID, int iLayer, int iImage );
void  GenerateTerrainTexture		( int iID, int iTextureSize );
bool  LockTexture					( LPDIRECT3DTEXTURE9 pTexture, DWORD** ppPixels, DWORD* dwPitch, DWORD* dwWidth, DWORD* dwHeight );
void  UnlockTexture					( LPDIRECT3DTEXTURE9 pTexture );
bool  ScaleHeightmap				( cArray* pSource, int iOldWidth, cArray *pDestination, int iNewWidth );
void  GenerateFinalTerrainTexture   ( sTerrain* pTerrain, cArray* pHeightmap, int iHeightmapWidth, int iTexSize, float fRandMod, char* szSave );
void GenerateTerrainPlasmaHeightMap ( char* szFilename, int iSize, float fRoughness );

void SetBitmapFormat ( int iFormat );
void SetSpritesFilterMode ( int iMode );

#include "DBOData.h"
#include <vector>
#include <time.h>
using namespace std;

sObject* dbGetObject ( int iID );

// the entry point for the application
void DarkSDK ( void )
{
	// if this program not compile you are a lover of the winkle

	char szFilename [ 256 ] = "";

	for ( int i = 0; i < 10; i++ )
	{
		sprintf ( szFilename, "large%i.bmp", i );

		srand ( dbTimer ( ) );

		float fValue = ( float ) ( 1 + dbRnd ( 100 ) ) / 100.0f;

		GenerateTerrainPlasmaHeightMap ( szFilename, 512, fValue );
	}
	return;

	// get the direct 3d device
	m_pD3DDevice = dbGetDirect3DDevice ( );

	// load the textures
	dbLoadImage ( "set2\\texture1.tga", 1 );
	dbLoadImage ( "set2\\texture2.tga", 2 );
	dbLoadImage ( "set2\\texture3.tga", 3 );
	dbLoadImage ( "set2\\texture4.tga", 4 );

	// set the terrain height map, number of layers and the image ID
	// of each layer, last of all generate a final texture of size
	// 2048 x 2048
	SetTerrainHeightMap     ( 1, "map.bmp" );
	SetTerrainLayerCount    ( 1, 4 );
	SetTerrainLayerTexture  ( 1, 0, 1 );
	SetTerrainLayerTexture  ( 1, 1, 2 );
	SetTerrainLayerTexture  ( 1, 2, 3 );
	SetTerrainLayerTexture  ( 1, 3, 4 );
	GenerateTerrainTexture  ( 1, 2048 );
}

void SetTerrainHeightMap ( int iID, char* szFile )
{
	// set the height map of the terrain
	sTerrain* pTerrain = &g_Terrain [ iID ];
	strcpy ( &pTerrain->szHeightMap, szFile );
}

void SetTerrainLayerCount ( int iID, int iCount )
{
	// store the number of layers
	sTerrain* pTerrain = &g_Terrain [ iID ];
	pTerrain->iLayerCount = iCount;
}

void SetTerrainLayerTexture ( int iID, int iLayer, int iImage )
{
	// set the texture
	sTerrain*		pTerrain = &g_Terrain [ iID ];
	D3DSURFACE_DESC	desc;

	// get pointer to the image
	pTerrain->layers [ iLayer ].lpTexture = dbGetImagePointer ( iImage );

	// save information
	pTerrain->layers [ iLayer ].lpTexture->GetLevelDesc ( 0, &desc );
	pTerrain->layers [ iLayer ].iHeight = desc.Height;
	pTerrain->layers [ iLayer ].iWidth = desc.Width;
}

cArray* ExtractHeightMap ( int iID, D3DXIMAGE_INFO* pInfo )
{
	// our local variables - terrain, height map array, texture and information about the texture
	sTerrain*			pTerrain = &g_Terrain [ iID ];
	LPDIRECT3DTEXTURE9	lpTexture = NULL;
	DWORD*				pPixels;
	DWORD				dwWidth, dwHeight, dwPitch;
	cArray*				pHeightMap = 0;

	// load the height map image
	D3DXCreateTextureFromFileEx ( m_pD3DDevice, &pTerrain->szHeightMap, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, pInfo, NULL, &lpTexture );

	// check image is okay
	if ( !lpTexture )
		return NULL;

	// lock the texture
	if ( !LockTexture ( lpTexture, &pPixels, &dwPitch, &dwWidth, &dwHeight ) )
		return NULL;

	// allocate array to store height map data
	pHeightMap = new cArray ( pInfo->Width, pInfo->Height );

	// check memory was allocated
	if ( !pHeightMap )
		return NULL;
	
	// extract data from the height map, ideally use one function to constantly extract
	// the data rather than replicating code constantly
	for ( int i = 0; i < pInfo->Width; i++ )
	{
		for ( int j = 0; j < pInfo->Height; j++ )
		{
			DWORD pGet    = pPixels [ i + j * dwPitch / 4 ];
			DWORD dwAlpha = pGet >> 24;
			DWORD dwRed   = ( ( ( pGet ) >> 16 ) & 0xff );
			DWORD dwGreen = ( ( ( pGet ) >>  8 ) & 0xff );
			DWORD dwBlue  = ( ( ( pGet ) )       & 0xff );
			
			pHeightMap->m_Array [ i ] [ j ] = ( float ) dwRed;
		}
	}

	// unlock the texture
	UnlockTexture ( lpTexture );

	// return the height map data
	return pHeightMap;
}

void GenerateTerrainTexture ( int iID, int iTextureSize )
{
	// extract height map
	D3DXIMAGE_INFO	Info;
	sTerrain*		pTerrain = &g_Terrain [ iID ];
	cArray*			pHeightMap = ExtractHeightMap ( iID, &Info );

	// now move onto last step and generate the textures
	GenerateFinalTerrainTexture ( pTerrain, pHeightMap, Info.Height, iTextureSize, 10.0f, "final.bmp" );
}

void GenerateFinalTerrainTexture ( sTerrain* pTerrain, cArray* pHeightmap, int iHeightmapWidth, int iTexSize, float fRandMod, char* szSave )
{
	// generate the final texture we need

	// our local variables
	int		iTexIndexCe;
	int		iTexIndexFl;
	int		iIndex;
	int		i;
	int		j;
	float	fLowTexWeight;
	float	fHighTexWeight;
	float	fTexIndex;
	UCHAR*	pTextureData = 0;
	UCHAR	cTexel [ 3 ];
	cArray*	pHeight = 0;
	bool	bReturn;
	sImage	pTextures [ 10 ];

	// go through each layer
	for ( i = 0; i < pTerrain->iLayerCount; i++ )
	{
		// information about the texture
		DWORD*	pPixels	 = NULL;
		DWORD	dwWidth	 = 0;
		DWORD	dwHeight = 0;
		DWORD	dwPitch  = 0;
		int     iOffset  = 0;

		// get the width, height and allocate array to store texture data
		pTextures [ i ].iWidth  = pTerrain->layers [ i ].iWidth;
		pTextures [ i ].iHeight = pTerrain->layers [ i ].iHeight;
		pTextures [ i ].pData   = new unsigned char [ pTerrain->layers [ i ].iWidth * pTerrain->layers [ i ].iHeight * 3 ];

		// lock the texture
		if ( !LockTexture ( pTerrain->layers [ i ].lpTexture, &pPixels , &dwPitch, &dwWidth, &dwHeight ) )
			return;

		// extract data
		for ( int x = 0; x < pTerrain->layers [ i ].iWidth; x++ )
		{
			for ( int y = 0; y < pTerrain->layers [ i ].iHeight; y++ )
			{
				DWORD pGet    = pPixels [ x + y * dwPitch / 4 ];
				DWORD dwRed   = ( ( ( pGet ) >> 16 ) & 0xff );
				DWORD dwGreen = ( ( ( pGet ) >>  8 ) & 0xff );
				DWORD dwBlue  = ( ( ( pGet ) )       & 0xff );

				pTextures [ i ].pData [ iOffset++ ] = dwBlue;
				pTextures [ i ].pData [ iOffset++ ] = dwGreen;
				pTextures [ i ].pData [ iOffset++ ] = dwRed;
			}
		}

		// unlock the texture
		UnlockTexture ( pTerrain->layers [ i ].lpTexture );
	}

	// now we have all the pixel data from each layer we use

	// create a new array that will be the new height map
	pHeight = new cArray ( iTexSize, iTexSize );

	// scale the original height map to the size of the texture we want
	bReturn = ScaleHeightmap ( pHeightmap, iHeightmapWidth, pHeight, iTexSize );

	// allocate an array to hold texture data
	pTextureData = new unsigned char [ iTexSize * iTexSize * 3 ];

	// now build the texture
	for ( i = 0; i < iTexSize; i++ )
	{
		for ( j = 0; j < iTexSize; j++ )
		{
			// apply some random factors to make height map appear more natural, try and experiment with
			// this to see what works best
			pHeight->m_Array [ i ] [ j ] += ( ( float ) rand ( ) / ( float ) RAND_MAX * fRandMod - ( fRandMod / 2.0f ) );

			// limit the height map data
			if ( pHeight->m_Array [ i ] [ j ] > 255 )
				pHeight->m_Array [ i ] [ j ] = 255;

			if ( pHeight->m_Array [ i ] [ j ] < 0 )
				pHeight->m_Array [ i ] [ j ] = 0;

			// calculate the average texture index and get floor and ceiling
			fTexIndex      = pHeight->m_Array [ i ] [ j ] / 255.0f * pTerrain->iLayerCount;
			iTexIndexCe    = ( ( float ) ceil ( fTexIndex ) );
			iTexIndexFl    = floor ( fTexIndex );
			fHighTexWeight = fTexIndex - iTexIndexFl;
			fLowTexWeight  = iTexIndexCe - fTexIndex;

			// adjust texture weighting
			if ( fHighTexWeight == 0.0f && fLowTexWeight == 0.0f )
			{
				fHighTexWeight = 0.5f;
				fLowTexWeight  = 0.5f;
			}

			// change texture index if needed
			if ( iTexIndexCe > pTerrain->iLayerCount - 1 )
				iTexIndexCe = pTerrain->iLayerCount - 1;

			// change texture index if needed
			if ( iTexIndexFl > pTerrain->iLayerCount - 1 )
				iTexIndexFl = pTerrain->iLayerCount - 1;
			
			// calculate the texel offset in the lower texture array
			iIndex = ( ( j % pTextures [ iTexIndexFl ].iWidth ) * pTextures [ iTexIndexFl ].iWidth + ( i % pTextures [ iTexIndexFl ].iHeight ) ) * 3;
			
			// add the lower texture
			cTexel [ 0 ] = BYTE ( ( pTextures [ iTexIndexFl ].pData [ iIndex + 0 ] * fLowTexWeight ) );
			cTexel [ 1 ] = BYTE ( ( pTextures [ iTexIndexFl ].pData [ iIndex + 1 ] * fLowTexWeight ) );
			cTexel [ 2 ] = BYTE ( ( pTextures [ iTexIndexFl ].pData [ iIndex + 2 ] * fLowTexWeight ) );

			// calculate the texel offset in the higher texture array
			iIndex = ( ( j % pTextures [ iTexIndexCe ].iWidth ) * pTextures [ iTexIndexCe ].iWidth + ( i % pTextures [ iTexIndexCe ].iHeight ) ) * 3;

			// add the higher texture
			cTexel [ 0 ] += BYTE ( ( pTextures [ iTexIndexCe ].pData [ iIndex + 0 ] ) * fHighTexWeight );
			cTexel [ 1 ] += BYTE ( ( pTextures [ iTexIndexCe ].pData [ iIndex + 1 ] ) * fHighTexWeight );
			cTexel [ 2 ] += BYTE ( ( pTextures [ iTexIndexCe ].pData [ iIndex + 2 ] ) * fHighTexWeight );

			// copy texel into texture array
			memcpy ( &pTextureData [ ( j * iTexSize + i ) * 3 ], cTexel, 3 );
		}
	}

	// now create a bitmap in which we can save this, maybe best to use D3DX functions for this
	// rather than the bitmap class, uses some old source code to deal with this
	bitmap bmp;

	// create a bitmap of the desire size
	bmp.create ( iTexSize, iTexSize );

	// run through and set color data for each part
	int iOffset = 0;

	for ( int iX = 0; iX < iTexSize; iX++ )
	{
		for ( int iY = 0; iY < iTexSize; iY++ )
		{
			bmp.setcolor ( iX, iY, pTextureData [ iOffset + 2 ], pTextureData [ iOffset + 1 ], pTextureData [ iOffset + 0 ] );
			iOffset += 3;
		}
	}

	// save the bitmap
	bmp.save ( szSave );
	
	// clear up memory
	delete [ ] pTextureData;
	pTextureData = 0;
	delete pHeight;
	pHeight = 0;
}

bool LockTexture ( LPDIRECT3DTEXTURE9 pTexture, DWORD** ppPixels, DWORD* dwPitch, DWORD* dwWidth, DWORD* dwHeight )
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

void UnlockTexture ( LPDIRECT3DTEXTURE9 pTexture )
{
	// unlock texture
	pTexture->UnlockRect ( NULL );
}

bool ScaleHeightmap ( cArray* pSource, int iOldWidth, cArray *pDestination, int iNewWidth )
{
	// scale the height map to match the size of the new texture

	// our local variables
	int			i, j;
	int			iXSrcFl, iXSrcCe, iYSrcFl, iYSrcCe;
	float		fXSrc, fYSrc;
	float		fHeight [ 4 ];
	float		fHeightWeight [ 4 ];
	float		fHeightBottom;
	float		fHeightTop;

	iOldWidth--;

	for ( i = 0; i < iNewWidth; i++ )
	{
		// calculate the average source array position
		fXSrc = i / ( float ) iNewWidth * iOldWidth;

		// precalculate floor and ceiling values
		iXSrcFl = floor        ( fXSrc );
		iXSrcCe = float ( ( float ) ceil ( fXSrc ) );
		
		// distribution between left and right height values
		fHeightWeight [ 0 ] = ( float ) iXSrcCe - fXSrc;
		fHeightWeight [ 1 ] = fXSrc - ( float ) iXSrcFl;

		// avoid error when floor ( ) and ceil ( ) return the same value
		if ( fHeightWeight [ 0 ] == 0.0f && fHeightWeight [ 1 ] == 0.0f )
		{
			fHeightWeight [ 0 ] = 0.5f;
			fHeightWeight [ 1 ] = 0.5f;
		}

		for ( j = 0; j < iNewWidth; j++ )
		{
			// calculate the average source array position
			fYSrc = j / ( float ) iNewWidth * iOldWidth;

			// precalculate floor and ceiling values
			iYSrcFl = floor ( fYSrc );
			iYSrcCe = ( float ) ( ( float ) ceil ( fYSrc ) );
			
			// get the four nearest height values
			fHeight [ 0 ] = pSource->m_Array [ iXSrcFl ] [ iYSrcFl ];
			fHeight [ 1 ] = pSource->m_Array [ iXSrcCe ] [ iYSrcFl ];
			fHeight [ 2 ] = pSource->m_Array [ iXSrcFl ] [ iYSrcCe ];
			fHeight [ 3 ] = pSource->m_Array [ iXSrcCe ] [ iYSrcCe ];

			// calculate how much weight each height value has

			// distribution between top and bottom height values
			fHeightWeight [ 2 ] = ( float ) iYSrcCe - fYSrc;
			fHeightWeight [ 3 ] = fYSrc - ( float ) iYSrcFl;

			if ( fHeightWeight [ 2 ] == 0.0f && fHeightWeight [ 3 ] == 0.0f )
			{
				fHeightWeight [ 2 ] = 0.5f;
				fHeightWeight [ 3 ] = 0.5f;
			}

			// interpolate between the four nearest height values
	
			// get the height for the given x position through interpolation between
			// the left and the right height
			fHeightBottom = ( fHeight [ 0 ] * fHeightWeight [ 0 ] + fHeight [ 1 ] * fHeightWeight [ 1 ] );
			fHeightTop    = ( fHeight [ 2 ] * fHeightWeight [ 0 ] + fHeight [ 3 ] * fHeightWeight [ 1 ] );

			// set the new value in the destination heightmap
			pDestination->m_Array [ i ] [ j ] = fHeightBottom * fHeightWeight [ 2 ] + fHeightTop * fHeightWeight [ 3 ];
		}
	}

	return true;
}

cArray::cArray ( int iDimensionA, int iDimensionB )
{
	m_iDimensionA = iDimensionA;
	m_iDimensionB = iDimensionB;

	m_Array = new float* [ m_iDimensionA ];
	
	for ( int i = 0; i < m_iDimensionA; i++ )
	{
		m_Array [ i ] = new float [ m_iDimensionB ];
		memset ( &m_Array [ i ] [ 0 ], 0, m_iDimensionB * sizeof ( float ) );
	}
}

cArray::~cArray ( )
{
	for ( int i = 0; i < m_iDimensionA; i++ ) 
		delete [ ] m_Array [ i ];
		
	delete [ ] m_Array;
	m_Array = 0;
}

inline float RangedRandom ( float f1, float f2 )
{
	return ( f1 + ( f2 - f1 ) * ( ( float ) rand ( ) ) / ( ( float ) RAND_MAX ) );
}

void NormalizeTerrain ( int iSize, float* fpHeightData )
{
	float fMin, fMax;
	float fHeight;
	int i;

	fMin = fpHeightData [ 0 ];
	fMax = fpHeightData [ 0 ];

	for ( i = 1; i < iSize * iSize; i++ )
	{
		if ( fpHeightData [ i ] > fMax )
		{
			fMax = fpHeightData [ i ];
		}
		else if ( fpHeightData [ i ] < fMin )
		{
			fMin = fpHeightData [ i ];
		}
	}

	if ( fMax <= fMin )
		return;

	fHeight = fMax - fMin;

	for ( i = 0; i < iSize * iSize; i++ )
		fpHeightData [ i ]= ( ( fpHeightData [ i ] - fMin ) / fHeight ) * 255.0f;
}

inline void SetHeightAtPoint ( unsigned char* ucpData, int iSize, unsigned char ucHeight, int x, int z )
{
	ucpData [ ( z * iSize ) + x ] = ucHeight;
}

inline unsigned char GetTrueHeightAtPoint ( unsigned char* ucpData, int iSize, int x, int z )
{
	return ( ucpData [ ( z * iSize ) + x ] );
}

void GenerateTerrainPlasmaHeightMap ( char* szFilename, int iSize, float fRoughness )
{
	sHeightData m_heightData;
	int			m_iSize;
	float*		fTempBuffer;
	float		fHeight;
	float		fHeightReducer;
	int			iRectSize = iSize;
	int			ni, nj;
	int			mi, mj;
	int			pmi, pmj;
	int			i, j;
	int			x, z;

	if ( fRoughness < 0 )
		fRoughness *= -1;

	fHeight		   = ( float ) iRectSize / 2;
	fHeightReducer = ( float ) pow ( ( float ) 2, ( float ) -1 * fRoughness );
	m_iSize        = iSize;

	m_heightData.m_ucpData = new unsigned char [ m_iSize * m_iSize ];
	fTempBuffer = new float [ m_iSize * m_iSize ];

	if ( !m_heightData.m_ucpData )
	{
		return;
	}

	if ( !fTempBuffer )
	{
		return;
	}

	fTempBuffer [ 0 ] = 0.0f;

	while ( iRectSize > 0 )
	{
		for ( i = 0; i < m_iSize; i += iRectSize )
		{
			for ( j = 0; j < m_iSize; j += iRectSize )
			{
				ni = ( i + iRectSize ) % m_iSize;
				nj = ( j + iRectSize ) % m_iSize;
				mi = ( i + iRectSize / 2 );
				mj = ( j + iRectSize / 2 );

				fTempBuffer [ mi + mj * m_iSize ] = 
					( float ) ( ( fTempBuffer [ i + j * m_iSize ] + fTempBuffer [ ni + j * m_iSize ] + fTempBuffer [ i + nj * m_iSize ]
								+ fTempBuffer [ ni + nj * m_iSize ] ) / 4 + RangedRandom ( -fHeight / 2, fHeight / 2 ) );
			}
		}

		for ( i = 0; i < m_iSize; i += iRectSize )
		{
			for ( j = 0; j < m_iSize; j += iRectSize )
			{
				ni  = ( i + iRectSize ) % m_iSize;
				nj  = ( j + iRectSize ) % m_iSize;
				mi  = ( i + iRectSize / 2 );
				mj  = ( j + iRectSize / 2 );
				pmi = ( i - iRectSize / 2 + m_iSize ) % m_iSize;
				pmj = ( j - iRectSize / 2 + m_iSize ) % m_iSize;
				
				fTempBuffer [ mi + j * m_iSize ] = ( float )( ( fTempBuffer [ i  + j   * m_iSize ]	  +
																fTempBuffer [ ni + j   * m_iSize ]	  +
																fTempBuffer [ mi + pmj * m_iSize ]	  +
																fTempBuffer [ mi + mj  * m_iSize ] ) / 4 +
																RangedRandom ( -fHeight / 2, fHeight / 2 ) );

				fTempBuffer [ i + mj * m_iSize ] = ( float )( ( fTempBuffer [ i   + j  * m_iSize ]	  +
																fTempBuffer [ i   + nj * m_iSize ]	  +
																fTempBuffer [ pmi + mj * m_iSize ]	  +
																fTempBuffer [ mi  + mj * m_iSize ] ) / 4 + 
																RangedRandom ( -fHeight / 2, fHeight / 2 ) );
			}
		}

		iRectSize /= 2;
		fHeight*= fHeightReducer;
	}

	NormalizeTerrain ( iSize, fTempBuffer );

	for ( z = 0; z < m_iSize; z++ )
	{
		for ( x = 0; x < m_iSize; x++ )
			SetHeightAtPoint ( m_heightData.m_ucpData, iSize, ( unsigned char ) fTempBuffer [ ( z * m_iSize ) + x ], x, z );			
	}

	if ( fTempBuffer )
	{
		delete [ ] fTempBuffer;
	}

	bitmap bmp;
	bmp.create ( m_iSize, m_iSize );

	for ( x = 0; x < m_iSize; x++ )
	{
		for ( int y = 0; y < m_iSize; y++ )
		{
			UCHAR uc = GetTrueHeightAtPoint ( m_heightData.m_ucpData, m_iSize, x, y );

			bmp.setcolor ( x, y, uc, uc, uc );	
		}
	}

	bmp.save ( szFilename );
}
