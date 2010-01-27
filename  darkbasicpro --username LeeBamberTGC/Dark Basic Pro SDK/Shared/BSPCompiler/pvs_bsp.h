#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#define D3DFVF_LVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

const int			CP_FRONT	= 1001;
const int			CP_BACK		= 1002;
const int			CP_ONPLANE	= 1003;
const int			CP_SPANNING	= 1004;

//const FLOAT			g_EPSILON	= 0.001f;
//const FLOAT			g_EPSILON	= 0.1f;
extern float			g_EPSILON;

struct D3DLVERTEX
{
    float		x; 
    float		y;    
    float		z;  
	D3DCOLOR	color;
	float		tu;
	float		tv;
};

struct PLANE
{
	D3DXVECTOR3 PointOnPlane;
	D3DXVECTOR3 Normal;
};


struct PLANE2
{
 	float		Distance;
	D3DXVECTOR3	Normal;
};

struct CLIPPLANES
{
	WORD		NumberOfPlanes;
	PLANE*		Planes;
};

struct POLYGON
{
	D3DLVERTEX*	VertexList;
	D3DXVECTOR3 Normal;
	WORD		NumberOfVertices;
	WORD		NumberOfIndices;
	WORD*		Indices;
	POLYGON*	Next;
	BOOL		BeenUsedAsSplitter;
	WORD		TextureIndex;
};

struct PORTAL
{
	D3DLVERTEX*	VertexList;
	D3DXVECTOR3 Normal;
	WORD		NumberOfVertices;
	WORD		NumberOfIndices;
	WORD*		Indices;
	PORTAL*		Next;
	PORTAL*		Prev;
	BYTE		NumberOfLeafs;
	long		LeafOwnerArray [ 2 ];
};

struct BOUNDINGBOX
{
	D3DXVECTOR3 BoxMin;
	D3DXVECTOR3 BoxMax;
};

									
struct LEAF
{
	long		StartPolygon;
	long		EndPolygon;
	long		PortalIndexList [ 500 ];
	long		NumberOfPortals;
	long		PVSIndex;
	BOUNDINGBOX BoundingBox;
};

struct NODE
{
	BYTE		IsLeaf;
	ULONG		Plane;
	ULONG		Front;
	LONG		Back;
	BOUNDINGBOX BoundingBox;
};

typedef struct
{
	BOOL			DontDelete;
	WORD			NumberOfVertices;
	D3DXVECTOR3*	VertexList;		
} PORTALPOINTS;


struct PVSPORTAL
{
	PORTALPOINTS*	Points;
	PLANE			Plane;
	long			NeighbourLeaf;
	BYTE*			PossibleVisBits;
	BYTE*			ActualVisBits;
	int				NumPossible;
	BYTE			status;
};

typedef struct
{
	PORTALPOINTS*	SourcePoints, *TargetPoints;
	PLANE			TargetPlane;
	BYTE*			VisBits;
} PVSRECURSEDATA;

void PrintPVSStatus ( long leaf );


extern LPD3DXFONT	MainFont;
extern PLANE2		FrustumPlanes [ 6 ];
extern BOOL			DontFrustumReject;

extern D3DXVECTOR3 vecPos;
extern D3DXVECTOR3 vecLook;
extern D3DXVECTOR3 vecUp;
extern D3DXVECTOR3 vecRight;

extern long BytesPerSet;

extern POLYGON*		PolygonArray;
extern NODE*		NodeArray;
extern LEAF*		LeafArray;
extern PLANE*		PlaneArray;
extern PORTAL**		PortalArray;
extern PVSPORTAL*	PortalDupArray;
extern BYTE*		PVSData;


extern long NumberOfPolygons;
extern long NumberOfNodes;
extern long NumberOfLeafs;
extern long NumberOfPlanes;
extern long NumberOfPortals;

extern LPDIRECT3DDEVICE9       m_pD3D;

extern POLYGON* PolygonList;

extern LPDIRECT3DTEXTURE9*			lpTextureSurface;
extern WORD							NumberOfTextures;
extern POLYGON**					pTexturePolygonList;
extern long							PVSCompressedSize;

extern char				( *TextureLUT ) [ 21 ];

