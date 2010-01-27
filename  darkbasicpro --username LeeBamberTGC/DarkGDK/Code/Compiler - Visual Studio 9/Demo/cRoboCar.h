
#include "DarkSDK.h"
#include "D:\coding\Dark Basic Pro Shared\Projects\Expansion Packs\Meqon\Code\Meqon.h"
#include ".\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\DBOData.h"
#include <stdio.h>
#include <vector>
#include "timer.h"

class cRoboCar
{
	// the robocar class is responsible for AI cars

	public:
		 cRoboCar ( int iID, int iCarID );
		 cRoboCar ( int iID, int iCarID, float fXOffset, float fYOffset, float fZOffset );
		~cRoboCar ( );

		virtual void Update          ( void );
		virtual void UpdateMovement  ( void );
		virtual void UpdateCollision ( void );
		virtual void UpdateAction    ( void );
		virtual void UpdateWheels	 ( void );

		void AddPathPoint ( float fX, float fY, float fZ );
	
	public:
		enum eState
		{
			eStateActive,
			eStateCollided,
			eStateAction,
			eStateEnd,
			eStateError,
		};

		struct sPath
		{
			float x;
			float y;
			float z;
		};

		int						m_iID;
		int						m_iCarID;
		int						m_iPointID;

		float					m_fX;
		float					m_fY;
		float					m_fZ;

		int						m_iWheel1;
		int						m_iWheel2;
		int						m_iWheel3;
		int						m_iWheel4;
		float					m_fWheelRotation;

		float					m_fXOffset;
		float					m_fYOffset;
		float					m_fZOffset;

		eState					m_eState;

		std::vector < sPath >	m_Path;
		int						m_iPathPoint;
};

