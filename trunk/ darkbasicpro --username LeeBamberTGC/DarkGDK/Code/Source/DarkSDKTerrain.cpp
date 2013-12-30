/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TERRAIN COMMANDS /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKTerrain.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void  SetupTerrain				( void );

void  LoadTerrain               ( DWORD dwFilename, int iID );
void  SaveTerrain               ( DWORD dwFilename, int iID );

void  MakeObjectTerrain			( int iID );
void  BuildTerrain				( int iID );
int   GetTerrainExist         ( int iID );   // patrick - 291213 r111 - New function.
void  DeleteTerrain				( int iID );
void  DeleteTerrain				( int iID, int iUserData );

void  SetTerrainScale			( int iID, float fXScale, float fYScale, float fZScale );
void  SetTerrainSplit			( int iID, int iSplit );
void  SetTerrainTiling			( int iID, int iTile );
void  SetTerrainHeightMap		( int iID, DWORD dwHeightMap );
void  SetTerrainTexture			( int iID, int iBase, int iDetail );
void  SetTerrainLight			( int iID, float fX, float fY, float fZ, float fRed, float fGreen, float fBlue, float fScale );

DWORD GetTerrainGroundHeight    ( int iID, float fX, float fZ );
float GetTerrainXSizeEx         ( int iID );
float GetTerrainZSizeEx         ( int iID );

void  UpdateTerrain             ( void );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbSetupTerrain ( void )
{
	SetupTerrain ( );
}

void dbLoadTerrain ( char* dwFilename, int iID )
{
	LoadTerrain ( ( DWORD ) dwFilename, iID );
}

void dbSaveTerrain ( char* dwFilename, int iID )
{
	SaveTerrain ( ( DWORD ) dwFilename, iID );
}

void dbMakeObjectTerrain ( int iID )
{
	MakeObjectTerrain ( iID );
}

void dbBuildTerrain ( int iID )
{
	BuildTerrain ( iID );
}

// patrick - 291213 r111 - There had been no dbTerrainExist() function.
int dbTerrainExist ( int iID )
{
   return GetTerrainExist ( iID );
}

void dbDeleteTerrain ( int iID )
{
	DeleteTerrain ( iID );
}

void dbDeleteTerrain ( int iID, int iUserData )
{
    DeleteTerrain ( iID, iUserData );
}

void dbSetTerrainScale ( int iID, float fXScale, float fYScale, float fZScale )
{
	SetTerrainScale ( iID, fXScale, fYScale, fZScale );
}

void dbSetTerrainSplit ( int iID, int iSplit )
{
	SetTerrainSplit ( iID, iSplit );
}

void dbSetTerrainTiling ( int iID, int iTile )
{
	SetTerrainTiling ( iID, iTile );
}

void dbSetTerrainHeightMap ( int iID, char* dwHeightMap )
{
	SetTerrainHeightMap ( iID, ( DWORD ) dwHeightMap );
}

void dbSetTerrainTexture ( int iID, int iBase, int iDetail )
{
	SetTerrainTexture ( iID, iBase, iDetail );
}

void dbSetTerrainLight ( int iID, float fX, float fY, float fZ, float fRed, float fGreen, float fBlue, float fScale )
{
	SetTerrainLight ( iID, fX, fY, fZ, fRed, fGreen, fBlue, fScale );
}

float dbGetTerrainGroundHeight ( int iID, float fX, float fZ )
{
	DWORD dwReturn = GetTerrainGroundHeight ( iID, fX, fZ );
	
	return *( float* ) &dwReturn;
}

float dbGetTerrainXSize ( int iID )
{
	return GetTerrainXSizeEx ( iID );
}

float dbGetTerrainZSize ( int iID )
{
	return GetTerrainZSizeEx ( iID );
}

void dbUpdateTerrain ( void )
{
	UpdateTerrain ( );
}

void  dbSetTerrainHeightmap	( int iID, char* dwHeightMap )
{
	dbSetTerrainHeightMap ( iID, dwHeightMap );
}
