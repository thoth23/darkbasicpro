
#include "cSlider.h"


bool cSlider::Create ( char* szName, float fInitial, float fMin, float fMax, float fDelta, int iSliderBackSprite, int iSliderSprite )
{
	strcpy ( m_szName,  szName  );

	m_fCurrent			= fInitial;
	m_fMin				= fMin;
	m_fMax				= fMax;
	m_fDelta			= fDelta;
	m_iSliderBackSprite = iSliderBackSprite;
	m_iSliderSprite		= iSliderSprite;

	m_bChange = false;

	return true;
}

bool cSlider::Create ( char* szName, int iInitial, int iMin, int iMax, int iDelta, int iSliderBackSprite, int iSliderSprite )
{
	strcpy ( m_szName,  szName );

	m_fCurrent			= iInitial;
	m_fMin				= iMin;
	m_fMax				= iMax;
	m_fDelta			= iDelta;
	m_iSliderBackSprite = iSliderBackSprite;
	m_iSliderSprite		= iSliderSprite;

	m_bChange = false;

	return true;
}

void cSlider::Update ( int iX, int iY )
{
	dbText ( iX, iY, m_szName );

	dbSprite ( m_iSliderBackSprite, iX + dbTextWidth ( m_szName ) + 12, iY - 2, 100 );

	int iSpriteWidth = dbSpriteWidth ( m_iSliderBackSprite );
	float fOffset = iSpriteWidth / m_fMax;
	float fA = fOffset * ( m_fCurrent );
	dbSprite ( m_iSliderSprite, dbSpriteX ( m_iSliderBackSprite ) + ( fA - 8 ), iY + 1, 101 );

	char szText [ 256 ] = "";
	sprintf ( szText, " = %f", m_fCurrent / m_fDelta );
	dbText ( iX + dbTextWidth ( m_szName )  + dbSpriteWidth ( m_iSliderBackSprite ) + 10, iY, szText );


	if ( dbMouseClick ( ) )
	{
		int iX = dbMouseX ( );
		int iY = dbMouseY ( );

		int iStartX = dbSpriteX ( m_iSliderBackSprite );
		int iStartY = dbSpriteY ( m_iSliderBackSprite );
		int iEndX   = dbSpriteX ( m_iSliderBackSprite ) + dbSpriteWidth  ( m_iSliderBackSprite );
		int iEndY   = dbSpriteY ( m_iSliderBackSprite ) + dbSpriteHeight ( m_iSliderBackSprite );

		if ( iX >= iStartX && iX <= iEndX && iY >= iStartY && iY <= iEndY )
		{
			// this is the point closest to the offset
			int iPoint = iX - iStartX;

			for ( int i = 0; i < ( int ) m_fMax; i++ )
			{
				if ( iPoint >= ( iSpriteWidth / m_fMax * i ) && iPoint <= ( iSpriteWidth / m_fMax * ( i + 1 ) ) )
				{
					m_fCurrent = ( int ) i + 1;
					m_bChange = true;
					break;
				}
			}
		}
	}
}

bool cSlider::GetChange ( void )
{
	if ( m_bChange )
	{
		m_bChange = false;
		return true;
	}

	return false;
}

float cSlider::GetValue ( void )
{
	return m_fCurrent / m_fDelta;
}