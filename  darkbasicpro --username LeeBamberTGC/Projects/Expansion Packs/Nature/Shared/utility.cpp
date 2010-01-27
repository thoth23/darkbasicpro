////////////////////////////////////////////////////////////////////
// INFORMATION /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

/*
	this file contains utility functions that operate on objects
	and meshes
*/

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// INCLUDES ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#include "utility.h"

#include "..\..\..\..\Dark Basic Pro SDK\Shared\Core\globstruct.h"

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// FUNCTIONS ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#ifdef DARKSDK_COMPILE
	sObject* GetObject ( int iID );
	
#endif

//lee - 130206 - 
//extern GlobStruct*				g_pGlobal;
extern GlobStruct*				g_pGlobForTerrain;

sObject* GetObjectData ( int iObject )
{
	
	// returns a pointer to the object, first we need to load
	// the basic 3D DLL, then we can use a function pointer and
	// get the address of the "GetObject" function, then this
	// function is called and gives us a pointer to the object

	// type defintion for the function pointer
	typedef sObject* ( *GetObjectPFN ) ( int );

	// variable declarations, handle to DLL and function pointer
	HMODULE			hDLL		= NULL;
	GetObjectPFN	GetObjectEx	= NULL;

	#ifndef DARKSDK_COMPILE
		// load the basic 3D DLL
		//hDLL = LoadLibrary ( "DBProBasic3DDebug.dll" );

		// check we were able to load the DLL
		//if ( !hDLL )
		//	return NULL;

		// get the function address
		//GetObjectEx = ( GetObjectPFN ) GetProcAddress ( hDLL, "?GetObjectA@@YAPAUsObject@@H@Z" );
		GetObjectEx = ( GetObjectPFN ) GetProcAddress ( g_pGlobForTerrain->g_Basic3D, "?GetObjectA@@YAPAUsObject@@H@Z" );

		// check the function pointer is valid
		//if ( !GetObject )
		//	return NULL;
	#else
		GetObjectEx = GetObject;
	#endif

	

	// get pointer to the object
	sObject* pObject = GetObjectEx ( iObject );

		//FreeLibrary ( hDLL );

		return pObject;
}


bool GetFVFValueOffsetMapEx ( DWORD dwFVF, sOffsetMap* psOffsetMap )
{
	// get the offset map for the given flexible vertex format

	// check the offset map pointer is valid
	SAFE_MEMORY ( psOffsetMap );

	// variable declarations
	int		iOffset    = 0;									// offset
	int		iPosition  = 0;									// position
	DWORD	dwFVFSize  = D3DXGetFVFVertexSize ( dwFVF );	// get fvf size
	DWORD	dwTexCount = 0;									// texture count

	// clear the offset map
	memset ( psOffsetMap, 0, sizeof ( sOffsetMap ) );

	// check for x, y, z
	if ( dwFVF & D3DFVF_XYZ )
	{
		psOffsetMap->dwX  = iOffset + 0;
		psOffsetMap->dwY  = iOffset + 1;
		psOffsetMap->dwZ  = iOffset + 2;
		iOffset          += 3;
	}

	// check for x, y, z, rhw
	if ( dwFVF & D3DFVF_XYZRHW )
	{
		psOffsetMap->dwRWH  = iOffset + 0;
		iOffset            += 1;
	}

	// check for normals
	if ( dwFVF & D3DFVF_NORMAL )
	{
		psOffsetMap->dwNX  = iOffset + 0;
		psOffsetMap->dwNY  = iOffset + 1;
		psOffsetMap->dwNZ  = iOffset + 2;
		iOffset           += 3;
	}

	// check for point size
	if ( dwFVF & D3DFVF_PSIZE )
	{
		psOffsetMap->dwPointSize  = iOffset + 0;
		iOffset                  += 1;
	}

	// diffuse
	if ( dwFVF & D3DFVF_DIFFUSE )
	{
		psOffsetMap->dwDiffuse  = iOffset + 0;
		iOffset                += 1;
	}

	// and specular
	if ( dwFVF & D3DFVF_SPECULAR )
	{
		psOffsetMap->dwSpecular  = iOffset + 0;
		iOffset                 += 1;
	}

	// finally texture coordinates
	if ( dwFVF & D3DFVF_TEX1 )
	{
		psOffsetMap->dwTU [ 0 ] = iOffset + 0;
		psOffsetMap->dwTV [ 0 ] = iOffset + 1;

		iOffset += 2;
	}
	
	// calculate byte offset
	psOffsetMap->dwByteSize = sizeof ( DWORD ) * iOffset;

	// store number of offsets
	psOffsetMap->dwSize = iOffset;

	// check if matches byte size of actual FVF
	if ( dwFVFSize != psOffsetMap->dwByteSize )
		return false;
	
	return true;
}


