
#include "pvs_bsp.h"

PORTAL* CalculateInitialPortal ( long Node )
{
	// this function builds a portal on the node plane large enough to
	// encompass all the leaves in its front and back trees, this portal will
	// then get sent down the tree to get carved into possible portals
	
	D3DXVECTOR3		MaxP,
					MinP,
					CB,
					CP,
					U,
					V;

	D3DXVECTOR3 PlaneNormal = PlaneArray [ NodeArray [ Node ].Plane ].Normal;

	// get this nodes bounding box ranges
	MaxP = NodeArray [ Node ].BoundingBox.BoxMax;
	MinP = NodeArray [ Node ].BoundingBox.BoxMin;
	
	// retrieve the world space centre point of the bounding box
	CB = ( MaxP + MinP ) / 2;

	// calculate the distance to the plane from the centre point origin
	float DistanceToPlane = D3DXVec3Dot ( &( PlaneArray [ NodeArray [ Node ].Plane ].PointOnPlane - CB ), &PlaneNormal );

	// using this information we can calculate the centre point for the plane
	CP = CB + ( PlaneNormal * DistanceToPlane );

	// calculate major axis
	D3DXVECTOR3 A = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );

	if ( fabs ( PlaneNormal.y ) > fabs ( PlaneNormal.z ) )
	{
		if ( fabs ( PlaneNormal.z ) < fabs ( PlaneNormal.x ) )
		{
			A.z = 1;
		}
		else 
		{
			A.x = 1;
		}
	}
	else 
	{
		if ( fabs ( PlaneNormal.y ) <= fabs ( PlaneNormal.x ) )
		{
			A.y = 1;
		}
		else
		{
			A.x = 1;
		}
	}

	// generate U / V vectors
	D3DXVec3Cross ( &U, &A, &PlaneNormal );
	D3DXVec3Normalize ( &U, &U );
	D3DXVec3Cross ( &V, &U, &PlaneNormal );
	D3DXVec3Normalize ( &V, &V );

	// finally create the 4 portal vertices
	D3DXVECTOR3 BoxHalfLength = ( MaxP - CB );
	float Length = D3DXVec3Length ( &BoxHalfLength );
	
	U = U * Length;
	V = V * Length;

	D3DXVECTOR3 P [ 4 ];
	P [ 0 ] = CP + U - V; // bottom right
	P [ 1 ] = CP + U + V; // top right
	P [ 2 ] = CP - U + V; // top left
	P [ 3 ] = CP - U - V; // bottom left
	
	// generate the final portal from this info
	PORTAL* Portal = new PORTAL;
	ZeroMemory ( Portal, sizeof ( PORTAL ) );

	Portal->Normal			 = PlaneNormal;
	Portal->NumberOfVertices = 4;
	Portal->NumberOfIndices	 = 6;
	Portal->VertexList		 = new D3DLVERTEX [ Portal->NumberOfVertices ];
	Portal->Indices			 = new WORD       [ Portal->NumberOfIndices  ];

	// copy vertices
	for ( int i = 0; i < 4; i++ )
	{
		Portal->VertexList [ i ].x		= P [ i ].x;
		Portal->VertexList [ i ].y		= P [ i ].y;
		Portal->VertexList [ i ].z		= P [ i ].z;
		Portal->VertexList [ i ].color	= D3DCOLOR_RGBA ( 0, 0, 255, 255 );
		Portal->VertexList [ i ].tu	    = 0;
		Portal->VertexList [ i ].tv	    = 0;
	}

	// fill out indices
	Portal->Indices [ 0 ] = 0;
	Portal->Indices [ 1 ] = 1;
	Portal->Indices [ 2 ] = 3;
	Portal->Indices [ 3 ] = 3;
	Portal->Indices [ 4 ] = 1;
	Portal->Indices [ 5 ] = 2;
	Portal->Next		  = NULL;
	Portal->Prev		  = NULL;
	Portal->NumberOfLeafs = 0;
	
	return Portal;
}

