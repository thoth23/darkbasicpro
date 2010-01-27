
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKMatrix.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void	MakeEx						( int iID, float fWidth, float fDepth, int iXSegmented, int iZSegmented );
void	Delete						( int iID );

void	PrepareTexture				( int iID, int iImage, int iAcross, int iDown );

//void	PositionEx					( int iID, float fX, float fY, float fZ );
void dbtMatrixPosition  ( int iID, float fX, float fY, float fZ );

void	Fill						( int iID, float fHeight, int iTile );
void	Randomize					( int iID, int iHeight );
void	SetTransparencyOn			( int iID );
void	SetTransparencyOff			( int iID );
void	SetWireframeOn				( int iID );
void	SetWireframeOff				( int iID );
void	GetPositionVector3 			( int iVector, int iID );
void	SetTexture					( int iID, int iTextureMode, int iMipGen );
void	SetEx						( int iID, int bWireframe, int bTransparency, int bCull, int iFilter, int bLight, int bFog, int bAmbient );
void	SetTrim						( int iID, float fTrimX, float fTrimY );
void	SetPriority					( int iID, int iPriority );
void	ShiftDown					( int iID );
void	ShiftLeft					( int iID );
void	ShiftRight					( int iID );
void	ShiftUp						( int iID );
void	SetNormal					( int iID, int iX, int iZ, float fX, float fY, float fZ );
void	SetHeight					( int iID, int iX, int iZ, float fHeight );
void	SetTile						( int iID, int iX, int iZ, int iTile );
void	Apply						( int iID );

int		GetExistEx        			( int iID );
*/
/*
DWORD	GetPositionXEx				( int iID );
DWORD	GetPositionYEx				( int iID );
DWORD	GetPositionZEx				( int iID );
*/

/*
DWORD dbtMatrixGetXPositionEx ( int iID );
DWORD dbtMatrixGetYPositionEx ( int iID );
DWORD dbtMatrixGetZPositionEx ( int iID );

int		GetTileCountEx    			( int iID );
int		GetTilesExistEx   			( int iID );
int		GetWireframeEx				( int iID );
DWORD	GetGroundHeightEx			( int iID, float fX, float fZ );
DWORD	GetHeightEx					( int iID, int iX, int iZ );

void	UpdateMatrix				( void );
void	UpdateLastMatrix			( void );
void	ConstructorMatrix			( HINSTANCE hSetup, HINSTANCE hImage );
void	DestructorMatrix			( void );
void	SetErrorHandlerMatrix		( LPVOID pErrorHandlerPtr );
void	PassCoreDataMatrix			( LPVOID pGlobPtr );
void	RefreshD3DMatrix			( int iMode );

bool	Make						( int iID, int iWidth, int iDepth, int iXSegmented, int iZSegmented );
void	SetTransparency				( int iID, bool bFlag );	
void	SetCull						( int iID, bool bCull );	
void	SetFilter					( int iID, int  iFilter );	
void	SetLight					( int iID, bool bLight );	
void	SetFog						( int iID, bool bFog );	
void	SetAmbient					( int iID, bool bAmbient );
void	Set							( int iID, bool bWireframe, bool bTransparency, bool bCull );
void	Set							( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter );
void	Set							( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter, bool bLight );
void	Set							( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter, bool bLight, bool bFog );
void	Set							( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter, bool bLight, bool bFog, bool bAmbient );
void	SetTransparency				( int iID, bool bFlag );
void	SetWireframe				( int iID, bool bFlag );	
//void	SetSphereMap				( int iID, int iSphereImage );
void	SetCubeMap					( int iID, int iCubeImage );
void	SetBumpMap					( int iID, int iEnvironmentImage, int iBumpMap );
int		GetWidth        			( int iID );
int		GetDepth        			( int iID );
int		GetXSegments    			( int iID );
int		GetZSegments    			( int iID );
int		GetTilesAcross  			( int iID );
int		GetTilesDown    			( int iID );
int		GetFilter       			( int iID );
int		GetGhostMode    			( int iID );
bool	GetVisible      			( int iID );
bool	GetWireframe    			( int iID );
bool	GetLight        			( int iID );
bool	GetCull         			( int iID );
bool	GetTransparency 			( int iID );
bool	GetZDepth       			( int iID );
bool	GetFog          			( int iID );
bool	GetAmbient      			( int iID );
bool	GetGhost        			( int iID );
bool	GetLock         			( int iID );
void	SetGlobalColor  			( int iID, int iA, int iR, int iG, int iB );
void	SetTileColor    			( int iID, int iA, int iR, int iG, int iB );
bool	LockVertexData   			( int iID );	
bool	UnlockVertexData 			( int iID );	
bool	LockIndexData    			( int iID );	
bool	UnlockIndexData  			( int iID );
void*	GetVertexData    			( int iID );
void*	GetIndexData     			( int iID );
int		GetNumVertices   			( int iID );
int		GetNumIndices    			( int iID );
void	SetWireframeOff				( int iID );
void	SetPositionVector3 			( int iID, int iVector );
void	SetRotationVector3 			( int iID, int iVector );
void	GetRotationVector3			( int iVector, int iID );

////

void dbtMatrixPosition  ( int iID, float fX, float fY, float fZ );
void dbtMatrixScale     ( int iID, float fX, float fY, float fZ );
void dbtMatrixRotate    ( int iID, float fX, float fY, float fZ );
void dbtMatrixMove      ( int iID, float fStep );
void dbtMatrixPoint     ( int iID, float fX, float fY, float fZ );
void dbtMatrixMoveDown  ( int iID, float fStep );
void dbtMatrixMoveLeft  ( int iID, float fStep );
void dbtMatrixMoveRight ( int iID, float fStep );
void dbtMatrixMoveUp    ( int iID, float fStep );
void dbtMatrixXRotate   ( int iID, float fX );
void dbtMatrixYRotate   ( int iID, float fY );
void dbtMatrixZRotate   ( int iID, float fZ );
void dbtMatrixTurnLeft  ( int iID, float fAngle );
void dbtMatrixTurnRight ( int iID, float fAngle );
void dbtMatrixPitchUp   ( int iID, float fAngle );
void dbtMatrixPitchDown ( int iID, float fAngle );
void dbtMatrixRollLeft  ( int iID, float fAngle );
void dbtMatrixRollRight ( int iID, float fAngle );

/////
*/


