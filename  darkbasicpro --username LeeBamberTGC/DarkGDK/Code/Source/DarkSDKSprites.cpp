
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// SPRITE COMMANDS //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKSprites.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void	SetSprite						( int iID, int iBacksave, int iTransparent );
void	Sprite							( int iID, int iX, int iY, int iImage );							// create a sprite
void	PasteSprite						( int iID, int iX, int iY );										// paste sprite on the screen
void	SizeSprite						( int iID, int iXSize, int iYSize );								// set the size of the sprite
void	ScaleSprite						( int iID, float fScale );											// scale a sprite
void	StretchSprite					( int iID, int iXStretch, int iYStretch );							// stretch the sprite
void	MirrorSprite					( int iID );														// mirror a sprite
void	FlipSprite						( int iID );														// flip a sprite
void	OffsetSprite					( int iID, int iXOffset, int iYOffset );							// offset sprite
void	HideSprite						( int iID );														// hide sprite
void	ShowSprite						( int iID );														// show sprite
void	HideAllSprites					( void );															// hide all sprites
void	ShowAllSprites					( void );															// show all sprites
void	DeleteSprite					( int iID );														// delete sprite

void	MoveSprite						( int iID, float velocity );										// rotate the sprite
void	RotateSprite					( int iID, float fRotate );											// rotate the sprite
void	SetSpriteImage					( int iID, int iImage );											// set the image that the sprite uses
void	SetSpriteAlpha					( int iID, int iValue );											// set the alpha level of a sprite from 0 - 255
void	SetSpriteDiffuse				( int iID, int iR, int iG, int iB );								// set the diffuse colours
void	PlaySprite						( int iID, int iStart, int iEnd, int iDelay );						// play an animation
void	SetSpriteFrame					( int iID, int iFrame );											// set the current frame of animation
void	SetSpriteTextureCoordinates		( int iID, int iVertex, float tu, float tv );						// set texture coordinates
void	CreateAnimatedSprite			( int iID, char* szImage, int iWidth, int iHeight, int iImageID );	// create an animated sprite from 1 image
void	CloneSprite						( int iID, int iDestinationID );									// clones a sprite
void	SetSpritePriority				( int iID, int iPriority );											// set priority

int		SpriteExist						( int iID );														// does sprite exist?
int		SpriteX							( int iID );														// get x pos
int		SpriteY							( int iID );														// get y pos
int		SpriteImage						( int iID );														// get image number
int		SpriteWidth						( int iID );														// get width
int		SpriteHeight					( int iID );														// get height
int		SpriteScaleX					( int iID );														// get x scale
int		SpriteScaleY					( int iID );														// get y scale
int		SpriteMirrored					( int iID );														// is mirrored?
int		SpriteFlipped					( int iID );														// is flipped
int		SpriteOffsetX					( int iID );														// get x offset
int		SpriteOffsetY					( int iID );														// get y offset
int		SpriteHit						( int iID, int iTarget );											// has sprite hit another?
int		SpriteCollision					( int iID, int iTarget );											// is sprite overlapping?

DWORD	SpriteAngle						( int iID );														// get the alpha of a sprite
int		SpriteAlpha						( int iID );														// get the alpha of a sprite
int		SpriteRed						( int iID );														// get the red colour
int		SpriteGreen						( int iID );														// get the green colour
int		SpriteBlue						( int iID );														// get the blue colour
int		SpriteFrame						( int iID );														// get current animation frame
int		SpriteVisible					( int iID );														// see if sprite is visible
*/

