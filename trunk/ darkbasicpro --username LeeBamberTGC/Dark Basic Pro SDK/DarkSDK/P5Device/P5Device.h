//
// Header File
//

// Common Includes
#include "windows.h"

// Add P5 Includes
#include "P5\\P5DLL.h"
#include "P5\\P5Bend.h"
#include "P5\\P5Motion.h"

// Define macro
#define MYCOMMAND __declspec ( dllexport )

// Constructor Functions (optional)
MYCOMMAND void Constructor ( void );
MYCOMMAND void Destructor ( void );
MYCOMMAND void ReceiveCoreDataPtr ( LPVOID pCore );

// Do not decorate DLL-function-names
extern "C"
{
	// My Commands
	MYCOMMAND void		MotionProcess			( void );
	MYCOMMAND void		Reset					( void );

	// My Expressions
	MYCOMMAND int		IsPresent				( void );

	MYCOMMAND DWORD		GetX					( void );
	MYCOMMAND DWORD		GetY					( void );
	MYCOMMAND DWORD		GetZ					( void );

	MYCOMMAND DWORD		GetAbsYaw				( void );
	MYCOMMAND DWORD		GetAbsPitch				( void );
	MYCOMMAND DWORD		GetAbsRoll				( void );

	MYCOMMAND DWORD		GetRelYaw				( void );
	MYCOMMAND DWORD		GetRelPitch				( void );
	MYCOMMAND DWORD		GetRelRoll				( void );

	MYCOMMAND int		GetClickThumb			( void );
	MYCOMMAND int		GetClickIndex			( void );
	MYCOMMAND int		GetClickMiddle			( void );
	MYCOMMAND int		GetClickRing			( void );
	MYCOMMAND int		GetClickPinky			( void );

	MYCOMMAND int		GetBendThumb			( void );
	MYCOMMAND int		GetBendIndex			( void );
	MYCOMMAND int		GetBendMiddle			( void );
	MYCOMMAND int		GetBendRing				( void );
	MYCOMMAND int		GetBendPinky			( void );
}
