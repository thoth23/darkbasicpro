
#include "pvs_bsp.h"

BOOL	m_FastPVSCompile = FALSE;
BYTE	PortalVis [ 5000 ];
int		LeafVisCount, PVisCount;
int		m_ClipTests = 2;

#define ON_EPSILON    0.1f

void SetPVSBit ( BYTE* VisArray, long DestLeaf )
{
	// pass in a destination leaf and it will set it's corresponding uncompressed
	// bit position to 1 in the array passed

	VisArray [ DestLeaf >> 3 ] |= ( 1 << ( DestLeaf & 7 ) );
}

BOOL GetPVSBit ( BYTE* VisArray, long DestLeaf )
{
	// pass in a destination leaf and it will return if it's corresponding 
	// uncompressed bit position is set to 1 in the array passed

	return VisArray [ DestLeaf >> 3 ] & ( 1 << ( DestLeaf & 7 ) );
}

void CalculateDuplicatePortals ( void )
{
	// given our initial portals, we want to put them into a format suitable for
	// our PVS calculations ( front and back facing portals )

	int				i, p;
	PORTALPOINTS*	pp;

	// each portal previously calculated must now be split into two separate portals
	PortalDupArray = new PVSPORTAL [ NumberOfPortals * 2 ];

	ZeroMemory ( PortalDupArray, ( NumberOfPortals * 2 ) * sizeof ( PVSPORTAL ) );

	// when we are actually calculating the pvs we adjust the leaves here and place
	// each new front / back as new portal index into the needed leaf
	
	// so we'll set each leafs numberofportals to 0 here so that we can
	// build a new portal index at each leaf
	for ( i = 0; i < NumberOfLeafs; i++ )
	{
		LeafArray [ i ].NumberOfPortals = 0;
	}

	for ( i = 0, p = 0; i < NumberOfPortals; i++ )
	{
		// allocate memory for the new set of PortalPoints
		pp = PortalDupArray[p].Points = NewPortalPoints ( PortalArray [ i ]->NumberOfVertices );
		
		// flag this new set of points not to be deleted when
		// freeing up the portal it's in, because the memory
		// belongs to one of the master portals
		pp->DontDelete = TRUE;
		
		// copy the vertex points into this new portalpoint structure
		for ( int v = 0; v < PortalArray [ i ]->NumberOfVertices; v++ )
		{
			pp->VertexList [ v ] = *( D3DXVECTOR3* ) &PortalArray [ i ]->VertexList [ v ];
		}
		
		// retrieve this portals leaves and their bounding box centres
		// so that we can classify which duplicate portal leads where
		int Leaf0				= PortalArray [ i ]->LeafOwnerArray [ 0 ];
		int Leaf1				= PortalArray [ i ]->LeafOwnerArray [ 1 ];
		D3DXVECTOR3 Leaf0Center = ( LeafArray [ Leaf0 ].BoundingBox.BoxMax + LeafArray [ Leaf0 ].BoundingBox.BoxMin ) / 2;
		D3DXVECTOR3 Leaf1Center = ( LeafArray [ Leaf1 ].BoundingBox.BoxMax + LeafArray [ Leaf1 ].BoundingBox.BoxMin ) / 2;
		int Leaf0Location		= ClassifyPoint ( &Leaf0Center, &GetPortalPlane ( PortalArray [ i ] ) );

		// create portal duplicates, set it's neighbour leaf and add the index to it's current leaf
		switch ( Leaf0Location )
		{
			case CP_FRONT:
			{
				// if leaf 0 is in front of the plane then this portal must point
				// into that leaf and Leaf 1 must be our current
				LeafArray [ Leaf1 ].PortalIndexList [ LeafArray [ Leaf1 ].NumberOfPortals ] = p;
				LeafArray [ Leaf1 ].NumberOfPortals++;

				PortalDupArray [ p ].NeighbourLeaf		= Leaf0;
				PortalDupArray [ p ].Points				= pp;
				PortalDupArray [ p ].Plane.Normal		= PortalArray [ i ]->Normal;
				PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
				p++;

				// calculate second portal and add it to the leaf
				LeafArray [ Leaf0 ].PortalIndexList [ LeafArray [ Leaf0 ].NumberOfPortals ] = p;
				LeafArray [ Leaf0 ].NumberOfPortals++;

				PortalDupArray [ p ].NeighbourLeaf		= Leaf1;
				PortalDupArray [ p ].Points				= pp;
				PortalDupArray [ p ].Plane.Normal		= -PortalArray [ i ]->Normal;
				PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
				p++;
			}
			break;

			case CP_BACK:
			{
				// if leaf 0 is behind this portal then it must be leaf 0's
				// exit, so leaf 0 must be our current, and leaf1 our neighbour
				LeafArray [ Leaf0 ].PortalIndexList [ LeafArray [ Leaf0 ].NumberOfPortals ] = p;
				LeafArray [ Leaf0 ].NumberOfPortals++;

				PortalDupArray [ p ].NeighbourLeaf		= Leaf1;
				PortalDupArray [ p ].Points				= pp;
				PortalDupArray [ p ].Plane.Normal		= PortalArray [ i ]->Normal;
				PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
				p++;

				// calculate second portal and add it to the leaf
				LeafArray [ Leaf1 ].PortalIndexList [ LeafArray [ Leaf1 ].NumberOfPortals ] = p;
				LeafArray [ Leaf1 ].NumberOfPortals++;

				PortalDupArray [ p ].NeighbourLeaf		= Leaf0;
				PortalDupArray [ p ].Points				= pp;
				PortalDupArray [ p ].Plane.Normal		= -PortalArray [ i ]->Normal;
				PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
				p++;
			}
			break;

			case CP_ONPLANE:
			{
				// if the ceaf centre ended up on the plane then we're screwed
				// so test Leaf1 and check it's position ( last chance )

				int Leaf1Location = ClassifyPoint ( &Leaf1Center, &GetPortalPlane ( PortalArray [ i ] ) );
				
				switch ( Leaf1Location )
				{
					case CP_FRONT:
					{
						// if leaf 1 is in front of this portal then it must be leaf 0's
						// exit, so leaf 0 must be our current, and leaf1 our neighbour
						LeafArray [ Leaf0 ].PortalIndexList [ LeafArray [ Leaf0 ].NumberOfPortals ] = p;
						LeafArray [ Leaf0 ].NumberOfPortals++;

						PortalDupArray [ p ].NeighbourLeaf		= Leaf1;
						PortalDupArray [ p ].Points				= pp;
						PortalDupArray [ p ].Plane.Normal		= PortalArray [ i ]->Normal;
						PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
						p++;

						// calculate second portal and add it to the leaf
						LeafArray [ Leaf1 ].PortalIndexList [ LeafArray [ Leaf1 ].NumberOfPortals ] = p;
						LeafArray [ Leaf1 ].NumberOfPortals++;

						PortalDupArray [ p ].NeighbourLeaf		= Leaf0;
						PortalDupArray [ p ].Points				= pp;
						PortalDupArray [ p ].Plane.Normal		= -PortalArray [ i ]->Normal;
						PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
						p++;
					}
					break;

					case CP_BACK:
					{
						// if leaf 1 is behind the plane then this portal must point
						// into leaf 0 and Leaf 1 must be our current.
						LeafArray [ Leaf1 ].PortalIndexList [ LeafArray [ Leaf1 ].NumberOfPortals ] = p;
						LeafArray [ Leaf1 ].NumberOfPortals++;

						PortalDupArray [ p ].NeighbourLeaf		= Leaf0;
						PortalDupArray [ p ].Points				= pp;
						PortalDupArray [ p ].Plane.Normal		= PortalArray [ i ]->Normal;
						PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
						p++;

						// calculate second portal and add it to the leaf
						LeafArray [ Leaf0 ].PortalIndexList [ LeafArray [ Leaf0 ].NumberOfPortals] = p;
						LeafArray [ Leaf0 ].NumberOfPortals++;

						PortalDupArray [ p ].NeighbourLeaf		= Leaf1;
						PortalDupArray [ p ].Points				= pp;
						PortalDupArray [ p ].Plane.Normal		= -PortalArray [ i ]->Normal;
						PortalDupArray [ p ].Plane.PointOnPlane	= pp->VertexList [ 0 ];
						p++;
					}
					break;

					case CP_ONPLANE:
					{
						MessageBox ( 0, "Cannot calculate portal to leaf direction","Error", MB_OK | MB_ICONSTOP );
						PostQuitMessage ( 0 );
					}
					break;
				}
			}
			break;
		}
	}
}

