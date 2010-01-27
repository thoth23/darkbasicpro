
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include "..\Include\DarkSDKParticles.h"
//#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void		CreateEx					( int iID, int iImageIndex, int maxParticles, float fRadius );
void		CreateSnowEffect			( int iID, int iImageIndex, int maxParticles, float fX, float fY, float fZ, float fWidth, float fHeight, float fDepth );
void		CreateFireEffect			( int iID, int iImageIndex, int maxParticles, float fX, float fY, float fZ, float fWidth, float fHeight, float fDepth );
void		DeleteEx					( int iID );
void		Show						( int iID );
void		Hide						( int iID );
void		SetPosition					( int iID, float fX, float fY, float fZ );
void		SetEmitPosition				( int iID, float fX, float fY, float fZ );
void		SetRotation					( int iID, float fX, float fY, float fZ );
void		GetPositionVector3			( int iVector, int iID );
void		GetRotationVector3			( int iVector, int iID );
void		dbtSetColorExParticles		( int iID, int iRed, int iGreen, int iBlue );
void		SetNumberOfEmmissions		( int iID, int iNumber );
void		SetVelocity					( int iID, float fVelocity );
void		SetGravity					( int iID, float fGravity );
void		SetChaos					( int iID, float fChaos );
void		SetFloor					( int iID, int iFlag );
void		SetLife						( int iID, int iLifeValue );
void		GhostOn						( int iID, int iMode );
void		GhostOff					( int iID );
SDK_BOOL	GetExist					( int iID );
SDK_FLOAT	GetPositionXEx				( int iID );
SDK_FLOAT	GetPositionYEx				( int iID );
SDK_FLOAT	GetPositionZEx				( int iID );

void		ConstructorParticles		( HINSTANCE hSetup, HINSTANCE hImage );
void		DestructorParticles			( void );
void		SetErrorHandlerParticles	( LPVOID pErrorHandlerPtr );
void		PassCoreDataParticles		( LPVOID pGlobPtr );
void		RefreshD3DParticles			( int iMode );
void		UpdateParticles				( void );

bool		Create						( int iID, DWORD dwFlush, DWORD dwDiscard, float fRadius, int iImage );
void		Delete						( int iID );
void		SetColor					( int iID, int iRed, int iGreen, int iBlue );
void		SetTime						( int iID, float fTime );
void		SetSecondsPerFrame			( int iID, float fTime );
void		Ghost						( int iID, int iMode, float fPercentage );
void		SetPositionVector3			( int iID, int iVector );
void		SetRotationVector3			( int iID, int iVector );
*/

/*
void		ConstructorParticles			( HINSTANCE hSetup, HINSTANCE hImage );
void		DestructorParticles				( void );
void		SetErrorHandlerParticles		( LPVOID pErrorHandlerPtr );
void		PassCoreDataParticles			( LPVOID pGlobPtr );
void		RefreshD3DParticles				( int iMode );
void		UpdateParticles					( void );

bool		ParticlesCreate					( int iID, DWORD dwFlush, DWORD dwDiscard, float fRadius, int iImage );
void		ParticlesDelete					( int iID );
void		SetParticlesColor				( int iID, int iRed, int iGreen, int iBlue );
void		SetParticlesTime				( int iID, float fTime );

void		MakeParticles					( int iID, int iImageIndex, int maxParticles, float fRadius );
void		DeleteParticles					( int iID );
void		SetParticleSpeed				( int iID, float fTime );
void		SetParticleEmissions			( int iID, int iNumber );
void		SetParticleVelocity				( int iID, float fVelocity );
void		ParticlePosition				( int iID, float fX, float fY, float fZ );
void		PositionParticleEmissions		( int iID, float fX, float fY, float fZ );
void		ColorParticles					( int iID, int iRed, int iGreen, int iBlue );
void		RotateParticles					( int iID, float fX, float fY, float fZ );
void		SetParticleGravity				( int iID, float fGravity );
void		SetParticleChaos				( int iID, float fChaos );
void		SetParticleLife					( int iID, int iLifeValue );
void		SetParticleFloor				( int iID, int iFlag );
void		GhostParticlesOn				( int iID, int iMode );
void		GhostParticlesOff				( int iID );
void		GhostParticlesOn				( int iID, int iMode, float fPercentage );
void		HideParticles					( int iID );
void		ShowParticles					( int iID );
void		CreateSnowEffect				( int iID, int iImageIndex, int maxParticles, float fX, float fY, float fZ, float fWidth, float fHeight, float fDepth );
void		CreateFireEffect				( int iID, int iImageIndex, int maxParticles, float fX, float fY, float fZ, float fWidth, float fHeight, float fDepth );
void		PositionParticles				( int iID, int iVector );
void		SetVector3ToParticlesPosition	( int iVector, int iID );
void		RotateParticles					( int iID, int iVector );
void		SetVector3ToParticlesRotation	( int iVector, int iID );
SDK_BOOL	ParticlesExist					( int iID );
SDK_FLOAT	ParticlesPositionX				( int iID );
SDK_FLOAT	ParticlesPositionY				( int iID );
SDK_FLOAT	ParticlesPositionZ				( int iID );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbMakeParticles ( int iID, int iImageIndex, int maxParticles, float fRadius )
{
	MakeParticles (  iID,  iImageIndex,  maxParticles,  fRadius );
}

void dbMakeSnowParticles ( int iID, int iImageIndex, int maxParticles, float fX, float fY, float fZ, float fWidth, float fHeight, float fDepth )
{
	CreateSnowEffect (  iID,  iImageIndex, maxParticles,  fX,  fY,  fZ,  fWidth, fHeight, fDepth );
}

void dbMakeFireParticles ( int iID, int iImageIndex, int maxParticles, float fX, float fY, float fZ, float fWidth, float fHeight, float fDepth )
{
	CreateFireEffect ( iID,  iImageIndex, maxParticles, fX, fY, fZ, fWidth, fHeight, fDepth );
}

void dbDeleteParticles ( int iID )
{
	DeleteParticles ( iID );
}

void dbShowParticles ( int iID )
{
	ShowParticles ( iID );
}

void dbHideParticles ( int iID )
{
	HideParticles ( iID );
}

void dbPositionParticles ( int iID, float fX, float fY, float fZ )
{
	ParticlePosition ( iID, fX, fY, fZ );
}

void dbPositionParticlesEmissions ( int iID, float fX, float fY, float fZ )
{
	PositionParticleEmissions (  iID,  fX,  fY,  fZ );
}

void dbRotateParticles ( int iID, float fX, float fY, float fZ )
{
	RotateParticles (  iID,  fX,  fY,  fZ );
}
*/

