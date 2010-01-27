//
// P5Device
//

// Extra Includes
#include "stdio.h"

// P5 Include and global data
#include "P5Device.h"
CP5DLL P5;
P5BOOL iP5Present;
extern float fAbsYawPos, fAbsPitchPos, fAbsRollPos;
extern float fRelYawPos, fRelPitchPos, fRelRollPos;
extern float fFilterX, fFilterY, fFilterZ;

// Include Core (optional)
#include "globstruct.h"
GlobStruct* g_pGlob = NULL;

//
// Constructors (optional)
//

void Constructor ( void )
{
	// Create memory here
}

void Destructor ( void )
{
	// Close P5
	P5.P5_Close();
}

void ReceiveCoreDataPtr ( LPVOID pCore )
{
	// Get Core Data Pointer here
	g_pGlob = (GlobStruct*)pCore;

	// Initialise P5
	iP5Present = P5.P5_Init();

	// initialize P5Motion object
	RECT region;
	P5Motion_Init(&P5, 0);
	// invert mouse as needed
	P5Motion_InvertMouse(P5MOTION_INVERTAXIS, P5MOTION_NORMALAXIS, P5MOTION_NORMALAXIS);
	// will get the mouse bounding data
	GetClipCursor(&region);
	// will set this data to clip the P5mouse
	P5Motion_SetClipRegion(region.left, region.right, region.top, region.bottom, 0, 1000);

	// initialize P5Bend object
	P5Bend_Init( &P5, 0 ); 
//	P5Bend_SetClickSensitivity( P5_THUMB, 10 );
}

//
// Standard DLL Main
//

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//
// Commands
//

void MotionProcess( void )
{
	// Called to update P5 data
	P5Motion_Process();
	P5Bend_Process();
}

void Reset ( void )
{
	P5Motion_Reset();
}

//
// Expressions
//

int IsPresent ( void )
{
	if ( iP5Present )
		return 1;
	else
		return 0;
}

DWORD GetX ( void )
{
	return *(DWORD*)&fFilterX;
}

DWORD GetY ( void )
{
	return *(DWORD*)&fFilterY;
}

DWORD GetZ ( void )
{
	return *(DWORD*)&fFilterZ;
}

DWORD GetAbsYaw ( void )
{
	return *(DWORD*)&fAbsYawPos;
}

DWORD GetAbsPitch ( void )
{
	return *(DWORD*)&fAbsPitchPos;
}

DWORD GetAbsRoll ( void )
{
	return *(DWORD*)&fAbsRollPos;
}

DWORD GetRelYaw ( void )
{
	return *(DWORD*)&fRelYawPos;
}

DWORD GetRelPitch ( void )
{
	return *(DWORD*)&fRelPitchPos;
}

DWORD GetRelRoll ( void )
{
	return *(DWORD*)&fRelRollPos;
}

int GetClickThumb ( void )
{
	return nP5ClickEdge[P5_THUMB];
}
int GetClickIndex ( void )
{
	return nP5ClickEdge[P5_INDEX];
}
int GetClickMiddle ( void )
{
	return nP5ClickEdge[P5_MIDDLE];
}
int GetClickRing ( void )
{
	return nP5ClickEdge[P5_RING];
}
int GetClickPinky ( void )
{
	return nP5ClickEdge[P5_PINKY];
}
int GetBendThumb ( void )
{
	return P5.m_P5Devices[0].m_byBendSensor_Data[P5_THUMB];
}
int GetBendIndex ( void )
{
	return P5.m_P5Devices[0].m_byBendSensor_Data[P5_INDEX];
}
int GetBendMiddle ( void )
{
	return P5.m_P5Devices[0].m_byBendSensor_Data[P5_MIDDLE];
}
int GetBendRing ( void )
{
	return P5.m_P5Devices[0].m_byBendSensor_Data[P5_RING];
}
int GetBendPinky ( void )
{
	return P5.m_P5Devices[0].m_byBendSensor_Data[P5_PINKY];
}