/*
void	Sprite						( int iID, int iX, int iY, int iImage );	
void	SetSprite					( int iID, int iBacksave, int iTransparent );
void	Delete						( int iID );
void	dbtCloneSprite				( int iID, int iDestinationID ); 
void	ShowSprite					( int iID );
void	HideSprite					( int iID );
void	HideAllSprites				( void );
void	ShowAllSprites				( void ); 

void	dbtMoveSprite				( int iID, float velocity );
void	Offset						( int iID, int iXOffset, int iYOffset );
void	Scale						( int iID, float fScale );
void	Size						( int iID, int iXSize, int iYSize );
void	Stretch						( int iID, int iXStretch, int iYStretch );
void	Rotate						( int iID, float fRotate );
void	Flip						( int iID );
void	Mirror						( int iID );

void	Paste						( int iID, int iX, int iY );
void	CreateAnimatedSprite		( int iID, char* szImage, int iWidth, int iHeight, int iImageID );

void	Play						( int iID, int iStart, int iEnd, int iDelay );	
void	SetFrame					( int iID, int iFrame ); 
void	SetPriority					( int iID, int iPriority );
void	SetImage					( int iID, int iImage ); 
void	SetAlpha					( int iID, int iValue ); 
void	SetDiffuse					( int iID, int iR, int iG, int iB );
void	SetTextureCoordinates		( int iID, int iVertex, float tu, float tv ); 

int		GetExist					( int iID ); 
int		GetX						( int iID ); 
int		GetY						( int iID ); 
int		GetXOffset					( int iID ); 
int		GetYOffset					( int iID ); 
int		GetXScale					( int iID ); 
int		GetYScale					( int iID ); 
int		dbtSpriteGetWidth			( int iID ); 
int		dbtSpriteGetHeight			( int iID );
int		GetImage					( int iID );
int		GetMirrored					( int iID ); 
int		GetFlipped					( int iID ); 
int		GetHit						( int iID, int iTarget );
int		dbtGetCollision				( int iID, int iTarget ); 
DWORD	GetAngle					( int iID );
int		GetFrame					( int iID );
int		GetAlpha					( int iID );
int		GetRed						( int iID ); 
int		GetGreen					( int iID ); 
int		GetBlue						( int iID );
int		GetVisible					( int iID );
*/

