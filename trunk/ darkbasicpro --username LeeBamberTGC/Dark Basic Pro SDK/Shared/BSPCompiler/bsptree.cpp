
#include "pvs_bsp.h"

// polygons will be selected from their leaves each frame
// and added to the polygon list of the corresponding texture
// this allows us to batch the polygons to minimize texture changes
POLYGON** pTexturePolygonList;

void BuildBspTree ( long Node, POLYGON* PolyList )
{
	// build's the entire BSP tree using the already initialised poly data

	POLYGON* polyTest	 = NULL;
	POLYGON* NextPolygon = NULL;
	POLYGON* FrontList	 = NULL;
	POLYGON* BackList	 = NULL;
	POLYGON* FrontSplit	 = NULL;
	POLYGON* BackSplit	 = NULL;
	
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 vec2;
	D3DXVECTOR3 a;
	D3DXVECTOR3 b;
	float		result;

	// select the best splitting plane from the polygon list
	NodeArray [ Node ].Plane = SelectBestSplitter ( PolyList );

	// store the poly list ( we need to use the original later )
	polyTest = PolyList;

	// set up dummy bounding boxes for the node so it can be altered
	NodeArray [ Node ].BoundingBox.BoxMax = D3DXVECTOR3 ( -40000.0f, -40000.0f, -40000.0f );
	NodeArray [ Node ].BoundingBox.BoxMin = D3DXVECTOR3 (  40000.0f,  40000.0f,  40000.0f );

	while ( polyTest != NULL )
	{
		// remember to store because polytest->next will be altered
		NextPolygon = polyTest->Next;

		switch ( ClassifyPoly ( &PlaneArray [ NodeArray [ Node ].Plane ], polyTest ) )
		{
			case CP_ONPLANE:
			{
				// if the polygon ends up on the plane we will need to pass
				// it down the side the plane is facing so we can do a quick
				// test and pass it down the right side
				a = PlaneArray [ NodeArray [ Node ].Plane ].Normal;
				b = polyTest->Normal;	

				result = ( float ) fabs ( ( a.x - b.x ) + ( a.y - b.y ) + ( a.z - b.z ) );

				// check which side we're on
				if ( result < 0.1 )
				{
					// front side
					polyTest->Next = FrontList;
					FrontList	   = polyTest;
				}
				else
				{
					// back side
					polyTest->Next = BackList;
					BackList	   = polyTest;
				}
			}
			break;

			case CP_FRONT:
			{
				// pass the polygon straight down the front list
				polyTest->Next = FrontList;
				FrontList	   = polyTest;
			}
			break;

			case CP_BACK:
			{
				// pass the polygon straight down the back list
				polyTest->Next = BackList;
				BackList	   = polyTest;
			}
			break;

			case CP_SPANNING:
			{
				// if polygon is spanning the plane we need to split
				// it and pass each fragment down the appropriate side
				FrontSplit	= new POLYGON;
				BackSplit	= new POLYGON;

				ZeroMemory ( FrontSplit, sizeof ( POLYGON ) );
				ZeroMemory ( BackSplit,  sizeof ( POLYGON ) );
	
				// split the polygon
				SplitPolygon ( polyTest, &PlaneArray [ NodeArray [ Node ].Plane ], FrontSplit, BackSplit );
    
				// copy details
				FrontSplit->BeenUsedAsSplitter	= polyTest->BeenUsedAsSplitter;
				BackSplit->BeenUsedAsSplitter	= polyTest->BeenUsedAsSplitter;
				FrontSplit->TextureIndex		= polyTest->TextureIndex;
				BackSplit-> TextureIndex		= polyTest->TextureIndex;
	
				// free up original polygon
				DeletePolygon ( polyTest );
	
				// add fragments to the front / back lists
				FrontSplit->Next = FrontList;
				FrontList		 = FrontSplit;
				BackSplit->Next	 = BackList;
				BackList		 = BackSplit;
			}
			break;
		}
		
		polyTest = NextPolygon;
	}
   
	// calculate the nodes bounding box
	CalculateBox ( &NodeArray [ Node ].BoundingBox, FrontList );

	BOUNDINGBOX LeafBox = NodeArray [ Node ].BoundingBox;

	CalculateBox ( &NodeArray [ Node ].BoundingBox, BackList );
	
	// count the splitters remaining in this list
	int		 count	= 0;
	POLYGON* tempf	= FrontList;
	
	while ( tempf != NULL )
	{
		if ( tempf->BeenUsedAsSplitter == false )
			count++;

		tempf = tempf->Next;
	}

	// if there are no splitters remaining we can go ahead and add the leaf
	if ( count == 0 )
	{
		POLYGON *Iterator = FrontList;
		POLYGON *Temp;

		LeafArray [ NumberOfLeafs ].StartPolygon = NumberOfPolygons;

		while ( Iterator != NULL )
		{
			PolygonArray [ NumberOfPolygons ] = *Iterator;

			IncreaseNumberOfPolygons ( );

			Temp	 = Iterator;
			Iterator = Iterator->Next;
			
			// we use the normal delete operator here because we dont want to delete the actual 
			// vertexLists and index lists because we are still referenceing them in the polygon array
			delete Temp;
		}
		
		// finish setting up details
		LeafArray [ NumberOfLeafs ].EndPolygon	= NumberOfPolygons;
		LeafArray [ NumberOfLeafs ].BoundingBox	= LeafBox;
		NodeArray [ Node ].Front				= NumberOfLeafs;
		NodeArray [ Node ].IsLeaf				= 1;

		IncreaseNumberOfLeafs ( );
	}
	else
	{
		// otherwise create a new node, and push the front list down the tree
		NodeArray [ Node ].IsLeaf = 0;
		NodeArray [ Node ].Front  = NumberOfNodes + 1;
		
		IncreaseNumberOfNodes ( );

		BuildBspTree ( NumberOfNodes, FrontList );
	}

	// if the back list is empty flag this as solid otherwise
	// push the back list down the back of this node
	if ( BackList == NULL )
	{
		NodeArray [ Node ].Back = -1;
	}
	else
	{
		NodeArray [ Node ].Back = NumberOfNodes + 1;

		IncreaseNumberOfNodes ( );

		BuildBspTree ( NumberOfNodes,BackList );
	}
}

