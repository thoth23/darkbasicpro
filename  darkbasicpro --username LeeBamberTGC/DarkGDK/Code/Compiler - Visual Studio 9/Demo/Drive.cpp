
#define INITGUID
#include <dxdiag.h>

#include "DarkSDK.h"
#include "resource.h"
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
float		g_fSpeed			= 0.1f;				// speed at which camera moves
float		g_fTurn				= 0.03f;			// how quickly the camera turns
float		g_fOldCamAngleX		= 0.0f;				// used to store old x angle
float		g_fOldCamAngleY		= 0.0f;				// used to store old x angle
float		g_fCameraAngleX		= 0.0f;				// current x angle
float		g_fCameraAngleY		= 0.0f;				// current y angle
cCar*		g_pCar				= NULL;				// player controlled car


// function listings
bool		SetupApplication	( void );					// used to set up general application properties like sync rate
bool		SetupScenery		( void );					// will set up a scene to be used
bool		SetupCars			( void );					// will set up player and robo cars
bool		SetupSliders		( void );					// set up the sliders
void		ControlCars			( void );					// function to control the cars
void		UpdateApplication	( void );					// updates the application with functions like sync
void		showFPS				( void );					// show the frame rate on screen
void		showCarInfo			( void );					// display information about the car like it's speed
void		ControlCamera		( void );					// handle the camera
void		userInput			( void );
sObject*	dbGetObject			( int iID );

int g_iID               = -1;
int g_iBodyType         = -1;
int g_iType             = -1;
int g_iColliderID       = -1;
int g_iColliderBodyType = -1;
int g_iColliderType     = -1;


