
#include "DarkSDK.h"
#include "D:\coding\Dark Basic Pro Shared\Projects\Expansion Packs\Meqon\Code\Meqon.h"
#include ".\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\DBOData.h"
#include <stdio.h>
#include "timer.h"

class cCar
{
	// this car class is used to create meqon cars, control their properties and drive
	// them around, the virtual drive function can be overriden for a manual implementation
	// of driving, a default behaviour is provided

	public:
		// list of public functions

		// set up functions
		 cCar ( int iID, char* szFile, float fX, float fY, float fZ );						// constructor
		~cCar ( );												// destructor

		// main update functions
		void         UpdateSettings		( void );				// call this to update the settings of the car
		void         Update				( void );				// call this function each frame to update the car

		// virtual functions
		virtual void Drive				( void );				// used to drive the car
		virtual void ControlSteering    ( void );				// control steering of the car
		virtual void ApplyTorque		( void );				// apply torque to the wheels
		virtual void ApplyAcceleration  ( void );				// apply acceleration
		virtual void ApplyBrakes	    ( void );				// deal with the brakes
		virtual void ApplyHandBrake	    ( void );				// last of all sort the handbrake
	
	public:
		// list of public member variables and structures

		// this structure will hold data for us
		struct sData
		{
			float  a;											// value a
			float  b;											// value b
			float  c;											// value c
			float  d;											// value d
			float* p;											// pointer to first value

			sData ( float a1, float b1, float c1, float d1 );	// data constructor
		};

		int				m_iID;									// ID of the car
		char			m_szFile [ 256 ];						// file that contains car settings
		float			m_fWorldScale;							// world scale divider

		bool			m_bControlSteering;						// flag to control steering
		bool			m_bApplyTorque;							// should we apply torque
		bool			m_bApplyAcceleration;					// do we need to apply acceleration
		bool			m_bApplyBrakes;							// deal with brakes
		bool			m_bApplyHandBrake;						// and the hand brake
		
		int				m_iDriveMode;							// current drive mode
		int				m_iMaximumIterations;					// number of iterations, increasing this value makes more accurate but slower simulation

		sData*			m_fFrontAxisCentre;						// front axis centre point
		sData*			m_fFrontAxisExtent;						// front axis extent
		sData*			m_fRearAxisCentre;						// rear axis centre point
		sData*			m_fRearAxisExtent;						// rear axis extent

		float			m_fFrontDifferentialLock;				// front differential lock
		float			m_fRearDifferentialLock;				// rear differential lock

		float			m_fDriveConstant0;						// motor drive constant 0 - power of engine
		float			m_fDriveConstant1;						// motor drive constant 1 - level of acceleration
		float			m_fDriveConstant2;						// motor drive constant 2 - 
		float			m_fBrakeConstant0;						// motor brake constant 0 -
		float			m_fBrakeConstant1;						// motor brake constant 1 - 
		float			m_fBrakeConstant2;						// motor brake constant 2 -

		sData*			m_fWheelRadius;							// radius of the wheels
		sData*			m_fWheelNominalSpringLength;			// nominal spring length
		sData*			m_fWheelSpringStiffness;				// spring stiffness
		sData*			m_fWheelSpringDamping;					// spring damping
		sData*			m_fWheelStatic;							// static wheel material
		sData*			m_fWheelDynamic;						// dynamic wheel material
		sData*			m_fWheelRestitution;					// wheel restitution
		sData*			m_fWheelRolling;						// wheel rolling
		sData*			m_fWheelLateralStiffness;				// the cornering property
		sData*			m_fWheelLongitudinalStiffness;			// the cornering property
		sData*			m_fWheelBrakeTorque;					// brake torque for each frame
		sData*			m_fWheelHandBrakeTorque;				// hand brake torque

		float			m_fWheelBrakeIncrement;					// wheel brake increment
		float			m_fWheelBrakeDelay;						// wheel brake delay
		float			m_fWheelBrakeInitial;					// initial level of wheel brake
		float			m_fWheelBrakeMaximum;					// maxium permitted level for braking
		cCustomTimer	m_WheelBrakeTimer;						// brake timer

		float			m_fSteeringAngle;						// current steering angle
		float			m_fSteeringAngleIncrementA;				// increment on left
		float			m_fSteeringAngleIncrementB;				// increment on right
		float			m_fSteeringAngleLimitA;					// limit on left
		float			m_fSteeringAngleLimitB;					// limit on right

		bool			m_bReverse;								// reverse flag

		int				m_iGear;								// current gear
		float			m_fSpeed;								// current speed in km/h
		float			m_fRPM;									// current rpm count

		bool			m_bAutomaticGearBox;					// are we to use an automatic gear box
		int				m_iGearCount;							// number of gears
		float			m_fGearDelay;							// delay on gear switching
		float*			m_fGearRatios;							// gear ratios ( control how fast wheels spin )

		float			m_fMotorInput;							// motor input level
		float			m_fMotorInputIncrement;					// input increment
		float			m_fMotorInputDelay;						// delay before incrementing
		cCustomTimer	m_MotorInputTimer;						// motor timer

		float			m_fAckermanRatio;

		float			m_fScale;

		float m_fSide    [ 15 ];
		float m_fForward [ 15 ];
};