int ClassifyPoly ( PLANE* Plane, POLYGON* Poly )
{
	// given a plane and a polygon return the polygons position
	// relative to the plane itself - front, back, on, spanning

	int			 Infront = 0;
	int			 Behind  = 0;
	int			 OnPlane = 0;
	float		 result  = 0;
	D3DXVECTOR3* vec1    = &Plane->PointOnPlane;

	// loop round each vertex
	for ( int a = 0; a < Poly->NumberOfVertices; a++ )
	{
		D3DXVECTOR3* vec2      = ( D3DXVECTOR3* ) &Poly->VertexList [ a ];
		D3DXVECTOR3  Direction = ( *vec1 ) - ( *vec2 );

		result = D3DXVec3Dot ( &Direction, &Plane->Normal );

		// check the position
		if ( result > g_EPSILON )
		{
			Behind++;
		}
		else if ( result < -g_EPSILON )
		{
			Infront++;
		}
		else
		{
			OnPlane++;
			Infront++;
			Behind++;
		}
	}

	if ( OnPlane == Poly->NumberOfVertices )
		return CP_ONPLANE;

	if ( Behind == Poly->NumberOfVertices )
		return CP_BACK;

	if ( Infront == Poly->NumberOfVertices )
		return CP_FRONT;
	
	return CP_SPANNING;
}

int ClassifyPoint( D3DXVECTOR3 *pos, PLANE *Plane ) 
{
	// given a plane and a point return the points position
	// relative to the plane itself - front, back, on

	float		 result;
	D3DXVECTOR3* vec1		= &Plane->PointOnPlane;
	D3DXVECTOR3  Direction	= ( *vec1 ) - ( *pos );

	result = D3DXVec3Dot ( &Direction, &Plane->Normal );

	if ( result < -g_EPSILON )
		return CP_FRONT;

	if ( result > g_EPSILON )
		return CP_BACK;

	return CP_ONPLANE;
}

