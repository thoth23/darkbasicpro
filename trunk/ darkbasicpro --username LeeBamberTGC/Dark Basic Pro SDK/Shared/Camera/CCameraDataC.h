#ifndef _CCAMERADATA_H_
#define _CCAMERADATA_H_

#pragma comment ( lib, "d3dx9.lib"  )
#pragma comment ( lib, "gdi32.lib" )
#include <D3DX9.h>

struct tagCameraData
{
	// variables for the camera

	D3DXMATRIX		matProjection;	// projection matrix
	D3DXMATRIX		matView;		// view matrix

	D3DXVECTOR3		vecLook;		// look vector
	D3DXVECTOR3		vecUp;			// up vector
	D3DXVECTOR3		vecRight;		// right vector
	D3DXVECTOR3		vecPosition;	// position vector

	D3DVIEWPORT9	viewPort2D;		// view port for 2D ( area on screen to take up )
	D3DVIEWPORT9	viewPort3D;		// view port for 3D ( area on screen to take up )

	int					iCameraToImage;
	LPDIRECT3DTEXTURE9	pCameraToImageTexture;
	LPDIRECT3DSURFACE9  pCameraToImageSurface;
	LPDIRECT3DTEXTURE9	pCameraToImageAlphaTexture;
	LPDIRECT3DSURFACE9  pCameraToImageAlphaSurface;

	// U69 - 180508 - new stereoscopic mode for camera images
	int					iStereoscopicMode;
	tagCameraData*		pStereoscopicFirstCamera;
	int					iCameraToStereoImageBack;
	int					iCameraToStereoImageFront;
	LPDIRECT3DTEXTURE9	pCameraToStereoImageBackTexture;
	LPDIRECT3DTEXTURE9	pCameraToStereoImageFrontTexture;
	LPDIRECT3DSURFACE9	pCameraToStereoImageBackSurface;
	LPDIRECT3DSURFACE9	pCameraToStereoImageFrontSurface;	

	int				iBackdropState;	// used as an automatic clear
	DWORD			dwBackdropColor;// color of backdrop

	// Can use XYZ rotation or FreeFlight(YPR)
	bool			bUseFreeFlightRotation;
	D3DXMATRIX		matFreeFlightRotate;
	float			fXRotate, fYRotate, fZRotate;	// rotateangle

	float			fAspect;		// aspect ratio
	float			fAspectMod;
	float			fFOV;			// field of view
	float			fZNear;			// z near
	float			fZFar;			// z far

	bool			bRotate;		// which rotate to use

	// mike - 250903 - override camera
	bool			bOverride;
	D3DXMATRIX		matOverride;

	// leeadd - 310506 - u62 - added clipping planes (so can do reflection tricks)
	int				iClipPlaneOn;
	D3DXPLANE		planeClip;

    // u74b7 - Moved to the end of the structure to maintain backwards compatibility.
	// U74 - 070409 - added for image ptr storage (DarkSKY) and also some reserves
	int					iBackdropTextureMode;
	LPDIRECT3DTEXTURE9	pBackdropTexture;

	// U74 - 070409 - added for image ptr storage (DarkSKY) and also some reserves
	DWORD				dwRes1;
	DWORD				dwRes2;
	DWORD				dwRes3;
	DWORD				dwRes4;
	DWORD				dwRes5;

	tagCameraData ( )
	{
		// setup default values for the structure, by creating a constructor
		// here it saves us having to call memset every time we create an instance
		// of it and we can also setup default values

		// clear the structure
		memset ( this, 0, sizeof ( tagCameraData ) );

		// mike - 250903 - override camera
		bOverride = false;

		// setup default values
		fAspect     = 0.0f;							// the aspect will come in later on
		fAspectMod  = 0.0f;							// modifier
		fFOV        = D3DX_PI / 4;						// setup the field of view (degrees)
		fZNear      = 1.0;							// near clipping range
		fZFar       = 3000.0;						// far clipping range
		vecLook     = D3DXVECTOR3 ( 0, 0, 1 );		// look vector
		vecUp       = D3DXVECTOR3 ( 0, 1, 0 );		// up vector
		vecRight    = D3DXVECTOR3 ( 1, 0, 0 );		// right vector
		vecPosition = D3DXVECTOR3 ( 0, 0, 0 );		// position

		D3DXMatrixIdentity ( &matFreeFlightRotate );// freeflight

		bRotate     = true;							// rotation for x, y and then z

		iBackdropState=0;							// used as an automatic clear
		dwBackdropColor=D3DCOLOR_XRGB(0,0,128);		// color of backdrop

		iBackdropTextureMode=0;						// no backdrop texture by default
		pBackdropTexture=NULL;
	}

};

#endif _CCAMERADATA_H_