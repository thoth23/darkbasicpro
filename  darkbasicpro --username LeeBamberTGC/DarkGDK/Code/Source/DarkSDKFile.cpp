
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FILE COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKFile.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	ConstructorFile			( void );
void	DestructorFile			( void );
void	SetErrorHandlerFile		( LPVOID pErrorHandlerPtr );
void	PassCoreDataFile		( LPVOID pGlobPtr );

void	SetDir					( DWORD pString );
void	Dir						( void );
void	DriveList				( void );
void	ChecklistForFiles		( void );
void	ChecklistForDrives		( void );
void	FindFirst				( void );
void	FindNext				( void );
void	MakeFile				( DWORD pFilename );
void	DeleteFile				( DWORD pFilename );
void	CopyFile				( DWORD pFilename, DWORD pFilename2 );
void	RenameFile				( DWORD pFilename, DWORD pFilename2 );
void	MoveFile				( DWORD pFilename, DWORD pFilename2 );
void	WriteByteToFile			( DWORD pFilename, int iPos, int iByte );
int		ReadByteFromFile		( DWORD pFilename, int iPos );
void	MakeDir					( DWORD pFilename );
void	DeleteDir				( DWORD pFilename );
void	DeleteDirEx				( DWORD pFilename, int iFlag );
void	ExecuteFile				( DWORD pFilename, DWORD pFilename2, DWORD pFilename3 );
void	ExecuteFileEx			( DWORD pFilename, DWORD pFilename2, DWORD pFilename3, int iFlag );
DWORD	ExecuteFileIndi			( DWORD pFilename, DWORD pFilename2, DWORD pFilename3 );
DWORD	ExecuteFileIndi			( DWORD pFilename, DWORD pFilename2, DWORD pFilename3, int iPriority );
void	StopExecutable			( DWORD hIndiExecuteFileProcess );

void	WriteFilemapValue		( DWORD pFilemapname, DWORD dwValue );
void	WriteFilemapString		( DWORD pFilemapname, DWORD pString );
DWORD	ReadFilemapValue		( DWORD pFilemapname );
DWORD	ReadFilemapString		( DWORD pDestStr, DWORD pFilemapname );

void	OpenToRead				( int f, DWORD pFilename );
void	OpenToWrite				( int f, DWORD pFilename );
void	CloseFile				( int f );
int		ReadByte				( int f );
int		ReadWord				( int f );
int		ReadLong				( int f );
DWORD	ReadFloat				( int f );
DWORD	ReadString				( int f, DWORD pDestStr );
void	ReadFileBlock			( int f, DWORD pFilename );
void	SkipBytes				( int f, int iSkipValue );
void	ReadDirBlock			( int f, DWORD pFilename );
void	WriteByte				( int f, int iValue );
void	WriteWord				( int f, int iValue );
void	WriteLong				( int f, int iValue );
void	WriteFloat				( int f, float fValue );
void	WriteString				( int f, DWORD pString );
void	WriteFileBlock			( int f, DWORD pFilename );
void	WriteFileBlockEx		( int f, DWORD pFilename, int iFlag );
void	WriteDirBlock			( int f, DWORD pFilename );

void	ReadMemblock			( int f, int mbi );
void	MakeMemblockFromFile	( int mbi, int f );
void	WriteMemblock			( int f, int mbi );
void	MakeFileFromMemblock	( int f, int mbi );

DWORD	GetDir					( DWORD pDestStr );
DWORD	GetFileName				( DWORD pDestStr );
int		GetFileType				( void );
DWORD	GetFileDate				( DWORD pDestStr );
DWORD	GetFileCreation			( DWORD pDestStr );
int		FileExist				( DWORD pFilename );
int		FileSize				( DWORD pFilename );
int		PathExist				( DWORD pFilename );
int		FileOpen				( int f );
int		FileEnd					( int f );

DWORD	Appname					( DWORD pDestStr );
DWORD	Windir					( DWORD pDestStr );
int		GetExecutableRunning	( DWORD hIndiExecuteFileProcess );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbDir ( void )
{
	Dir ( );
}

void dbDriveList ( void )
{
	DriveList ( );
}

void dbSetDir ( char* pString )
{
	SetDir ( ( DWORD ) pString );
}

void dbMakeFile ( char* pFilename )
{
	MakeFile ( ( DWORD ) pFilename );
}

void dbMakeDir ( char* pFilename )
{
	MakeDir ( ( DWORD ) pFilename );
}

void dbDeleteFile ( char* pFilename )
{
	DeleteFile ( ( DWORD ) pFilename );
}

void dbDeleteDir ( char* pFilename )
{
	DeleteDir ( ( DWORD ) pFilename );
}

void dbDeleteDir ( char* pFilename, int iFlag  )
{
	// DeleteDirEx ( ( DWORD ) pFilename, iFlag );
}

void dbCopyFile ( char* pFilename, char* pFilename2 )
{
	CopyFile ( ( DWORD ) pFilename, ( DWORD ) pFilename2 );
}

void dbMoveFile ( char* pFilename, char* pFilename2 )
{
	MoveFile ( ( DWORD ) pFilename, ( DWORD ) pFilename2 );
}

void dbRenameFile ( char* pFilename, char* pFilename2 )
{
	RenameFile ( ( DWORD ) pFilename, ( DWORD ) pFilename2 );
}

void dbExecuteFile ( char* pFilename, char* pFilename2, char* pFilename3 )
{
	ExecuteFile ( ( DWORD ) pFilename, ( DWORD ) pFilename2, ( DWORD ) pFilename3 );
}

void dbExecuteFile ( char* pFilename, char* pFilename2, char* pFilename3, int iFlag )
{
	ExecuteFileEx ( ( DWORD ) pFilename, ( DWORD ) pFilename2, ( DWORD ) pFilename3, iFlag );
}

