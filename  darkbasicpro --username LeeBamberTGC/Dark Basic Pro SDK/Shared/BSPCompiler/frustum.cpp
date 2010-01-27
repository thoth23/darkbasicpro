
#include "pvs_bsp.h"

void ExtractFrustumPlanes ( PLANE2* Planes )
{
	D3DXMATRIX	ViewMatrix,
				ProjMatrix,
				ViewProj;

	m_pD3D->GetTransform ( D3DTS_PROJECTION, &ProjMatrix );

	D3DXMatrixIdentity ( &ViewMatrix );

	ViewMatrix._11 = vecRight.x; ViewMatrix._12 = vecUp.x; ViewMatrix._13 = vecLook.x;
	ViewMatrix._21 = vecRight.y; ViewMatrix._22 = vecUp.y; ViewMatrix._23 = vecLook.y;
	ViewMatrix._31 = vecRight.z; ViewMatrix._32 = vecUp.z; ViewMatrix._33 = vecLook.z;
	ViewMatrix._41 =- D3DXVec3Dot (	&vecPos, &vecRight );
	ViewMatrix._42 =- D3DXVec3Dot (	&vecPos, &vecUp    );
	ViewMatrix._43 =- D3DXVec3Dot (	&vecPos, &vecLook  );

	D3DXMatrixMultiply ( &ViewProj, &ViewMatrix, &ProjMatrix );

	Planes [ 0 ].Normal.x = -( ViewProj._14 + ViewProj._11 );
	Planes [ 0 ].Normal.y = -( ViewProj._24 + ViewProj._21 );
	Planes [ 0 ].Normal.z = -( ViewProj._34 + ViewProj._31 );
	Planes [ 0 ].Distance = -( ViewProj._44 + ViewProj._41 );

	Planes [ 1 ].Normal.x = -( ViewProj._14 - ViewProj._11 );
	Planes [ 1 ].Normal.y = -( ViewProj._24 - ViewProj._21 );
	Planes [ 1 ].Normal.z = -( ViewProj._34 - ViewProj._31 );
	Planes [ 1 ].Distance = -( ViewProj._44 - ViewProj._41 );

	Planes [ 2 ].Normal.x = -( ViewProj._14 - ViewProj._12 );
	Planes [ 2 ].Normal.y = -( ViewProj._24 - ViewProj._22 );
	Planes [ 2 ].Normal.z = -( ViewProj._34 - ViewProj._32 );
	Planes [ 2 ].Distance = -( ViewProj._44 - ViewProj._42 );

	Planes [ 3 ].Normal.x = -( ViewProj._14 + ViewProj._12 );
	Planes [ 3 ].Normal.y = -( ViewProj._24 + ViewProj._22 );
	Planes [ 3 ].Normal.z = -( ViewProj._34 + ViewProj._32 );
	Planes [ 3 ].Distance = -( ViewProj._44 + ViewProj._42 );

	Planes [ 4 ].Normal.x = -ViewProj._13;
	Planes [ 4 ].Normal.y = -ViewProj._23;
	Planes [ 4 ].Normal.z = -ViewProj._33;
	Planes [ 4 ].Distance = -ViewProj._43;

	Planes [ 5 ].Normal.x = -( ViewProj._14 - ViewProj._13 );
	Planes [ 5 ].Normal.y = -( ViewProj._24 - ViewProj._23 );
	Planes [ 5 ].Normal.z = -( ViewProj._34 - ViewProj._33 );
	Planes [ 5 ].Distance = -( ViewProj._44 - ViewProj._43 );

	for ( int i = 0; i < 6; i++ )
	{
		float denom = 1.0f / D3DXVec3Length ( &Planes [ i ].Normal );

		Planes [ i ].Normal.x *= denom;
		Planes [ i ].Normal.y *= denom;
		Planes [ i ].Normal.z *= denom;
		Planes [ i ].Distance *= denom;
	}
}

BOOL LeafInFrustum ( long Leaf )
{
	// tests if the bounding box surrounding the leaf is completely outside 
	// the frustum

	// this check is only done on visible leafs in a leafs PVS Set to 
	// further carve off more polygons than just the straight PVS set
	// remember a PVS set does not take into account direction of camera so
	// some of the PVS set will not be visible

	D3DXVECTOR3 bMax = LeafArray [ Leaf ].BoundingBox.BoxMax;
	D3DXVECTOR3 bMin = LeafArray [ Leaf ].BoundingBox.BoxMin;

	D3DXVECTOR3 NearPoint,
				FarPoint;

	PLANE2* Plane = FrustumPlanes;
	
	for ( int i = 0; i < 6; i++ )
	{
		if ( Plane->Normal.x > 0.0f )
		{
			if ( Plane->Normal.y > 0.0f )
			{
				if ( Plane->Normal.z > 0.0f )
				{
					NearPoint.x = bMin.x;
					NearPoint.y = bMin.y;
					NearPoint.z = bMin.z;
				}
				else
				{
					NearPoint.x = bMin.x;
					NearPoint.y = bMin.y;
					NearPoint.z = bMax.z;
				}
			}
			else 
			{
				if ( Plane->Normal.z > 0.0f )
				{
					NearPoint.x = bMin.x;
					NearPoint.y = bMax.y;
					NearPoint.z = bMin.z;
				}
				else 
				{
					NearPoint.x = bMin.x;
					NearPoint.y = bMax.y;
					NearPoint.z = bMax.z;
				}
			}
		} 
		else 
		{
			if ( Plane->Normal.y > 0.0f )
			{
				if ( Plane->Normal.z > 0.0f )
				{
					NearPoint.x = bMax.x;
					NearPoint.y = bMin.y;
					NearPoint.z = bMin.z;
				}
				else 
				{
					NearPoint.x = bMax.x;
					NearPoint.y = bMin.y;
					NearPoint.z = bMax.z;
				}
			}
			else 
			{
				if ( Plane->Normal.z > 0.0f )
				{
					NearPoint.x = bMax.x;
					NearPoint.y = bMax.y;
					NearPoint.z = bMin.z;
				}
				else 
				{
					NearPoint.x = bMax.x;
					NearPoint.y = bMax.y;
					NearPoint.z = bMax.z;
				}
			}
		}

		// near extreme point is outside, and thus the AABB is totally outside the polyhedron
		if ( D3DXVec3Dot ( &Plane->Normal, &NearPoint ) + Plane->Distance > 0 )
			return FALSE;

		Plane++;
	}
	
	return TRUE;
}