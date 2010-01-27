
// TimeTest.cpp : The main entrypoint for the application.
//
#define INITGUID
#include <dxdiag.h>

#include "DarkSDK.h"
#include "resource.h"
#include "stdio.h"
#include "stdlib.h"
#include "io.h"

#include <list>
using namespace std;



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


int g_iGo = 0;

int g_iID               = -1;
int g_iBodyType         = -1;
int g_iType             = -1;
int g_iColliderID       = -1;
int g_iColliderBodyType = -1;
int g_iColliderType     = -1;

// main program
void DarkSDK ( void )
{

	vector < int > a;

	a.push_back ( 1 );
	a.push_back ( 2 );
	a.push_back ( 3 );

	int d = 0;

	a.erase ( &a [ 1 ] );

	d = 0;


	//dbLoadObject ( "test.dbo", 1 );

	//dbMakeObjectCube ( 1, 1 );
	//dbLoadImage ( "test.bmp", 1 );
	//dbTextureObject ( 1, 1 );
	
	//dbSetObjectCull ( 1, 0 );
	//dbSetObjectWireframe ( 1, 1 );

	


	//dbMakeObjectCube ( 2, 1 );
	//dbSaveObject ( "test1.dbo", 2 );
	//dbDeleteObject ( 2 );
	//dbLoadObject ( "test1.dbo", 2 );

	SetBitmapFormat ( 6 );

	// set up general application properties, the scenery and last of all the cars
	if ( !SetupApplication ( ) || !SetupScenery ( ) || !SetupCars ( ) )
		return;

	
	int iState = 0;

	//dbPositionCamera ( 0, 0, -800 );

	int iZ = 0;

	// loop round until the escape key is pressed
	while ( !dbEscapeKey ( ) )
	{
		// call all the relvant functions for the loop
		showFPS     ( );
		userInput			( );
		ControlCars			( );

		while ( dbMeqGetCollisionData ( ) )
		{
			g_iID               = dbMeqGetCollisionID       ( );
			g_iBodyType         = dbMeqGetCollisionBodyType ( );
			g_iType             = dbMeqGetCollisionType     ( );
			g_iColliderID       = dbMeqGetColliderID        ( );
			g_iColliderBodyType = dbMeqGetColliderBodyType  ( );
			g_iColliderType     = dbMeqGetColliderType      ( );
		}

		char szInfo [ 256 ] = "";

		dbPrint ( "" );
		
		sprintf ( szInfo, "iID = %i", g_iID );								dbPrint ( szInfo );
		sprintf ( szInfo, "iBodyType = %i", g_iBodyType );					dbPrint ( szInfo );
		sprintf ( szInfo, "iType = %i", g_iType );							dbPrint ( szInfo );
		sprintf ( szInfo, "iColliderID = %i", g_iColliderID );				dbPrint ( szInfo );
		sprintf ( szInfo, "iColliderBodyType = %i", g_iColliderBodyType );	dbPrint ( szInfo );
		sprintf ( szInfo, "iColliderType = %i", g_iColliderType );			dbPrint ( szInfo );

		UpdateApplication	( );

		/*
		dbPositionCamera ( dbObjectPositionX ( g_pCar->m_iID ) , dbObjectPositionY ( g_pCar->m_iID ) + 8 , dbObjectPositionZ ( g_pCar->m_iID ) );
		dbSetCameraToObjectOrientation (  g_pCar->m_iID );
		dbMoveCamera (  -0.7f );
		dbYRotateCamera (  dbCameraAngleY ( ) + 90 );
		dbMoveCamera ( 3.0f );
		dbYRotateCamera (  dbCameraAngleY ( ) - 90 );
		
		
		dbHideObject ( g_pCar->m_iID );
		dbPositionObject ( 13, dbObjectPositionX ( g_pCar->m_iID ) , dbObjectPositionY ( g_pCar->m_iID ) + 6 , dbObjectPositionZ ( g_pCar->m_iID ) );
		dbYRotateObject ( 13, dbObjectAngleY ( g_pCar->m_iID ) );
		//dbSetObjectToObjectOrientation ( 13, 500 );
		*/

		if ( dbMeqRigidBodyGetCollision ( 500, 8 ) ) dbMeqRigidBodyResetLocalOrigin ( 8 );
		if ( dbMeqRigidBodyGetCollision ( 500, 9 ) ) dbMeqRigidBodyResetLocalOrigin ( 9 );
		if ( dbMeqRigidBodyGetCollision ( 500, 11 ) ) dbMeqRigidBodyResetLocalOrigin ( 11 );
		if ( dbMeqRigidBodyGetCollision ( 500, 12 ) ) dbMeqRigidBodyResetLocalOrigin ( 12 );
		
		

		iState = 99999;
		
		if ( iState == 0 )
		{
			dbPositionObject ( 10, dbObjectPositionX ( 10 ) , dbObjectPositionY ( 10 ) , dbObjectPositionZ ( 10 ) + 0.5f );

			dbMeqPointCreate ( 1 );
			dbMeqPointSet ( 1, dbObjectPositionX ( 10 ) , dbObjectPositionY ( 10 ) , dbObjectPositionZ ( 10 ) );
			
			float fDistance = dbMeqPointGetDistanceTo ( 1, dbObjectPositionX ( g_pCar->m_iID ), dbObjectPositionY ( g_pCar->m_iID ), dbObjectPositionZ ( g_pCar->m_iID ) );

			if ( fDistance < 40 )
			{
				iState++;
			}
		}

		if ( iState == 1 )
		{
			// set the origin to match the position of the car
			dbMeqRigidBodySetOrigin  ( 10, dbObjectPositionX ( 10 ) , dbObjectPositionY ( 10 ), dbObjectPositionZ ( 10 ) );

			// enable matrix updates, set dynamic state to true and switch active on
			dbMeqRigidBodySetUpdate  ( 10, true );
			dbMeqRigidBodySetDynamic ( 10, true );
			//dbMeqRigidBodySetActive  ( 10, true );

			// add velocity in the direction the car is facing
			dbMeqRigidBodySetVelocity ( 10, 0, 0.0, 70.0f );

			iState++;
		}
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

	// dbMeqSetCollisionEpsilon ( dbMeqGetCollisionEpsilon ( ) * 100.0f );

	dbMeqSetCollisionEpsilon ( 0.99 );

	// need to see if car has hit kerb
	// need to see if car has hit bin
	// need to adjust centre of origin for models back to 0, 0, 0 when a collision takes place ( for bins )
	// check cars can be deleted and recreated
	

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

	

	//dbLoadObject ( "test.dbo", 1 );
	//dbLoadObject ( "corsa_2.X", 1 );

	dbSetDir ( "models" );

	
	
	// motor bike
	//dbLoadObject ( "8000012.dbo", 2 );

	// curved piece
	//dbLoadObject ( "1001.dbo", 2 );
	//dbPositionObject ( 2, -500, 0, 0 );

	// curved piece
	//dbLoadObject ( "1002.dbo", 3 );

	dbLoadObject ( "1000.dbo", 1 );			// roundabout
	dbLoadObject ( "70000.dbo", 2 );		// roundabout path
	//dbLoadObject ( "80000.dbo", 3 );		// hedge
	
	
	dbSetObjectCull ( 2, 0 );
	dbSetObjectCull ( 3, 0 );

	dbMeqRigidBodyMeshCreate  ( 1, 1 );
	dbMeqRigidBodySetDynamic ( 1, 0 );

	dbMeqRigidBodyMeshCreate  ( 2, 2 );
	dbMeqRigidBodySetDynamic ( 2, 0 );
	dbMeqRigidBodySetType ( 2, 100 );

	//dbMeqRigidBodyMeshCreate  ( 3, 3 );
	//dbMeqRigidBodySetDynamic ( 3, 0 );

	
	
	// house
	//dbLoadObject ( "68000.dbo", 6 );
	dbLoadObject ( "semi_1.X", 6 );
	dbRotateObject ( 6, 0, 90, 0 );
	dbPositionObject ( 6, 75, 1, 200 );
	dbMeqRigidBodyBoxCreate  ( 6, 6 );
	dbMeqRigidBodySetDynamic ( 6, 0 );

	dbLoadObject ( "semi_1.X", 7 );
	dbRotateObject ( 7, 0, 90, 0 );
	dbPositionObject ( 7, 75, 1, 119 );
	dbMeqRigidBodyBoxCreate  ( 7, 7 );
	dbMeqRigidBodySetDynamic ( 7, 0 );
	
	//dbLoadObject ( "68030.dbo", 8 );
	dbLoadObject ( "bigwheelie.X", 8 );
	dbPositionObject ( 8, -75, 1, -100 );

	dbLoadObject ( "bigwheelie.X", 9 );
	dbPositionObject ( 9, -50, 1, -100 );

	dbLoadObject ( "hatchback2.x", 10 );
	dbPositionObject ( 10, 0, 1, -220 );
	dbMeqRigidBodyBoxCreate  ( 10, 10 );
	//dbMeqRigidBodySetMass ( 10, 1750 );
	dbMeqRigidBodySetUpdate  ( 10, false );		// do not update world matrix
	dbMeqRigidBodySetActive  ( 10, false );		// set active off
	dbMeqRigidBodySetDynamic ( 10, false );		// set dynamic off
	
	
	dbLoadObject ( "bin1.x", 12 );
	dbPositionObject ( 12, -80, 1, 170 );
	//dbPositionObject ( 12, 0, 1, 140 );

	//0, 15, 160

	
	dbMeqRigidBodyBoxCreate  ( 12, 12 );
	
	//dbMeqRigidBodyConvexCreate ( 11, 11 );
	//dbMeqRigidBodyConvexCreate ( 12, 12 );

	//dbMeqRigidBodySetTranslation ( 11, 0, 0, 0 );
	//dbMeqRigidBodySetOrigin ( 12, -80, 1, 170 );

	
	

	dbMeqRigidBodyBoxCreate  ( 8, 8 );

	//dbMeqRigidBodySetMass ( 8, 1 );

	dbMeqRigidBodyBoxCreate  ( 9, 9 );


	dbLoadObject ( "bin1.x", 11 );
	dbPositionObject ( 11, -80, 1, 180 );
	dbMeqRigidBodyBoxCreate  ( 11, 11 );
	

	dbLoadObject ( "interior.x", 13 );
	dbScaleObject ( 13, 300, 300, 300 );
	//dbPositionObject ( 12, -80, 1, 170 );
	

	dbSetDir ( ".." );
	
	return true;
}

bool SetupCars ( void )
{
	// set up the cars

	if ( !( g_pCar = new cCar ( 500, ".\\mycar.meqv", 0, 15, 160 ) ) )
		return false;

	delete g_pCar;
	g_pCar = NULL;

	if ( !( g_pCar = new cCar ( 500, ".\\mycar.meqv", 0, 15, 160 ) ) )
		return false;
	

	dbMeqCarRotate ( 500, 0, -180, 0 );

	dbGhostObjectOn ( 500, 0 );
	dbSetObjectCull ( 500, 0 );
	

	return true;
}

void ControlCars ( void )
{
	// this function will control the player and robo car

	//return;

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