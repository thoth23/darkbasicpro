//
// Only need to tell C compiler to export these functions
//

#include "Activexpression.h"
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "resource.h"
//#ifndef UNICODE - watch for this!
//#ifndef _UNICODE
//#error This code does not work for non-Unicode builds
//#endif
//#endif

// My define to make things more readable
#define MYDLL __declspec ( dllexport )

// Constructor Functions (optional)
MYDLL void Constructor ( void );
MYDLL void Destructor ( void );
MYDLL void ReceiveCoreDataPtr ( LPVOID pCore );
MYDLL int GetAssociatedDLLs ( void );

// My exported functions
MYDLL int ACTInit ( void );
MYDLL void ACTUpdate ( void );
MYDLL void ACTClose ( void );
MYDLL DWORD ACTGetErrorMessage ( DWORD pDestStr );
MYDLL void ACTSessionStop ( void );
MYDLL void ACTPerformChecklistForSession ( void );

// My exported session functions
MYDLL int ACTSessionStartMultichoice ( int iQuestionID, int iDisplayOption, int iOptions, int iResponses );
MYDLL int ACTSessionStartYesNo ( int iQuestionID, int iDontKnowRequired );
MYDLL int ACTSessionStartTrueFalse ( int iQuestionID, int iDontKnowRequired );
MYDLL int ACTSessionStartLikert ( int iQuestionID, int iMaxScaleValue );
MYDLL int ACTSessionStartNumeric ( int iQuestionID, DWORD dwQuestion );
MYDLL int ACTSessionStartText ( int iQuestionID, DWORD dwQuestion );
MYDLL int ACTSessionStartSortInOrder ( int iQuestionID, int iDisplayOption, int iOptions, int iResponses );
MYDLL int ACTSessionStartMessage ( int iQuestionID, DWORD dwMessage );