PORTALPOINTS* NewPortalPoints ( int NumberOfPoints )
{
	PORTALPOINTS* NewPoints     = new PORTALPOINTS;
	NewPoints->DontDelete       = FALSE;
	NewPoints->VertexList       = new D3DXVECTOR3 [ NumberOfPoints ];
	NewPoints->NumberOfVertices = NumberOfPoints;
	
	return NewPoints;
}

void CalculatePVS ( void )
{
	// calculate number of bytes needed to store each leafs
	// vis array in BIT form ( i.e 8 leafs vis per byte uncompressed )
	BytesPerSet = ( NumberOfLeafs + 7 ) / 8;

	// 32 bit align the bytes per set to allow for our early out long conversion
	BytesPerSet = ( BytesPerSet * 3 + 3 ) & 0xFFFFFFFC;

	// reserve enough space to hold every leafs PVS set ( plus some to spare )
	PVSData	= ( BYTE* ) malloc ( ( NumberOfLeafs + 5 ) * BytesPerSet );
	
	ZeroMemory ( PVSData, ( NumberOfLeafs + 5 ) * BytesPerSet );

	// retrieve all of our one way portals
	CalculateDuplicatePortals ( );

	// fill out our portals initial vis array
	InitialPortalVis ( );

	// start the actual PVS calculations here
	CalcPortalVis ( );
	
	// now write the PVS information to the master array
	LeafPVSWrite ( );

}

