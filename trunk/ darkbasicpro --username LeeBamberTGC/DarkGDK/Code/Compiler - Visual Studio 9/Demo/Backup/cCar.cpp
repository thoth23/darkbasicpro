
#include "cCar.h"

cCar::sData::sData ( float a1, float b1, float c1, float d1 ) : a ( a1 ), b ( b1 ), c ( c1 ), d ( d1 ), p ( &a )
{
	// the constructor will copy all parameters passed in to the member variables, it will
	// also set the member variable p to point to the member variable a, we can then easily
	// move through the values instead of referencing a, b, c, d for loops
};

cCar::cCar ( int iID, char* szFile, float fX, float fY, float fZ )
{
	// car constructor, will create the car and set up default values

	// check file name is valid
	if ( !szFile )
		return;

	// change the scale of everything to suit meqon
	m_fScale = 10.0f;

	// store the ID of the car
	m_iID = iID;

	// retain the file name
	strcpy ( m_szFile, szFile );

	// adjust the gravity based on the scale of the car
	//dbMeqSetGravity ( m_fScale );

	float fY1 = dbMeqGetGravityY ( );

	//dbMeqSetGravity ( dbMeqGetGravityY ( ) * m_fScale );
	dbMeqSetGravity ( 100.0f );

	// attempt to load the car
	dbMeqCarLoad ( m_iID, m_iID, m_szFile, fX, fY, fZ );
	
	// the world scale, controls how values returned from meqon such as speed are handled e.g.
	// when a value of 40 is returned for speed it will be divided by 2
	m_fWorldScale				= 2.0f;

	// these flags all determine which functions can be called when the main Update function is
	// called, by default they are all true but can be turned off it they are not neede
	m_bControlSteering			= true;
	m_bApplyTorque				= true;
	m_bApplyAcceleration		= true;
	m_bApplyBrakes				= true;
	m_bApplyHandBrake			= true;

	// set the drive mode to 1 meaning front wheel drive, other options include mode 2 giving us
	// a rear wheel drive and also mode 3 for a 4 wheel drive, the maximum iterations is set to
	// a level of 5, increasing this will give us a much more accurate result at the impact of
	// more computations so slowing the frame rate
	m_iDriveMode				=  1;
	m_iMaximumIterations		= 10;

	// sort out the front and rear axis centre points and their extents
	m_fFrontAxisCentre			= new sData ( 0.0f, 0.2f,  8.25f, 0.0f );
	m_fFrontAxisExtent			= new sData ( 6.0f, 0.0f,  0.0f, 0.0f );
	m_fRearAxisCentre			= new sData ( 0.0f, 0.2f, -9.5f, 0.0f );
	m_fRearAxisExtent			= new sData ( 4.5f, 0.0f,  0.0f, 0.0f );

	// set lock values for front and rear diffential
	m_fFrontDifferentialLock	= 0.0f;
	m_fRearDifferentialLock		= 0.0f;

	// motor drive and brake constants
	m_fDriveConstant0			= 600.0f    * m_fScale * m_fScale;			// control how powerful the engine is
	m_fDriveConstant1			=   0.3f    * m_fScale * m_fScale;			// the velocity curve, lower values take longer to accelerate, higher is quicker
	m_fDriveConstant2			=   0.0f    * m_fScale * m_fScale;			// ?
	m_fBrakeConstant0			=  10.0f    * m_fScale * m_fScale;			// rate of slow down when brakes are used
	m_fBrakeConstant1			=   0.0f    * m_fScale * m_fScale;			// ?
	m_fBrakeConstant2			=   0.0005f * m_fScale * m_fScale;			// ?

	// alternative spring stiffness = 35000
	// alternative spring damping   =  4000

	// set up wheel radius, nominal spring length, stiffness and damping
	m_fWheelRadius                  = new sData (     2.1f,     2.1f,     2.1f,     2.1f );
	m_fWheelNominalSpringLength     = new sData (     6.0f,     6.0f,     6.0f,     6.0f );
	m_fWheelSpringStiffness         = new sData ( 45000.0f, 45000.0f, 45000.0f, 45000.0f );
	m_fWheelSpringDamping           = new sData ( 10000.0f, 10000.0f, 10000.0f, 10000.0f );
	
	// create a material for front wheels - static, dynamic, restitution, rolling, lateral stiffness, longitudinal stiffness
	m_fWheelStatic					= new sData (   1.5f * m_fScale,   1.5f * m_fScale,   1.5f * m_fScale,   1.5f * m_fScale );
	m_fWheelDynamic					= new sData (   1.4f * m_fScale,   1.4f * m_fScale,   1.4f * m_fScale,   1.4f * m_fScale);
	m_fWheelRestitution				= new sData (   0.0f * m_fScale,   0.0f * m_fScale,   0.0f * m_fScale,   0.0f * m_fScale );
	m_fWheelRolling					= new sData (   0.2f * m_fScale,   0.2f * m_fScale,   0.2f * m_fScale,   0.2f * m_fScale );
	m_fWheelLateralStiffness		= new sData (  20.0f * m_fScale,  20.0f * m_fScale,  20.0f * m_fScale,  20.0f * m_fScale );
	m_fWheelLongitudinalStiffness	= new sData ( 100.0f * m_fScale, 100.0f * m_fScale, 100.0f * m_fScale, 100.0f * m_fScale );

	// set up brake and hand brake levels
	m_fWheelBrakeTorque			    = new sData (  1000.0f,  1000.0f,  1000.0f,  1000.0f );
	m_fWheelHandBrakeTorque	        = new sData ( 30000.0f, 30000.0f, 30000.0f, 30000.0f );
	
	// initial brake levels, level of increment, delay between improving brakes and the
	// maximum brake level, by using these we can slowly increase the braking rather than
	// slamming them on straight away
	m_fWheelBrakeInitial		=  1000.0f;
	m_fWheelBrakeIncrement		=  1000.0f;
	m_fWheelBrakeDelay			=     0.01f;
	m_fWheelBrakeMaximum		= 10000.0f;

	// the initial steering angle is 0.0f meaning straight forward, bent, and then we have
	// the steering angle increments and limits, these can be changed to make the turning
	// much quicker or slowe
	m_fSteeringAngle			=     0.0f;
	m_fSteeringAngleIncrementA  =     0.04f;
	m_fSteeringAngleIncrementB  =     0.02f;
	m_fSteeringAngleLimitA      =     0.5f;
	m_fSteeringAngleLimitB      =    -0.5f;

	// this flag indicates reverse
	m_bReverse					= false;

	// speed, rpm and gear all set to 0
	m_fSpeed					=     0.0f;
	m_fRPM						=     0.0f;
	m_iGear						=     0;

	// use this to handle the gears, for this demo we'll use an automatic gear box
	// with 6 gears and a delay of 0.2
	m_bAutomaticGearBox			= true;
	m_iGearCount				=     6;
	m_fGearDelay				=     0.2f;
	m_fGearRatios				= new float [ m_iGearCount ];
	m_fGearRatios [ 0 ]			=    -0.03f;
	m_fGearRatios [ 1 ]			=     0.05f;
	m_fGearRatios [ 2 ]			=     0.1f;
	m_fGearRatios [ 3 ]			=     0.2f;
	m_fGearRatios [ 4 ]			=     0.27f;
	m_fGearRatios [ 5 ]			=     0.33f;

	// motor values, this works in the same way as the brakes with slowly increasing
	// the level of input applied instead of going straight to full throttle
	m_fMotorInput				=     0.0f;
	m_fMotorInputIncrement		=     0.1f;
	m_fMotorInputDelay			=     0.001f;

	// ackerman ratio is used to control difference between front and back steering
	m_fAckermanRatio			= 0.22f;

	// reset all timers
	m_WheelBrakeTimer.Reset ( );
	m_WheelBrakeTimer.Start ( );
	m_MotorInputTimer.Reset ( );
	m_MotorInputTimer.Start ( );

	// offsets for wheels
	m_fSide    [ 0 ] = -0.5f;
	m_fForward [ 0 ] = -3.0f;
	m_fSide    [ 1 ] =  0.5f;
	m_fForward [ 1 ] = -3.0f;
	m_fSide    [ 2 ] =  1.5f;
	m_fForward [ 2 ] = -1.0f;
	m_fSide    [ 3 ] = -1.5f;
	m_fForward [ 3 ] = -1.0f;

	// update the car with the new settings
	UpdateSettings ( );
}