/*
bool GetFVFValueOffsetMapEx ( DWORD dwFVF, sOffsetMap* psOffsetMap )
{
	// get the offset map for the given flexible vertex format

	// check the offset map pointer is valid
	SAFE_MEMORY ( psOffsetMap );

	// variable declarations
	int		iOffset    = 0;									// offset
	int		iPosition  = 0;									// position
	DWORD	dwFVFSize  = D3DXGetFVFVertexSize ( dwFVF );	// get fvf size
	DWORD	dwTexCount = 0;									// texture count

	// clear the offset map
	memset ( psOffsetMap, 0, sizeof ( sOffsetMap ) );

	// check for x, y, z
	if ( dwFVF & D3DFVF_XYZ )
	{
		psOffsetMap->dwX  = iOffset + 0;
		psOffsetMap->dwY  = iOffset + 1;
		psOffsetMap->dwZ  = iOffset + 2;
		iOffset          += 3;
	}

	// check for x, y, z, rhw
	if ( dwFVF & D3DFVF_XYZRHW )
	{
		psOffsetMap->dwRWH  = iOffset + 0;
		iOffset            += 1;
	}

	// check for normals
	if ( dwFVF & D3DFVF_NORMAL )
	{
		psOffsetMap->dwNX  = iOffset + 0;
		psOffsetMap->dwNY  = iOffset + 1;
		psOffsetMap->dwNZ  = iOffset + 2;
		iOffset           += 3;
	}

	// check for point size
	if ( dwFVF & D3DFVF_PSIZE )
	{
		psOffsetMap->dwPointSize  = iOffset + 0;
		iOffset                  += 1;
	}

	// diffuse
	if ( dwFVF & D3DFVF_DIFFUSE )
	{
		psOffsetMap->dwDiffuse  = iOffset + 0;
		iOffset                += 1;
	}

	// and specular
	if ( dwFVF & D3DFVF_SPECULAR )
	{
		psOffsetMap->dwSpecular  = iOffset + 0;
		iOffset                 += 1;
	}

	// finally texture coordinates
	if ( dwFVF & D3DFVF_TEX1 )
	{
		psOffsetMap->dwTU [ 0 ] = iOffset + 0;
		psOffsetMap->dwTV [ 0 ] = iOffset + 1;

		iOffset += 2;
	}
	
	// calculate byte offset
	psOffsetMap->dwByteSize = sizeof ( DWORD ) * iOffset;

	// store number of offsets
	psOffsetMap->dwSize = iOffset;

	// check if matches byte size of actual FVF
	if ( dwFVFSize != psOffsetMap->dwByteSize )
		return false;
	
	return true;
}
*/

bool SetupVertex ( DWORD dwFVF, BYTE* pVertex, int iOffset, float x, float y, float z, float nx, float ny, float nz, float tu, float tv, float tu1, float tv1, DWORD dwDiffuse )
{
	// setup a standard vertex

	// check the memory pointer is valid
	SAFE_MEMORY ( pVertex );

	// get the offset map
	sOffsetMap	offsetMap;

	GetFVFValueOffsetMapEx ( dwFVF, &offsetMap );

	// check we have position data
	if ( dwFVF & D3DFVF_XYZ )
	{
		*( ( float* ) pVertex + offsetMap.dwX + ( offsetMap.dwSize * iOffset ) ) = x;
		*( ( float* ) pVertex + offsetMap.dwY + ( offsetMap.dwSize * iOffset ) ) = y;
		*( ( float* ) pVertex + offsetMap.dwZ + ( offsetMap.dwSize * iOffset ) ) = z;
	}

	// check we have normals
	if ( dwFVF & D3DFVF_NORMAL )
	{
		*( ( float* ) pVertex + offsetMap.dwNX + ( offsetMap.dwSize * iOffset ) ) = nx;
		*( ( float* ) pVertex + offsetMap.dwNY + ( offsetMap.dwSize * iOffset ) ) = ny;
		*( ( float* ) pVertex + offsetMap.dwNZ + ( offsetMap.dwSize * iOffset ) ) = nz;
	}
	
	
	// check that we have texture coordinates
	if ( dwFVF & D3DFVF_TEX1 )
	{
		*( ( float* ) pVertex + offsetMap.dwTU [ 0 ] + ( offsetMap.dwSize * iOffset ) ) = tu;
		*( ( float* ) pVertex + offsetMap.dwTV [ 0 ] + ( offsetMap.dwSize * iOffset ) ) = tv;
	}
	
	if ( dwFVF & D3DFVF_TEX2 )
	{
		*( ( float* ) pVertex + offsetMap.dwTU [ 0 ] + ( offsetMap.dwSize * iOffset ) ) = tu;
		*( ( float* ) pVertex + offsetMap.dwTV [ 0 ] + ( offsetMap.dwSize * iOffset ) ) = tv;

		*( ( float* ) pVertex + offsetMap.dwTU [ 1 ] + ( offsetMap.dwSize * iOffset ) ) = tu1;
		*( ( float* ) pVertex + offsetMap.dwTV [ 1 ] + ( offsetMap.dwSize * iOffset ) ) = tv1;
	}


		// check that we have texture coordinates
	if ( dwFVF & D3DFVF_DIFFUSE )
	{
		//( ( DWORD* ) pVertex + offsetMap.dwDiffuse = dwDiffuse;

		*( ( DWORD* ) pVertex + offsetMap.dwDiffuse + ( offsetMap.dwSize * iOffset ) ) = dwDiffuse;
	}

	return true;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////