// main program
void DarkSDK ( void )
{
	// set up general application properties, the scenery and last of all the cars
	if ( !SetupApplication ( ) || !SetupScenery ( ) || !SetupCars ( ) )
		return;

	dbColorBackdrop ( 0 );

	int iState = 0;

	// working
	dbMeqRigidBodyResetLocalOrigin ( 8 );
	dbMeqRigidBodyResetLocalOrigin ( 9 );
	dbMeqRigidBodyResetLocalOrigin ( 11 );
	dbMeqRigidBodyResetLocalOrigin ( 12 );
	

	// set all bins to static
	// when car collides with bins
	// set bins to dynamic


	/*
	dbMeqRigidBodySetActive ( 8, false );
	dbMeqRigidBodySetActive ( 9, false );
	dbMeqRigidBodySetActive ( 11, false );
	dbMeqRigidBodySetActive ( 12, false );

	for ( int l = 0; l < 100; l++ )
	{
		//dbMeqUpdate ( );
		//dbSleep ( 1 );
	}
	*/

	// working
	/*
	dbMeqRigidBodySetDynamic ( 8, false );
	dbMeqRigidBodySetDynamic ( 9, false );
	//dbMeqRigidBodySetDynamic ( 11, false );
	//dbMeqRigidBodySetDynamic ( 12, false );
	*/
	
	iState = 99;

	//dbMakeObjectCube ( 5000, 1 );
	dbMakeObjectCube ( 5000, 10 );
	dbMakeObjectBox ( 5001, 20, 1, 20 );
	dbMakeObjectCube ( 5002, 1 );
	dbMakeObjectCube ( 5003, 1 );
	
	
	int iOffset = 0;

	/*
	dbPositionObject ( 5000, 0 + iOffset, 16, 160 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 5000, 5000 )

	dbPositionObject ( 5001, 0 + iOffset, 10, 160 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 5001, 5001 );
	dbMeqRigidBodySetDynamic ( 5001, false );

	//dbMeqRigidBodySetActive ( 5000, false );

	//	dbMeqRigidBodySetActive ( 5000, true );
	dbMeqRigidBodySetDeactivationThreshold ( 5000, 1.0f );
	*/
	

	//dbMeqSetGravity ( 0.01f );

	/*
	dbMeqSetAngularDamping ( float fValue );
	dbMeqSetDeactivationThreshold ( float fValue );
	dbMeqSetDeactivationTimer ( float fValue );
	dbMeqSetLinearDamping ( float fValue );
	dbMeqSetRestVelocity ( float fValue );
	dbMeqSetErrorReduction ( float fValue );
	dbMeqSetMaximumIterations ( int iValue );
	dbMeqSetRotationEpsilon ( float fValue );
	dbMeqSetTolerance ( float fValue );
	*/

	/*
	dbMeqSetDeactivationThreshold ( 0.8f );
	dbMeqSetDeactivationTimer ( 0.1f );
	dbMeqSetRestVelocity ( 1.0f );
	dbMeqSetErrorReduction ( 10.0f );
	dbMeqSetMaximumIterations ( 100 );
	dbMeqSetTolerance ( 5.0f );

	dbMeqSetFrictionMode ( 0 );
	*/

	//dbMeqSetRestVelocity ( 0.6f );
	//dbMeqSetDeactivationThreshold ( 2.0f );


	//if ( !( g_pCar = new cCar ( 500, ".\\mycar.meqv", 10 + iOffset, 15, 210 + iOffset ) ) )
	

	
	//dbMeqRigidBodySetActive ( 501, false );
	//dbMeqRigidBodySetDynamic ( 5000, false );
	
	//dbMakeObjectSphere ( 5000, 1 );

	//dbMakeObjectSphere ( 5000, 1 );

	

	//dbMeqRigidBodyRayCastAdd ( 1 );
	//dbMeqRigidBodyRayCastAdd ( 2 );

	
	dbMakeObjectCube ( 501, 15 );
	dbPositionObject ( 501, -90, 35, 210 );
	dbRotateObject ( 501, 0, 90, 0 );
	dbMeqRigidBodyBoxCreate ( 501, 501 );
	dbMeqRigidBodySetUpdate  ( 501, false );

	// loop round until the escape key is pressed
	while ( !dbEscapeKey ( ) )
	{
		// call all the relvant functions for the loop
		showFPS     ( );
		userInput	( );
		UpdateApplication ( );
		ControlCars	( );

		//int iHit = dbMeqRigidBodyGetRayCast ( 9974, 10, 10114, 0, -1, 0 );

		
		//int iHit = dbMeqRigidBodyGetRayCast ( dbObjectPositionX ( 500 ), dbObjectPositionY ( 500 ) - 5, dbObjectPositionZ ( 500 ), 0, -1, 0 );
		//int iHit = dbMeqRigidBodyGetRayCast ( dbObjectPositionX ( 500 ), dbObjectPositionY ( 500 ) + 100, dbObjectPositionZ ( 500 ), 0, -1, 0 );
		//float fDistance = dbMeqRigidBodyGetRayCastDistance ( );
		//dbPositionObject ( 5000, dbMeqRigidBodyGetRayCastHitPointX ( ), dbMeqRigidBodyGetRayCastHitPointY ( ), dbMeqRigidBodyGetRayCastHitPointZ ( ) );

		/*
		char szInfo [ 256 ] = "";
		//sprintf ( szInfo, "iID = %i", dbMeqRigidBodyGetRayCastID ( ) );
		//dbPrint ( szInfo );

		

		while ( dbMeqGetCollisionData ( ) )
		{
			g_iID               = dbMeqGetCollisionID       ( );
			g_iBodyType         = dbMeqGetCollisionBodyType ( );
			g_iType             = dbMeqGetCollisionType     ( );
			g_iColliderID       = dbMeqGetColliderID        ( );
			g_iColliderBodyType = dbMeqGetColliderBodyType  ( );
			g_iColliderType     = dbMeqGetColliderType      ( );

			// activate dynamic objects
			//if ( g_iID == 500 && ( g_iColliderID >= 8 && g_iColliderID <= 12 ) )
			//	dbMeqRigidBodySetDynamic ( g_iColliderID, true );
		}

		

		dbPrint ( "" );
		
		sprintf ( szInfo, "iID = %i", g_iID );								dbPrint ( szInfo );
		sprintf ( szInfo, "iBodyType = %i", g_iBodyType );					dbPrint ( szInfo );
		sprintf ( szInfo, "iType = %i", g_iType );							dbPrint ( szInfo );
		sprintf ( szInfo, "iColliderID = %i", g_iColliderID );				dbPrint ( szInfo );
		sprintf ( szInfo, "iColliderBodyType = %i", g_iColliderBodyType );	dbPrint ( szInfo );
		sprintf ( szInfo, "iColliderType = %i", g_iColliderType );			dbPrint ( szInfo );
		*/
		

		if ( dbSpaceKey ( ) )
		{
			dbMeqRigidBodySetAngularVelocity ( 5000, 0.0f, 0.0f, 0.0f );
			dbMeqRigidBodySetVelocity ( 5000, 0.0f, 0.0f, 0.0f );
		}

		//dbMeqRigidBodySetOrigin  ( 501, dbObjectPositionX ( 500 ) - 7.6, dbObjectPositionY ( 500 ), dbObjectPositionZ ( 500 ) );
		//dbMeqRigidBodySetDynamic ( 501, false );
		//dbMeqRigidBodySetActive ( 501, false );

		/*
		dbPositionObject ( 5000, dbObjectPositionX ( 500 ) , dbObjectPositionY ( 500 ) , dbObjectPositionZ ( 500 ) );

		// reset origin of dynamic objects when the car hits them
		//if ( dbMeqRigidBodyGetCollision ( 500,  8 ) ) dbMeqRigidBodyResetLocalOrigin ( 8 );
		//if ( dbMeqRigidBodyGetCollision ( 500,  9 ) ) dbMeqRigidBodyResetLocalOrigin ( 9 );
		if ( dbMeqRigidBodyGetCollision ( 500, 11 ) )
		{
			int d = 0;
			//dbMeqRigidBodyResetLocalOrigin ( 11 );
		}
		
		if ( dbMeqRigidBodyGetCollision ( 500, 12 ) )
		{
			int d = 0;
			//dbMeqRigidBodyResetLocalOrigin ( 12 );
		}
		
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
			
			// add velocity in the direction the car is facing
			dbMeqRigidBodySetVelocity ( 10, 0, 0.0, 70.0f );

			iState++;
		}

		if ( dbSpaceKey ( ) )
		{
			dbMeqRigidBodySetDynamic ( 8, true );
			dbMeqRigidBodySetDynamic ( 9, true );
			dbMeqRigidBodySetDynamic ( 11, true );
			dbMeqRigidBodySetDynamic ( 12, true );
			
			//break;
		}
		*/
	}

	// delete everything
	for ( int i = 0; i < 10000; i++ )
	{
		if ( dbObjectExist ( i ) ) 
			dbDeleteObject ( i );
	}

	delete g_pCar;
	g_pCar = NULL;

	dbMeqClear ( );

	

	//SetupScenery ( );
	//SetupCars    ( );

	// loop round until the escape key is pressed
	while ( !dbEscapeKey ( ) )
	{
		// call all the relvant functions for the loop
		showFPS     ( );
		userInput	( );
		UpdateApplication ( );
		ControlCars	( );

		if ( dbSpaceKey ( ) )
		{
			dbMeqClear ( );
			break;
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
	//dbPositionCamera		( 1, 65, 175 );			// position the camera
	dbSetNormalizationOn	( );					// set normalization on due to scaling of cars
	dbMeqStart				( );					// start meqon physics

	//dbMeqSetCollisionEpsilon ( 0.99 );

	dbPositionCamera ( 0, 0, 0 );

	//dbMeqSetWorldScaling ( 10 );

	return true;
}

bool SetupScenery ( void )
{
	dbSetDir ( "models" );

	dbLoadObject ( "1000.dbo", 1 );
	dbLoadObject ( "70000.dbo", 2 );
	dbSetObjectCull ( 2, 0 );
	dbSetObjectCull ( 3, 0 );

	int iOffset = 0;


	dbPositionObject ( 1, 0 + iOffset, 0, 0 + iOffset );
	dbPositionObject ( 2, 0 + iOffset, 0, 0 + iOffset );

dbMeqRigidBodyMeshCreate  ( 1, 1 );
dbMeqRigidBodySetDynamic ( 1, 0 );
dbMeqRigidBodySetActive ( 1, false );

	dbMeqRigidBodyMeshCreate  ( 2, 2 );
	dbMeqRigidBodySetDynamic ( 2, 0 );
	dbMeqRigidBodySetType ( 2, 100 );

	
	dbMeqRigidBodySetActive ( 2, false );

	// house
	dbLoadObject ( "semi_1.X", 6 );
	dbRotateObject ( 6, 0, 90, 0 );
	dbPositionObject ( 6, 75 + iOffset, 1, 200 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 6, 6 );
	dbMeqRigidBodySetDynamic ( 6, 0 );
	//dbMeqRigidBodySetActive  ( 6, false );

	dbLoadObject ( "semi_1.X", 7 );
	dbRotateObject ( 7, 0, 90, 0 );
	dbPositionObject ( 7, 75 + iOffset, 1, 119 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 7, 7 );
	dbMeqRigidBodySetDynamic ( 7, 0 );
	//dbMeqRigidBodySetActive  ( 7, false );
	
	dbLoadObject ( "bigwheelie.X", 8 );
	dbPositionObject ( 8, -75 + iOffset, 1, -100 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 8, 8 );
	
	dbLoadObject ( "bigwheelie.X", 9 );
	dbPositionObject ( 9, -50 + iOffset, 1, -100 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 9, 9 );

	dbLoadObject ( "hatchback2.x", 10 );
	dbPositionObject ( 10, 0 + iOffset, 1, -220 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 10, 10 );
	dbMeqRigidBodySetUpdate  ( 10, false );
	dbMeqRigidBodySetActive  ( 10, false );
	dbMeqRigidBodySetDynamic ( 10, false );
	
	dbLoadObject ( "bin1.x", 11 );
	dbPositionObject ( 11, -80 + iOffset, 1, 180 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 11, 11 );

	dbLoadObject ( "bin1.x", 12 );
	dbPositionObject ( 12, -80 + iOffset, 1, 170 + iOffset );
	dbMeqRigidBodyBoxCreate  ( 12, 12 );

	dbMeqRigidBodySetType ( 11, 999 );
	dbMeqRigidBodySetType ( 12, 999 );
	
	//dbLoadObject ( "interior.x", 13 );
	//dbScaleObject ( 13, 300, 300, 300 );
	//dbPositionObject ( 12, -80, 1, 170 );
	
	
	dbSetDir ( ".." );
	
	return true;
}

bool SetupCars ( void )
{
	return true;
	
	//return true;

	// set up the cars

		int iOffset = 0;

	//if ( !( g_pCar = new cCar ( 500, ".\\mycar.meqv", 0 + iOffset, 15, 160 + iOffset ) ) )
		if ( !( g_pCar = new cCar ( 500, ".\\mycar.meqv", 10 + iOffset, 15, 210 + iOffset ) ) )
		return false;

	//delete g_pCar;
	//g_pCar = NULL;

	//if ( !( g_pCar = new cCar ( 500, ".\\mycar.meqv", 0, 15, 160 ) ) )
	//	return false;

	dbMeqCarRotate ( 500, 0, -180, 0 );

	dbGhostObjectOn ( 500, 0 );
	dbSetObjectCull ( 500, 0 );

	//dbMakeObjectCube ( 30 , 100 );
	//dbGlueObjectToLimb ( 30 , 1 , 0 );

	/*
	dbMakeObjectBox ( 501, 1, 5, 5 );
	dbPositionObject ( 501, -7.4 + iOffset, 4, 160 + iOffset );
	dbMeqRigidBodyBoxCreate ( 501, 501 );
	dbMeqRigidBodySetActive ( 501, false );
	dbMeqRigidBodySetDynamic ( 501, false );
	*/
	
	return true;

	//int iOffset = 10000;

	/*
	dbLoadObject ( "corsa_2.x", 500 );
	dbPositionObject ( 500, 0 + iOffset, 10, 160 + iOffset );
	dbRotateObject ( 500, 0, -180, 0 );
	dbScaleObject ( 500, 700, 700, 700 );

	dbMeqRigidBodyBoxCreate ( 500, 500 );
	*/

	//dbMakeObjectSphere ( 500, 20 );
	dbMakeObjectBox ( 500, 10, 5, 25 );
	dbPositionObject ( 500, 0 + iOffset, 10, 160 + iOffset );
	dbMeqRigidBodyBoxCreate ( 500, 500 );

	iOffset = 0;

	dbMakeObjectSphere ( 501, 5 );
	dbPositionObject ( 501, -5 + iOffset, -5, 15 + iOffset );
	
	dbMakeObjectSphere ( 502, 5 );
	dbPositionObject ( 502, 5 + iOffset, -5, 15 + iOffset );
	

	dbMakeObjectSphere ( 503, 5 );
	dbPositionObject ( 503, -5 + iOffset, -5, -15 + iOffset );

	dbMakeObjectSphere ( 504, 5 );
	dbPositionObject ( 504, 5 + iOffset, -5, -15 + iOffset );

	dbMeqRigidBodyCreate ( 501 );
	dbMeqRigidBodyCreate ( 502 );
	dbMeqRigidBodyCreate ( 503 );
	dbMeqRigidBodyCreate ( 504 );

	dbMeqRigidBodySphereInsert ( 500, 501, 501 );
	dbMeqRigidBodySphereInsert ( 500, 502, 502 );
	dbMeqRigidBodySphereInsert ( 500, 503, 503 );
	dbMeqRigidBodySphereInsert ( 500, 504, 504 );

	//dbMeqRigidBodyResetLocalOrigin ( 500 );

	return true;
}

void ControlCars ( void )
{
	sObject* pObject = dbGetObject ( 501 );

	// pObject->position.vecPosition += ( fStep * pObject->position.vecLook );

	static bool bCollision = false;


	if ( bCollision == false && dbKeyState ( 17 ) == 1 )
	{
		dbMoveObject ( 501, 5.1f );

		dbMeqRigidBodySetOrigin ( 501, dbObjectPositionX ( 501 ) , dbObjectPositionY ( 501 ), dbObjectPositionZ ( 501 ) );
		dbMeqRigidBodySetRotation ( 501, dbObjectAngleX ( 501 ) , dbObjectAngleY ( 501 ) , dbObjectAngleZ ( 501 ) );

		/*
		D3DXVECTOR3 vecOldPosition = pObject->position.vecPosition;
		D3DXVECTOR3 vecNewPosition = pObject->position.vecPosition += ( 20.0f * pObject->position.vecLook );
		D3DXVECTOR3 vecDir         = vecNewPosition - vecOldPosition;
	
		dbMeqRigidBodySetVelocity ( 501, vecDir.x, vecDir.y, vecDir.z );
		*/
	}

	char szInfo [ 256 ] = "";
	
	while ( dbMeqGetCollisionData ( ) )
	{
		g_iID               = dbMeqGetCollisionID       ( );
		g_iBodyType         = dbMeqGetCollisionBodyType ( );
		g_iType             = dbMeqGetCollisionType     ( );
		g_iColliderID       = dbMeqGetColliderID        ( );
		g_iColliderBodyType = dbMeqGetColliderBodyType  ( );
		g_iColliderType     = dbMeqGetColliderType      ( );

		//if ( g_iID == 501 && ( g_iColliderID == 6 || g_iColliderID == 7 ) )
		if ( g_iID == 6 )
		{
			bCollision = true;

			dbMeqRigidBodySetUpdate ( 501, true );
			dbMeqRigidBodySetDynamic ( 501, true );

			sObject* pObject = dbGetObject ( 501 );

			float fMove = 1;

			D3DXVECTOR3 vecOldPosition = pObject->position.vecPosition;
			D3DXVECTOR3 vecNewPosition = pObject->position.vecPosition += ( fMove * pObject->position.vecLook );
			D3DXVECTOR3 vecDir         = vecNewPosition - vecOldPosition;

			dbMeqRigidBodySetVelocity ( 501, vecDir.x, vecDir.y, vecDir.z );
			
			//dbMeqRigidBodySetDynamic ( g_iColliderID, true );
			
		}
	}

	

	dbPrint ( "" );
	
	sprintf ( szInfo, "iID = %i", g_iID );								dbPrint ( szInfo );
	sprintf ( szInfo, "iBodyType = %i", g_iBodyType );					dbPrint ( szInfo );
	sprintf ( szInfo, "iType = %i", g_iType );							dbPrint ( szInfo );
	sprintf ( szInfo, "iColliderID = %i", g_iColliderID );				dbPrint ( szInfo );
	sprintf ( szInfo, "iColliderBodyType = %i", g_iColliderBodyType );	dbPrint ( szInfo );
	sprintf ( szInfo, "iColliderType = %i", g_iColliderType );			dbPrint ( szInfo );

	return;
	
	static float fY = 0;

	//float difx = sin ( dbObjectAngleY ( 501 ) ) * 5;
	//float difz = cos ( dbObjectAngleY ( 501 ) ) * 5;

	float difx = sin ( 0.0f ) * 5;
	float difz = cos ( 0.0f ) * 5;

	float velx = 0;
	float vely = 0;
	float velz = 0;

	if ( dbKeyState ( 17 ) == 1 )
	{
		velx = velx + difx;
		velz = velz + difz;
	}

	if ( dbKeyState ( 31 ) == 1 )
	{
		velx = velx - difx;
		velz = velz - difz;
	}

	//dbMeqRigidBodySetRotation ( 501, 0, fY, 0 );

	if ( dbKeyState ( 30 ) )
		fY -= 0.5;

	if ( dbKeyState ( 32 ) )
		fY += 0.5;

	float currentVelX = dbMeqRigidBodyGetVelocityX ( 501 );
	float currentVelZ = dbMeqRigidBodyGetVelocityZ ( 501 );

	float accelX = ( velx - currentVelX ) * 5 * 10.0f;
	float accelZ = ( velz - currentVelZ ) * 5 * 10.0f;

	float x = dbObjectPositionX ( 501 );
	float y = dbObjectPositionY ( 501 );
	float z = dbObjectPositionZ ( 501 );

	dbMeqRigidBodyApplyForce ( 501, accelX, vely, accelZ );
	

	//dbMeqRigidBodySetRotation ( 500, 0, 0, 0 );
	

	/*
	if ( dbKeyState ( 17 ) == 1 )
	{
		dbMeqRigidBodySetVelocity ( 500,0,0, -30.0f );
	}
	*/
	
	/*
	if ( dbKeyState ( 30 ) )
	{
		fY -= 0.5;
		dbMeqRigidBodySetRotation ( 500, 0, fY, 0 );
	}

	if ( dbKeyState ( 32 ) )
	{
		fY += 0.5;
		dbMeqRigidBodySetRotation ( 500, 0, fY, 0 );
	}
	*/

	return;

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


/*
{
		dbSyncOn				( );
		dbSyncRate				( 0 );
		dbAutoCamOff			( );
		dbMeqStart				( );
		dbMeqSetGravity ( 1 );

		int iOffset = 10000;

		//dbMeqSetWorldScaling ( 10 );

		dbMakeObjectBox ( 1, 20, 1, 20 );
		dbPositionObject ( 1, 0 + iOffset, 0, 0 + iOffset );
		dbMeqRigidBodyBoxCreate ( 1, 1 );
		dbMeqRigidBodySetDynamic ( 1, 0 );
		dbMeqRigidBodySetActive ( 1, false );

		dbMakeObjectBox ( 2, 10, 1, 10 );
		dbPositionObject ( 2, 0 + iOffset, 5, 0 + iOffset );
		dbMeqRigidBodyBoxCreate ( 2, 2 );
		dbMeqRigidBodySetDynamic ( 2, 0 );
		dbMeqRigidBodySetActive ( 2, false );

		dbMakeObjectBox ( 3, 5, 5, 5 );
		dbPositionObject ( 3, 0 + iOffset, 15, 0 + iOffset );
		dbMeqRigidBodyBoxCreate ( 3, 3 );

		dbPositionCamera ( 10000, 0, 9975 );

		while ( !dbEscapeKey ( ) )
		{
			showFPS     ( );
			userInput	( );
			dbMeqUpdate ( );
			dbSync      ( );
		}
	}

	return;
*/


/*
void DarkSDK ( void )
{
	int		iID			= 2;
	int		xcount		= 9;
	int		zcount		= 1;
	int		levels		= 3;
	float	x			= 0.0f;
	float	z			= 0.0f;
	float	ballBreak	= 50;
	float	oriBreak	= 25;
	int		i, j, k, l  = 0;

	int		sticks  [ 250 ] = { 0 };
	int		xsticks [ 250 ] = { 0 };
	int		zsticks [ 250 ] = { 0 };
	
	dbSyncOn     ( );
	dbSyncRate   ( 0 );
	dbAutoCamOff ( );
	dbMeqStart   ( );

	dbMakeObjectBox				( 1, 12, 0.5, 10 );
	dbMeqRigidBodyBoxCreate		( 1, 1 );
	dbMeqRigidBodySetDynamic	( 1, 0 );

	dbMeqSetInteractionBlock ( 4, 4 );

	int iCounter = 0;

	// create the vertical sticks
	for ( l = 0; l < levels; l++ )
	{
		for ( j = 0; j < zcount; j++ )
		{
			for ( k = 0; k < xcount; k++ )
			{
				int index = l * xcount * zcount + j * xcount + k;

				x = k - xcount / 2.0f;
				z = j - zcount / 2.0f;

				dbMakeObjectBox				( iID, 0.1f, 0.95f, 0.1f );
				dbPositionObject			( iID, x, 0.5f + l, z );
				dbMeqRigidBodyBoxCreate		( iID, iID );
				dbMeqRigidBodySetType		( iID, 4 );

				sticks [ index ] = iID;

				iCounter++;

				iID++;
			}
		}
	}

	// cross joints
	for ( l = 0; l < levels; l++ )
	{
		for ( j = 0; j < zcount; j++ )
		{
			for ( k = 0; k < xcount - 1; k++ )
			{
				int index = l * xcount * zcount + j * xcount + k;

				x = k - xcount / 2.0f + 0.5f;
				z = j - zcount / 2.0f;

				dbMakeObjectBox				( iID, 0.1f, 0.95f, 0.1f );
				dbPositionObject			( iID, x, 1.0f + l, z );
				dbRotateObject				( iID, 0, 0, 90 );
				dbMeqRigidBodyBoxCreate		( iID, iID );
				dbMeqRigidBodySetType		( iID, 4 );

				xsticks [ index ] = iID;

				iID++;
			}
		}
	}

	// supports going into the scene on z
	for ( l = 0; l < levels; l++ )
	{
		for ( j = 0; j < zcount - 1; j++ )
		{
			for ( k = 0; k < xcount; k++ )
			{
				int index = l * xcount * zcount + j * xcount + k;

				x = k - xcount / 2.0f;
				z = j - zcount / 2.0f + 0.5f;

				dbMakeObjectBox				( iID, 0.1f, 0.95f, 0.1f );
				dbPositionObject			( iID, x, 1.0f + l, z );
				dbRotateObject				( iID, 90, 0, 0 );
				dbMeqRigidBodyBoxCreate		( iID, iID );
				dbMeqRigidBodySetType		( iID, 4 );

				zsticks [ index ] = iID;

				iID++;
			}
		}
	}

	iID = 1;
	
	for ( j = 0; j < zcount; j++ )
	{
		for ( k = 0; k < xcount; k++ )
		{
			int index = j * xcount + k;

			x = k - xcount / 2.0f;
			z = j - zcount / 2.0f;

			dbMeqFixedJointCreate ( iID++, 1, sticks [ index ], x, 0, z, 150 );

			//dbMeqFixedJointCreate ( int iID, int iObjectA, int iObjectB, float fX, float fY, float fZ, float fThreshold );
		}
	}

	// connect sticks with ball joints and fixed orientation joints
	for ( l = 0; l < levels - 1; l++ )
	{
		for ( j = 0; j < zcount; j++ )
		{
			for ( k = 0; k < xcount; k++ )
			{
				int index = l * xcount * zcount + j * xcount + k;

				x = k - xcount / 2.0f;
				z = j - zcount / 2.0f;
				
				dbMeqBallJointCreate			 ( iID++, sticks [ index ], sticks [ index + xcount * zcount ], x, l + 1.0f, z, ballBreak ); 
				dbMeqFixedOrientationJointCreate ( iID++, sticks [ index ], sticks [ index + xcount * zcount ], oriBreak );
			}
		}
	}
	
	// connect sticks with ball joints and fixed orientation joints
	for ( l = 0; l < levels; l++ )
	{
		for ( j = 0; j < zcount; j++ )
		{
			for ( k = 0; k < xcount - 1; k++ )
			{
				int index = l * xcount * zcount + j * xcount + k;
				
				x = k - xcount / 2.0f;
				z = j - zcount / 2.0f;

				dbMeqBallJointCreate ( iID++, xsticks [ index ], sticks [ index     ], x,        l + 1.0f, z, ballBreak );
				dbMeqBallJointCreate ( iID++, xsticks [ index ], sticks [ index + 1 ], x + 1.0f, l + 1.0f, z, ballBreak );

				dbMeqFixedOrientationJointCreate ( iID++, xsticks [ index ], sticks [ index ], oriBreak );
			}
		}
	}

	// connect sticks with ball joints and fixed orientation joints
	for (l=0; l<levels; l++)
	{
		for (j=0; j<zcount-1; j++)
		{
			for (k=0; k<xcount; k++)
			{
				int index = l*xcount*zcount + j*xcount + k;

				x = k - xcount/2.0f;
				z = j - zcount/2.0f;

				dbMeqBallJointCreate ( iID++, zsticks [ index ], sticks [ index          ], x, l + 1.0f, z,        ballBreak );
				dbMeqBallJointCreate ( iID++, zsticks [ index ], sticks [ index + xcount ], x, l + 1.0f, z + 1.0f, ballBreak );

				dbMeqFixedOrientationJointCreate ( iID++, zsticks [ index ], sticks [ index ], oriBreak );
			}
		}
	}
	
	for ( i = 500, k = 20; i < 1000; i++, k += 5 )
	{
		dbMakeObjectCube			( i, 0.4 );
		dbColorObject				( i, dbRgb ( dbRnd ( 255 ), dbRnd ( 255 ), dbRnd ( 255 ) ) );
		dbPositionObject			( i, -4 + dbRnd ( 8 ), k, -1 + dbRnd ( 1 ) );
		dbMeqRigidBodyBoxCreate		( i, i );
	}

	dbSetCameraToObjectOrientation ( 0, 1 );
	
	dbPositionCamera ( 0, 4, -8 );

	float a = 0.0f;
	float b = 0.0f;
	float c = 0.0f;
	
	while ( !dbEscapeKey ( ) )
	{
		a = dbWrapValue ( a + 1.5f );
		b = dbWrapValue ( b + 1.2f );
		c = dbWrapValue ( c + 1.05f );

		x = dbCos ( c ) * 8.0f;
		z = dbSin ( c ) * 10.0f;

		dbPositionLight ( 0, x, 4, z );

		dbPointLight ( 0, 0, 4, 0 );

		dbPositionCamera ( x, 4, z );
		dbPointCamera ( 0, 4, 0 );

		showFPS     ( );
		dbMeqUpdate ( );
		dbSync      ( );
	}
}
*/