cCar::~cCar ( )
{
	dbDeleteObject ( m_iID );

	dbMeqCarRemove ( m_iID );
}

void cCar::UpdateSettings ( void )
{
	// set the new drive mode
	dbMeqCarSetDriveMode ( m_iID, m_iDriveMode );

	// set maximum number of iterations, higher levels of iterations will result in
	// a more accurate simulation but will have an impact on the frame rate
	dbMeqCarSetMaximumIterations ( m_iID, m_iMaximumIterations );

	// sort out the front axis of the car
	dbMeqCarSetFrontAxis		 ( 
									m_iID,
									m_fFrontAxisCentre->a, m_fFrontAxisCentre->b, m_fFrontAxisCentre->c,
									m_fFrontAxisExtent->a, m_fFrontAxisExtent->b, m_fFrontAxisExtent->c
								 );

	// and now the rear axis
	dbMeqCarSetRearAxis			 ( 
									m_iID,
									m_fRearAxisCentre->a, m_fRearAxisCentre->b, m_fRearAxisCentre->c,
									m_fRearAxisExtent->a, m_fRearAxisExtent->b, m_fRearAxisExtent->c
								 );

	// set front and rear differential locks
	dbMeqCarSetFrontDifferentialLock ( m_iID, m_fFrontDifferentialLock );

	// now handle the motor drive and brake constants
	dbMeqCarSetMotorDriveConstant0 ( m_iID, m_fDriveConstant0 );
	dbMeqCarSetMotorDriveConstant1 ( m_iID, m_fDriveConstant1 );
    dbMeqCarSetMotorDriveConstant2 ( m_iID, m_fDriveConstant2 );
    dbMeqCarSetMotorBrakeConstant0 ( m_iID, m_fBrakeConstant0 );
    dbMeqCarSetMotorBrakeConstant1 ( m_iID, m_fBrakeConstant1 );
    dbMeqCarSetMotorBrakeConstant2 ( m_iID, m_fBrakeConstant2 );

	// update wheel properties
	for ( int i = 0; i < 4; i++ )
	{
		dbMeqCarSetWheelRadius				( m_iID, i, m_fWheelRadius->p				[ i ] );
		dbMeqCarSetWheelSpringStiffness		( m_iID, i, m_fWheelSpringStiffness->p		[ i ] );
		dbMeqCarSetWheelSpringDamping		( m_iID, i, m_fWheelSpringDamping->p		[ i ] );
		dbMeqCarSetWheelNominalSpringLength ( m_iID, i, m_fWheelNominalSpringLength->p	[ i ] );

		dbMeqCarSetWheelMaterial			( 
												m_iID,
												i,
												m_fWheelStatic->p				 [ i ],
												m_fWheelDynamic->p				 [ i ],
												m_fWheelRestitution->p			 [ i ],
												m_fWheelRolling->p				 [ i ],
												m_fWheelLateralStiffness->p		 [ i ],
												m_fWheelLongitudinalStiffness->p [ i ]
											);

		dbMeqCarSetWheelForwardOffset ( m_iID, i, m_fForward [ i ] );
		dbMeqCarSetWheelSideOffset    ( m_iID, i, m_fSide    [ i ] );
	}

	// set ackerman ratio
	dbMeqCarSetAckermanRatio ( m_iID, m_fAckermanRatio );

	// scale axle components
	dbMeqCarScaleAxle ( m_iID, m_fScale );

	// create an automatic or manual gear box
	if ( m_bAutomaticGearBox )
	{
		// the automatic gear box
		dbMeqCarCreateAutomaticGearBox ( m_iID, m_iGearCount, m_fGearDelay );
	}
	else
	{
		// the manual gear box
		dbMeqCarCreateManualGearBox ( m_iID, m_iGearCount );
	}

	// set all gear ratiors
	for ( int i = 0; i < m_iGearCount; i++ )
	{
		dbMeqCarSetGearBoxRatio ( m_iID, i, m_fGearRatios [ i ] );
	}
}