void RenderTree ( D3DXVECTOR3 pos )
{
	// run through the tree and find the current leaf the camera is in
	// then we can draw the tree
	int Node = 0;
	int leaf = 0;
	
	while ( 1 )
	{
		switch ( ClassifyPoint ( &pos, &PlaneArray [ NodeArray [ Node ].Plane ] ) )
		{
			case CP_FRONT:
			{
				if ( NodeArray [ Node ].IsLeaf != 0 )
				{
					leaf = NodeArray [ Node ].Front;
					DrawTree ( leaf );
					return;
				}
				else
				{
					Node = NodeArray [ Node ].Front;
				}
			}
			break;

			case CP_BACK:
			{
				if ( NodeArray [ Node ].Back == -1 )
				{
					return;
				}
				else
				{
					Node = NodeArray [ Node ].Back;
				}
			}
			break;

			case CP_ONPLANE:
			{
				if ( NodeArray [ Node ].IsLeaf != 0 )
				{
					leaf = NodeArray [ Node ].Front;
					DrawTree ( leaf );
					return;
				}
				else
				{
					Node = NodeArray [ Node ].Front;
				}
			}
			break;

			default:
				break;
		}
	}
}

void DrawTree ( long leaf )
{
	// draw the pvs set

	int		 i;
	long	 currentleaf = 0;
	long	 PVSOFFSET	 = LeafArray [ leaf ].PVSIndex;
	BYTE*	 PVSPointer	 = PVSData;
	POLYGON* CurrentPoly = NULL;
	
	PVSPointer += PVSOFFSET;
	
	// set all the texture batch pointers to NULL
	for ( i = 0; i < NumberOfTextures; i++ )
		pTexturePolygonList [ i ] = NULL;

	// loop around leaves
	while ( currentleaf < NumberOfLeafs )
	{
		// if this is a non 0 PVS byte
		if ( *PVSPointer != 0 )
		{
			for ( i = 0; i < 8; i++ )
			{
				BYTE mask = 1 << i;
				BYTE pvs  = *PVSPointer;
				
				if ( pvs & mask )
				{
					if ( LeafInFrustum ( currentleaf ) || DontFrustumReject )
					{
						unsigned long start	 = LeafArray [ currentleaf ].StartPolygon;
						unsigned long finish = LeafArray [ currentleaf ].EndPolygon; 
						unsigned long Count  = 0;

						// collect the polygons from this leaf and add them to the correct texture list
						for ( Count = start; Count < finish; Count++ )
						{
							CurrentPoly		                                  = &PolygonArray [ Count ];
							CurrentPoly->Next                                 = pTexturePolygonList [ CurrentPoly->TextureIndex ];
							pTexturePolygonList [ CurrentPoly->TextureIndex ] = CurrentPoly;
						}
					}
				}

				currentleaf++;
			}

			PVSPointer++;
		}
		else
		{
			// we have hit a zero so read in the next byte and see how long the run of zeros is
			PVSPointer++;
			BYTE RunLength = *PVSPointer;
			PVSPointer++;
			currentleaf += RunLength * 8;
		}
	}

	// we now have all the polygons in linked lists grouped by texture, so just set each
	// texture and render all the polygons in that textures list
	for ( i = 0; i < NumberOfTextures; i++ )
	{
		if ( pTexturePolygonList [ i ] != NULL )
		{
			if ( lpTextureSurface [ i ] ) 
				m_pD3D->SetTexture ( 0, lpTextureSurface [ i ] );
			else 
				m_pD3D->SetTexture ( 0, NULL );
			
			CurrentPoly = pTexturePolygonList [ i ];
			
			// traverse linked list
			while ( CurrentPoly != NULL )
			{
				m_pD3D->DrawIndexedPrimitiveUP (
													D3DPT_TRIANGLELIST,
													0,
													CurrentPoly->NumberOfVertices,
													CurrentPoly->NumberOfIndices / 3,
													&CurrentPoly->Indices [ 0 ],
													D3DFMT_INDEX16,
													&CurrentPoly->VertexList [ 0 ],
													sizeof ( D3DLVERTEX )
												 );
				
				CurrentPoly = CurrentPoly->Next;
			}
		}
	}

	m_pD3D->SetTexture ( 0, NULL );
}