/*
void  MakeMatrix					( int iID, float fWidth, float fDepth,  int iXSegmented, int iZSegmented );
void  DeleteMatrix					( int iID );
void  FillMatrix					( int iID, float fHeight, int iTile );
void  GhostMatrixOn					( int iID );
void  GhostMatrixOff				( int iID );
void  PositionMatrix				( int iID, float fX, float fY, float fZ );
void  PrepareMatrixTexture			( int iID, int iImage, int iAcross, int iDown );
void  RandomizeMatrix				( int iID, int iHeight );							// randomize height of a matrix
void  SetMatrixHeight				( int iID, int iX, int iZ, float fHeight );			// set the height of a block
void  SetMatrixNormal				( int iID, int iX, int iZ, float fX, float fY, float fZ );
void  SetMatrixTexture				( int iID, int iTextureMode, int iMipGen );
void  SetMatrixTile					( int iID, int iX, int iZ, int iTile );				// set the texture on a block
void  SetMatrixWireframeOn			( int iID );
void  GhostMatrixOn					( int iID, int iMode );
void  SetMatrixWireframeOff			( int iID );
void  SetMatrix						( int iID, int bWireframe, int bTransparency, int bCull, int iFilter, int bLight, int bFog, int bAmbient );
void  ShiftMatrixUp					( int iID );										// shift matrix up and wrap around
void  ShiftMatrixDown				( int iID );										// shift matrix down and wrap around
void  ShiftMatrixLeft				( int iID );										// shift matrix left and wrap around
void  ShiftMatrixRight				( int iID );										// shift matrix right and wrap around
void  UpdateMatrix					( int iID );

void  SetMatrixTrim					( int iID, float fTrimX, float fTrimY );
void  SetMatrixPriority				( int iID, int iPriority );

void  PositionMatrix				( int iID, int iVector );
void  SetVector3ToMatrixPosition	( int iVector, int iID );

// DBV1 expressions
DWORD MatrixPositionX				( int iID );
DWORD MatrixPositionY				( int iID );
DWORD MatrixPositionZ				( int iID );
DWORD GetGroundHeight				( int iID, float fX, float fZ );					// get y height at tile
DWORD GetMatrixHeight				( int iID, int iX, int iZ );						// get height at tile
int   MatrixExist					( int iID );										// does matrix exist
int   MatrixTileCount			    ( int iID );										// get number of texture tiles
int   MatrixTilesExist				( int iID );										// do tiles exist
int   MatrixWireframeState			( int iID );


	void UpdateMatrix      ( void );
	void UpdateLastMatrix  ( void );
	void ConstructorMatrix ( HINSTANCE hSetup, HINSTANCE hImage );
	void DestructorMatrix  ( void );
	void SetErrorHandlerMatrix ( LPVOID pErrorHandlerPtr );
	void PassCoreDataMatrix( LPVOID pGlobPtr );
	void RefreshD3DMatrix ( int iMode );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbMakeMatrix ( int iID, float fWidth, float fDepth, int iXSegmented, int iZSegmented )
{
	MakeMatrix (  iID,  fWidth, fDepth,  iXSegmented, iZSegmented );
}

void dbDeleteMatrix ( int iID )
{
	DeleteMatrix ( iID );
}

void dbPrepareMatrixTexture	( int iID, int iImage, int iAcross, int iDown )
{
	PrepareMatrixTexture ( iID, iImage, iAcross, iDown );
}

void dbPositionMatrix ( int iID, float fX, float fY, float fZ )
{
	PositionMatrix ( iID, fX, fY, fZ );
}

void dbFillMatrix ( int iID, float fHeight, int iTile )
{
	FillMatrix ( iID, fHeight, iTile );
}

void dbRandomizeMatrix ( int iID, int iHeight )
{
	RandomizeMatrix ( iID, iHeight );
}

void dbGhostMatrixOn ( int iID )
{
	GhostMatrixOn ( iID );
}

void dbGhostMatrixOff ( int iID )
{
	GhostMatrixOff ( iID );
}

void dbSetMatrixWireframeOn	( int iID )
{
	SetMatrixWireframeOn ( iID );
}

void dbSetMatrixWireframeOff ( int iID )
{
	SetMatrixWireframeOff	(  iID );
}

void dbSetVector3ToMatrixPosition ( int iVector, int iID )
{
	SetVector3ToMatrixPosition (  iVector,  iID );
}

void dbSetMatrixTexture ( int iID, int iTextureMode, int iMipGen )
{
	SetMatrixTexture ( iID,  iTextureMode,  iMipGen );
}

void dbSetMatrix ( int iID, int bWireframe, int bTransparency, int bCull, int iFilter, int bLight, int bFog, int bAmbient )
{
	SetMatrix ( iID,  bWireframe,  bTransparency,  bCull,  iFilter,  bLight,  bFog,  bAmbient );
}

void dbSetMatrixTrim ( int iID, float fTrimX, float fTrimY )
{
	SetMatrixTrim (  iID,  fTrimX,  fTrimY );
}

void dbSetMatrixPriority ( int iID, int iPriority )
{
	SetMatrixPriority (  iID,  iPriority );
}

void dbShiftMatrixDown ( int iID )
{
	ShiftMatrixDown ( iID );
}
 
void dbShiftMatrixLeft ( int iID )
{
	ShiftMatrixLeft ( iID );
}
 
void dbShiftMatrixRight ( int iID )
{
	ShiftMatrixRight ( iID );
}

void dbShiftMatrixUp ( int iID )
{
	ShiftMatrixUp ( iID );
}

void dbSetMatrixNormal ( int iID, int iX, int iZ, float fX, float fY, float fZ )
{
	SetMatrixNormal (  iID, iX, iZ,  fX,  fY,  fZ );
}

void dbSetMatrixHeight ( int iID, int iX, int iZ, float fHeight )
{
	SetMatrixHeight (  iID,  iX,  iZ,  fHeight );
}

void dbSetMatrixTile ( int iID, int iX, int iZ, int iTile )
{
	SetMatrixTile ( iID,  iX,  iZ,  iTile );
}

void dbUpdateMatrix ( int iID )
{
	UpdateMatrix ( iID );
}

int dbMatrixExist ( int iID )
{
	return MatrixExist (  iID );
}
*/

