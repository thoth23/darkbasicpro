
#ifndef _CTERRAIN_H_
#define _CTERRAIN_H_

#include "..\..\..\..\Dark Basic Pro SDK\Shared\Core\globstruct.h"
#include <vector>

class cTerrainBlock;

class cTerrain
{
	public:
		D3DXVECTOR3         m_vecScale;         // scale to be applied to the terrain
		float*				m_pHeightMap;       // actual height map data
		int		            m_iHeightMapWidth;  // height map width
		int	                m_iHeightMapHeight; // height map height
		int					m_iBlockWidth;      // block width
		int					m_iBlockHeight;     // block height
		cTerrainBlock**		m_pBlock;           // array of terrain blocks
		int					m_iBlockCount;      // number of terrain blocks
		int					m_iQuadsWide;       // number of quads wide per block
		int					m_iQuadsHigh;       // number of quads height per block
		int					m_iBlocksWide;      // number of wide blocks
		int					m_iBlocksHigh;      // number of high blocks
		LPDIRECT3DDEVICE9   m_pD3DDevice;       // direct 3D device
		int					m_iTile;
		int					m_iSplit;
		float				m_fLightXDirection;
		float				m_fLightYDirection;
		float				m_fLightZDirection;
		float				m_fLightRed;
		float				m_fLightGreen;
		float				m_fLightBlue;
		float				m_fScale;

		bool				LockTexture			  ( LPDIRECT3DTEXTURE9 pTexture, DWORD** ppPixels, DWORD* dwPitch, int* dwWidth, int* dwHeight );
		void				UnlockTexture         ( LPDIRECT3DTEXTURE9 pTexture );
		int					AddTerrainBlock       ( int iCount );
		int					AddTerrainLayer       ( int iCount );
		bool				GenerateTerrainBlocks ( void );
		void				FilterHeightMap       ( void );
		bool				BuildHeightMapData	  ( char* szFile );

	public:
		cTerrain  ( );
		~cTerrain ( );

		void                SetD3DDevice          ( LPDIRECT3DDEVICE9 pD3DDevice );
		bool                LoadTerrain           ( 
													char* szFile, float fXScale, float fYScale, float fZScale, int iSplit, int iTile, float fLightXDirection,
													float fLightYDirection, float fLightZDirection, float fLightRed, float fLightGreen, float fLightBlue, float fScale, int iExcludeSmoothing
												  );
		void                Destroy				  ( void );

		float               GetHeight             ( float x, float z, bool ReverseQuad = false );
		float*				GetHeightMap          ( void );
		D3DXVECTOR3         GetHeightMapNormal    ( int x, int z );
		int                 GetTerrainWidth       ( void );
		int                 GetTerrainHeight      ( void );
		D3DXVECTOR3&		GetScale              ( void );
		LPDIRECT3DDEVICE9   GetD3DDevice          ( void );
};

class cTerrainBlock
{
	// a section of the terrain from the height map

	public:

		int                   m_iStartX;				// x start position from height map
		int                   m_iStartZ;				// z start position from height map
		int                   m_iBlockWidth;			// width of block
		int                   m_iBlockHeight;			// height of block
		int                   m_iQuadsWide;				// number of wide quads in block
		int                   m_iQuadsHigh;				// number of high quads in block
		cTerrain*			  m_pParent;				// parent terrain
		
		struct sVertex
		{
			float x, y, z;
			DWORD dwDiffuse;
			float tu, tv;
			float tu1, tv1;
		};

		std::vector < sVertex > m_VertexList;
		std::vector < WORD    > m_IndexList;

		float GetX ( int iID );
		float GetY ( int iID );
		float GetZ ( int iID );
		DWORD GetDiffuse ( int iID );
		float GetTU ( int iID );
		float GetTV ( int iID );
		float GetTU1 ( int iID );
		float GetTV1 ( int iID );

		//LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;		// vertex buffer
		D3DXVECTOR3             m_BoundsMin;			// minimum bounds
		D3DXVECTOR3             m_BoundsMax;			// maximum bounds

				 cTerrainBlock      ( );
		virtual ~cTerrainBlock      ( );

		bool    GenerateBlock       ( cTerrain* pParent, int StartX, int StartZ, int BlockWidth, int BlockHeight);
		bool    GenerateVertices  ( void );
		bool    GenerateIndices  ( void );
		
		//LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;     // index buffer for splat
		int                   m_iIndexCount;      // number of indices for rendering
		int                   m_iPrimitiveCount;  // number of primitives
		int                  m_iLayerIndex;      // layer index
		//LPDIRECT3DTEXTURE9      m_pBlendTexture;    // the blend texture
};

#endif