/*
void dbSetVector3ToParticlesPosition ( int iVector, int iID )
{
	SetVector3ToParticlesPosition (  iVector, iID );
}

void dbSetVector3ToParticlesRotation ( int iVector, int iID )
{
	GetRotationVector3 ( iVector,  iID );
}
*/

/*
void dbColorParticles ( int iID, int iRed, int iGreen, int iBlue )
{
	ColorParticles (  iID,  iRed,  iGreen, iBlue );
}

void dbSetParticleEmissions	( int iID, int iNumber )
{
	SetParticleEmissions ( iID, iNumber );
}

void dbSetParticleVelocity ( int iID, float fVelocity )
{
	SetParticleVelocity ( iID, fVelocity );
}

void dbSetParticleSpeed ( int iID, float fTime )
{
	SetParticleSpeed ( iID, fTime );
}

void dbSetParticleGravity ( int iID, float fGravity )
{
	SetParticleGravity (  iID, fGravity );
}

void dbSetParticleChaos ( int iID, float fChaos )
{
	SetParticleChaos ( iID, fChaos );
}

void dbSetParticleFloor ( int iID, int iFlag )
{
	SetParticleFloor ( iID,  iFlag );
}

void dbSetParticleLife ( int iID, int iLifeValue )
{
	SetParticleLife (  iID, iLifeValue );
}

void dbGhostParticlesOn ( int iID, int iMode )
{
	GhostParticlesOn ( iID, iMode );
}

void dbGhostParticlesOff ( int iID )
{
	GhostParticlesOff ( iID );
}

SDK_BOOL dbParticlesExist ( int iID )
{
	return ParticlesExist ( iID );
}

float dbParticlesPositionX	( int iID )
{
	DWORD dwReturn = ParticlesPositionX ( iID );
	
	return *( float* ) &dwReturn;
}

float dbParticlesPositionY	( int iID )
{
	DWORD dwReturn = ParticlesPositionY ( iID );
	
	return *( float* ) &dwReturn;
}

float dbParticlesPositionZ ( int iID )
{
	DWORD dwReturn = ParticlesPositionZ ( iID );
	
	return *( float* ) &dwReturn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbConstructorParticles ( HINSTANCE hSetup, HINSTANCE hImage )
{
	ConstructorParticles ( hSetup, hImage );
}

void dbDestructorParticles ( void )
{
	DestructorParticles ( );
}

void dbSetErrorHandlerParticles ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerParticles ( pErrorHandlerPtr );
}

void dbPassCoreDataParticles ( LPVOID pGlobPtr )
{
	PassCoreDataParticles ( pGlobPtr );
}

void dbRefreshD3DParticles ( int iMode )
{
	RefreshD3DParticles ( iMode );
}

void dbUpdateParticles ( void )
{
	UpdateParticles ( );
}
*/

/*
bool dbCreate ( int iID, DWORD dwFlush, DWORD dwDiscard, float fRadius, int iImage )
{
	return Create (  iID, dwFlush, dwDiscard, fRadius,  iImage );
}

void dbDelete ( int iID )
{
	Delete (  iID );
}

void dbSetColor ( int iID, int iRed, int iGreen, int iBlue )
{
	SetColor (  iID,  iRed, iGreen, iBlue );
}

void dbSetTime ( int iID, float fTime )
{
	SetTime ( iID, fTime );
}

void dbSetSecondsPerFrame ( int iID, float fTime )
{
	SetSecondsPerFrame ( iID, fTime );
}

void dbGhost ( int iID, int iMode, float fPercentage )
{
	Ghost (  iID, iMode, fPercentage );
}

void dbSetPositionVector3 ( int iID, int iVector )
{
	SetPositionVector3 (  iID,  iVector );
}

void dbSetRotationVector3 ( int iID, int iVector )
{
	SetRotationVector3 ( iID, iVector );
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////