/*
float dbMatrixPositionX	( int iID )
{
	DWORD dwReturn = GetPositionXEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbMatrixPositionY	( int iID )
{
	DWORD dwReturn = GetPositionYEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbMatrixPositionZ	( int iID )
{
	DWORD dwReturn = GetPositionZEx ( iID );
	
	return *( float* ) &dwReturn;
}
*/

/*
float dbMatrixPositionX	( int iID )
{
	DWORD dwReturn = MatrixPositionX ( iID );
	
	return *( float* ) &dwReturn;
}

float dbMatrixPositionY	( int iID )
{
	DWORD dwReturn = MatrixPositionY ( iID );
	
	return *( float* ) &dwReturn;
}

float dbMatrixPositionZ	( int iID )
{
	DWORD dwReturn = MatrixPositionZ ( iID );
	
	return *( float* ) &dwReturn;
}

int dbMatrixTileCount ( int iID )
{
	return MatrixTileCount ( iID );
}
 
int dbMatrixTilesExist ( int iID )
{
	return MatrixTilesExist ( iID );
}
 
int dbMatrixWireframe ( int iID )
{
	return MatrixWireframeState (  iID );
}

float dbMatrixGroundHeight	( int iID, float fX, float fZ )
{
	DWORD dwReturn = GetGroundHeight	( iID,  fX,  fZ );
	
	return *( float* ) &dwReturn;
}

float dbMatrixHeight ( int iID, int iX, int iZ )
{
	DWORD dwReturn = GetMatrixHeight	( iID,  iX,  iZ );
	
	return *( float* ) &dwReturn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbUpdateMatrix ( void )
{
	UpdateMatrix ( );
}

void dbUpdateLastMatrix ( void )
{
	UpdateLastMatrix ( );
}

void dbConstructorMatrix ( HINSTANCE hSetup, HINSTANCE hImage )
{
	ConstructorMatrix ( hSetup, hImage );
}

void dbDestructorMatrix ( void )
{
	DestructorMatrix ( );
}

void dbSetErrorHandlerMatrix ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerMatrix ( pErrorHandlerPtr );
}

void dbPassCoreDataMatrix( LPVOID pGlobPtr )
{
	PassCoreDataMatrix( pGlobPtr );
}

void dbRefreshD3DMatrix ( int iMode )
{
	RefreshD3DMatrix ( iMode );
}
*/

