
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// SOUND COMMANDS ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/*

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKSound.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void					ConstructorSound			( HWND hWnd );
void					DestructorSound				( void );
void					SetErrorHandlerSound		( LPVOID pErrorHandlerPtr );
void					PassCoreDataSound			( LPVOID pGlobPtr );
void					RefreshD3DSound				( int iMode );
void					UpdateSound					( void );

void					LoadSound					( LPSTR szFilename, int iID );
void					LoadSound					( LPSTR szFilename, int iID, int iFlag );
void					CloneSound					( int iDestination, int iSource );
void					DeleteSound					( int iID );	

void					PlaySound					( int iID );	
void					PlaySoundOffset				( int iID, int iOffset );
void					LoopSound					( int iID );				
void					LoopSound					( int iID, int iStart );
void					LoopSound					( int iID, int iStart, int iEnd );
void					LoopSound					( int iID, int iStart, int iEnd, int iInitialPos );

void					StopSound					( int iID );				
void					ResumeSound					( int iID );				
void					PauseSound					( int iID );				

void					SetSoundPan					( int iID, int iPan );	
void					SetSoundSpeed				( int iID, int iFrequency );
void					SetSoundVolume				( int iID, int iVolume );

void					RecordSound					( int iID );				
void					RecordSound					( int iID, int iCaptureDuration );
void					StopRecordingSound			( void );						
void					SaveSound					( LPSTR szFilename, int iID );	

void					Load3DSound					( LPSTR szFilename, int iID );	
void					PositionSound				( int iID, float fX, float fY, float fZ );
void					PositionListener			( float fX, float fY, float fZ );
void					RotateListener				( float fX, float fY, float fZ );
void					ScaleListener				( float fScale );

int						GetSoundExist				( int iID );						
int						GetSoundType				( int iID );						
int						GetSoundPlaying				( int iID );						
int						GetSoundLooping				( int iID );						
int						GetSoundPaused				( int iID );						
int						GetSoundPan					( int iID );						
int						GetSoundSpeed				( int iID );						
int						GetSoundVolume				( int iID );							

DWORD					GetSoundPositionXEx			( int iID );						
DWORD					GetSoundPositionYEx			( int iID );						
DWORD					GetSoundPositionZEx			( int iID );						
DWORD					GetListenerPositionXEx		( void );						
DWORD					GetListenerPositionYEx		( void );						
DWORD					GetListenerPositionZEx		( void );						
DWORD					GetListenerAngleXEx			( void );						
DWORD					GetListenerAngleYEx			( void );						
DWORD					GetListenerAngleZEx			( void );						

void					GetSoundData				( int iID, DWORD* dwBitsPerSecond, DWORD* Frequency, DWORD* Duration, LPSTR* pData, DWORD* dwDataSize, bool bLockData );
void					SetSoundData				( int iID, DWORD dwBitsPerSecond, DWORD Frequency, DWORD Duration, LPSTR pData, DWORD dwDataSize );

LPDIRECTSOUND8			GetSoundInterface			( void );
IDirectSound3DBuffer8*	GetSoundBuffer				( int iID );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbLoadSound ( LPSTR szFilename, int iID )
{
	LoadSound ( szFilename, iID );
}

void dbLoadSound ( LPSTR szFilename, int iID, int iFlag )
{
	LoadSound ( szFilename, iID, iFlag );
}

void dbLoad3DSound ( LPSTR szFilename, int iID )
{
	Load3DSound ( szFilename, iID );
}

void dbSaveSound ( LPSTR szFilename, int iID )
{
	SaveSound ( szFilename, iID );
}

void dbDeleteSound ( int iID )
{
	DeleteSound ( iID );
}

void dbCloneSound ( int iDestination, int iSource )
{
	CloneSound ( iDestination, iSource );
}

void dbPlaySound ( int iID )
{
	PlaySound ( iID );
}
	
void dbPlaySound ( int iID, int iOffset )
{
	PlaySoundOffset ( iID, iOffset );
}

void dbLoopSound ( int iID )
{
	LoopSound ( iID );
}
 
void dbLoopSound ( int iID, int iStart )
{
	LoopSound ( iID, iStart );
}

void dbLoopSound ( int iID, int iStart, int iEnd )
{
	LoopSound ( iID, iStart, iEnd );
}

void dbLoopSound ( int iID, int iStart, int iEnd, int iInitialPos )
{
	LoopSound ( iID, iStart, iEnd, iInitialPos );
}

void dbStopSound ( int iID )
{
	StopSound ( iID );
}

void dbPauseSound ( int iID )
{
	PauseSound ( iID );
}
 
void dbResumeSound ( int iID )
{
	ResumeSound ( iID );
}