void BuildPortals ( void )
{
	// creates and manages all the portals, it creates the 
	// intial portal and sends it through the tree, the returned fragments 
	// are checked to see whether they are valid portals, if they are valid
	// portals they are added to the master portal array

	int portalindex;

	for ( int i = 0; i < NumberOfNodes; i++ )
	{
		PORTAL* InitialPortal =	CalculateInitialPortal ( i );
		PORTAL* PortalList	  = ClipPortal ( 0, InitialPortal );
		PORTAL* Iterator	  = PortalList; 

		while ( Iterator != NULL )
		{
			if ( Iterator->NumberOfLeafs != 2 )
			{
				PORTAL* temp = Iterator->Next;
				RemovePortalFromList ( Iterator );
				Iterator = temp;
			}
			else 
			{
				if ( CheckDuplicatePortal ( Iterator, &portalindex ) )
				{
					PORTAL* temp = Iterator->Next;
					RemovePortalFromList ( Iterator );
					Iterator = temp;
				}
				else 
				{ 
					PortalArray [ portalindex ] = Iterator;

					if ( portalindex == NumberOfPortals )
					{
						for ( int a = 0; a < Iterator->NumberOfLeafs; a++ )
						{
							long Index = Iterator->LeafOwnerArray [ a ];

							LeafArray [ Index ].PortalIndexList [ LeafArray [ Index ].NumberOfPortals ] = NumberOfPortals;
							LeafArray [ Index ].NumberOfPortals++;
						}

						IncreaseNumberOfPortals ( );
					}
					
					Iterator = Iterator->Next;
				}
			}
		}
	}
}

PORTAL* ClipPortal ( long Node, PORTAL *Portal )
{
	// this recursive function repeatedly clips the current portal to the 
	// tree until it ends up in a leaf at which point it is returned

	PORTAL* PortalList     = NULL, *FrontPortalList = NULL;
	PORTAL* BackPortalList = NULL, *Iterator        = NULL;
	PORTAL* FrontSplit     = NULL, *BackSplit       = NULL;

	switch ( ClassifyPoly ( &PlaneArray [ NodeArray [ Node ].Plane ], ( POLYGON* ) Portal ) )
	{
		case CP_ONPLANE:
		{
			// the portal has to be sent down both sides of the tree and tracked, send
			// it down front first but do no delete any bits that end up in solid space,
			// just ignore them
			if ( NodeArray [ Node ].IsLeaf != 0 )
			{
				 // this is a leaf node ( the front is a leaf )
				Portal->LeafOwnerArray [ Portal->NumberOfLeafs ] = NodeArray [ Node ].Front;
				Portal->NumberOfLeafs++;
				Portal->Next	= NULL;
				Portal->Prev	= NULL;
				FrontPortalList	= Portal;  
			}
			else
			{
				// send the portal down the front list and get returned a 
				// list of PortalFragments that survived the front tree
				FrontPortalList = ClipPortal ( NodeArray [ Node ].Front, Portal );
			}

			// then send each fragment down the back tree.
			if ( FrontPortalList         == NULL ) return NULL;;
			if ( NodeArray [ Node ].Back == -1   ) return FrontPortalList;

			// loop through each front list fragment and send it down the back tree
			while ( FrontPortalList != NULL )
			{
				PORTAL* tempnext = FrontPortalList->Next;
				BackPortalList	 = NULL;
				BackPortalList	 = ClipPortal ( NodeArray [ Node ].Back, FrontPortalList );

				// now set to the end of the returned back fragment list to get the last fragment
				if ( BackPortalList != NULL )
				{
					Iterator = BackPortalList;

					while ( Iterator->Next != NULL )
					{
						Iterator = Iterator->Next;
					}

					// attach the last fragment to the first fragment from a previous loop
					Iterator->Next  = PortalList;
					
					if ( PortalList != NULL )
					{
						PortalList->Prev = Iterator;
					}

					// portal list now points at the current complete list of fragment collected from each loop
					PortalList = BackPortalList;	 
				}

				FrontPortalList = tempnext;
			}

			return PortalList;
		}
		break;
	
		case CP_FRONT:
		{
			// either send it down the front tree or add it to the portal 
			// list because it has come out in empty space
			if ( NodeArray [ Node ].IsLeaf == 0 )
			{
				PortalList = ClipPortal ( NodeArray [ Node ].Front, Portal );
				
				return PortalList;
			}
			else
			{
				// the front node is an empty leaf so add the fragment to the portal list
				Portal->LeafOwnerArray [ Portal->NumberOfLeafs ] = NodeArray [ Node ].Front;
				Portal->NumberOfLeafs++;
				Portal->Next = NULL;
				Portal->Prev = NULL;
			
				return Portal;
			}
		}
		break;

		case CP_BACK:
		{
			// either send it down the back tree or delete it if no back tree exists
			// because it means it has come out in solid space
			if ( NodeArray [ Node ].Back != -1 )
			{
				PortalList = ClipPortal ( NodeArray [ Node ].Back, Portal );

				return PortalList;
			} 
			else
			{
				DeletePortal ( Portal );
				Portal = NULL;
				return NULL; 
			}
		}
		break;

		case CP_SPANNING:
		{
			// portal fragment is spanning the plane, so it must be split
			FrontSplit = new PORTAL;
			BackSplit  = new PORTAL;
			
			ZeroMemory ( FrontSplit, sizeof ( PORTAL ) );
			ZeroMemory ( BackSplit,  sizeof ( PORTAL ) );
	
			// split the portal fragment into two
   			SplitPortal ( Portal, &PlaneArray [ NodeArray [ Node ].Plane ], FrontSplit, BackSplit );

			// delete the original portal fragment
			DeletePortal ( Portal );
			Portal = NULL;
	
			// there is another front node
			if ( NodeArray [ Node ].IsLeaf == 0 )
			{
				FrontPortalList = ClipPortal ( NodeArray [ Node ].Front, FrontSplit );
			}
			else
			{
				// it's about to get pushed into a leaf
				FrontSplit->LeafOwnerArray [ FrontSplit->NumberOfLeafs ] = NodeArray [ Node ].Front;
				FrontSplit->NumberOfLeafs++;
				FrontSplit->Prev = NULL;
				FrontSplit->Next = NULL;
				FrontPortalList  = FrontSplit;
			}
	
			// the backsplit is in solid space
			if ( NodeArray [ Node ].Back != -1 )
			{
				BackPortalList = ClipPortal ( NodeArray [ Node ].Back, BackSplit );
			}
			else 
			{
				// we ended up in solid space
				DeletePortal ( BackSplit );
				BackSplit = NULL;
			}
    
			// find the end of the list and attach it to front back list
			if ( FrontPortalList != NULL )
			{
				// there is something in the front list
				Iterator = FrontPortalList;

				while ( Iterator->Next != NULL )
				{
					Iterator = Iterator->Next;
				}
				
				if ( BackPortalList != NULL )
				{
					Iterator->Next		 = BackPortalList;
					BackPortalList->Prev = Iterator;
				}

				return FrontPortalList;
			}
			else
			{
				// there is nothing in the front list
				if ( BackPortalList != NULL )
					return BackPortalList;

				return NULL;
			}
			
			// if we got here, we are fresh out of portal fragments
			// so simply return NULL
			return NULL;
		}
		break;

		default:
			return NULL;
		break;
	}

	return NULL;
}