bool Get_Intersect ( 
						D3DXVECTOR3* linestart,
						D3DXVECTOR3* lineend,
						D3DXVECTOR3* vertex,
						D3DXVECTOR3* normal,
						D3DXVECTOR3* intersection,
						float* percentage
				   )
{
	// return the intersection point between a ray and a plane
	D3DXVECTOR3 direction;
	D3DXVECTOR3 L1;
	float	    linelength;
	float		dist_from_plane;

	direction.x = lineend->x - linestart->x;
	direction.y = lineend->y - linestart->y;
	direction.z = lineend->z - linestart->z;

	linelength = D3DXVec3Dot ( &direction, normal );

	if ( fabsf ( linelength ) < g_EPSILON )
		return false; 

	L1.x = vertex->x - linestart->x;
	L1.y = vertex->y - linestart->y;
	L1.z = vertex->z - linestart->z;

	dist_from_plane = D3DXVec3Dot ( &L1, normal );

	// how far from linestart, intersection is as a percentage of 0 to 1 
	*percentage = dist_from_plane / linelength; 

	// the plane is behind the start of the line or the line does not reach the plane
	if ( *percentage < 0.0f || *percentage > 1.0f )
		return false;

	// add the percentage of the line to line start
	intersection->x = linestart->x + direction.x * ( *percentage );
	intersection->y = linestart->y + direction.y * ( *percentage );
	intersection->z = linestart->z + direction.z * ( *percentage );
	return true;
}