void LeafPVSWrite ( void )
{
	// this writes out all of the final calculated PVS information into the
	// master PVS array, and the pointer into it, into the leaves themselves

	BYTE* LeafPVS = new BYTE [ BytesPerSet ];
	int PVSMasterWritePointer = 0;
	
	// loop round each leaf and collect the vis info this is all OR'd together
	// and ZRLE compressed then finally stored in the master array
	for ( int i = 0; i < NumberOfLeafs; i++ )
	{
		ZeroMemory ( LeafPVS, BytesPerSet );

		LeafArray [ i ].PVSIndex = PVSMasterWritePointer;

		// current leaf is always visible
		SetPVSBit ( LeafPVS, i );

		for ( int p = 0; p < LeafArray [ i ].NumberOfPortals; p++ )
		{
			// or the vis bits together
			for ( int j = 0; j < BytesPerSet; j++ )
			{
				LeafPVS [ j ] |= PortalDupArray [ LeafArray [ i ].PortalIndexList [ p ] ].ActualVisBits [ j ];
			}
		}

		PVSMasterWritePointer += CompressLeafSet ( LeafPVS, PVSMasterWritePointer );
	}

	// clean up after ourselves
	delete [ ] LeafPVS;

	// so resize the array so we don't waste any memory
	PVSData           = ( BYTE* ) realloc ( PVSData, PVSMasterWritePointer );
	PVSCompressedSize = PVSMasterWritePointer;

}

long CompressLeafSet ( BYTE* VisArray, long WritePos )
{
	// ZRLE compresses the uncompressed vis bit array which was passed in, and
	// compresses and adds it to the master PVS array, this function returns
	// the size of the compressed set so we can update our write pointer

	int		rep;
	BYTE*	dest = &PVSData [ WritePos ];
	BYTE*	dest_p;
	dest_p = dest;
	
	for ( int j=0; j < BytesPerSet; j++ )
	{
		*dest_p++ = VisArray [ j ];

		if ( VisArray [ j ] )
			continue;

		rep = 1;

		for ( j++; j < BytesPerSet; j++ )
		{
			if ( VisArray [ j ] || rep == 255 )
				break;
			else
				rep++;
		}

		*dest_p++ = rep;
		j--;
	}
	
	return dest_p - dest;
}

