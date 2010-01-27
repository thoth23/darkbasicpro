//
// Only need to tell C compiler to export these functions
//

// My define to make things more readable
#define MYDLL __declspec ( dllexport )

// Constructor Functions (optional)
MYDLL void Constructor ( void );
MYDLL void Destructor ( void );
MYDLL void ReceiveCoreDataPtr ( LPVOID pCore );
MYDLL int GetAssociatedDLLs ( void );

// My exported functions
MYDLL void MakeGame		( LPSTR, LPSTR, LPSTR, LPSTR );
MYDLL void PlayGame		( void );
MYDLL void EndGame		( void );

extern "C"
{
	MYDLL void MessageBoxPopup ( DWORD pString );
}
