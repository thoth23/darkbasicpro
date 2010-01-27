
#define INITGUID
#include <dxdiag.h>

#include "DarkSDK.h"
//#include "resource.h"
#include "stdio.h"
#include "stdlib.h"
#include "io.h"
#include <list>
using namespace std;

// includes
#include "DarkSDK.h"
#include "D:\coding\Dark Basic Pro Shared\Projects\Expansion Packs\Meqon\Code\Meqon.h"
#include ".\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\DBOData.h"
#include "timer.h"
#include <stdio.h>
#include "cCar.h"
#include "cRoboCar.h"

// global variables
float		g_fSpeed			= 10.1f;				// speed at which camera moves
float		g_fTurn				= 0.03f;			// how quickly the camera turns
float		g_fOldCamAngleX		= 0.0f;				// used to store old x angle
float		g_fOldCamAngleY		= 0.0f;				// used to store old x angle
float		g_fCameraAngleX		= 0.0f;				// current x angle
float		g_fCameraAngleY		= 0.0f;				// current y angle


// function listings
bool		SetupApplication	( void );					// used to set up general application properties like sync rate
void		UpdateApplication	( void );					// updates the application with functions like sync
void		showFPS				( void );					// show the frame rate on screen
void		ControlCamera		( void );					// handle the camera
void		userInput			( void );
sObject*	dbGetObject			( int iID );


// main program
void DarkSDK ( void )
{
	if ( !SetupApplication ( ) )
		return;

	//dbMakeObjectCube ( 1, 1 );
	dbLoadObject ( "test.dbo", 1 );

	sObject* pObject = dbGetObject ( 1 );

	//pObject->ppMeshList [ 0 ]->pTextures [ 1 ].dwBlendMode = D3DTOP_MODULATE2X;
	//pObject->ppMeshList [ 0 ]->pTextures [ 1 ].dwBlendArg2 = D3DTA_CURRENT;

	//pTexture1->dwStage        = 1;
	//pTexture1->dwBlendMode    = D3DTOP_MODULATE;
	//pTexture1->dwBlendArg1    = D3DTA_TEXTURE;
	//pTexture1->dwBlendArg2    = D3DTA_CURRENT;
	//pTexture1->dwTexCoordMode = 0;

	// loop round until the escape key is pressed
	while ( !dbEscapeKey ( ) )
	{
		// call all the relvant functions for the loop
		showFPS     ( );
		userInput	( );
		UpdateApplication ( );
	}
}

bool SetupApplication ( void )
{
	// this function is responsible for setting application properties
	// and getting things started

	dbSyncOn				( );					// turn sync on
	dbSyncRate				( 0 );					// set sync rate to 0
	//dbAutoCamOff			( );					// turn the auto camera off
	dbSetNormalizationOn	( );					// set normalization on due to scaling of cars
	
	return true;
}

void UpdateApplication ( void )
{
	// update the application with calls to display the frame rate,
	// handle user input, update the physics and draw to the screen
	dbSync      ( );
}

void showFPS ( void )
{
	// show the frame rate on screen

	// set up a character array
	char szFPS [ 256 ] = "";
	
	// set the cursor to top left
	dbSetCursor ( 0, 0 );

	// output text to the array
	sprintf ( szFPS, "fps = %i", dbScreenFPS ( ) );

	// and finally print to screen
	dbPrint ( szFPS );

	strcpy ( szFPS, dbStr ( dbCameraPositionX ( ) ) );
	dbPrint ( szFPS );
	strcpy ( szFPS, dbStr ( dbCameraPositionY ( ) ) );
	dbPrint ( szFPS );
	strcpy ( szFPS, dbStr ( dbCameraPositionZ ( ) ) );
	dbPrint ( szFPS );
}

void userInput ( void )
{
	dbControlCameraUsingArrowKeys ( 0, g_fSpeed, g_fTurn );

	g_fOldCamAngleY = g_fCameraAngleY;
	g_fOldCamAngleX = g_fCameraAngleX;

	g_fCameraAngleY = dbWrapValue ( g_fCameraAngleY + dbMouseMoveX ( ) * 0.4f );
	g_fCameraAngleX = dbWrapValue ( g_fCameraAngleX + dbMouseMoveY ( ) * 0.4f );

	dbYRotateCamera ( dbCurveAngle ( g_fCameraAngleY, g_fOldCamAngleY, 24 ) );
	dbXRotateCamera ( dbCurveAngle ( g_fCameraAngleX, g_fOldCamAngleX, 24 ) );

	char* szKey = dbInKey ( );

	if ( strcmp ( szKey, "+" ) == 0 )
	{
		if ( g_fSpeed < 1000 )
			g_fSpeed = g_fSpeed + 0.01f;
	}

	if ( strcmp ( szKey, "-" ) == 0 )
	{
		if ( g_fSpeed > 0.02f )
			g_fSpeed = g_fSpeed - 0.01f;
	}
}