void InitialPortalVis ( void )
{
	PORTALPOINTS*	pp;

	// loop round the portal array allocating and checking portal visibility
	// against every other portal
	for ( int p1 = 0; p1 < NumberOfPortals * 2; p1++ )
	{
		PortalDupArray [ p1 ].PossibleVisBits = new BYTE [ BytesPerSet ];

		ZeroMemory ( PortalDupArray [ p1 ].PossibleVisBits, BytesPerSet );
		
		PVisCount = 0;
		ZeroMemory ( PortalVis, NumberOfPortals * 2 );

		// for this portal loop round all other portals
		for ( int p2 = 0; p2 < NumberOfPortals * 2; p2++ )
		{
			if ( p2 == p1 )
				continue;
			
			// test to see if any of p2's portal points are in front of p1's plane
			pp = PortalDupArray [ p2 ].Points;

			for ( int i = 0; i < pp->NumberOfVertices; i++ )
			{
				if ( ClassifyPoint ( &pp->VertexList [ i ], &PortalDupArray [ p1 ].Plane ) == CP_FRONT )
					break;
			}
			
			// if the loop reached the end there were no points in front so continue
			if ( i == pp->NumberOfVertices )
				continue;
			
			// test to see if any of p1's portal points are in behind p2's plane
			pp = PortalDupArray [ p1 ].Points;

			for ( i = 0; i < pp->NumberOfVertices; i++)
			{
				if ( ClassifyPoint ( &pp->VertexList [ i ], &PortalDupArray [ p2 ].Plane ) == CP_BACK )
					break;
			}
			
			// if the loop reached the end there were no points in front so continue
			if ( i == pp->NumberOfVertices )
				continue;

			// fill out the temporary portal visibility array
			PortalVis [ p2 ] = 1;		
			PVisCount++;
		}
		
		// now flood through all the portals which are visible
		// from the source portal through into the neighbour leaf
		// and flag any leaves which are visible ( the leaves which
		// remain set to 0 can never possibly be seen from this portal )
		LeafVisCount = 0;
		PortalFlood ( &PortalDupArray [ p1 ], PortalDupArray [ p1 ].NeighbourLeaf );
		PortalDupArray [ p1 ].NumPossible = LeafVisCount;
	}
}

void PortalFlood ( PVSPORTAL* SourcePortal, int Leaf )
{
	// this function does a basic flood fill through all of the portals from this
	// leaf recursively and flags all leaves which the flood fill reached
	
	// test the source portals might see list to see if
	// this leaf has already been set.
	if ( GetPVSBit ( SourcePortal->PossibleVisBits, Leaf ) )
		return;
	
	// set the might see bits in this leaf
	SetPVSBit ( SourcePortal->PossibleVisBits, Leaf );
	
	LeafVisCount++;
	
	for ( int i = 0; i < LeafArray [ Leaf ].NumberOfPortals; i++ )
	{
		// if this portal was not flagged as allowed
		// to pass through, then continue to next portal
		if ( !PortalVis [ LeafArray [ Leaf ].PortalIndexList [ i ] ] )
			continue;

		PortalFlood ( SourcePortal, PortalDupArray [ LeafArray [ Leaf ].PortalIndexList [ i ] ].NeighbourLeaf );
	}
}

void CalcPortalVis ( void )
{
	int		i;

	// if we want to perform a quick vis ( not at all accurate ) we can
	// simply use the mightsee array as our pvs bytes.
	if ( m_FastPVSCompile )
	{
		for ( i = 0; i < NumberOfPortals * 2; i++ )
		{
			PortalDupArray [ i ].ActualVisBits = PortalDupArray [ i ].PossibleVisBits;
		}

		return;
	}
	
	PVSRECURSEDATA data;
	ZeroMemory ( &data, sizeof ( PVSRECURSEDATA ) );

	char ProgressBuffer [ 128 ];
	int	 ProgressCount = 0;
	RECT rect;
	
	rect.left   = 10;
	rect.right  = 640;
	rect.top    = 440;
	rect.bottom = 480;

	// loop round each of our duplicate portals
	while ( ( i = GetNextPortal ( ) ) != -1 )
	{
		ProgressCount++;

		
		g_iProgress = ( int ) ( ( ( float ) ProgressCount / ( NumberOfPortals * 2 ) ) * 100.0f );

		/*
		sprintf ( ProgressBuffer, "PVS Compile Progress : %i%%", ( int ) ( ( ( float ) ProgressCount / ( NumberOfPortals * 2 ) ) * 100.0f ) );
		m_pD3D->Clear ( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB ( 0, 0, 0), 1.0f, 0 );
		MainFont->DrawTextA ( ProgressBuffer, strlen ( ProgressBuffer ) ,&rect, DT_LEFT | DT_NOCLIP,D3DCOLOR_XRGB ( 255, 50, 50 ) );
		m_pD3D->Present ( NULL, NULL, NULL, NULL );
		*/

		// fill our our initial data structure
		data.SourcePoints = PortalDupArray [ i ].Points;
		data.TargetPlane  = PortalDupArray [ i ].Plane;
		data.VisBits	  = PortalDupArray [ i ].PossibleVisBits;

		// allocate portals visbit array
		PortalDupArray [ i ].ActualVisBits = new BYTE [ BytesPerSet ];
		ZeroMemory ( PortalDupArray [ i ].ActualVisBits, BytesPerSet );
		
		RecursePVS ( PortalDupArray [ i ].NeighbourLeaf, &PortalDupArray [ i ], &data );

		PortalDupArray [ i ].status = PS_PROCESSED;
	}
}

