
#include "cRoboCar.h"

sObject* GetObject ( int iID );

cRoboCar::cRoboCar ( int iID, int iCarID )
{
	// call the main constructor passing in default offsets
	cRoboCar ( iID, iCarID, 0.0f, 0.0f, 0.0f );
}

cRoboCar::cRoboCar ( int iID, int iCarID, float fXOffset, float fYOffset, float fZOffset )
{
	// main constructor for the robocar, takes the ID of the robocar, the ID of the player
	// controlled car and offset parameters
	
	// ensure the object exists before continuing
	sObject* pObject = GetObject ( iID );

	// check the object pointer
	if ( !pObject )
	{
		// set state to error so we don't crash later on and
		// return out of the constructor
		m_eState = eStateError;
		return;
	}

	// assign properties
	m_iID				= iID;								// the ID of car and object
	m_iCarID			= iCarID;							// store ID of user controlled car
	m_iPointID			= iID;								// use same ID as car for the point
	m_fX				= dbObjectPositionX ( iID );		// object position x
	m_fY				= dbObjectPositionY ( iID );		// object position y
	m_fZ				= dbObjectPositionZ ( iID );		// object position z
	m_fXOffset			= fXOffset;							// x offset
	m_fYOffset			= fYOffset;							// y offset
	m_fZOffset			= fZOffset;							// z offset
	m_eState			= eStateActive;						// initial state of car is active
	m_iPathPoint		= 0;								// which point of the path we are on
	m_fWheelRotation	= 0.0f;								// wheel rotation angle

	// create a rigid body box and set default mass
	dbMeqRigidBodyBoxCreate ( m_iID, m_iID );
	dbMeqRigidBodySetMass   ( m_iID, 1750 );

	// create a new point for distance checking
	dbMeqPointCreate ( m_iPointID );

	// offset all limbs
	for ( int i = 0; i < pObject->iFrameCount; i++ )
		dbOffsetLimb ( m_iID, i, m_fXOffset, m_fYOffset, m_fZOffset );

	// change properties of the rigid body so that it will not be taking
	// up any processing time until it is required to be used within
	// the simulation for a crash
	dbMeqRigidBodySetUpdate  ( m_iID, false );		// do not update world matrix
	dbMeqRigidBodySetActive  ( m_iID, false );		// set active off
	dbMeqRigidBodySetDynamic ( m_iID, false );		// set dynamic off
}

cRoboCar::~cRoboCar ( )
{
	// the destructor is used to free up any previously
	// allocated memory

	// delete the rigid body box and point
	dbMeqRigidBodyDelete ( m_iID      );
	dbMeqPointDelete     ( m_iPointID );
}

void cRoboCar::AddPathPoint ( float fX, float fY, float fZ )
{
	// add a new point to the path
	sPath path = { fX, fY, fZ };

	m_Path.push_back ( path );
}

void cRoboCar::Update ( void )
{
	// the update function must be called each frame, it in turn
	// is responsible for the behaviour of the robocar, it runs
	// using a simple state manager

	switch ( m_eState )
	{
		case eStateActive:
		{
			// when the car is active it's position and rotation
			// are being manually controlled, the updatemovement
			// function must also check for when a collision is
			// set to take place between a robocar and a real car

			UpdateMovement ( );
		}
		break;
		
		case eStateCollided:
		{
			// if a collision has taken place the state will be set
			// to eStateCollided at which point the physics takes
			// over and the robocar is no longer moved manually, a
			// velocity must be applied to the robocar in the direction
			// it was moving in order to make the contact and the
			// appearance of the robocar still driving

			UpdateCollision ( );
		}
		break;
		
		case eStateAction:
		{
			// this function simply waits until the physics has
			// carried out its work, it tests for the end of a collision
			// by checking the velocity of the object, if this is
			// below a certain threshold then the collision has come
			// to and end and can be moved to the next sate

			UpdateAction ( );
		}
		break;

		case eStateEnd:
		{
			// at this juncture the whole collision process has
			// ended and nothing more needs to take place
		}
		break;

		case eStateError:
		{
			// an error has occured in the constructor, do nothing
		}
		break;

		default:
			break;
	};
}

void cRoboCar::UpdateMovement ( void )
{
	// set the point to the position of the player controlled car
	dbMeqPointSet ( m_iPointID, dbObjectPositionX ( m_iCarID ), dbObjectPositionY ( m_iCarID ), dbObjectPositionZ ( m_iCarID ) );
	
	// obtain the distance from the robocar to the player controlled car
	float fDistance = dbMeqPointGetDistanceTo ( m_iPointID, dbObjectPositionX ( m_iID ), dbObjectPositionY ( m_iID ), dbObjectPositionZ ( m_iID ) );

	// at this stage we check the distance and see if it is less than
	// a certain value and at which point we change the state to that
	// of collided
	if ( fDistance < 40 )
	{
		// switch the state to collided
		m_eState = eStateCollided;
	}
	else
	{
		// set the new position based on the list of points to move to

		UpdateWheels ( );

		// check the path index is not greater than the size of the array
		if ( m_iPathPoint > m_Path.size ( ) )
			m_iPathPoint = m_Path.size ( ) - 1;

		// set the new x, y and z points
		if ( m_iPathPoint < m_Path.size ( ) )
		{
			m_fX = m_Path [ m_iPathPoint ].x;
			m_fY = m_Path [ m_iPathPoint ].y;
			m_fZ = m_Path [ m_iPathPoint ].z;
		}

		// position the object
		dbPositionObject ( m_iID, m_fX, m_fY, m_fZ );
	}
}

void cRoboCar::UpdateCollision ( void )
{
	// when a collision takes place we need to apply a velocity in the
	// direction that the robocar is facing - this will allow meqon to
	// take over and control the action

	// set the origin to match the position of the car
	dbMeqRigidBodySetOrigin  ( m_iID, m_fX, m_fY, m_fZ );

	// enable matrix updates, set dynamic state to true and switch active on
	dbMeqRigidBodySetUpdate  ( m_iID, true );
	dbMeqRigidBodySetDynamic ( m_iID, true );
	dbMeqRigidBodySetActive  ( m_iID, true );

	// add velocity in the direction the car is facing
	dbMeqRigidBodySetVelocity ( m_iID, 0, 0, -50.0f );
	
	// change the state to action
	m_eState = eStateAction;
}

void cRoboCar::UpdateAction ( void )
{
	// in this stage we check what is going on with the robocar
	// after it has collded with the player car, when the velocity
	// of the robocar has come to a halt after a crash we change
	// the state to end of collision

	// check velocity of the robocar
	float fX = dbMeqRigidBodyGetVelocityX ( m_iID );
	float fY = dbMeqRigidBodyGetVelocityY ( m_iID );
	float fZ = dbMeqRigidBodyGetVelocityZ ( m_iID );

	UpdateWheels ( );

	// if movement on all 3 axes is small then change state
	if ( fX < 0.01f && fY < 0.01f && fZ < 0.01f )
	{
		// change to end of collision
		m_eState = eStateEnd;
	}
}

void cRoboCar::UpdateWheels	( void )
{
	// update the rotation of the wheels
	dbRotateLimb ( m_iID, m_iWheel1, m_fWheelRotation, 0, 0 );
	dbRotateLimb ( m_iID, m_iWheel2, m_fWheelRotation, 0, 0 );
	dbRotateLimb ( m_iID, m_iWheel3, m_fWheelRotation, 0, 0 );
	dbRotateLimb ( m_iID, m_iWheel4, m_fWheelRotation, 0, 0 );
}

