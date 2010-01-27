//
// Only need to tell C compiler to export these functions
//

// My define to make things more readable
#define MYDLL __declspec ( dllexport )

// Constructor Functions (optional)
MYDLL void Constructor ( void );
MYDLL void Destructor ( void );
MYDLL void PreDestructor ( void );
MYDLL void ReceiveCoreDataPtr ( LPVOID pCore );
MYDLL int GetAssociatedDLLs ( void );
MYDLL int GetNumDependencies ( void );
MYDLL const char * GetDependencyID ( int n );
MYDLL void ReceiveDependenciesHinstance ( LPSTR pDLLName, HINSTANCE hModule );
MYDLL void OptionalSecurityCode ( int iInitCode );
MYDLL void D3DDeviceLost ( void );
MYDLL void D3DDeviceNotReset ( void );

// My exported functions
MYDLL void MakeGame		( LPSTR, LPSTR, LPSTR, LPSTR );
MYDLL void PlayGame		( void );
MYDLL void EndGame		( void );

extern "C"
{
	MYDLL void MessageBoxPopup ( DWORD pString );
}