int GetNextPortal ( void )
{
	// returns the next portal in order of complexity
	// this means that all the least complex portals are processed first so that
	// these portals vis info can be used in the early out system in RecursePVS
	// to help speed things up

	int	p = -1, min = 999999;

	for ( int i = 0; i < NumberOfPortals * 2; i++ )
	{
		// if this portals complexity is the lowest and it has not already been processed then we could use it
		if ( PortalDupArray [ i ].NumPossible < min && PortalDupArray [ i ].status == PS_NOTPROCESSED )
		{
			min = PortalDupArray [ i ].NumPossible;
			p = i;
		}
	}

	// set our status flag to currently being worked on
	if ( p > -1 )
		PortalDupArray [ p ].status = PS_PROCESSING;

	return p;
}

void RecursePVS ( int LeafNum, PVSPORTAL* SourcePortal, PVSRECURSEDATA* prevdata )
{
	PVSRECURSEDATA	data;
	PVSPORTAL*		GeneratorPortal;
	PLANE			ReverseGenPlane;
	PORTALPOINTS*	SourcePoints, *GeneratorPoints;
	int				i, j;
	long			*test, *possible, *vis;
	BOOL			More;

	// store the leaf for easy access
	LEAF* Leaf = &LeafArray [ LeafNum ];
	
	// mark this leaf as visible
	SetPVSBit ( SourcePortal->ActualVisBits, LeafNum );

	data.VisBits = new BYTE [ BytesPerSet ];
	possible	 = ( long* ) data.VisBits;
	vis			 = ( long* ) SourcePortal->ActualVisBits;
	
	// check all portals for flowing into other leafs	
	for ( i = 0 ; i < Leaf->NumberOfPortals; i++ )
	{
		// store the portal for easy access
		GeneratorPortal = &PortalDupArray [ Leaf->PortalIndexList [ i ] ];

		// can't possibly recurse through this portal if
		// it's neighbour leaf is set to invisible in the target portals pvs
		if ( ! GetPVSBit ( prevdata->VisBits, GeneratorPortal->NeighbourLeaf ) )
			continue;

		// if the portal can't see anything we haven't already seen, skip it
		if ( GeneratorPortal->status == PS_PROCESSED )
			test = ( long* )GeneratorPortal->ActualVisBits;
		else
			test = ( long* ) GeneratorPortal->PossibleVisBits;

		// check to see if we have processed as much as we need to
		// this is an early out system, we check in 32 bit chunks
		// to speed things up a bit
		More = FALSE;
		
		for ( j = 0; j < int ( BytesPerSet / sizeof ( long ) ); j++ )
		{
			possible [ j ] = ( ( long* ) prevdata->VisBits ) [ j ] & test [ j ];
			
			if ( possible [ j ] & ~vis [ j ] )
				More = TRUE;
		}
		
		// can't see anything new
		if ( !More )
			continue;
		
		// the current generator plane will become the next recurs target plane 
		data.TargetPlane = GeneratorPortal->Plane;

		// we can't go out a coplanar face so check this
		ReverseGenPlane.Normal       = -GeneratorPortal->Plane.Normal;
		ReverseGenPlane.PointOnPlane = GeneratorPortal->Plane.PointOnPlane;

		if ( fabs ( prevdata->TargetPlane.Normal.x - ReverseGenPlane.Normal.x ) < 0.001 &&
			 fabs ( prevdata->TargetPlane.Normal.y - ReverseGenPlane.Normal.y ) < 0.001 &&
			 fabs ( prevdata->TargetPlane.Normal.z - ReverseGenPlane.Normal.z ) < 0.001   )
			 continue;	
		
		// clip the generator portal to the source if none remains continue
		GeneratorPoints = ClipPortalPoints ( GeneratorPortal->Points, &SourcePortal->Plane, FALSE );

		if ( !GeneratorPoints )
			continue;
			
		// the second leaf can only be blocked if coplanar
		if ( !prevdata->TargetPoints )
		{
			data.SourcePoints = prevdata->SourcePoints;
			data.TargetPoints = GeneratorPoints;

			RecursePVS ( GeneratorPortal->NeighbourLeaf, SourcePortal, &data );

			FreePortalPoints ( GeneratorPoints );

			continue;
		}

		// clip the generator portal to the previous target if non remains continue
		GeneratorPoints = ClipPortalPoints ( GeneratorPoints, &prevdata->TargetPlane, FALSE );

		if ( !GeneratorPoints )
			continue;
		
		// make a copy of the source portals points
		SourcePoints = CopyPortalPoints ( prevdata->SourcePoints );

		// clip the source portal
		SourcePoints = ClipPortalPoints ( SourcePoints, &ReverseGenPlane, FALSE );

		// if no remains continue to the next portal
		if ( !SourcePoints )
		{ 
			FreePortalPoints ( GeneratorPoints );
			continue;
		}

		// go clip
		if ( m_ClipTests > 0 )
		{
			GeneratorPoints = ClipToAntiPenumbra ( SourcePoints, prevdata->TargetPoints, GeneratorPoints, FALSE );

			if ( !GeneratorPoints )
			{ 
				FreePortalPoints ( SourcePoints );
				continue;
			}
		}
		
		if ( m_ClipTests > 1 )
		{
			GeneratorPoints = ClipToAntiPenumbra ( prevdata->TargetPoints, SourcePoints, GeneratorPoints, TRUE );
			
			if ( !GeneratorPoints )
			{ 
				FreePortalPoints ( SourcePoints );
				continue;
			}
		}
		
		if ( m_ClipTests > 2 )
		{
			SourcePoints = ClipToAntiPenumbra ( GeneratorPoints, prevdata->TargetPoints, SourcePoints, FALSE );
			
			if ( !SourcePoints )
			{ 
				FreePortalPoints ( GeneratorPoints );
				continue;
			}
		}
		
		if ( m_ClipTests > 3 )
		{
			SourcePoints = ClipToAntiPenumbra ( prevdata->TargetPoints, GeneratorPoints, SourcePoints, TRUE );
			
			if ( !SourcePoints )
			{ 
				FreePortalPoints ( GeneratorPoints );
				continue;
			}
		}

		data.SourcePoints = SourcePoints;
		data.TargetPoints = GeneratorPoints;
	
		// flow through it for real
		RecursePVS ( GeneratorPortal->NeighbourLeaf, SourcePortal, &data );
		
		FreePortalPoints ( SourcePoints );
		FreePortalPoints ( GeneratorPoints );
	}
	
	delete [ ]data.VisBits;
}