void SplitPortal ( PORTAL* Portal, PLANE* Plane, PORTAL* FrontSplit, PORTAL* BackSplit )
{
	// this function simply calls SplitPolygon, and is a convenient wrapper
	// for the splitting of the POLYGON safe type castable portal type

	SplitPolygon ( ( POLYGON* ) Portal, Plane, ( POLYGON* ) FrontSplit, ( POLYGON* ) BackSplit );

	FrontSplit->NumberOfLeafs = Portal->NumberOfLeafs;
	BackSplit->NumberOfLeafs  = Portal->NumberOfLeafs;

	memcpy ( FrontSplit->LeafOwnerArray, Portal->LeafOwnerArray, sizeof ( long ) * Portal->NumberOfLeafs );
	memcpy ( BackSplit->LeafOwnerArray,  Portal->LeafOwnerArray, sizeof ( long ) * Portal->NumberOfLeafs );
}

void RemovePortalFromList ( PORTAL* RemovePortal )
{
	// removes a portal from the linked list of portals, this is used to 
	// remove a portal fragment from the fragment array when it fails the 
	// test of either not being in two leafs or the portal already in the 
	// array is larger

	PORTAL* temp = RemovePortal;
	PORTAL* PrevPortal;
	PORTAL* NextPortal;
	
	if ( RemovePortal->Prev != NULL )
	{
		PrevPortal = RemovePortal->Prev;

		if ( RemovePortal->Next != NULL )
		{
			PrevPortal->Next = RemovePortal->Next;
		}
		else
		{
			PrevPortal->Next=NULL;
		}
	}
	
	if ( RemovePortal->Next != NULL )
	{
		NextPortal = RemovePortal->Next;

		if ( RemovePortal->Prev != NULL )
		{
			NextPortal->Prev = RemovePortal->Prev;
		}
		else
		{
			NextPortal->Prev = NULL;
		}
	}

	DeletePortal ( temp );
	temp = NULL;
}

