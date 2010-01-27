
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ANIMATION COMMANDS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKAnimation.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	LoadAnimation				( DWORD pFilename, int iIndex );
void	LoadDVDAnimation			( int animindex );
void	DeleteAnimation				( int animindex );

void	PlayAnimation				( int animindex );
void	PlayAnimation				( int animindex, int x1, int y1 );
void	PlayAnimation				( int animindex, int x1, int y1, int x2, int y2 );
void	PlayAnimationToImage		( int animindex, int imageindex, int x1, int y1, int x2, int y2 );
void	LoopAnimation				( int animindex );
void	StopAnimation				( int animindex );
void	PauseAnimation				( int animindex );
void	ResumeAnimation				( int animindex );
void	PlaceAnimation				( int animindex, int x1, int y1, int x2, int y2);

void	SetAnimationVolume			( int animindex, int ivolume );
void	SetAnimationSpeed			( int animindex, int ispeed );
void	SetDVDChapter				( int animindex, int iTitle, int iChapterNumber );

int		AnimationExist				( int animindex );
int		AnimationWidth				( int animindex );
int		AnimationHeight				( int animindex );
int		AnimationPlaying			( int animindex );
int		AnimationLooping			( int animindex );
int		AnimationPaused				( int animindex );
int		AnimationPositionX			( int animindex );
int		AnimationPositionY			( int animindex );
int		AnimationVolume				( int animindex );
int		AnimationSpeed				( int animindex );
int		TotalDVDChapters			( int animindex, int iTitle );

void	ConstructorAnimation		( void );
void	DestructorAnimation			( void );
void	SetErrorHandlerAnimation	( LPVOID pErrorHandlerPtr );
void	PassCoreDataAnimation		( LPVOID pGlobPtr );
void	RefreshD3DAnimation			( int iMode );
void	UpdateAllAnimation			( void );

void	PreventTextureLock			( bool bDoNotLock );
void	PlayAnimationToBitmap		( int animindex, int bitmapindex, int x1, int y1, int x2, int y2 );
void	LoopAnimationToBitmap		( int animindex, int bitmapindex, int x1, int y1, int x2, int y2 );

void	ConstructorAnimation		( void );
void	DestructorAnimation			( void );
void	SetErrorHandlerAnimation	( LPVOID pErrorHandlerPtr );
void	PassCoreDataAnimation		( LPVOID pGlobPtr );
void	RefreshD3DAnimation			( int iMode );
void	UpdateAllAnimation			( void );

void	PreventTextureLock			( bool bDoNotLock );
void	PlayAnimationToBitmap		( int animindex, int bitmapindex, int x1, int y1, int x2, int y2 );
void	LoopAnimationToBitmap		( int animindex, int bitmapindex, int x1, int y1, int x2, int y2 );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbLoadAnimation ( char* szFilename, int iIndex )
{
	LoadAnimation ( ( DWORD ) szFilename, iIndex );
}

void dbLoadDVDAnimation ( int animindex )
{
	LoadDVDAnimation ( animindex );
}

void dbDeleteAnimation ( int animindex )
{
	DeleteAnimation ( animindex );
}

void dbPlayAnimation ( int animindex )
{
	PlayAnimation (  animindex );
}

void dbPlayAnimation ( int animindex, int x1, int y1 )
{
	PlayAnimation ( animindex, x1, y1 );
}

void dbPlayAnimation ( int animindex, int x1, int y1, int x2, int y2 )
{
	PlayAnimation ( animindex, x1, y1, x2, y2 );
}

void dbPlayAnimationToImage ( int animindex, int imageindex, int x1, int y1, int x2, int y2 )
{
	PlayAnimationToImage ( animindex, imageindex, x1, y1, x2, y2 );
}

void dbLoopAnimation ( int animindex )
{
	LoopAnimation ( animindex );
}

void dbStopAnimation ( int animindex )
{
	StopAnimation ( animindex );
}

void dbPauseAnimation ( int animindex )
{
	PauseAnimation ( animindex );
}

void dbResumeAnimation ( int animindex )
{
	ResumeAnimation ( animindex );
}

void dbPlaceAnimation ( int animindex, int x1, int y1, int x2, int y2 )
{
	PlaceAnimation ( animindex, x1, y1, x2, y2 );
}

void dbSetAnimationVolume ( int animindex, int ivolume )
{
	SetAnimationVolume ( animindex, ivolume );
}

void dbSetAnimationSpeed ( int animindex, int ispeed )
{
	SetAnimationSpeed ( animindex, ispeed );
}

void dbSetDVDChapter ( int animindex, int iTitle, int iChapterNumber )
{
	SetDVDChapter ( animindex, iTitle, iChapterNumber );
}

int dbAnimationExist ( int animindex )
{
	return AnimationExist ( animindex );
}

int dbAnimationWidth ( int animindex )
{
	return AnimationWidth ( animindex );
}

int dbAnimationHeight ( int animindex )
{
	return AnimationHeight ( animindex );
}

int dbAnimationPlaying ( int animindex )
{
	return AnimationPlaying ( animindex );
}

int dbAnimationLooping ( int animindex )
{
	return AnimationLooping ( animindex );
}

int dbAnimationPaused ( int animindex )
{
	return AnimationPaused ( animindex );
}

int dbAnimationPositionX ( int animindex )
{
	return AnimationPositionX ( animindex );
}

int dbAnimationPositionY ( int animindex )
{
	return AnimationPositionY ( animindex );
}

int dbAnimationVolume ( int animindex )
{
	return AnimationVolume ( animindex );
}

int dbAnimationSpeed ( int animindex )
{
	return AnimationSpeed ( animindex );
}

int dbTotalDVDChapters ( int animindex, int iTitle )
{
	return TotalDVDChapters ( animindex, iTitle );
}

/////////

void dbUpdateAllAnimation ( void )
{
	UpdateAllAnimation ( );
}

void dbConstructorAnimation ( void )
{
	ConstructorAnimation ( );
}

void dbDestructorAnimation ( void )
{
	DestructorAnimation ( );
}

void dbSetErrorHandlerAnimation ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerAnimation ( pErrorHandlerPtr );
}

void dbPassCoreDataAnimation ( LPVOID pGlobPtr )
{
	PassCoreDataAnimation ( pGlobPtr );
}

void dbRefreshD3DAnimation ( int iMode )
{
	RefreshD3DAnimation ( iMode );
}

void dbPreventTextureLock ( bool bDoNotLock )
{
	PreventTextureLock ( bDoNotLock );
}

void dbPlayAnimationToBitmap ( int animindex, int bitmapindex, int x1, int y1, int x2, int y2 )
{
	PlayAnimationToBitmap ( animindex, bitmapindex, x1, y1, x2, y2 );
}

void dbLoopAnimationToBitmap ( int animindex, int bitmapindex, int x1, int y1, int x2, int y2 )
{
	LoopAnimationToBitmap ( animindex, bitmapindex, x1, y1, x2, y2 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
*/