PORTALPOINTS* ClipToAntiPenumbra ( PORTALPOINTS* Source, PORTALPOINTS* Target, PORTALPOINTS* Generator, BOOL ReverseClip )
{
	int			i, j, k, l;
	PLANE		plane;
	D3DXVECTOR3	v1, v2;
	float		length;
	int			counts [ 3 ];
	BOOL		ReverseTest;

	// check all combinations	
	for ( i = 0; i < Source->NumberOfVertices; i++ )
	{
		l  = ( i + 1 ) % Source->NumberOfVertices;
		v1 = Source->VertexList [ l ] - Source->VertexList [ i ];

		// find a vertex of generator that makes a plane that puts all of the
		// vertices of pass on the front side and all of the vertices of
		// source on the back side
		for ( j = 0 ; j < Target->NumberOfVertices; j++ )
		{
			v2 = Target->VertexList [ j ] - Source->VertexList [ i ];
			
			D3DXVec3Cross ( &plane.Normal, &v1, &v2 );

			// if points don't make a valid plane, skip it
			length = plane.Normal.x * plane.Normal.x +
					 plane.Normal.y * plane.Normal.y +
					 plane.Normal.z * plane.Normal.z;
			
			if ( length < ON_EPSILON )
				continue;

			length = 1 / sqrtf ( length );
			
			plane.Normal.x *= length;
			plane.Normal.y *= length;
			plane.Normal.z *= length;

			plane.PointOnPlane = Target->VertexList [ j ];

			// find out which side of the generated seperating plane has the
			// source portal

			ReverseTest = FALSE;

			for ( k = 0; k < Source->NumberOfVertices; k++ )
			{
				if ( k == i || k == l )
					continue;

				int Location = ClassifyPoint ( &Source->VertexList [ k ], &plane );
				
				if ( Location == CP_BACK )
				{
					// source is on the negative side, so we want all
					// pass and target on the positive side
					ReverseTest = FALSE;
					break;
				} 
				else if ( Location == CP_FRONT )
				{
					// source is on the positive side, so we want all
					// pass and target on the negative side
					ReverseTest = TRUE;
					break;
				}
			}

			// planar with source portal
			if ( k == Source->NumberOfVertices )
				continue;

			// flip the normal if the source portal is backwards
			if ( ReverseTest )
			{ 
				plane.Normal = -plane.Normal;
			}
			
			// if all of the pass portal points are now on the positive side,
			// this is the seperating plane
			counts [ 0 ] = counts [ 1 ] = counts [ 2 ] = 0;

			for ( k = 0; k < Target->NumberOfVertices; k++ )
			{
				if ( k == j )
					continue;

				int Location = ClassifyPoint ( &Target->VertexList [ k ], &plane );

				if ( Location == CP_BACK )
					break;
				else if ( Location == CP_FRONT )
					counts [ 0 ]++;
				else
					counts [ 2 ]++;
			}

			// points on negative side, not a seperating plane
			if ( k != Target->NumberOfVertices )
				continue;	
			
			// planar with seperating plane
			if ( counts [ 0 ] == 0 )
				continue;
		
			// flip the normal if we want the back side
			if ( ReverseClip )
			{ 
				plane.Normal = -plane.Normal;
			}
			
			// clip target by the seperating plane
			Generator = ClipPortalPoints ( Generator, &plane, FALSE );
			
			// target is not visible
			if ( !Generator )
				return NULL;		
		}
	}
	
	return Generator;
}