void SplitPolygon ( POLYGON* Poly, PLANE* Plane, POLYGON* FrontSplit, POLYGON* BackSplit )
{
	// this function is used to do ALL the clipping and splitting of portals 
	// and polygons, it takes a polygon and a plane and splits 
	// the polygon into to two seperate polygons, when used for clipping to 
	// a plane, call this function and then simple discard the front or
	// back

	// FRONT and BACK MUST be valid pointers to empty polygon structures as 
	// this function does NOT allocate the memory for them, the reason for
	// this is that this function is used in so many cases and some of them
	// required the front and back already be initialized

	// 50 is used here  we should never really have more points on a portal than this
	D3DLVERTEX		FrontList [ 50 ];
	D3DLVERTEX		BackList  [ 50 ];
	int				FrontCounter = 0;
	int				BackCounter  = 0;

	int				PointLocation [ 50 ];

	int				InFront       = 0;
	int				Behind        = 0;
	int				OnPlane       = 0;
	int				CurrentVertex = 0;
	int				Location      = 0;

	// determine each points location relative to the plane
	for ( int i = 0; i < Poly->NumberOfVertices; i++ )
	{
		Location = ClassifyPoint ( ( D3DXVECTOR3* ) &Poly->VertexList [ i ], Plane );

		if ( Location == CP_FRONT )
		{
			InFront++;
		}
		else if ( Location == CP_BACK )
		{
			Behind++;
		}
		else
		{
			OnPlane++;
		}

		PointLocation [ i ] = Location;
	}

	if ( !InFront )
	{
		memcpy ( BackList, Poly->VertexList, Poly->NumberOfVertices * sizeof ( D3DLVERTEX ) );
		BackCounter = Poly->NumberOfVertices;
	}

	if ( !Behind )
	{
		memcpy ( FrontList, Poly->VertexList, Poly->NumberOfVertices * sizeof ( D3DLVERTEX ) );
		FrontCounter = Poly->NumberOfVertices;
	}

	if ( InFront && Behind )
	{
		for ( i = 0; i < Poly->NumberOfVertices; i++ )
		{
			// store current vertex remembering to MOD with number of vertices
			CurrentVertex = ( i + 1 ) % Poly->NumberOfVertices;

			if ( PointLocation [ i ] == CP_ONPLANE )
			{
				FrontList [ FrontCounter ] = Poly->VertexList [ i ];
				FrontCounter++;

				BackList [ BackCounter ] = Poly->VertexList [ i ];
				BackCounter++;

				continue;
			}

			if ( PointLocation [ i ] == CP_FRONT )
			{
				FrontList [ FrontCounter ] = Poly->VertexList [ i ];
				FrontCounter++;
			}
			else
			{
				BackList [ BackCounter ] = Poly->VertexList [ i ];
				BackCounter++;
			}
			
			// if the next vertex is not causing us to span the plane then continue
			if ( PointLocation [ CurrentVertex ] == CP_ONPLANE || PointLocation [ CurrentVertex ] == PointLocation [ i ] )
				continue;
			
			// Otherwise create the new vertex
			D3DXVECTOR3 IntersectPoint;
			float		percent;

			Get_Intersect ( 
							( D3DXVECTOR3* ) &Poly->VertexList [ i ],
							( D3DXVECTOR3* ) &Poly->VertexList [ CurrentVertex ],
							&Plane->PointOnPlane,
							&Plane->Normal,
							&IntersectPoint,
							&percent
						  );

			// create new vertex and calculate new texture coordinate
			D3DLVERTEX copy;
			float deltax	= Poly->VertexList [ CurrentVertex ].tu - Poly->VertexList [ i ].tu;
			float deltay	= Poly->VertexList [ CurrentVertex ].tv - Poly->VertexList [ i ].tv;
			float texx		= Poly->VertexList [ i ].tu + ( deltax * percent );
			float texy		= Poly->VertexList [ i ].tv + ( deltay * percent );
			copy.x			= IntersectPoint.x; 
			copy.y          = IntersectPoint.y;
			copy.z          = IntersectPoint.z;
			copy.color		= Poly->VertexList [ i ].color;
			copy.tu			= texx;
			copy.tv         = texy;
	
			BackList  [ BackCounter++  ] = copy;			
			FrontList [ FrontCounter++ ] = copy;
		}
	}

	// reserve memory for front and back vertex lists 
	FrontSplit->VertexList		 = new D3DLVERTEX [ FrontCounter ];
	BackSplit->VertexList		 = new D3DLVERTEX [ BackCounter  ];

	FrontSplit->NumberOfVertices = 0;
	BackSplit->NumberOfVertices  = 0;

	// copy over the vertices into the new polys
	FrontSplit->NumberOfVertices = FrontCounter;
	memcpy ( FrontSplit->VertexList, FrontList, FrontCounter * sizeof ( D3DLVERTEX ) );
	
	BackSplit->NumberOfVertices = BackCounter;
	memcpy ( BackSplit->VertexList, BackList, BackCounter * sizeof ( D3DLVERTEX ) );

	BackSplit->NumberOfIndices	= ( BackSplit->NumberOfVertices  - 2 ) * 3;
	FrontSplit->NumberOfIndices	= ( FrontSplit->NumberOfVertices - 2 ) * 3;
	
	// reserve memory for front and back index lists 
	FrontSplit->Indices		= new WORD [ FrontSplit->NumberOfIndices ];
	BackSplit->Indices		= new WORD [ BackSplit->NumberOfIndices  ];

	// fill in the indices
	short IndxBase;

	for ( short loop = 0, v1 = 1, v2 = 2; loop < FrontSplit->NumberOfIndices / 3; loop++, v1 = v2, v2++ )
	{
		IndxBase = loop * 3;

		FrontSplit->Indices [ IndxBase     ] =  0;
		FrontSplit->Indices [ IndxBase + 1 ] = v1;
		FrontSplit->Indices [ IndxBase + 2 ] = v2;
	}

	for ( loop = 0, v1 = 1, v2 = 2; loop < BackSplit->NumberOfIndices / 3; loop++, v1 = v2, v2++ )
	{
		IndxBase = loop * 3;

		BackSplit->Indices [ IndxBase     ] =  0;
		BackSplit->Indices [ IndxBase + 1 ] = v1;
		BackSplit->Indices [ IndxBase + 2 ] = v2;
	}

	// copy extra values
	FrontSplit->Normal = Poly->Normal;
	BackSplit->Normal  = Poly->Normal;

}