void cCar::Update ( void )
{
	// update the car speed, rpm and gear
	m_fSpeed = dbMeqCarGetSpeed ( m_iID ) / m_fWorldScale;
	m_fRPM   = dbMeqCarGetRPM   ( m_iID ) / m_fWorldScale;
	m_iGear  = dbMeqCarGetGear  ( m_iID );
}

void cCar::Drive ( void )
{
	// default implementation of driving a car using w, s, a and d keys

	if ( m_bControlSteering ) ControlSteering ( );
	if ( m_bApplyTorque     ) ApplyTorque     ( );

	//ApplyAcceleration ( );
	//return;

	// check to accelerate, slow down, apply handbrake or no input
	if ( dbKeyState ( 17 ) )
	{
		// state of acceleration
		if ( m_bApplyAcceleration )
			ApplyAcceleration ( );
	}
	else if ( dbKeyState ( 31 ) )
	{
		// state of braking
		if ( m_bApplyBrakes )
			ApplyBrakes ( );
	}
	else if ( dbKeyState ( 57 ) )
	{
		// state of handbrake when space key is pressed
		if ( m_bApplyHandBrake )
			ApplyHandBrake ( );
	}
	else
	{
		// state of no input

		// set clutch, motor input and gear to 0
		dbMeqCarSetClutch	  ( m_iID, 0 );
		dbMeqCarSetMotorInput ( m_iID, 0 );
		dbMeqCarSetGear		  ( m_iID, 0 );
	}
}