void FreePortalPoints ( PORTALPOINTS* Points )
{
	if ( !Points->DontDelete )
	{ 
		delete [ ] Points->VertexList;
		delete Points;
	}
}

PORTALPOINTS* CopyPortalPoints ( PORTALPOINTS* Points )
{
	PORTALPOINTS* NewPoints = new PORTALPOINTS;
	memcpy ( NewPoints, Points, sizeof ( PORTALPOINTS ) );
	NewPoints->VertexList = new D3DXVECTOR3 [ Points->NumberOfVertices ];
	memcpy ( NewPoints->VertexList, Points->VertexList, Points->NumberOfVertices * sizeof ( D3DXVECTOR3 ) );
	NewPoints->DontDelete = FALSE;
	return NewPoints;
}

PORTALPOINTS* ClipPortalPoints ( PORTALPOINTS* Points, PLANE* Plane, BOOL KeepOnPlane )
{
	// clips the portal points against the plane passed in. it only returns the
	// points which are in front of the plane, this also frees the input Points
	// if allowed to do so if true was passed to KeepOnPlane then if the points
	// were all exactly on the plane, they will be saved, otherwise they will be
	// removed

	PORTALPOINTS*	NewPoints = NULL;
	float			PointDistance [ 50 ];
	int				PointLocation [ 50 ];
	int				InFront = 0, Behind = 0, OnPlane = 0;
	int				maxpts = 0;
	int				Location;
	D3DXVECTOR3		Mid;

	float Dist = D3DXVec3Dot ( &Plane->PointOnPlane, &Plane->Normal );

	// determine each points location relative to the plane
	for ( int i = 0; i < Points->NumberOfVertices; i++ )
	{
		Location = ClassifyPoint ( &Points->VertexList [ i ], Plane );

		PointDistance [ i ] = D3DXVec3Dot ( &Points->VertexList [ i ], &Plane->Normal ) - Dist;

		if ( Location == CP_FRONT )
		{
			PointLocation [ i ] = CP_FRONT;
			InFront++;
		}
		else if ( Location == CP_BACK )
		{
			PointLocation [ i ] = CP_BACK;
			Behind++;
		}
		else
		{
			PointLocation [ i ] = CP_ONPLANE;
			OnPlane++;
		}
	}

	PointLocation [ Points->NumberOfVertices ] = PointLocation [ 0 ];
	PointDistance [ Points->NumberOfVertices ] = PointDistance [ 0 ];
	
	if ( KeepOnPlane && InFront == 0 && Behind == 0 )
		return Points;
		
	if ( InFront == 0 )
	{
		FreePortalPoints ( Points );
		return NULL;
	}

	if ( Behind == 0 )
		return Points;
	
	maxpts	  = Points->NumberOfVertices + 4;
	NewPoints = NewPortalPoints ( maxpts );
	NewPoints->NumberOfVertices = 0;

	for ( i = 0; i < Points->NumberOfVertices; i++ )
	{
		if ( PointLocation [ i ] == CP_ONPLANE )
		{
			NewPoints->VertexList [ NewPoints->NumberOfVertices ] = Points->VertexList [ i ];
			NewPoints->NumberOfVertices++;

			continue;
		}

		if ( PointLocation [ i ] == CP_FRONT )
		{
			NewPoints->VertexList [ NewPoints->NumberOfVertices ] = Points->VertexList [ i ];
			NewPoints->NumberOfVertices++;
		}
		
		if ( PointLocation [ i + 1 ] == CP_ONPLANE || PointLocation [ i + 1 ] == PointLocation [ i ] )
			continue;
			
		D3DXVECTOR3 tvert = Points->VertexList [ ( i + 1 ) % Points->NumberOfVertices ];
		
		float dot = PointDistance [ i ] / ( PointDistance [ i ] - PointDistance [ i + 1 ] );
		
		// we do this to try and avoid rounding errors for each x / y / z
		if		( Plane->Normal.x ==  1 ) Mid.x =  Dist; 
		else if ( Plane->Normal.x == -1 ) Mid.x = -Dist;
		else	Mid.x = Points->VertexList [ i ].x + dot * ( tvert.x - Points->VertexList [ i ].x );
		if		( Plane->Normal.y ==  1 ) Mid.y =  Dist; 
		else if ( Plane->Normal.y == -1 ) Mid.y = -Dist;
		else	Mid.y = Points->VertexList [ i ].y + dot *  (tvert.y - Points->VertexList [ i ].y );
		if		( Plane->Normal.z ==  1 ) Mid.z =  Dist; 
		else if ( Plane->Normal.z == -1 ) Mid.z = -Dist;
		else	Mid.z = Points->VertexList [ i ].z + dot * ( tvert.z - Points->VertexList [ i ].z );
		
		NewPoints->VertexList [ NewPoints->NumberOfVertices ] = Mid;
		NewPoints->NumberOfVertices++;
	}
	
	if ( NewPoints->NumberOfVertices > maxpts )
		MessageBox ( 0, "ClipPortalPoints - number of vertices exceeded estimate", "Error", MB_OK | MB_ICONSTOP );
		
	// free the original portal points if allowed
	FreePortalPoints ( Points );
	
	return NewPoints;
}

void FreeDuplicatePortals ( void )
{
	if ( !PortalDupArray )
		return;

	for ( int i = 0; i < NumberOfPortals * 2; i += 2 )
	{
		delete [ ] PortalDupArray [ i ].Points->VertexList;
		delete PortalDupArray [ i ].Points;
		delete [ ] PortalDupArray [ i     ].PossibleVisBits;
		delete [ ] PortalDupArray [ i + 1 ].PossibleVisBits;
		
		if ( !m_FastPVSCompile )
			delete [ ]PortalDupArray [ i ].ActualVisBits;

		if ( !m_FastPVSCompile )
			delete [ ] PortalDupArray [ i + 1 ].ActualVisBits;
	}

	delete [ ] PortalDupArray;
}