/*
void	ConstructorSprites			( HINSTANCE hSetup, HINSTANCE hImage );
void	DestructorSprites			( void );
void	SetErrorHandlerSprites		( LPVOID pErrorHandlerPtr );
void	PassCoreDataSprites			( LPVOID pGlobPtr );
void	RefreshD3DSprites			( int iMode );
void	UpdateSprites				( void );
void	UpdateAllSpritesSprites		( void );

void	SaveBack					( void );
void	RestoreBack					( void );
void	GetDiffuse					( int iID, int* piRed, int* piGreen, int* piBlue );
void	PasteImage					( int iImageID, int iX, int iY, float fU, float fV );
void	PasteImageEx				( int iImageID, int iX, int iY, float fU, float fV, int iTransparent );
void	PasteTextureToRect			( LPDIRECT3DTEXTURE9 pTexture, float fU, float fV, RECT Rect );		

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbSprite ( int iID, int iX, int iY, int iImage )
{
	Sprite (  iID,  iX,  iY,  iImage );
}

void dbSetSprite ( int iID, int iBacksave, int iTransparent )
{
	SetSprite ( iID, iBacksave, iTransparent );
}

void dbDeleteSprite ( int iID )
{
	DeleteSprite ( iID );
}

void dbCloneSprite ( int iID, int iDestinationID )
{
	CloneSprite ( iID, iDestinationID );
}

void dbShowSprite ( int iID )
{
	ShowSprite ( iID );
}
	
void dbHideSprite ( int iID )
{
	HideSprite ( iID );
}
 
void dbHideAllSprites ( void )
{
	HideAllSprites ( );
}
 
void dbShowAllSprites ( void )
{
	ShowAllSprites ( );
}

void dbMoveSprite ( int iID, float velocity )
{
	MoveSprite ( iID, velocity );
}

void dbOffsetSprite ( int iID, int iXOffset, int iYOffset )
{
	OffsetSprite ( iID, iXOffset, iYOffset );
}

void dbScaleSprite ( int iID, float fScale )
{
	ScaleSprite ( iID, fScale );
}

void dbSizeSprite ( int iID, int iXSize, int iYSize )
{
	SizeSprite( iID, iXSize, iYSize );
}

void dbStretchSprite ( int iID, int iXStretch, int iYStretch )
{
	StretchSprite ( iID, iXStretch, iYStretch );
}
 
void dbRotateSprite ( int iID, float fRotate )
{
	RotateSprite ( iID, fRotate );
}

void dbFlipSprite ( int iID )
{
	FlipSprite ( iID );
}

void dbMirrorSprite ( int iID )
{
	MirrorSprite ( iID );
}

void dbPasteSprite ( int iID, int iX, int iY )
{
	PasteSprite ( iID, iX, iY );
}

void dbCreateAnimatedSprite ( int iID, char* szImage, int iWidth, int iHeight, int iImageID )
{
	CreateAnimatedSprite ( iID, szImage, iWidth, iHeight, iImageID );
}

void dbPlaySprite ( int iID, int iStart, int iEnd, int iDelay )
{
	PlaySprite ( iID, iStart, iEnd, iDelay );
}
	
void dbSetSpriteFrame ( int iID, int iFrame )
{
	SetSpriteFrame ( iID, iFrame );
}

void dbSetSpritePriority ( int iID, int iPriority )
{
	SetSpritePriority ( iID, iPriority );
}

void dbSetSpriteImage ( int iID, int iImage )
{
	SetSpriteImage ( iID, iImage );
}

void dbSetSpriteAlpha ( int iID, int iValue )
{
	SetSpriteAlpha ( iID, iValue );
}
 
void dbSetSpriteDiffuse ( int iID, int iR, int iG, int iB )
{
	SetSpriteDiffuse ( iID, iR, iG, iB );
}

void dbSetSpriteTextureCoordinates ( int iID, int iVertex, float tu, float tv )
{
	SetSpriteTextureCoordinates ( iID, iVertex, tu, tv );
}

int dbSpriteExist ( int iID )
{
	return SpriteExist ( iID );
}
 
int dbSpriteX ( int iID )
{
	return SpriteX ( iID );
}

int dbSpriteY ( int iID )
{
	return SpriteY ( iID );
}

int dbSpriteOffsetX ( int iID )
{
	return SpriteOffsetX ( iID );
}

int dbSpriteOffsetY ( int iID )
{
	return SpriteOffsetY ( iID );
}

int dbSpriteScaleX ( int iID )
{
	return SpriteScaleX ( iID );
}
 
int dbSpriteScaleY ( int iID )
{
	return SpriteScaleY ( iID );
}

int dbSpriteWidth ( int iID )
{
	return SpriteWidth ( iID );
}
 
int dbSpriteHeight ( int iID )
{
	return SpriteHeight ( iID );
}
 
int dbSpriteImage ( int iID )
{
	return SpriteImage ( iID );
}
 
int dbSpriteMirrored ( int iID )
{
	return SpriteMirrored ( iID );
}
 
int dbSpriteFlipped ( int iID )
{
	return SpriteFlipped ( iID );
}

int dbSpriteHit ( int iID, int iTarget )
{
	return SpriteHit ( iID, iTarget );
}
 
int dbSpriteCollision ( int iID, int iTarget )
{
	return SpriteCollision ( iID, iTarget );
}

float dbSpriteAngle ( int iID )
{
	DWORD dwReturn = SpriteAngle ( iID );
	
	return *( float* ) &dwReturn;
}

int dbSpriteFrame ( int iID )
{
	return SpriteFrame ( iID );
}
 
int dbSpriteAlpha ( int iID )
{
	return SpriteAlpha ( iID );
}
 
int dbSpriteRed ( int iID )
{
	return SpriteRed ( iID );
}
 
int dbSpriteGreen ( int iID )
{
	return SpriteGreen ( iID );
}
 
int dbSpriteBlue ( int iID )
{
	return SpriteBlue ( iID );
}
 
int dbSpriteVisible ( int iID )
{
	return SpriteVisible ( iID );
}

void dbConstructorSprites ( HINSTANCE hSetup, HINSTANCE hImage )
{
	ConstructorSprites ( hSetup, hImage );
}

void dbDestructorSprites ( void )
{
	DestructorSprites ( );
}

void dbSetErrorHandlerSprites ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerSprites ( pErrorHandlerPtr );
}

void dbPassCoreDataSprites ( LPVOID pGlobPtr )
{
	PassCoreDataSprites ( pGlobPtr );
}

void dbRefreshD3DSprites ( int iMode )
{
	RefreshD3DSprites ( iMode );
}

void dbUpdateSprites ( void )
{
	UpdateSprites ( );
}

void dbUpdateAllSpritesSprites ( void )
{
	UpdateAllSpritesSprites ( );
}

void dbSaveBack ( void )
{
	SaveBack ( );
}
 
void dbRestoreBack ( void )
{
	RestoreBack ( );
}

void dbGetDiffuse ( int iID, int* piRed, int* piGreen, int* piBlue )
{
	//SpriteDiffuse ( iID, piRed, piGreen, piBlue );
}

void dbPasteImage ( int iImageID, int iX, int iY, float fU, float fV )
{
	PasteImage ( iImageID, iX, iY, fU, fV );
}	

void dbPasteImageEx ( int iImageID, int iX, int iY, float fU, float fV, int iTransparent )
{
	PasteImageEx ( iImageID, iX, iY, fU, fV, iTransparent );
}

void dbPasteTextureToRect ( LPDIRECT3DTEXTURE9 pTexture, float fU, float fV, RECT Rect )
{
	PasteTextureToRect ( pTexture, fU, fV, Rect );
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////