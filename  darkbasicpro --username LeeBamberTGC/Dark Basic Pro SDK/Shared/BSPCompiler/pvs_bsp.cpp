#include "pvs_bsp.h"

long				PVSCompressedSize = 0;						// Size of compressed PVS array
char				(*TextureLUT)[21];							// Texture Name LookUpTable

D3DXVECTOR3			vecLook		(0.0f, 0.0f, 1.0f);				// Camera Look Vector
D3DXVECTOR3			vecUp		(0.0f, 1.0f, 0.0f);				// Camera Up Vector
D3DXVECTOR3			vecRight	(1.0f, 0.0f, 0.0f);				// Camera Right Vector
//D3DXVECTOR3			vecPos		(0.0f, 10.9f, -60.0f);				// Camera Position
D3DXVECTOR3			vecPos		(0.0f, 10.9f, 60.0f);				// Camera Position
//D3DXVECTOR3			vecPos		(-1018.0f, 80.0f, 958.7f);				// Camera Position

POLYGON				*PolygonList;								// Top of Polygon Linked List
BOOL				DontFrustumReject = TRUE;					// Disable Frustum Reject
PLANE2				FrustumPlanes[6];							// Frustum Clip Planes

LPDIRECT3DDEVICE9   m_pD3D			= NULL;					// Direct 3D Device

LPDIRECT3DTEXTURE9	*lpTextureSurface	= NULL;					// Direct 3D Texture Array
WORD				NumberOfTextures	= 0;

int g_iProgress = 0;

float			g_EPSILON = 0.1f;