void dbFindFirst ( void )
{
	FindFirst ( );
}

void dbFindNext ( void )
{
	FindNext ( );
}

void dbOpenToRead ( int f, char* pFilename )
{
	OpenToRead ( f, ( DWORD ) pFilename );
}

void dbOpenToWrite ( int f, char* pFilename )
{
	OpenToWrite ( f, ( DWORD ) pFilename );
}

void dbCloseFile ( int f )
{
	CloseFile ( f );
}

int dbReadByte ( int f )
{
	return ReadByte ( f );
}

int dbReadWord ( int f )
{
	return ReadWord ( f );
}

int dbReadLong ( int f )
{
	return ReadLong ( f );
}

float dbReadFloat ( int f )
{
	DWORD dwReturn = ReadFloat ( f );

	return *( float* ) &dwReturn;
}

char* dbReadString ( int f )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = ReadString ( f, NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

void dbReadFileBlock ( int f, char* pFilename )
{
	ReadFileBlock ( f, ( DWORD ) pFilename );
}

void dbReadDirBlock ( int f, char* pFilename )
{
	ReadDirBlock ( f, ( DWORD ) pFilename );
}

void dbReadMemblock ( int f, int mbi )
{
	ReadMemblock ( f, mbi );
}

void dbWriteByte ( int f, int iValue )
{
	WriteByte ( f, iValue );
}

void dbWriteWord ( int f, int iValue )
{
	WriteWord ( f, iValue );
}

void dbWriteLong ( int f, int iValue )
{
	WriteLong ( f, iValue );
}

void dbWriteFloat ( int f, float fValue )
{
	WriteFloat ( f, fValue );
}

void dbWriteString ( int f, char* pString )
{
	WriteString ( f, ( DWORD ) pString );
}

void dbWriteFileBlock ( int f, char* pFilename )
{
	WriteFileBlock ( f, ( DWORD ) pFilename );
}

void dbWriteDirBlock ( int f, char* pFilename )
{
	WriteDirBlock ( f, ( DWORD ) pFilename );
}

void dbWriteMemblock ( int f, int mbi )
{
	WriteMemblock ( f, mbi );
}

void dbSkipBytes ( int f, int iSkipValue )
{
	SkipBytes ( f, iSkipValue );
}

void dbWriteByteToFile ( char* pFilename, int iPos, int iByte )
{
	WriteByteToFile ( ( DWORD ) pFilename, iPos, iByte );
}

int dbReadByteFromFile ( char* pFilename, int iPos )
{
	return ReadByteFromFile ( ( DWORD ) pFilename, iPos );
}

void dbCheckListForFiles ( void )
{
	ChecklistForFiles ( );
}

void dbCheckListForDrives ( void )
{
	ChecklistForDrives ( );
}

void dbMakeMemblockFromFile ( int mbi, int f )
{
	MakeMemblockFromFile ( mbi, f );
}

void dbMakeFileFromMemblock ( int f, int mbi )
{
	MakeFileFromMemblock ( f, mbi );
}

int dbFileExist ( char* pFilename )
{
	return FileExist ( ( DWORD ) pFilename );
}

int dbPathExist ( char* pFilename )
{
	return PathExist ( ( DWORD ) pFilename );
}

char* dbGetDir ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetDir ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbAppName ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Appname ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbWinDir ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = Windir ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbFileEnd ( int f )
{
	return FileEnd ( f );
}

int dbFileOpen ( int f )
{
	return FileOpen ( f );
}

int dbFileSize ( char* pFilename )
{
	return FileSize ( ( DWORD ) pFilename );
}

char* dbGetFileDate ( DWORD pDestStr )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetFileDate ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

char* dbGetFileName ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetFileName ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbGetFileType ( void )
{
	return GetFileType ( );
}

char* dbGetFileCreation ( DWORD pDestStr )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetFileCreation ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

void dbConstructorFile ( void )
{
	ConstructorFile ( );
}

void dbDestructorFile ( void )
{
	DestructorFile ( );
}

DWORD dbExecuteFileIndi ( DWORD pFilename, DWORD pFilename2, DWORD pFilename3 )
{
	return ExecuteFileIndi ( pFilename, pFilename2, pFilename3 );
}

DWORD dbExecuteFileIndi ( DWORD pFilename, DWORD pFilename2, DWORD pFilename3, int iPriority )
{
	return ExecuteFileIndi ( pFilename, pFilename2, pFilename3, iPriority );
}

void dbSetErrorHandlerFile ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerFile ( pErrorHandlerPtr );
}

void dbPassCoreDataFile ( LPVOID pGlobPtr )
{
	PassCoreDataFile ( pGlobPtr );
}

void dbStopExecutable ( DWORD hIndiExecuteFileProcess )
{
	StopExecutable ( hIndiExecuteFileProcess );
}

void dbWriteFilemapValue ( DWORD pFilemapname, DWORD dwValue )
{
	WriteFilemapValue ( pFilemapname, dwValue );
}

void dbWriteFilemapString ( DWORD pFilemapname, DWORD pString )
{
	WriteFilemapString ( pFilemapname, pString );
}

DWORD dbReadFilemapValue ( DWORD pFilemapname )
{
	return ReadFilemapValue ( pFilemapname );
}

DWORD dbReadFilemapString ( DWORD pDestStr, DWORD pFilemapname )
{
	return ReadFilemapString ( pDestStr, pFilemapname );
}

int dbGetExecutableRunning ( DWORD hIndiExecuteFileProcess )
{
	return GetExecutableRunning ( hIndiExecuteFileProcess );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
*/