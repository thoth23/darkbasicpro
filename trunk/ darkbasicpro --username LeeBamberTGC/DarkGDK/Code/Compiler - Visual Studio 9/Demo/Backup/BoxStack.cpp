
// car scene test demo

// includes
#include "DarkSDK.h"
#include "D:\coding\Dark Basic Pro Shared\Projects\Expansion Packs\Meqon\Code\Meqon.h"
#include ".\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\DBOData.h"
#include "timer.h"
#include <stdio.h>
#include "cCar.h"
#include "cRoboCar.h"
#include "cSlider.h"

// enumerated values for the camera
enum eCameraMode
{
	eCameraRear = 1,								// rear view
	eCameraSide,									// side view
	eCameraOverhead,								// overhead view
};

// global variables
cSlider		g_Sliders [ 100 ];
float		g_fSpeed			= 0.1f;				// speed at which camera moves
float		g_fTurn				= 0.03f;			// how quickly the camera turns
float		g_fOldCamAngleX		= 0.0f;				// used to store old x angle
float		g_fOldCamAngleY		= 0.0f;				// used to store old x angle
float		g_fCameraAngleX		= 0.0f;				// current x angle
float		g_fCameraAngleY		= 0.0f;				// current y angle
cCar*		g_pCar				= NULL;				// player controlled car
eCameraMode g_CameraMode		= eCameraSide;		// initial camera mode

// function listings
bool SetupApplication	( void );					// used to set up general application properties like sync rate
bool SetupScenery		( void );					// will set up a scene to be used
bool SetupCars			( void );					// will set up player and robo cars
bool SetupSliders		( void );					// set up the sliders
void ControlCars		( void );					// function to control the cars
void ControlSliders		( void );					// control the sliders
void UpdateApplication	( void );					// updates the application with functions like sync
void showFPS			( void );					// show the frame rate on screen
void showCarInfo		( void );					// display information about the car like it's speed
void ControlCamera		( void );					// handle the camera
void userInput			( void );

void UnGlueAllObjects ( void );

void dbMeqRigidBodyMeshCreateEx ( int iID, int iObject, int iMaterial );

sObject*	dbGetObject			( int iID );

void SetBitmapFormat ( int iFormat );

// main program
void DarkSDK ( void )
{
	SetBitmapFormat ( 6 );

	// set up general application properties, the scenery and last of all the cars
	if ( !SetupApplication ( ) || !SetupScenery ( ) || !SetupCars ( ) )
		return;

	// loop round until the escape key is pressed
	while ( !dbEscapeKey ( ) )
	{
		

		// call all the relvant functions for the loop
		ControlCars			( );					// control the cars
		//ControlCamera ( );
		userInput			( );
		UpdateApplication	( );					// update the application

		//dbMeqEnd ( );
	}
}

bool SetupApplication ( void )
{
	// this function is responsible for setting application properties
	// and getting things started

	dbSyncOn				( );					// turn sync on
	dbSyncRate				( 0 );					// set sync rate to 0
	dbAutoCamOff			( );					// turn the auto camera off
	dbPositionCamera		( 1, 65, 175 );			// position the camera
	dbSetNormalizationOn	( );					// set normalization on due to scaling of cars
	dbMeqStart				( );					// start meqon physics

	return true;
}

bool SetupScenery ( void )
{
	// local variables
	int		iRoadStart		= 1;		// start ID for roads
	int		iRoadEnd		= 151;		// end ID for roads
	float	fX				= 0;		// temp X position
	float	fY				= 0;		// temp Y position
	float	fZ				= 0;		// temp Z position
	int		iObject			= 0;		// object number

	// load road and pavement images
	dbLoadImage ( "road2.png",    1 );
	dbLoadImage ( "pavement.png", 2 );

	// create a material
	dbMeqMaterialCreate ( 1, 0.9f, 0.7f, 0.1f );
	
	// create a grid of boxes as the road
	for ( iObject = iRoadStart; iObject < iRoadEnd; iObject++, fZ += 100.0f )
	{
		// increment x and reset z at intervals
		if ( iObject > 1 && iObject % 10 == 1 )
		{
			fX += 50;
			fZ = 0.0f;
		}

		// set up a box piece
		dbMakeObjectBox			 ( iObject, 50, 1, 100 );	// make the box
		dbPositionObject		 ( iObject, fX, 0, fZ );	// position it
		dbTextureObject			 ( iObject, 1 );			// texture
		dbSetObjectMipMapLODBias ( iObject, -2.0f );		// change mip map lod bias
	}

	/*
	// create a ramp

	fX = 200.0f;
	fZ = 400.0f;

	dbMakeObjectBox	( iObject, 50, 1, 100 );
	dbTextureObject	( iObject, 1 );
	dbRotateObject ( iObject, -20, 0, 0 );
	dbPositionObject ( iObject, fX, 17, fZ - 3 );
	dbMeqRigidBodyBoxCreate  ( iObject, iObject, 1 );
	//dbMeqRigidBodyMeshCreate  ( iObject, iObject, 1 );
	dbMeqRigidBodySetDynamic ( iObject, 0 );

	iObject++;

	fZ += 100.0f;

	dbMakeObjectBox	( iObject, 50, 1, 100 );
	dbTextureObject	( iObject, 1 );
	dbPositionObject ( iObject, fX, 34, fZ - 6.2 );
	dbMeqRigidBodyBoxCreate  ( iObject, iObject, 1 );
	dbMeqRigidBodySetDynamic ( iObject, 0 );
	*/

	// create one large rigid body box to cover the scene rather
	// than creating many individual smaller boxes
	dbMakeObjectBox			 ( 6000, 5000, 1, 5000 );
	//dbMeqRigidBodyBoxCreate  ( 6000, 6000, 1 );
	dbMeqRigidBodyMeshCreate  ( 6000, 6000, 1 );
	dbMeqRigidBodySetDynamic ( 6000, 0 );
	
	dbHideObject			 ( 6000 );

	/*
	// create a pavement
	for ( iObject = 162, fZ = 300, iObject = 162; iObject < 162 + 10; iObject++, fZ += 100 )
	{
		dbMakeObjectBox			 ( iObject, 50, 1, 100 );
		dbTextureObject			 ( iObject, 2 );
		dbPositionObject		 ( iObject, 250, 1, fZ );
		dbMeqRigidBodyBoxCreate  ( iObject, iObject, 1 );
		dbMeqRigidBodySetDynamic ( iObject, 0 );
	}
	
	// create a stack of boxes we can crash into
	iObject = 200;
	fZ      = 400;
	fY      = 5;

	int iX = 0;
	int iY = 50;
	int iLevel = 5;

	for ( iX = 0; iX < iLevel; iX++ )
	{
		for ( iY = 0; iY < iLevel; iY++ )
		{
			dbMakeObjectBox			( iObject, 5, 5, 5 );
			dbTextureObject			( iObject, 2 );
			dbPositionObject		( iObject, ( iX * 5 ) + 180, ( iY * 5 ) + 5, 700 );
			dbMeqRigidBodyBoxCreate ( iObject, iObject, 1 );
			dbMeqRigidBodySetActive ( iObject, 0 );

			iObject++;
		}
	}
	*/

	return true;
}