bool     Get_Intersect      ( D3DXVECTOR3* linestart, D3DXVECTOR3* lineend, D3DXVECTOR3* vertex, D3DXVECTOR3* normal, D3DXVECTOR3* intersection, float* percentage );
void	 SplitPolygon       ( POLYGON* Poly, PLANE* Plane, POLYGON* FrontSplit, POLYGON* BackSplit );
BOOL	 CollideSphere      ( D3DXVECTOR3* SphereCenter, float SphereRadius, long Node );
BOOL	 LineOfSight        ( D3DXVECTOR3* Start, D3DXVECTOR3* End, long Node );
void	 CalculateBox       ( BOUNDINGBOX* Box, POLYGON* Polylist );
int		 ClassifyPoint      ( D3DXVECTOR3* pos, PLANE* Plane );
POLYGON* AddPolygon         ( POLYGON* Parent, POLYGON* Child );
void	 BuildBspTree       ( long Node, POLYGON* PolyList );
int		 ClassifyPoly       ( PLANE* plane, POLYGON* Poly );
long	 SelectBestSplitter ( POLYGON* PolyList );
void	 RenderTree         ( D3DXVECTOR3 pos );
void	 DrawTree           ( long leaf );

void ReserveInitialMemoryForArrays ( void );
void IncreaseNumberOfPolygons	   ( void );
void IncreaseNumberOfPlanes		   ( void );
void IncreaseNumberOfNodes		   ( void );
void IncreaseNumberOfLeafs		   ( void );
void IncreaseNumberOfPortals	   ( void );
void DeletePolygonArray			   ( void );
void DeletePortalArray			   ( void );
void DeletePolygon				   ( POLYGON* Poly );
BOOL DeletePortal				   ( PORTAL* Portal );

extern long MAXNUMBEROFNODES;
extern long MAXNUMBEROFPLANES;
extern long MAXNUMBEROFPOLYGONS;
extern long MAXNUMBEROFPORTALS;
extern long MAXNUMBEROFLEAFS;

extern int g_iProgress;
							   
void ExtractFrustumPlanes ( PLANE2* Planes );
BOOL LeafInFrustum		  ( long Leaf );

PORTAL* ClipPortal ( long Node, PORTAL* Portal );
void	RemovePortalFromList ( PORTAL* RemovePortal );
BOOL	CheckDuplicatePortal ( PORTAL* CheckPortal, int* index );
void	SplitPortal ( PORTAL* Portal, PLANE* Plane, PORTAL* FrontSplit, PORTAL* BackSplit );
PLANE	GetPortalPlane  ( PORTAL* Portal );
void	GetPortalBounds ( PORTAL* Portal, D3DXVECTOR3* Min, D3DXVECTOR3* Max );
void	DrawPortals ( void );
void	BuildPortals ( void );

PORTALPOINTS* ClipToAntiPenumbra ( PORTALPOINTS* Source, PORTALPOINTS* Target, PORTALPOINTS* Generator, BOOL ReverseClip );
PORTALPOINTS* ClipPortalPoints ( PORTALPOINTS* Points, PLANE* Plane, BOOL KeepOnPlane );
PORTALPOINTS* CopyPortalPoints ( PORTALPOINTS* Points );
PORTALPOINTS* NewPortalPoints( int NumberOfPoints );

void			RecursePVS  ( int LeafNum, PVSPORTAL* SourcePortal, PVSRECURSEDATA* prevdata );
long			CompressLeafSet ( BYTE* VisArray, long WritePos );
void			PortalFlood ( PVSPORTAL* SourcePortal, int Leaf );
void			FreePortalPoints ( PORTALPOINTS* Points );
void			CalculateDuplicatePortals ( void );
void			FreeDuplicatePortals ( void );
void			InitialPortalVis ( void );
void			CalculatePVS ( void );
void			CalcPortalVis ( void );
int				GetNextPortal ( void );
void			LeafPVSWrite ( void );

void			SetPVSBit ( BYTE* VisArray, long DestLeaf );
BOOL			GetPVSBit ( BYTE* VisArray, long DestLeaf );

#define PS_NOTPROCESSED			0
#define PS_PROCESSING			1
#define PS_PROCESSED			2