/*
bool dbMake ( int iID, int iWidth, int iDepth, int iXSegmented, int iZSegmented )
{
	return Make (  iID,  iWidth, iDepth, iXSegmented, iZSegmented );
}

void dbSetTransparency ( int iID, bool bFlag )
{
	SetTransparency ( iID, bFlag );
}
	
void dbSetCull ( int iID, bool bCull )
{
	SetCull ( iID, bCull );
}
	
void dbSetFilter ( int iID, int iFilter )
{
	SetFilter ( iID, iFilter );
}
	
void dbSetLight ( int iID, bool bLight )
{
	SetLight ( iID, bLight );
}
	
void dbSetFog ( int iID, bool bFog )
{
	SetFog ( iID, bFog );
}
	
void dbSetAmbient ( int iID, bool bAmbient )
{
	SetAmbient ( iID, bAmbient );
}
	

void dbSet ( int iID, bool bWireframe, bool bTransparency, bool bCull )
{
	Set ( iID, bWireframe, bTransparency, bCull );
}

void dbSet ( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter )
{
	Set ( iID, bWireframe, bTransparency, bCull, iFilter );
}

void dbSet ( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter, bool bLight )
{
	Set ( iID, bWireframe, bTransparency, bCull, iFilter, bLight );
}

void dbSet ( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter, bool bLight, bool bFog )
{
	Set ( iID, bWireframe, bTransparency, bCull, iFilter, bLight, bFog );
}

void dbSet ( int iID, bool bWireframe, bool bTransparency, bool bCull, int iFilter, bool bLight, bool bFog, bool bAmbient )
{
	Set ( iID, bWireframe, bTransparency, bCull, iFilter, bLight, bFog, bAmbient );
}



void dbSetWireframe ( int iID, bool bFlag )
{
	SetWireframe ( iID, bFlag );
}
	

void dbSetSphereMap ( int iID, int iSphereImage )
{
	//SetSphereMap ( iID, iSphereImage );
}

void dbSetCubeMap ( int iID, int iCubeImage )
{
	SetCubeMap ( iID, iCubeImage );
}

void dbSetBumpMap ( int iID, int iEnvironmentImage, int iBumpMap )
{
	SetBumpMap ( iID, iEnvironmentImage, iBumpMap );
}


int dbGetWidth ( int iID )
{
	return GetWidth ( iID );
}

int dbGetDepth ( int iID )
{
	return GetDepth ( iID );
}

int dbGetXSegments ( int iID )
{
	return GetXSegments ( iID );
}

int dbGetZSegments ( int iID )
{
	return GetZSegments ( iID );
}

int dbGetTilesAcross ( int iID )
{
	return GetTilesAcross ( iID );
}

int dbGetTilesDown ( int iID )
{
	return GetTilesDown ( iID );
}

int dbGetFilter ( int iID )
{
	return GetFilter ( iID );
}

int dbGetGhostMode ( int iID )
{
	return GetGhostMode ( iID );
}

bool dbGetVisible ( int iID )
{
	return GetVisible ( iID );
}

bool dbGetWireframe ( int iID )
{
	return GetWireframe ( iID );
}

bool dbGetLight ( int iID )
{
	return GetLight ( iID );
}

bool dbGetCull ( int iID )
{
	return GetCull ( iID );
}

bool dbGetTransparency ( int iID )
{
	return GetTransparency ( iID );
}

bool dbGetZDepth ( int iID )
{
	return GetZDepth ( iID );
}

bool dbGetFog ( int iID )
{
	return GetFog ( iID );
}

bool dbGetAmbient ( int iID )
{
	return GetAmbient ( iID );
}

bool dbGetGhost ( int iID )
{
	return GetGhost ( iID );
}

bool dbGetLock ( int iID )
{
	return GetLock ( iID );
}


void dbSetGlobalColor ( int iID, int iA, int iR, int iG, int iB )
{
	SetGlobalColor ( iID, iA, iR, iG, iB );
}

void dbSetTileColor ( int iID, int iA, int iR, int iG, int iB )
{
	SetTileColor ( iID, iA, iR,  iG,  iB );
}


bool dbLockVertexData ( int iID )
{
	return LockVertexData (  iID );
}
	
bool dbUnlockVertexData ( int iID )
{
	return UnlockVertexData ( iID );
}
	
bool dbLockIndexData ( int iID )
{
	return LockIndexData ( iID );
}
	
bool dbUnlockIndexData ( int iID )
{
	return UnlockIndexData ( iID );
}

void* dbGetVertexData ( int iID )
{
	return GetVertexData (  iID );
}
	
void* dbGetIndexData ( int iID )
{
	return GetIndexData ( iID );
}
	
int dbGetNumVertices ( int iID )
{
	return GetNumVertices ( iID );
}
	
int dbGetNumIndices ( int iID )
{
	return GetNumIndices ( iID );
}
	












	


void dbSetTransparencyOn ( int iID, int iMode )
{
	//SetTransparencyOn (  iID,  iMode );
}
*/






 








/*
void dbSetPositionVector3 ( int iID, int iVector )
{
	SetPositionVector3 (  iID, iVector );
}



void dbSetRotationVector3 ( int iID, int iVector )
{
	SetRotationVector3 (  iID,  iVector );
}

void dbGetRotationVector3 ( int iVector, int iID )
{
	GetRotationVector3 ( iVector,  iID );
}


*/



	

 

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////