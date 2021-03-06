
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

#include "..\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\dbofile.h"

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// FUNCTIONS ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

sObject* GetObjectData ( int iObject );
bool     GetFVFOffsetMap ( sMesh* pMesh, sOffsetMap* psOffsetMap );
bool GetFVFValueOffsetMapEx ( DWORD dwFVF, sOffsetMap* psOffsetMap );
bool     SetupVertex ( DWORD dwFVF, BYTE* pVertex, int iOffset, float x, float y, float z, float nx, float ny, float nz,float tu, float tv, float tu1, float tv1, DWORD dwDiffuse );

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////