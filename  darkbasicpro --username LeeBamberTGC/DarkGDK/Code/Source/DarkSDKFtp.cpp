
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "..\Include\DarkSDKFTP.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	Connect				( DWORD dwString, DWORD dwString2, DWORD dwString3 );
void	ConnectEx			( DWORD dwString, DWORD dwString2, DWORD dwString3, int iUseWindow );
void	Disconnect			( void );
void	DisconnectEx		( int iFlag );

void	SetDir				( DWORD dwString );
void	PutFile				( DWORD dwString );
void	GetFile				( DWORD dwString, DWORD dwString2 );
void	GetFile				( DWORD dwString, DWORD dwString2, int iFlag );
void	DeleteFile			( DWORD dwString );
void	FindFirst			( void );
void	FindNext			( void );
void	Terminate			( void );
void	Proceed				( void );

DWORD	GetError			( DWORD pDestStr );
DWORD	GetDir				( DWORD pDestStr );
DWORD	GetFileName			( DWORD pDestStr );
int		GetFileType			( void );
int		GetFileSize			( void );
int		GetProgress			( void );
int		GetStatus			( void );
int		GetFailure			( void );

void	ConstructorFTP		( void );
void	DestructorFTP		( void );
void	SetErrorHandlerFTP	( LPVOID pErrorHandlerPtr );
void	PassCoreDataFTP		( LPVOID pGlobPtr );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbFTPConnect ( char* dwString, char* dwString2, char* dwString3 )
{
	Connect ( ( DWORD ) dwString, ( DWORD ) dwString2, ( DWORD ) dwString3 );
}

void dbFTPConnect ( char* dwString, char* dwString2, char* dwString3, int iUseWindow )
{
	ConnectEx ( ( DWORD ) dwString, ( DWORD ) dwString2,  ( DWORD ) dwString3,  iUseWindow );
}

void dbFTPDisconnect ( void )
{
	Disconnect ( );
}

void dbFTPDisconnect( int iFlag )
{
	DisconnectEx(  iFlag );
}

void dbFTPTerminate(void)
{
	Terminate ( );
}

void dbFTPSetDir( char* dwString )
{
	SetDir( ( DWORD ) dwString );
}

void dbFTPGetFile( char* dwString, char* dwString2 )
{
	GetFile ( ( DWORD ) dwString, ( DWORD ) dwString2 );
}

void dbFTPGetFile( char* dwString, char* dwString2, int iFlag )
{
	GetFile( ( DWORD ) dwString, ( DWORD ) dwString2, iFlag );
}

void dbFTPPutFile( char* dwString )
{
	PutFile( ( DWORD ) dwString );
}

void dbFTPDeleteFile( char* dwString )
{
	DeleteFile( ( DWORD ) dwString );
}

void dbFTPProceed(void)
{
	Proceed ( );
}

void dbFTPFindFirst(void)
{
	FindFirst ( );
}

void dbFTPFindNext(void)
{
	FindNext ( );
}

int dbGetFTPStatus(void)
{
	return GetStatus ( );
}

char* dbGetFTPDir( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetDir ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbGetFTPProgress(void)
{
	return GetProgress ( );
}

int dbGetFTPFailure(void)
{
	return GetFailure ( );
}

char* dbGetFTPError( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetError ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbGetFTPFileType(void)
{
	return GetFileType ( );
}

char* dbGetFTPFileName( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetFileName ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbGetFTPFileSize(void)
{
	return GetFileSize ( );
}

void dbConstructorFTP ( void )
{
	ConstructorFTP ( );
}

void dbDestructorFTP ( void )
{
	DestructorFTP ( );
}

void dbSetErrorHandlerFTP ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerFTP ( pErrorHandlerPtr );
}

void dbPassCoreDataFTP ( LPVOID pGlobPtr )
{
	PassCoreDataFTP ( pGlobPtr );
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////