POLYGON * AddPolygon(POLYGON* Parent, POLYGON *Poly)
{
	// takes any convex polygon and breaks in into multiple indexed triangle 
	// lists and adds the polygon to a linked list that will be sent to 
	// the BSP compiler

	int		loop;

	POLYGON* Child = new POLYGON;

	memcpy ( Child, Poly, sizeof ( POLYGON ) );

	Child->NumberOfIndices	  = ( Child->NumberOfVertices - 2 ) * 3;
	Child->Next				  = NULL;
	Child->BeenUsedAsSplitter = false;

	// reserve space for index lists
	Child->Indices = new WORD [ Child->NumberOfIndices ];

	// calculate indices
	WORD v0, v1, v2;

	for ( loop = 0; loop < Child->NumberOfIndices / 3; loop++ )
	{
		if ( loop == 0 )
		{
			v0 = 0;
			v1 = 1;
			v2 = 2;
		}
		else
		{
			v1 = v2;
			v2++;
		}
	
		Child->Indices [   loop * 3       ] = v0;
		Child->Indices [ ( loop * 3 ) + 1 ] = v1;
		Child->Indices [ ( loop * 3 ) + 2 ] = v2;
	}

	// generate polygon normal
	D3DXVECTOR3* vec0 = ( D3DXVECTOR3* ) &Child->VertexList [ Child->Indices [ 0 ] ];
	D3DXVECTOR3* vec1 = ( D3DXVECTOR3* ) &Child->VertexList [ Child->Indices [ 1 ] ];
	D3DXVECTOR3* vec2 = ( D3DXVECTOR3* ) &Child->VertexList [ Child->Indices [ Child->NumberOfIndices - 1 ] ];

	D3DXVECTOR3 edge1 = ( *vec1 ) - ( *vec0 );
	D3DXVECTOR3 edge2 = ( *vec2 ) - ( *vec0 );

	D3DXVec3Cross ( &Child->Normal, &edge1, &edge2 );
	D3DXVec3Normalize ( &Child->Normal, &Child->Normal );

	if ( Parent != NULL )
		Parent->Next=Child;

	return Child;
}

long SelectBestSplitter ( POLYGON* PolyList )
{
	// chooses the next polygon in the list to be used as the splitting
	// node of the BSP
	// you can changes the splitting heuristic from Splits * 3 to other values 
	// depending on whether you need fewer splits or a more balanced tree
 
	POLYGON* Splitter     = PolyList;
	POLYGON* CurrentPoly  = NULL;
	POLYGON* SelectedPoly = NULL;
	ULONG	 BestScore    = 1000000;
 
	// traverse list
	while ( Splitter != NULL )
	{
		// if this has not been used as a splitter 
		if ( !Splitter->BeenUsedAsSplitter )
		{
			 // create testing splitter plane
			PLANE SplittersPlane;
			SplittersPlane.Normal		= Splitter->Normal;
			SplittersPlane.PointOnPlane	= *( D3DXVECTOR3* ) &Splitter->VertexList [ 0 ];
		 
			CurrentPoly					= PolyList;
			
			ULONG score, splits, backfaces, frontfaces;
			score = splits = backfaces = frontfaces = 0;
		  
			// test against the other polygons and count the score
			while ( CurrentPoly != NULL )
			{
				int result = ClassifyPoly ( &SplittersPlane, CurrentPoly );

				switch ( result )
				{
					case CP_ONPLANE:
					break;
				
					case CP_FRONT:
						frontfaces++;
					break;

					case CP_BACK:
						backfaces++;
					break;

					case CP_SPANNING:
						splits++;
					break;

					default:
					break;
				}

				CurrentPoly = CurrentPoly->Next;
			}
			
			// tally the score ( modify the splits * n )
			//score = abs ( frontfaces - backfaces ) + ( splits * 3 );
			score = abs ( (long) (frontfaces - backfaces) ) + ( splits * 3 );
			
			if ( score < BestScore )
			{
				BestScore	 = score;
				SelectedPoly = Splitter;
			}
		 }

		Splitter = Splitter->Next;
	}

	if ( SelectedPoly == NULL )
	{
		MessageBox ( NULL, "Failed to select splitter", "Fatal Error - DB Pro BSP / PVS Compiler", MB_ICONSTOP );
		PostQuitMessage ( 0 );
		return -1;
	}

	SelectedPoly->BeenUsedAsSplitter = true;
 
	// return the selected polygons plane
	PlaneArray [ NumberOfPlanes ].PointOnPlane = *( ( D3DXVECTOR3* ) &SelectedPoly->VertexList [ 0 ] );
	PlaneArray [ NumberOfPlanes ].Normal	   = SelectedPoly->Normal;

	IncreaseNumberOfPlanes ( );

	return ( NumberOfPlanes - 1 );
}
 