void dbPositionSound ( int iID, float fX, float fY, float fZ )
{
	PositionSound ( iID, fX, fY, fZ );
}

void dbRecordSound ( int iID )
{
	RecordSound ( iID );
}
 
void dbRecordSound ( int iID, int iCaptureDuration )
{
	RecordSound ( iID, iCaptureDuration );
}

void dbStopRecordingSound ( void )
{
	StopRecordingSound ( );
}

void dbSetSoundPan ( int iID, int iPan )
{
	SetSoundPan ( iID, iPan );
}
	
void dbSetSoundSpeed ( int iID, int iFrequency )
{
	SetSoundSpeed ( iID, iFrequency );
}

void dbSetSoundVolume ( int iID, int iVolume )
{
	SetSoundVolume ( iID, iVolume );
}

void dbPositionListener ( float fX, float fY, float fZ )
{
	PositionListener (  fX, fY, fZ );
}

void dbRotateListener ( float fX, float fY, float fZ )
{
	RotateListener (  fX, fY, fZ );
}

void dbScaleListener ( float fScale )
{
	ScaleListener ( fScale );
}

int dbSoundExist ( int iID )
{
	return GetSoundExist ( iID );
}
 
int dbSoundType ( int iID )
{
	return GetSoundType ( iID );
}
 
int dbSoundPlaying ( int iID )
{
	return GetSoundPlaying ( iID );
}
 
int dbSoundLooping ( int iID )
{
	return GetSoundLooping ( iID );
}

int dbSoundPaused ( int iID )
{
	return GetSoundPaused ( iID );
}

float dbSoundPositionX ( int iID )
{
	DWORD dwReturn = GetSoundPositionXEx ( iID );
	
	return *( float* ) &dwReturn;
}
 
float dbSoundPositionY ( int iID )
{
	DWORD dwReturn = GetSoundPositionYEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbSoundPositionZ ( int iID )
{
	DWORD dwReturn = GetSoundPositionZEx ( iID );
	
	return *( float* ) &dwReturn;
}

int dbSoundPan ( int iID )
{
	return GetSoundPan ( iID );
}
 
int dbSoundSpeed ( int iID )
{
	return GetSoundSpeed ( iID );
}

int dbSoundVolume ( int iID )
{
	return GetSoundVolume ( iID );
}

float dbListenerPositionX ( void )
{
	DWORD dwReturn = GetListenerPositionXEx ( );
	
	return *( float* ) &dwReturn;
}

float dbListenerPositionY ( void )
{
	DWORD dwReturn = GetListenerPositionYEx ( );
	
	return *( float* ) &dwReturn;
}
 
float dbListenerPositionZ ( void )
{
	DWORD dwReturn = GetListenerPositionZEx ( );
	
	return *( float* ) &dwReturn;
}
 
float dbListenerAngleX ( void )
{
	DWORD dwReturn = GetListenerAngleXEx ( );
	
	return *( float* ) &dwReturn;
}
 
float dbListenerAngleY ( void )
{
	DWORD dwReturn = GetListenerAngleYEx ( );
	
	return *( float* ) &dwReturn;
}

float dbListenerAngleZ ( void )
{
	DWORD dwReturn = GetListenerAngleZEx ( );
	
	return *( float* ) &dwReturn;
}

void dbConstructorSound ( HWND hWnd )
{
	ConstructorSound ( hWnd );
}

void dbDestructorSound ( void )
{
	DestructorSound ( );
}

void dbSetErrorHandlerSound	( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerSound( pErrorHandlerPtr );
}

void dbPassCoreDataSound ( LPVOID pGlobPtr )
{
	PassCoreDataSound ( pGlobPtr );
}

void dbRefreshD3DSound ( int iMode )
{
	RefreshD3DSound ( iMode );
}

void dbUpdateSound ( void )
{
	 UpdateSound ( );
}

void dbGetSoundData ( int iID, DWORD* dwBitsPerSecond, DWORD* Frequency, DWORD* Duration, LPSTR* pData, DWORD* dwDataSize, bool bLockData )
{
	GetSoundData ( iID, dwBitsPerSecond, Frequency, Duration, pData,dwDataSize, bLockData );
}

void dbSetSoundData ( int iID, DWORD dwBitsPerSecond, DWORD Frequency, DWORD Duration, LPSTR pData, DWORD dwDataSize )
{
	SetSoundData ( iID, dwBitsPerSecond, Frequency, Duration, pData, dwDataSize );
}
 
LPDIRECTSOUND8 dbGetSoundInterface ( void )
{
	return GetSoundInterface ( );
}

IDirectSound3DBuffer8* dbGetSoundBuffer ( int iID )
{
	return GetSoundBuffer ( iID );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
*/