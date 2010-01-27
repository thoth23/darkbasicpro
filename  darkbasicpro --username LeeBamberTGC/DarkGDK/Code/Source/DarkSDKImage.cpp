
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMAGE COMMANDS ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include "..\Include\DarkSDKImage.h"
//#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void				LoadEx						( LPSTR szFilename, int iID );
void 				LoadEx						( LPSTR szFilename, int iID, int TextureFlag );
void 				SaveEx						( LPSTR szFilename, int iID ); 
void 				dbtDeleteImageEx			( int iID );
void 				GrabImageEx					( int iID, int iX1, int iY1, int iX2, int iY2 );
void 				GrabImageEx					( int iID, int iX1, int iY1, int iX2, int iY2, int iTextureFlag );
void 				PasteEx						( int iID, int iX, int iY ); 
void 				PasteEx						( int iID, int iX, int iY, int iFlag );
void 				SetColorKey					( int iR, int iG, int iB );
int 				GetExistEx					( int iID );

void 				ConstructorImage			( HINSTANCE hSetup );
void 				DestructorImage				( void );
void 				SetErrorHandlerImage		( LPVOID pErrorHandlerPtr );
void 				PassSpriteInstanceImage		( HINSTANCE hSprite );
void 				RefreshD3DImage				( int iMode );
void 				PassCoreDataImage			( LPVOID pGlobPtr );

int					LoadInternal				( char* szFilename );
int					LoadInternal				( char* szFilename, int iDivideTextureSize );
void				Delete						( int iID );
LPDIRECT3DTEXTURE9	Make						( int iID, int iWidth, int iHeight );
LPDIRECT3DTEXTURE9	MakeUsage					( int iID, int iWidth, int iHeight, DWORD dwUsage );
LPDIRECT3DTEXTURE9	MakeJustFormat				( int iID, int iWidth, int iHeight, D3DFORMAT Format );
LPDIRECT3DTEXTURE9	MakeRenderTarget			( int iID, int iWidth, int iHeight, D3DFORMAT Format );
void				SetCubeFace					( int iID, LPDIRECT3DCUBETEXTURE9 pCubeMap, int iFace );
void				GetCubeFace					( int iID, LPDIRECT3DCUBETEXTURE9* ppCubeMap, int* piFace );
LPDIRECT3DTEXTURE9	GetPointer					( int iID );
int					dbtGetWidthImage			( int iID ); 
int					dbtGetHeightImage			( int iID ); 
float				GetUMax						( int iID );
float				GetVMax						( int iID );
void				SetSharing					( bool bMode ); 
void				SetMemory					( int iMode ); 
void				Lock						( int iID ); 
void				Unlock						( int iID ); 
void				Write						( int iID, int iX, int iY, int iA, int iR, int iG, int iB );
void				Get							( int iID, int iX, int iY, int* piR, int* piG, int* piB );	
void				SetMipmapMode				( bool bMode );	
void				SetMipmapType				( int iType );	
void				SetMipmapBias				( float fBias );	
void				SetMipmapNum				( int iNum );	
void				SetTranslucency				( int iID, int iPercent );
bool				Load						( char* szFilename, int iID ); 
bool				Load						( char* szFilename, int iID, int TextureFlag, bool bIgnoreNegLimit, int iDivideTextureSize ); 
bool				Load						( char* szFilename, LPDIRECT3DTEXTURE9* pImage ); 
bool				LoadFullTex					( char* szFilename, LPDIRECT3DTEXTURE9* pImage, int iFullTexMode, int iDivideTextureSize );	
bool				Save						( char* szFilename, int iID );
bool				Save						( char* szFilename, int iID, int iCompressionMode );
bool				GrabImage					( int iID, int iX1, int iY1, int iX2, int iY2 );	
bool				GrabImage					( int iID, int iX1, int iY1, int iX2, int iY2, int iTextureFlag );	
void				Paste						( int iID, int iX, int iY ); 
void				Paste						( int iID, int iX, int iY, int iFlag ); 
bool				GetExist					( int iID ); 
LPSTR				GetName						( int iID );
void				Stretch						( int iID, int iSizeX, int iSizeY );
void 				LoadEx						( LPSTR szFilename, int iID, int TextureFlag, int iDivideTextureSize );
void 				SaveEx						( LPSTR szFilename, int iID, int iCompressionMode );
void 				RotateImageEx				( int iID, int iAngle );	
void 				SaveIconFromImage			( LPSTR pFilename, int iID );
DWORD 				LoadIcon					( LPSTR pFilename );
void 				FreeIcon					( DWORD dwIconHandle );
void 				GetImageData				( int iID, DWORD* dwWidth, DWORD* dwHeight, DWORD* dwDepth, LPSTR* pData, DWORD* dwDataSize, bool bLockData );
void 				SetImageData				( int iID, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, LPSTR pData, DWORD dwDataSize );
*/

