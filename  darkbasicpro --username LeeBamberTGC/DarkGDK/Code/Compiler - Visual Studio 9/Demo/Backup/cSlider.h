
#include "DarkSDK.h"
#include "D:\coding\Dark Basic Pro Shared\Projects\Expansion Packs\Meqon\Code\Meqon.h"
#include ".\..\..\..\Dark Basic Pro SDK\Shared\DBOFormat\DBOData.h"
#include <stdio.h>
#include <vector>

class cSlider
{
	private:
		char  m_szName [ 256 ];
		float m_fCurrent;
		float m_fMin;
		float m_fMax;
		float m_fDelta;
		int   m_iSliderBackSprite;
		int   m_iSliderSprite;
		bool  m_bChange;

	public:
		bool  Create    ( char* szName, float fInitial, float fMin, float fMax, float fDelta, int iSliderBackSprite, int iSliderSprite );
		bool  Create    ( char* szName, int iInitial, int iMin, int iMax, int iDelta, int iSliderBackSprite, int iSliderSprite );
		void  Update    ( int iX, int iY );

		float GetValue  ( void );
		bool  GetChange ( void );
};