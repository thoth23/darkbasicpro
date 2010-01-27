
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXT COMMANDS ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "..\Include\DarkSDKMemblocks.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	MakeMemblock				( int mbi, int size );
void	CreateMemblockFromBitmap	( int mbi, int bitmapindex );
void	CreateMemblockFromImage		( int mbi, int imageid );
void	CreateMemblockFromSound		( int mbi, int soundindex );
void	CreateMemblockFromMesh		( int mbi, int meshid );

void	DeleteMemblock				( int mbi );
void	CopyMemblock				( int mbiF, int mbiT, int posF, int posT, int size );

void	CreateBitmapFromMemblock	( int bitmapindex, int mbi );
void	CreateImageFromMemblock		( int imageid, int mbi );
void	CreateSoundFromMemblock		( int soundindex, int mbi );
void	CreateMeshFromMemblock		( int meshid, int mbi );
void	ChangeMeshFromMemblock		( int meshid, int mbi );

void	WriteMemblockByte			( int mbi, int pos, int data );
void	WriteMemblockWord			( int mbi, int pos, int data );
void	WriteMemblockDWord			( int mbi, int pos, DWORD data );
void	WriteMemblockFloat			( int mbi, int pos, float data );

int		MemblockExist				( int mbi );
DWORD	GetMemblockPtr				( int mbi );
int		GetMemblockSize				( int mbi );

int		ReadMemblockByte			( int mbi, int pos );
int		ReadMemblockWordLLL			( int mbi, int pos );
DWORD	ReadMemblockDWord			( int mbi, int pos );
DWORD	ReadMemblockFloat			( int mbi, int pos );

void	ConstructorMemblocks		( void );
void	DestructorMemblocks			( void );
void	SetErrorHandlerMemblocks	( LPVOID pErrorHandlerPtr );
void	PassCoreDataMemblocks		( LPVOID pGlobPtr );
void	RefreshD3DMemblocks			( int iMode );

void	ReturnMemblockPtrs			( LPVOID* pSizeArray, LPVOID* pDataArray );
LPSTR	ExtMakeMemblock				( int mbi, DWORD size );
void	ExtFreeMemblock				( int mbi );
DWORD	ExtGetMemblockSize			( int mbi );
void	ExtSetMemblockSize			( int mbi, DWORD size );
LPSTR	ExtGetMemblockPtr			( int mbi );
void	CreateMemblockFromArray		( int mbi, DWORD arrayptr );
void	CreateArrayFromMemblock		( DWORD arrayptr, int mbi );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbMakeMemblock( int mbi, int size )
{
	MakeMemblock( mbi, size );
}

void dbMakeMemblockFromBitmap( int mbi, int bitmapindex )
{
	CreateMemblockFromBitmap(  mbi,  bitmapindex );
}

void dbMakeMemblockFromImage( int mbi, int imageid )
{
	CreateMemblockFromImage(  mbi, imageid );
}

void dbMakeMemblockFromSound( int mbi, int soundindex )
{
	CreateMemblockFromSound(  mbi, soundindex );
}

void dbMakeMemblockFromMesh( int mbi, int meshid )
{
	CreateMemblockFromMesh(  mbi,  meshid );
}

void dbDeleteMemblock( int mbi )
{
	DeleteMemblock(  mbi );
}

void dbCopyMemblock( int mbiF, int mbiT, int posF, int posT, int size )
{
	CopyMemblock( mbiF, mbiT, posF,  posT,  size );
}

void dbMakeBitmapFromMemblock( int bitmapindex, int mbi )
{
	CreateBitmapFromMemblock(  bitmapindex,  mbi );
}

void dbMakeImageFromMemblock( int imageid, int mbi )
{
	CreateImageFromMemblock( imageid,  mbi );
}

void dbMakeSoundFromMemblock( int soundindex, int mbi )
{
	CreateSoundFromMemblock( soundindex,  mbi );
}

void dbMakeMeshFromMemblock( int meshid, int mbi )
{
	CreateMeshFromMemblock(  meshid,  mbi );
}

void dbChangeMeshFromMemblock( int meshid, int mbi )
{
	ChangeMeshFromMemblock( meshid, mbi );
}

void dbWriteMemblockByte( int mbi, int pos, int data )
{
	WriteMemblockByte(  mbi,  pos,  data );
}

void dbWriteMemblockWord( int mbi, int pos, int data )
{
	WriteMemblockWord(  mbi,  pos,  data );
}

void dbWriteMemblockDword( int mbi, int pos, DWORD data )
{
	WriteMemblockDWord(  mbi,  pos, data );
}

void dbWriteMemblockFloat( int mbi, int pos, float data )
{
	WriteMemblockFloat(  mbi, pos,  data );
}

int dbMemblockExist( int mbi )
{
	return MemblockExist(  mbi );
}

DWORD dbGetMemblockPtr( int mbi )
{
	return GetMemblockPtr(  mbi );
}

int dbGetMemblockSize( int mbi )
{
	return GetMemblockSize( mbi );
}

int dbReadMemblockByte( int mbi, int pos )
{
	return ReadMemblockByte(  mbi,  pos );
}

int dbReadMemblockWord( int mbi, int pos )
{
	return ReadMemblockWordLLL(  mbi, pos );
}

DWORD dbReadMemblockDword( int mbi, int pos )
{
	return ReadMemblockDWord( mbi, pos );
}

DWORD dbReadMemblockFloat( int mbi, int pos )
{
	return ReadMemblockFloat( mbi, pos );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbConstructorMemblocks ( void )
{
	ConstructorMemblocks ( );
}

void dbDestructorMemblocks ( void )
{
	DestructorMemblocks ( );
}

void dbSetErrorHandlerMemblocks ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerMemblocks ( pErrorHandlerPtr );
}

void dbPassCoreDataMemblocks( LPVOID pGlobPtr )
{
	PassCoreDataMemblocks( pGlobPtr );
}

void dbRefreshD3DMemblocks ( int iMode )
{
	RefreshD3DMemblocks ( iMode );
}

void dbReturnMemblockPtrs( LPVOID* pSizeArray, LPVOID* pDataArray )
{
	ReturnMemblockPtrs( pSizeArray, pDataArray );
}

LPSTR dbExtMakeMemblock ( int mbi, DWORD size )
{
	return ExtMakeMemblock ( mbi, size );
}

void dbExtFreeMemblock ( int mbi )
{
	ExtFreeMemblock ( mbi );
}

DWORD dbExtGetMemblockSize ( int mbi )
{
	return ExtGetMemblockSize ( mbi );
}

void dbExtSetMemblockSize ( int mbi, DWORD size )
{
	ExtSetMemblockSize ( mbi, size );
}

LPSTR dbExtGetMemblockPtr ( int mbi )
{
	return ExtGetMemblockPtr ( mbi );
}

void dbCreateMemblockFromArray( int mbi, DWORD arrayptr )
{
	CreateMemblockFromArray( mbi, arrayptr );
}

void dbCreateArrayFromMemblock( DWORD arrayptr, int mbi )
{
	CreateArrayFromMemblock( arrayptr,  mbi );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
*/