/*
void				LoadImage		( LPSTR szFilename, int iID );							// load an image specifying the filename
void				LoadImage		( LPSTR szFilename, int iID, int TextureFlag );							// load an image specifying the filename
void				LoadImage		( LPSTR szFilename, int iID, int TextureFlag, int iDivideTextureSize );							// load an image specifying the filename
void				SaveImage		( LPSTR szFilename, int iID );							// saves the image to disk
void				SaveImage		( LPSTR szFilename, int iID, int iCompressionMode );
void				GetImage		( int iID, int iX1, int iY1, int iX2, int iY2 );		// grab image
void				GetImage		( int iID, int iX1, int iY1, int iX2, int iY2, int iTextureFlag );		// grab image
void				PasteImage		( int iID, int iX, int iY );								// paste image to backbuffer
void				PasteImage		( int iID, int iX, int iY, int iFlag );					// paste image to backbuffer
void				DeleteImage	( int iID );											// delete an image in the list
void				RotateImage	( int iID, int iAngle );											// delete an image in the list
int					ImageExist		( int iID );											// does image exist

void				SetImageColorKey		( int iR, int iG, int iB );		// set image color key
void				SaveIconFromImage	( LPSTR pFilename, int iID );
DWORD				LoadIcon			( LPSTR pFilename );
void				DeleteIcon			( DWORD dwIconHandle );

void 				ConstructorImage			( HINSTANCE hSetup );
void 				DestructorImage				( void );
void 				SetErrorHandlerImage		( LPVOID pErrorHandlerPtr );
void 				PassSpriteInstanceImage		( HINSTANCE hSprite );
void 				RefreshD3DImage				( int iMode );
void 				PassCoreDataImage			( LPVOID pGlobPtr );


int					LoadImageInternal		( char* szFilename );
int					LoadImageInternal		( char* szFilename, int iDivideTextureSize );

LPDIRECT3DTEXTURE9	MakeImage				( int iID, int iWidth, int iHeight );							// makes a new image
LPDIRECT3DTEXTURE9	MakeImageUsage			( int iID, int iWidth, int iHeight, DWORD dwUsage );
LPDIRECT3DTEXTURE9	MakeImageJustFormat		( int iID, int iWidth, int iHeight, D3DFORMAT Format );
LPDIRECT3DTEXTURE9	MakeImageRenderTarget   ( int iID, int iWidth, int iHeight, D3DFORMAT Format );

void				SetImageCubeFace		( int iID, LPDIRECT3DCUBETEXTURE9 pCubeMap, int iFace );
void				GetImageCubeFace		( int iID, LPDIRECT3DCUBETEXTURE9* ppCubeMap, int* piFace );

LPDIRECT3DTEXTURE9	GetImagePointer			( int iID );													// get the image
int					GetImageWidth			( int iID );													// get image width
int					GetImageHeight			( int iID );													// get image height
float				GetImageUMax			( int iID );
float				GetImageVMax			( int iID );

void				SetImageSharing			( bool bMode );													// set image share mode
void				SetImageMemory			( int iMode );													// set memory location

void				LockImage				( int iID );													// lock image
void				UnlockImage				( int iID );													// unlock image
void				WriteImage				( int iID, int iX, int iY, int iA, int iR, int iG, int iB );	// write data to image
void				GetImage				( int iID, int iX, int iY, int* piR, int* piG, int* piB );		// get pixel data from an image

void				SetImageMipmapMode		( bool bMode );													// set mip map mode on / off
void				SetImageMipmapType		( int iType );													// set type e.g linear
void				SetImageMipmapBias		( float fBias );												// set mip map bias
void				SetImageMipmapNum		( int iNum );													// set num of mip maps when image is loaded
void				SetImageTranslucency	( int iID, int iPercent );										// set translucency

bool				LoadImageEx				( char* szFilename, int iID );							// load an image specifying the filename
bool				LoadImageEx				( char* szFilename, int iID, int TextureFlag, bool bIgnoreNegLimit, int iDivideTextureSize );			// load an image specifying the filename
bool				LoadImageEx				( char* szFilename, LPDIRECT3DTEXTURE9* pImage );		// loads an image direct to texture
bool				LoadImageFullTex		( char* szFilename, LPDIRECT3DTEXTURE9* pImage, int iFullTexMode, int iDivideTextureSize );		// loads an image direct to texture
bool				SaveImageEx				( char* szFilename, int iID );							// saves the image to disk
bool				SaveImageEx				( char* szFilename, int iID, int iCompressionMode );							// saves the image to disk
bool				GrabImage				( int iID, int iX1, int iY1, int iX2, int iY2 );		// grab image
bool				GrabImage				( int iID, int iX1, int iY1, int iX2, int iY2, int iTextureFlag );		// grab image
void				PasteImageEx			( int iID, int iX, int iY );								// paste image to backbuffer
void				PasteImageEx			( int iID, int iX, int iY, int iFlag );								// paste image to backbuffer
void				DeleteImageEx			( int iID );											// delete an image in the list
bool				GetImageExist			( int iID );											// does image exist
LPSTR				GetImageName			( int iID );

void				StretchImage			( int iID, int iSizeX, int iSizeY );

void				GetImageData( int iID, DWORD* dwWidth, DWORD* dwHeight, DWORD* dwDepth, LPSTR* pData, DWORD* dwDataSize, bool bLockData );
void				SetImageData( int iID, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, LPSTR pData, DWORD dwDataSize );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbGetImageData( int iID, DWORD* dwWidth, DWORD* dwHeight, DWORD* dwDepth, LPSTR* pData, DWORD* dwDataSize, bool bLockData )
{
	GetImageData(  iID,  dwWidth,  dwHeight,  dwDepth,  pData,  dwDataSize,  bLockData );
}

void dbSetImageData( int iID, DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, LPSTR pData, DWORD dwDataSize )
{
	SetImageData(  iID,  dwWidth,  dwHeight,  dwDepth,  pData,  dwDataSize );
}

void dbLoadImage ( LPSTR szFilename, int iID )
{
	LoadImage ( szFilename, iID );
}
 
void dbLoadImage ( LPSTR szFilename, int iID, int TextureFlag )
{
	LoadImage ( szFilename, iID, TextureFlag );
}

void dbSaveImage ( LPSTR szFilename, int iID )
{
	SaveImage ( szFilename, iID );
}

void dbDeleteImage	( int iID )
{
	DeleteImage ( iID );
}

void dbGetImage ( int iID, int iX1, int iY1, int iX2, int iY2 )
{
	GetImage ( iID, iX1, iY1, iX2, iY2 );
}
 
void dbGetImage ( int iID, int iX1, int iY1, int iX2, int iY2, int iTextureFlag )
{
	GetImage ( iID, iX1, iY1, iX2, iY2, iTextureFlag );
}

void dbPasteImage ( int iID, int iX, int iY )
{
	PasteImage ( iID, iX, iY );
}
 
void dbPasteImage ( int iID, int iX, int iY, int iFlag )
{
	PasteImage	( iID, iX, iY, iFlag );
}

void dbSetImageColorKey ( int iR, int iG, int iB )
{
	SetImageColorKey ( iR, iG, iB );
}

int dbImageExist ( int iID )
{
	return ImageExist ( iID );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbConstructorImage ( HINSTANCE hSetup )
{
	ConstructorImage ( hSetup );
}

void dbDestructorImage ( void )
{
	DestructorImage ( );
}

void dbSetErrorHandlerImage ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerImage ( pErrorHandlerPtr );
}

void dbPassSpriteInstanceImage ( HINSTANCE hSprite )
{
	PassSpriteInstanceImage ( hSprite );
}

void dbRefreshD3DImage ( int iMode )
{
	RefreshD3DImage ( iMode );
}

void dbPassCoreDataImage( LPVOID pGlobPtr )
{
	PassCoreDataImage( pGlobPtr );
}

int dbLoadImageInternal ( char* szFilename )
{
	return LoadImageInternal ( szFilename );
}

int	dbLoadImageInternal	( char* szFilename, int iDivideTextureSize )
{
	return LoadImageInternal ( szFilename, iDivideTextureSize );
}

LPDIRECT3DTEXTURE9 dbMakeImage ( int iID, int iWidth, int iHeight )
{
	return MakeImage ( iID, iWidth, iHeight );
}

LPDIRECT3DTEXTURE9 dbMakeImageUsage	( int iID, int iWidth, int iHeight, DWORD dwUsage )
{
	return MakeImageUsage ( iID, iWidth, iHeight, dwUsage );
}

LPDIRECT3DTEXTURE9 dbMakeImageJustFormat ( int iID, int iWidth, int iHeight, D3DFORMAT Format )
{
	return MakeImageJustFormat ( iID, iWidth, iHeight, Format );
}

LPDIRECT3DTEXTURE9 dbMakeImageRenderTarget ( int iID, int iWidth, int iHeight, D3DFORMAT Format )
{
	return MakeImageRenderTarget ( iID, iWidth, iHeight, Format );
}

void dbSetImageCubeFace	( int iID, LPDIRECT3DCUBETEXTURE9 pCubeMap, int iFace )
{
	SetImageCubeFace ( iID, pCubeMap, iFace );
}

void dbGetImageCubeFace	( int iID, LPDIRECT3DCUBETEXTURE9* ppCubeMap, int* piFace )
{
	GetImageCubeFace ( iID, ppCubeMap, piFace );
}

LPDIRECT3DTEXTURE9 dbGetImagePointer ( int iID )
{
	return GetImagePointer ( iID );
}

int	dbGetImageWidth	( int iID )
{
	return GetImageWidth ( iID );
}

int	dbGetImageHeight ( int iID )
{
	return GetImageHeight ( iID );
}

float dbGetImageUMax ( int iID )
{
	return GetImageUMax ( iID );
}

float dbGetImageVMax ( int iID )
{
	return GetImageVMax ( iID );
}

void dbSetImageSharing ( bool bMode )
{
	SetImageSharing ( bMode );
}

void dbSetImageMemory ( int iMode )
{
	SetImageMemory ( iMode );
}

void dbLockImage ( int iID )
{
	LockImage ( iID );
}

void dbUnlockImage ( int iID )
{
	UnlockImage ( iID );
}

void dbWriteImage ( int iID, int iX, int iY, int iA, int iR, int iG, int iB )
{
	WriteImage (  iID,  iX,  iY,  iA,  iR,  iG,  iB );
}

void dbGetImage	( int iID, int iX, int iY, int* piR, int* piG, int* piB )
{
	GetImage ( iID, iX, iY, piR, piG, piB );
}

void dbSetImageMipmapMode ( bool bMode )
{
	SetImageMipmapMode ( bMode );
}

void dbSetImageMipmapType ( int iType )
{
	SetImageMipmapType ( iType );
}

void dbSetImageMipmapBias ( float fBias )
{
	SetImageMipmapBias ( fBias );
}

void dbSetImageMipmapNum ( int iNum )
{
	SetImageMipmapNum ( iNum );
}

void dbSetImageTranslucency	( int iID, int iPercent )
{
	SetImageTranslucency ( iID, iPercent );
}

bool dbLoadImageEx ( char* szFilename, int iID )
{
	return LoadImageEx ( szFilename, iID );
}

bool dbLoadImageEx ( char* szFilename, int iID, int TextureFlag, bool bIgnoreNegLimit, int iDivideTextureSize )
{
	return LoadImageEx ( szFilename,  iID,  TextureFlag,  bIgnoreNegLimit,  iDivideTextureSize );
}

bool dbLoadImageEx ( char* szFilename, LPDIRECT3DTEXTURE9* pImage )
{
	//return LoadImageEx (  szFilename,  pImage );
	return false;
}

bool dbLoadImageFullTex ( char* szFilename, LPDIRECT3DTEXTURE9* pImage, int iFullTexMode, int iDivideTextureSize )
{
	//return LoadImageFullTex (  szFilename,  pImage,  iFullTexMode,  iDivideTextureSize );
	return false;
}

bool dbSaveImageEx ( char* szFilename, int iID )
{
	return SaveImageEx ( szFilename, iID );
}

bool dbSaveImageEx ( char* szFilename, int iID, int iCompressionMode )
{
	return SaveImageEx (  szFilename,  iID,  iCompressionMode );
}

bool dbGrabImage ( int iID, int iX1, int iY1, int iX2, int iY2 )
{
	return GrabImage (  iID,  iX1,  iY1,  iX2,  iY2 );
}

bool dbGrabImage ( int iID, int iX1, int iY1, int iX2, int iY2, int iTextureFlag )
{
	return GrabImage ( iID,  iX1,  iY1,  iX2,  iY2,  iTextureFlag );
}

void dbPasteImageEx ( int iID, int iX, int iY )
{
	PasteImageEx (  iID,  iX,  iY );
}

void dbPasteImageEx	( int iID, int iX, int iY, int iFlag )
{
	PasteImageEx(  iID,  iX,  iY,  iFlag );
}

void dbDeleteImageEx ( int iID )
{
	DeleteImageEx ( iID );
}

bool dbGetImageExist ( int iID )
{
	return GetImageExist ( iID );
}

LPSTR dbGetImageName ( int iID )
{
	return GetImageName ( iID );
}

void dbStretchImage	( int iID, int iSizeX, int iSizeY )
{
	StretchImage ( iID, iSizeX, iSizeY );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

  */