void CalculateBox ( BOUNDINGBOX* Box, POLYGON* Polylist )
{
	// work out the bounding box of the polygons

	WORD	i;
	POLYGON *PolyPointer = Polylist;

	while ( PolyPointer != NULL )
	{
		for ( i = 0; i < PolyPointer->NumberOfVertices; i++ )
		{
			if ( PolyPointer->VertexList [ i ].x < Box->BoxMin.x ) Box->BoxMin.x = PolyPointer->VertexList [ i ].x;
			if ( PolyPointer->VertexList [ i ].y < Box->BoxMin.y ) Box->BoxMin.y = PolyPointer->VertexList [ i ].y;
			if ( PolyPointer->VertexList [ i ].z < Box->BoxMin.z ) Box->BoxMin.z = PolyPointer->VertexList [ i ].z;
			
			if ( PolyPointer->VertexList [ i ].x > Box->BoxMax.x ) Box->BoxMax.x = PolyPointer->VertexList [ i ].x;
			if ( PolyPointer->VertexList [ i ].y > Box->BoxMax.y ) Box->BoxMax.y = PolyPointer->VertexList [ i ].y;
			if ( PolyPointer->VertexList [ i ].z > Box->BoxMax.z ) Box->BoxMax.z = PolyPointer->VertexList [ i ].z; 
		}

		PolyPointer = PolyPointer->Next;
	}
}

BOOL LineOfSight ( D3DXVECTOR3* Start, D3DXVECTOR3* End, long Node )
{
	// checks if there is a clear line of sight between two points be 
	// recursing the tree and making sure that none of the line segments
	// end up in solid space

	float		temp;
	D3DXVECTOR3 intersection;

	NODE*	CurrNode = &NodeArray  [ Node ];
	PLANE*	Plane	 = &PlaneArray [ CurrNode->Plane ];

	int PointA = ClassifyPoint ( Start,	Plane );
	int PointB = ClassifyPoint ( End,	Plane );

	if ( PointA == CP_ONPLANE && PointB == CP_ONPLANE )
	{
		if ( CurrNode->IsLeaf == 0 )
		{
			return LineOfSight ( Start, End, CurrNode->Front );
		}
		else 
		{
			// this is a front leaf - are always empty so this is empty space
			return TRUE;		
		}
	}

	if ( PointA == CP_FRONT && PointB == CP_BACK )
	{
		if ( CurrNode->Back == -1 )
			return FALSE; 

		Get_Intersect ( Start, End, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp );
		
		if ( CurrNode->IsLeaf == 0 )
		{
			return LineOfSight ( Start, &intersection, CurrNode->Front ) && LineOfSight ( End, &intersection, CurrNode->Back );
		}
		else 
		{
			return TRUE && LineOfSight ( End, &intersection, CurrNode->Back );
		}
	}

	if ( PointA == CP_BACK && PointB == CP_FRONT )
	{
		if ( CurrNode->Back == -1 )
			return FALSE;

		Get_Intersect ( Start, End, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp );
 
		if ( CurrNode->IsLeaf == 0 )
		{
			return LineOfSight ( End, &intersection, CurrNode->Front ) && LineOfSight ( Start, &intersection, CurrNode->Back );
		}
		else
		{
			return TRUE && LineOfSight ( Start, &intersection, CurrNode->Back );
		}
	}

	// if we get here one of the points is on the plane
	if ( PointA == CP_FRONT || PointB == CP_FRONT )
	{
		if ( CurrNode->IsLeaf == 0 )
		{
			return LineOfSight ( Start, End, CurrNode->Front );
		}
		else 
		{
			return TRUE;
		}
	}
	else 
	{
		if ( CurrNode->Back == -1 )
		{
			return FALSE;
		}
		else 
		{
			return LineOfSight ( Start, End, CurrNode->Back );
		}
	}

	return TRUE;
}
  