void cCar::ControlSteering ( void )
{
	// see which key has been pressed
	if ( dbKeyState ( 32 ) )
	{
		// steer right
		m_fSteeringAngle += ( m_fSteeringAngle < 0 ) ? m_fSteeringAngleIncrementA : m_fSteeringAngleIncrementB;
	}
	else if ( dbKeyState ( 30 ) )
	{
		// steer left
		m_fSteeringAngle -= ( m_fSteeringAngle > 0 ) ? m_fSteeringAngleIncrementA : m_fSteeringAngleIncrementB;
	}
	else
	{
		// level steering
		m_fSteeringAngle *= 0.95f;
	}

	// limit steering angle
	if ( m_fSteeringAngle > m_fSteeringAngleLimitA ) m_fSteeringAngle = m_fSteeringAngleLimitA;
	if ( m_fSteeringAngle < m_fSteeringAngleLimitB ) m_fSteeringAngle = m_fSteeringAngleLimitB;

	// set the steering angle
	dbMeqCarSetSteeringAngle ( m_iID, m_fSteeringAngle );
}

void cCar::ApplyTorque ( void )
{
	// set brake torque to 0 on all 4 wheels
	dbMeqCarSetWheelBrakeTorque ( m_iID, 0, 0 );
	dbMeqCarSetWheelBrakeTorque ( m_iID, 1, 0 );
	dbMeqCarSetWheelBrakeTorque ( m_iID, 2, 0 );
	dbMeqCarSetWheelBrakeTorque ( m_iID, 3, 0 );

	// update initial brake torque
	for ( int i = 0; i < 4; i++ )
	{
		m_fWheelBrakeTorque->p [ i ] = m_fWheelBrakeInitial;
	}
}

