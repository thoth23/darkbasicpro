

#define NOMINMAX
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "DarkSDK.h"
//#include "D:\Dark Basic Pro Shared\Projects\Expansion Packs\Meqon\Code\Meqon.h"
#include ".\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\DBOData.h"

#include "timer.h"

// IGNORE libc,msvcrtd

float g_fSpeed        = 0.1f;
float g_fTurn         = 0.03f;
float g_fOldCamAngleX = 0.0f;
float g_fOldCamAngleY = 0.0f;
float g_fCameraAngleX = 0.0f;
float g_fCameraAngleY = 0.0f;

void showFPS   ( void );
void userInput ( void );

#include <stdio.h>
#include "physxwrapper.h"

void CreateLevelMesh();
void TestCharacter ( void );
void TestMoveCharacter ( void );

sObject* dbGetObject ( int iID );

void InitCar();
NxActor* CreateCar(const NxVec3& pos);
void TickCar();

extern NxActor* car;

extern NxReal gSteeringDelta;
extern NxReal gMaxMotorPower;
extern NxReal gMaxSteeringAngle;
extern NxReal gMotorForce;
extern NxReal gSteeringValue;


void DarkSDK ( void )
{
	// entry point for the application

	//dbSetDir ( "media\\" );

	// initial application set up
	dbSyncOn         ( );				// turn sync on
	dbSyncRate       ( 0 );			// set sync rate to 60
	dbBackdropOn    ( );				// switch backdrop off - no need to clear screen
	dbSetCameraRange ( 0.5f, 30000 );	// set the camera range

	PhysStart ( );

	
		// floor
	dbMakeObjectBox			 ( 1, 100, 1, 100 );
	dbPositionObject		 ( 1, 0, 0, 0 );
	dbColorObject			 ( 1, dbRgb ( 100, 0, 0 ) );
	PhysCreateStaticRigidBodyMesh ( 1 );
	

	/*
	dbLoadObject ( "bin1.x", 2 );
	//dbPositionObject ( 2, 0, 1 + dbObjectCollisionCenterY ( 2 ), 0 );
	dbPositionObject ( 2, 0, 0, 0 );
	//dbOffsetLimb ( 2, 0, dbObjectCollisionCenterX ( 2 ) * -1, dbObjectCollisionCenterY ( 2 ) * -1, dbObjectCollisionCenterZ ( 2 ) * -1 );
	PhysCreateRigidBodyBox ( 2 );

	dbMakeObjectCube ( 3, 5 );
	dbPositionObject ( 3, 20, 10, 0 );
	PhysCreateRigidBodyBox ( 3 );
	*/



/*	const NxReal height = 0.3;
	const NxReal width = 2;
	const NxReal length = 4;

	dbMakeObjectBox ( 5, length*0.65, height*0.85, width*0.65 );

	InitCar ( );
	car = CreateCar(NxVec3(0,5,0));
*/
	

	dbPositionCamera ( 0, 20, -30 );


	dbLoadObject ( "corsa.x", 10 );
	dbPositionObject ( 10, 0, 1, 0 );
	PhysCreateRigidBodyBox ( 10 );
	
	// loop round until escape key is pressed
	while ( LoopSDK ( ) )
	{
		if ( dbEscapeKey ( ) )
			return;

		if ( dbSpaceKey ( ) )
			PhysSetVelocity ( 10, 0, 20, 0 );

		/*
		if ( dbSpaceKey ( ) )
			PhysSetVelocity ( 3, -20, 0, 0 );

		{
			bool bKey = false;

			if ( strcmp ( dbInKey ( ), "w" ) == 0 )
			{
				gMotorForce = gMaxMotorPower;
				bKey = true;
			}

			if ( strcmp ( dbInKey ( ), "s" ) == 0 )
			{
				gMotorForce = -gMaxMotorPower;
				bKey = true;
			}

			if ( strcmp ( dbInKey ( ), "a" ) == 0 )
			{
				if (gSteeringValue > (-1.0f + gSteeringDelta))
					gSteeringValue -= gSteeringDelta;

				bKey = true;
			}

			if ( strcmp ( dbInKey ( ), "d" ) == 0 )
			{
				if (gSteeringValue < (1.0f - gSteeringDelta))
					gSteeringValue += gSteeringDelta;

				bKey = true;
			}

			if ( !bKey )
			{
				gMotorForce = 0.0f;
				gSteeringValue = 0.0f;
			}

		}

		TickCar ( );
		*/

		// show the frame rate and handle user input
		showFPS   ( );
		userInput ( );

		PhysRun ( );
		PhysUpdate ( );

		// render everything
		dbSync ( );
	}
}


void showFPS ( void )
{
	char szFPS [ 256 ] = "";
	strcpy ( szFPS, "fps = " );
	strcat ( szFPS, dbStr ( dbScreenFPS ( ) ) );

	dbSetCursor ( 0, 0 );

	dbPrint ( szFPS );
	dbPrint ( "" );

	//strcpy ( szFPS, dbStr ( dbObjectAngleY ( 6000 ) ) );
	//dbPrint ( szFPS );
	
	
	strcpy ( szFPS, dbStr ( dbCameraPositionX ( ) ) );
	dbPrint ( szFPS );
	strcpy ( szFPS, dbStr ( dbCameraPositionY ( ) ) );
	dbPrint ( szFPS );
	strcpy ( szFPS, dbStr ( dbCameraPositionZ ( ) ) );
	dbPrint ( szFPS );

	/*
	dbPrint ( "" );
	strcpy ( szFPS, dbStr ( dbCameraAngleX ( ) ) );
	dbPrint ( szFPS );
	strcpy ( szFPS, dbStr ( dbCameraAngleY ( ) ) );
	dbPrint ( szFPS );
	strcpy ( szFPS, dbStr ( dbCameraAngleZ ( ) ) );
	dbPrint ( szFPS );
	*/

	
	dbPrint ( "" );
	strcpy ( szFPS, dbStr ( dbStatistic ( 1 ) ) );
	dbPrint ( szFPS );
	

	//dbPrint ( "" );
	//dbPrint ( dbStr ( dbKeyState ( ) ) );

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