BOOL CollideSphere ( D3DXVECTOR3* SphereCenter, float SphereRadius, long Node )
{
	// passes a sphere through the tree and checks for any parts in solid space
 
	float		temp;
	int			PointA;
	int			PointB;
	D3DXVECTOR3 intersection;
	D3DXVECTOR3	SphereRadiusEnd;

	PLANE* Plane	= &PlaneArray [ NodeArray [ Node ].Plane ];
	NODE*  CurrNode	= &NodeArray  [ Node ];

	PointA = ClassifyPoint ( SphereCenter, Plane );

	if ( PointA == CP_BACK )
	{
		SphereRadiusEnd = *SphereCenter + ( Plane->Normal * SphereRadius );
	}
	else
	{
		SphereRadiusEnd = *SphereCenter + ( -Plane->Normal * SphereRadius );
	}

	PointB = ClassifyPoint ( &SphereRadiusEnd, Plane );

	if ( PointA == CP_ONPLANE && PointB == CP_ONPLANE )
	{
 		if ( CurrNode->IsLeaf == 0 )
		{
			// this is not a leaf so recurse
			return CollideSphere ( SphereCenter, SphereRadius, CurrNode->Front );
		}
		else
		{
			// this is a front leaf - are always empty so this is empty space
			return FALSE;
		}
	}

	// spanning front to back
	if ( PointA == CP_FRONT && PointB == CP_BACK )
	{
		if ( CurrNode->Back == -1 )
			return TRUE;

		Get_Intersect ( SphereCenter, &SphereRadiusEnd, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp );

		if ( CurrNode->IsLeaf == 0 )
		{
			return CollideSphere (
									SphereCenter,
									D3DXVec3Length ( &( intersection - *SphereCenter ) ), CurrNode->Front )
									||
									CollideSphere (
													&SphereRadiusEnd,
													D3DXVec3Length ( &( intersection - SphereRadiusEnd ) ),
													CurrNode->Back
								 );
		}
		else 
		{
			return CollideSphere ( &SphereRadiusEnd, D3DXVec3Length ( &( intersection - SphereRadiusEnd ) ), CurrNode->Back );
		}
	}

	// spanning back to front
	if ( PointA == CP_BACK && PointB == CP_FRONT )
	{
		if ( CurrNode->Back == -1 )
			return TRUE;
			
		Get_Intersect ( &SphereRadiusEnd, SphereCenter, &Plane->PointOnPlane, &Plane->Normal, &intersection, &temp );
 
		if ( CurrNode->IsLeaf == 0 )
		{
			return CollideSphere ( 
									&SphereRadiusEnd,
									D3DXVec3Length ( &( intersection - SphereRadiusEnd ) ), CurrNode->Front )
									||
									CollideSphere ( 
													SphereCenter,
													D3DXVec3Length ( &( intersection - *SphereCenter ) ), 
													CurrNode->Back
												  );
		}
		else
		{
			return CollideSphere ( SphereCenter, D3DXVec3Length ( &( intersection - *SphereCenter ) ), CurrNode->Back );
		}
	}

	// if we get here one of the points is on the plane
	if ( PointA == CP_FRONT || PointB == CP_FRONT )
	{
		if ( CurrNode->IsLeaf == 0 )
		{
			return CollideSphere ( SphereCenter, SphereRadius, CurrNode->Front );
		}
		else 
		{
			return FALSE;
		}
	}
	else 
	{
		if ( CurrNode->Back == -1 )
		{
			return TRUE;
		}
		else
		{
			return CollideSphere ( SphereCenter, SphereRadius, CurrNode->Back );
		}
	}

	return FALSE;
}