void cCar::ApplyAcceleration ( void )
{
	// if the gear is in 0 then set to 1
	if ( dbMeqCarGetGear ( m_iID ) == 0 )
		dbMeqCarSetGear	( m_iID, 1 );

	// we need to control the gear box when in manual mode
	if ( m_bAutomaticGearBox == false )
	{
		if ( m_fSpeed >  7.0f && m_fSpeed < 15.0f ) dbMeqCarSetGear	( m_iID, 2 );
		if ( m_fSpeed > 15.0f && m_fSpeed < 20.0f ) dbMeqCarSetGear	( m_iID, 3 );
		if ( m_fSpeed > 20.0f && m_fSpeed < 30.0f ) dbMeqCarSetGear	( m_iID, 4 );
		if ( m_fSpeed > 40.0f )						dbMeqCarSetGear	( m_iID, 5 );
	}

	// increment the motor input instead of applying a full on acceleration
	if ( m_MotorInputTimer.GetElapsedTime ( ) > m_fMotorInputDelay )
	{
		m_fMotorInput += m_fMotorInputIncrement;

		if ( m_fMotorInput > 1.0f )
			m_fMotorInput = 1.0f;
	}

	// set input and clutch
	dbMeqCarSetMotorInput  ( m_iID, m_fMotorInput );
	dbMeqCarSetClutch	   ( m_iID, 1 );
}

void cCar::ApplyBrakes ( void )
{
	// set clutch and motor input to lowest levels
	// only when reverse is not being used
	if ( m_bReverse == false )
	{
		// slowly press harder on the brakes
		if ( m_WheelBrakeTimer.GetElapsedTime ( ) > m_fWheelBrakeDelay )
		{
			// apply wheel brakes
			for ( int i = 0; i < 4; i++ )
			{
				m_fWheelBrakeTorque->p [ i ] += m_fWheelBrakeIncrement;

				if ( m_fMotorInput > m_fWheelBrakeMaximum )
					m_fWheelBrakeTorque->p [ i ] = m_fWheelBrakeMaximum;
			}
		}

		// set the clutch and motor input to 0 when applying the brakes
		dbMeqCarSetClutch			( m_iID, 0 );
		dbMeqCarSetMotorInput		( m_iID, 0 );

		// apply a high level of torque on all 4 wheels
		dbMeqCarSetWheelBrakeTorque ( m_iID, 0, m_fWheelBrakeTorque->a );
		dbMeqCarSetWheelBrakeTorque ( m_iID, 1, m_fWheelBrakeTorque->b );
		dbMeqCarSetWheelBrakeTorque ( m_iID, 2, m_fWheelBrakeTorque->c );
		dbMeqCarSetWheelBrakeTorque ( m_iID, 3, m_fWheelBrakeTorque->d );

		// set the gear to 0
		dbMeqCarSetGear				( m_iID, 0 );
	}

	// reverse the car if we are nearly at a complete stop
	if ( dbMeqCarGetSpeed ( m_iID ) < 0.5f )
	{
		dbMeqCarSetGear			( m_iID, 0 );
		dbMeqCarSetMotorInput	( m_iID, 1.0f );	
		dbMeqCarSetClutch		( m_iID, 1 );

		m_bReverse = true;
	}
	else
		m_bReverse = false;
}

void cCar::ApplyHandBrake ( void )
{
	// set clutch and motor input to lowest levels
	dbMeqCarSetClutch			( m_iID, 0 );
	dbMeqCarSetMotorInput		( m_iID, 0 );

	// apply a high level of torque on all 4 wheels
	dbMeqCarSetWheelBrakeTorque ( m_iID, 0, m_fWheelHandBrakeTorque->a );
	dbMeqCarSetWheelBrakeTorque ( m_iID, 1, m_fWheelHandBrakeTorque->b );
	dbMeqCarSetWheelBrakeTorque ( m_iID, 2, m_fWheelHandBrakeTorque->c );
	dbMeqCarSetWheelBrakeTorque ( m_iID, 3, m_fWheelHandBrakeTorque->d );

	// set the gear to 0
	dbMeqCarSetGear				( m_iID, 0 );
}
