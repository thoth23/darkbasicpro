
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DISPLAY COMMANDS /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/*

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Include\DarkSDKDisplay.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL DB PRO FUNCTIONS ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool				Setup								( void );
void				FindHardwareInfo					( void );
void				GetValidBackBufferFormat			( void );
void				GetValidStencilBufferFormat			( void );
int					Create								( HWND hWnd, D3DPRESENT_PARAMETERS* d3dpp );

void				Begin								( void );
void				End									( void );
void				Render								( void );

IDirect3D9*			GetDirect3D							( void );
IDirect3DDevice9*	GetDirect3DDevice					( void );

bool				ConstructorDisplay					( void );
void				DestructorDisplay					( void );
void				SetErrorHandlerDisplay				( LPVOID pErrorHandlerPtr );
void				PassCoreDataDisplay					( LPVOID pGlobPtr, int iStage );

bool				SetDisplayDebugMode					( void );
bool				SetDisplayMode						( int iWidth, int iHeight );
bool				SetDisplayMode						( int iWidth, int iHeight, int iDepth );
bool				SetDisplayMode						( int iWidth, int iHeight, int iDepth, int iMode );
bool				SetDisplayMode						( int iWidth, int iHeight, int iDepth, int iMode, int iVertexProcessing );
bool				SetDisplayMode						( int iWidth, int iHeight, int iDepth, int iMode, int iVertexProcessing, int iLockable );
void				RestoreDisplayMode					( void );
void				RestoreLostDevice					( void );

void				AddSwapChain						( HWND hwnd );
void				StartSwapChain						( int iID );
void				EndSwapChain						( int iID );
void				UpdateSwapChain						( int iID );

void				Clear								( int iR, int iG, int iB );
void				GetGamma							( int* piR, int* piG, int* piB );

bool				CheckDisplayMode					( int iWidth, int iHeight );
bool				CheckDisplayMode					( int iWidth, int iHeight, int iDepth, int iMode );
bool				CheckDisplayMode					( int iWidth, int iHeight, int iDepth, int iMode, int iVertexProcessing );

bool				GetWindowedMode						( void );

int					GetNumberOfDisplayModes				( void );
void				GetDisplayMode						( int iID, char* szMode );

int					GetNumberOfDisplayDevices			( void );
void				SetDisplayDevice					( int iID );
void				GetDeviceName						( int iID, char* szDevice );
void				GetDeviceDriverName					( int iID, char* szDriver );

void				SetDitherMode						( int iMode );
void				SetShadeMode						( int iMode );
void				SetLightMode						( int iMode );
void				SetCullMode							( int iMode );
void				SetSpecularMode						( int iMode );

void				SetRenderState						( int iState, int iValue );

void				OverrideHWND						( HWND hWnd );
void				DisableWindowResize					( void );
void				EnableWindowResize					( void );

void				UpdateWindowSize					( int iWidth, int iHeight );
void				GetWindowSize						( int* piWidth, int* piHeight );

int					CheckDisplayMode					( int iWidth, int iHeight, int iDepth );
DWORD				CurrentGraphicsCard					( DWORD pDestStr );
int					EmulationMode						( void );
void				PerformChecklistForDisplayModes		( void );
void				PerformChecklistForGraphicsCards	( void );
int					GetDisplayType						( void );
int					GetDisplayWidth						( void );
int					GetDisplayHeight					( void );
int					GetDisplayDepth						( void );
int					GetDisplayFPS						( void );
int					GetDisplayInvalid					( void );
void				SetGamma							( int iR, int iG, int iB );
void				SetDisplayModeEx					( int iWidth, int iHeight, int iDepth );
void				SetEmulationOn						( void );
void				SetEmulationOff						( void );
void				SetGraphicsCard						( DWORD dwCardname );

void				SetWindowModeOn						( void );
void				SetWindowModeOff					( void );
void				SetWindowSettings					( int iStyle, int iCaption, int iIcon );
void				SetWindowPosition					( int posx, int posy );
void				SetWindowSize						( int sizex, int sizey );
void				HideWindow							( void );
void				ShowWindow							( void );
void				MinimiseWindow						( void );
void				MaximiseWindow						( void );
void				RestoreWindow						( void );
void				SetWindowTitle						( DWORD pTitleString );
int					WindowExist							( DWORD pTitleString );
void				WindowToBack						( void );
void				WindowToFront						( void );
void				WindowToFront						( DWORD pTitleString );
void				WindowToBack						( DWORD pTitleString );

void				LockBackbuffer						( void );
void				UnlockBackbuffer					( void );
DWORD				GetBackbufferPtr					( void );
int					GetBackbufferWidth					( void );
int					GetBackbufferHeight					( void );
int					GetBackbufferDepth					( void );
int					GetBackbufferPitch					( void );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbPerformChecklistForGraphicsCards ( void )
{
	PerformChecklistForGraphicsCards ( );
}

void dbPerformChecklistForDisplayModes ( void )
{
	PerformChecklistForDisplayModes ( );
}

bool dbSetDisplayMode ( int iWidth, int iHeight, int iDepth )
{
	SetDisplayModeEx ( iWidth, iHeight, iDepth );
	return true;
}

void dbSetGraphicsCard ( char* dwCardname )
{
	SetGraphicsCard ( ( DWORD ) dwCardname );
}

void dbSetGamma ( int iR, int iG, int iB )
{
	SetGamma ( iR, iG, iB );
}

void dbShowWindow ( void )
{
	ShowWindow ( );
}

void dbHideWindow ( void )
{
	HideWindow ( );
}

void dbMaximiseWindow ( void )
{
	MaximiseWindow ( );
}

void dbMinimiseWindow ( void )
{
	MinimiseWindow ( );
}

void dbRestoreWindow ( void )
{
	RestoreWindow ( );
}

void dbSetWindowOn ( void )
{
	SetWindowModeOn ( );
}

void dbSetWindowOff ( void )
{
	SetWindowModeOff ( );
}

void dbSetWindowSize ( int sizex, int sizey )
{
	SetWindowSize ( sizex, sizey );
}

void dbSetWindowTitle ( char* pTitleString )
{
	SetWindowTitle ( ( DWORD ) pTitleString );
}

void dbSetWindowLayout ( int iStyle, int iCaption, int iIcon )
{
	SetWindowSettings ( iStyle, iCaption, iIcon );
}

void dbSetWindowPosition ( int posx, int posy )
{
	SetWindowPosition ( posx, posy );
}

int dbWindowExist ( char* pTitleString )
{
	return WindowExist ( ( DWORD ) pTitleString );
}

void dbWindowToBack ( void )
{
	WindowToBack ( );
}

void dbWindowToFront ( void )
{
	WindowToFront ( );
}

void dbWindowToFront ( char* pTitleString )
{
	WindowToFront ( ( DWORD ) pTitleString );
}

void dbWindowToBack ( char* pTitleString )
{
	WindowToBack ( ( DWORD ) pTitleString );
}

void dbLockBackBuffer ( void )
{
	LockBackbuffer ( );
}

void dbUnlockBackBuffer ( void )
{
	UnlockBackbuffer ( );
}

int dbScreenType ( void )
{
	return GetDisplayType ( );
}

int dbScreenWidth ( void )
{
	return GetDisplayWidth ( );
}

int dbScreenHeight ( void )
{
	return GetDisplayHeight ( );
}

int dbScreenDepth ( void )
{
	return GetDisplayDepth ( );
}

int dbScreenInvalid ( void )
{
	return GetDisplayInvalid ( );
}

int dbScreenFPS ( void )
{
	return GetDisplayFPS ( );
}

char* dbCurrentGraphicsCard ( void )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = CurrentGraphicsCard ( NULL );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

bool dbCheckDisplayMode ( int iWidth, int iHeight, int iDepth, int iMode )
{
	return CheckDisplayMode ( iWidth, iHeight, iDepth, iMode );
}

DWORD dbGetBackbufferPtr ( void )
{
	return GetBackbufferPtr ( );
}

int dbGetBackbufferWidth ( void )
{
	return GetBackbufferWidth ( );
}

int dbGetBackbufferHeight ( void )
{
	return GetBackbufferHeight ( );
}

int dbGetBackbufferDepth ( void )
{
	return GetBackbufferDepth ( );
}

int dbGetBackbufferPitch ( void )
{
	return GetBackbufferPitch ( );
}

bool dbSetup ( void )
{
	return Setup ( );
}

void dbFindHardwareInfo ( void )
{
	FindHardwareInfo ( );
}

void dbGetValidBackBufferFormat ( void )
{
	GetValidBackBufferFormat ( );
}

void dbGetValidStencilBufferFormat ( void )
{
	GetValidStencilBufferFormat ( );
}

int dbCreate ( HWND hWnd, D3DPRESENT_PARAMETERS* d3dpp )
{
	return Create ( hWnd, d3dpp );
}

void dbBegin ( void )
{
	Begin ( );
}

void dbEnd ( void )
{
	End ( );
}

void dbRender ( void )
{
	Render ( );
}

IDirect3D9* dbGetDirect3D ( void )
{
	return GetDirect3D ( );
}

IDirect3DDevice9* dbGetDirect3DDevice ( void )
{
	return GetDirect3DDevice ( );
}

bool dbConstructorDisplay ( void )
{
	return ConstructorDisplay ( );
}

void dbDestructorDisplay ( void )
{
	DestructorDisplay ( );
}

void dbSetErrorHandlerDisplay ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerDisplay ( pErrorHandlerPtr );
}

void dbPassCoreDataDisplay( LPVOID pGlobPtr, int iStage )
{
	PassCoreDataDisplay ( pGlobPtr, iStage );
}

bool dbSetDisplayDebugMode ( void )
{
	return SetDisplayDebugMode ( );
}

bool dbSetDisplayMode ( int iWidth, int iHeight )
{
	return SetDisplayMode ( iWidth, iHeight );
}

bool dbSetDisplayMode ( int iWidth, int iHeight, int iDepth, int iMode )
{
	return SetDisplayMode ( iWidth, iHeight, iDepth, iMode );
}

bool dbSetDisplayMode ( int iWidth, int iHeight, int iDepth, int iMode, int iVertexProcessing )
{
	return SetDisplayMode ( iWidth, iHeight, iDepth, iMode, iVertexProcessing );
}

bool dbSetDisplayMode ( int iWidth, int iHeight, int iDepth, int iMode, int iVertexProcessing, int iLockable )
{
	return SetDisplayMode ( iWidth, iHeight, iDepth, iMode, iVertexProcessing, iLockable );
}

void dbRestoreDisplayMode ( void )
{
	RestoreDisplayMode ( );
}

void dbRestoreLostDevice ( void )
{
	RestoreLostDevice ( );
}

void dbAddSwapChain ( HWND hwnd )
{
	AddSwapChain ( hwnd );
}

void dbStartSwapChain ( int iID )
{
	StartSwapChain ( iID );
}

void dbEndSwapChain ( int iID )
{
	EndSwapChain ( iID );
}

void dbUpdateSwapChain ( int iID )
{
	UpdateSwapChain ( iID );
}

void dbGetGamma ( int piR, int piG, int piB )
{
	GetGamma ( &piR, &piG, &piB );
}

bool dbCheckDisplayMode1 ( int iWidth, int iHeight )
{
	return CheckDisplayMode ( iWidth, iHeight );
}

bool dbCheckDisplayMode3 ( int iWidth, int iHeight, int iDepth, int iMode, int iVertexProcessing )
{
	return CheckDisplayMode ( iWidth, iHeight, iDepth, iMode, iVertexProcessing );
}

bool dbGetWindowedMode ( void )
{
	return GetWindowedMode ( );
}

int dbGetNumberOfDisplayModes ( void )
{
	return GetNumberOfDisplayModes ( );
}

void dbGetDisplayMode ( int iID, char* szMode )
{
	GetDisplayMode ( iID, szMode );
}

int dbGetNumberOfDisplayDevices ( void )
{
	return GetNumberOfDisplayDevices ( );
}

void dbSetDisplayDevice ( int iID )
{
	SetDisplayDevice ( iID );
}

void dbGetDeviceName ( int iID, char* szDevice )
{
	GetDeviceName ( iID, szDevice );
}

void dbGetDeviceDriverName ( int iID, char* szDriver )
{
	GetDeviceDriverName ( iID, szDriver );
}

void dbSetDitherMode ( int iMode )
{
	SetDitherMode ( iMode );
}

void dbSetShadeMode ( int iMode )
{
	SetShadeMode ( iMode );
}

void dbSetLightMode ( int iMode )
{
	SetLightMode ( iMode );
}

void dbSetCullMode ( int iMode )
{
	SetCullMode ( iMode );
}

void dbSetSpecularMode ( int iMode )
{
	SetSpecularMode ( iMode );
}

void dbSetRenderState ( int iState, int iValue )
{
	SetRenderState ( iState, iValue );
}

void dbOverrideHWND ( HWND hWnd )
{
	OverrideHWND ( hWnd );
}

void dbDisableWindowResize ( void )
{
	DisableWindowResize ( );
}

void dbEnableWindowResize ( void )
{
	EnableWindowResize ( );
}

void dbUpdateWindowSize ( int iWidth, int iHeight )
{
	UpdateWindowSize ( iWidth, iHeight );
}

void dbGetWindowSize ( int* piWidth, int* piHeight )
{
	GetWindowSize ( piWidth, piHeight );
}

int dbCheckDisplayMode ( int iWidth, int iHeight, int iDepth )
{
	return CheckDisplayMode ( iWidth, iHeight, iDepth );
}

int dbEmulationMode ( void )
{
	return EmulationMode ( );
}

void dbSetDisplayModeEx ( int iWidth, int iHeight, int iDepth )
{
	SetDisplayModeEx ( iWidth, iHeight, iDepth );
}

void dbSetEmulationOn ( void )
{
	SetEmulationOn ( );
}

void dbSetEmulationOff ( void )
{
	SetEmulationOff ( );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
*/