void dbMeqRigidBodyBoxCreateEx ( int iID, int iObject, int iMaterial );

bool SetupCars ( void )
{
	// set up the cars

	if ( !( g_pCar = new cCar ( 500, ".\\mycar.meqv", 60, 15, 170 ) ) )
		return false;

	/*
		dbMakeObjectBox			 ( 900, 5, 5, 5 );
		dbTextureObject			 ( 900, 2 );
		dbPositionObject		 ( 900, 60, 5, 300 );
		dbMeqRigidBodyBoxCreate  ( 900, 900, 1 );
		*/
		
	dbLoadObject ( "bin1.x", 900 );
	//dbMakeObjectBox		 ( 900, 5, 10, 5 );
	dbPositionObject ( 900, 60, 1, 300 );
	dbMeqRigidBodyBoxCreate ( 900, 900 );


	//dbMeqRigidBodyBoxCreateEx ( 900, 900, 1 );
	//dbMeqRigidBodyConvexCreate ( 900, 900 );

	//dbMeqRigidBodySetTranslation ( 900, 0.0f, 1 + ( dbObjectSize ( 900 ) / 2.0f ), 0.0f );
	//dbMeqRigidBodySetTranslation ( 900, 0.0f, 100, 0.0f );

	sObject* pObject = dbGetObject ( 900 );
	
	dbMakeObjectCube		 ( 901, 50 );
	dbTextureObject			 ( 901, 2 );
	dbPositionObject		 ( 901, 60, 25, 400 );
	dbMeqRigidBodyBoxCreate  ( 901, 901, 1 );
	dbMeqRigidBodySetDynamic ( 901, 0 );

	return true;
}

void ControlCamera ( void )
{
	// control the camera mode

	// check the current mode
	switch ( g_CameraMode )
	{
		case eCameraRear:
		{
			// looking from behind
			dbSetCameraToFollow ( dbObjectPositionX ( g_pCar->m_iID ), dbObjectPositionY ( g_pCar->m_iID ), dbObjectPositionZ ( g_pCar->m_iID ), 0, 30, 15, 50, 0 );
		}
		break;

		case eCameraSide:
		{
			// side on
			dbSetCameraToFollow ( dbObjectPositionX ( g_pCar->m_iID ), dbObjectPositionY ( g_pCar->m_iID ), dbObjectPositionZ ( g_pCar->m_iID ), 90, 40, 15, 50, 0 );
		}
		break;

		case eCameraOverhead:
		{
			// over head
			dbSetCameraToFollow ( dbObjectPositionX ( g_pCar->m_iID ), dbObjectPositionY ( g_pCar->m_iID ), dbObjectPositionZ ( g_pCar->m_iID ), 120, 80, 45, 50, 0 );
		}
		break;
	}
}


void ControlCars ( void )
{
	// this function will control the player and robo car

	// call the function to drive the player car and update it
	if ( g_pCar )
	{
		g_pCar->Drive  ( );
		g_pCar->Update ( );
	}
}

void UpdateApplication ( void )
{
	// update the application with calls to display the frame rate,
	// handle user input, update the physics and draw to the screen
	showFPS     ( );
	dbMeqUpdate ( );
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

	
	/*
	if ( dbMeqRigidBodyGetCollision ( 500, 900 ) )
	{
		dbPrint ( "" );
		dbPrint ( "collision between car and cube" );
	}
	*/
}

void showCarInfo ( void )
{
	// show information about the car

	// character array to store the text
	char szText [ 255 ] = "";

	// move the cursor to the right side of the screen
	dbSetCursor ( 500, 0 );

	// speed of the car
	sprintf ( szText, "speed = %f km/h", g_pCar->m_fSpeed );
	dbText ( 800, 10, szText );
	
	// rpm of the car
	sprintf ( szText, "rpm   = %f", g_pCar->m_fRPM );
	dbText ( 800, 30, szText );

	// gear that the car is currently in
	sprintf ( szText, "gear  = %i", g_pCar->m_iGear );
	dbText ( 800, 50, szText );
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