BOOL CheckDuplicatePortal ( PORTAL* CheckPortal, int* index )
{
	// this is used to check the current portal with the portal already 
	// sharing the same leaves in tree, it is only accepted if the portal is
	// larger than the one that is ready occupying this place in the master 
	// portal array.

	long CheckPortalLeaf1 = CheckPortal->LeafOwnerArray [ 0 ];
	long CheckPortalLeaf2 = CheckPortal->LeafOwnerArray [ 1 ];
	long PALeaf1          = 0;
	long PALeaf2          = 0;
	
	// loop through the portals
	for ( long i = 0; i < NumberOfPortals; i++ )
	{
		PALeaf1 = PortalArray [ i ]->LeafOwnerArray [ 0 ];
		PALeaf2 = PortalArray [ i ]->LeafOwnerArray [ 1 ];

		if ( 
				( CheckPortalLeaf1 == PALeaf1 && CheckPortalLeaf2 == PALeaf2 )
				|| 
				( CheckPortalLeaf1 == PALeaf2 && CheckPortalLeaf2 == PALeaf1 )
			)
		{
			
			D3DXVECTOR3 Max1,
						Min1,
						Max2,
						Min2;

			GetPortalBounds ( CheckPortal,       &Min1, &Max1 );
			GetPortalBounds ( PortalArray [ i ], &Min2, &Max2 );

			// measure the length of the vectors to see which is bigger
			float NewSize = D3DXVec3Length ( &( Max1 - Min1 ) );
			float OldSize = D3DXVec3Length ( &( Max2 - Min2 ) );
 
			if ( fabs ( NewSize ) > fabs ( OldSize ) )
			{
				PORTAL* temp = PortalArray [ i ];
				DeletePortal ( temp );
				temp   = NULL;
				*index = i;

				return FALSE;
			}
			else 
			{ 
				// this portal is already in the array
				return TRUE;
			}
		}
	}

	*index = NumberOfPortals;
	
	// this portal was not found in the array
	return FALSE;
}
 
void GetPortalBounds ( PORTAL* Portal, D3DXVECTOR3* Min, D3DXVECTOR3* Max )
{
	// get the bounds of a portal

	float XMax = -99999, XMin = 99999;
	float YMax = -99999, YMin = 99999;
	float ZMax = -99999, ZMin = 99999;
 
	for ( WORD i = 0; i < Portal->NumberOfVertices; i++ )
	{
		if ( Portal->VertexList [ i ].x > XMax ) XMax = Portal->VertexList [ i ].x;
		if ( Portal->VertexList [ i ].y > YMax ) YMax = Portal->VertexList [ i ].y;
		if ( Portal->VertexList [ i ].z > ZMax ) ZMax = Portal->VertexList [ i ].z;
		if ( Portal->VertexList [ i ].x < XMin ) XMin = Portal->VertexList [ i ].x;
		if ( Portal->VertexList [ i ].y < YMin ) YMin = Portal->VertexList [ i ].y;
		if ( Portal->VertexList [ i ].z < ZMin ) ZMin = Portal->VertexList [ i ].z;
	}
 
	*Min = D3DXVECTOR3 ( XMin, YMin, ZMin );
	*Max = D3DXVECTOR3 ( XMax, YMax, ZMax );
}
 
PLANE GetPortalPlane ( PORTAL* Portal )
{
	// create a Plane structure from a portal
	// this saves us having to fill out a PLANE structure manually
	// everytime we want to use a portals plane

	PLANE plane;

	plane.Normal	   = Portal->Normal;
	plane.PointOnPlane = *( D3DXVECTOR3* ) &Portal->VertexList [ 0 ];

	return plane;
}

void DrawPortals ( void )
{
	// used to view the portals

	m_pD3D->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );

	for ( int index = 0; index < NumberOfPortals; index++ )
	{
		m_pD3D->DrawIndexedPrimitiveUP ( 
											D3DPT_TRIANGLELIST,
											0,
											PortalArray  [ index ]->NumberOfVertices,
											PortalArray  [ index ]->NumberOfIndices / 3,
											&PortalArray [ index ]->Indices [ 0 ],
											D3DFMT_INDEX16,
											&PortalArray [ index ]->VertexList [ 0 ],
											sizeof ( D3DLVERTEX ) 
										  );

	}

	m_pD3D->SetRenderState ( D3DRS_CULLMODE,D3DCULL_CCW ); 
}








