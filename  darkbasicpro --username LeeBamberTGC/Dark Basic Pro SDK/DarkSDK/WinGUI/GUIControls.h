//
// Controls Header
//

// Common Includes
#include <windows.h>
#include "macros.h"

// Include Common Controls
//#define _WIN32_IE 0x0400
#include <commctrl.h>

// Internal Control Defines
enum
{
	WINCONTROL_TEXT,
	WINCONTROL_BUTTON,
	WINCONTROL_PROGRESS,
	WINCONTROL_TAB,
	WINCONTROL_LISTVIEW,
	WINCONTROL_TREEVIEW,
	WINCONTROL_SCROLLBAR,
	WINCONTROL_COMBOBOX,
	WINCONTROL_CLEARAREA,
	WINCONTROL_SLIDER,
	WINCONTROL_EDIT,
	WINCONTROL_UPDOWN,
	WINCONTROL_TOOLBAR,
	WINCONTROL_POPUP
};

// Internal Control Structure
struct WinControlData
{
	short				sType;
	HWND				hWnd;
	LONG				lOriginalWinProc;
	int					iClicked;
	int					iLastSelected;
	int					iMouseAtX;
	int					iMouseAtY;
	bool				bVisible;
	int					iX;
	int					iY;
	int					iWidth;
	int					iHeight;
	int					iUpdateSum;
	HFONT				hfont;
	HWND				hWndMasterControl;
	int					iMasterIndexID;
    HIMAGELIST			himlImageList;
};

// External Global Data Pointers
extern int				g_iControlsMax;
extern WinControlData*	g_pControls;

// Internal Control Functions

LRESULT					SubClassControlsWinProc	( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void					InitGUIControls			( void );
void					FreeAllControls			( HWND hwndParent );
bool					ConfirmControlIndex		( int iControlID );
bool					ConfirmNewControlIndex	( int iControlID );
DWORD					GetExtendedStyle		( int iExtendedStyle );

bool					CreateControl			( int iControlID, HWND hwndParent, LPCTSTR pWindowType, DWORD dwExtendedStyle, DWORD dwWindowStyle, LPSTR pText, int iX, int iY, int iWidth, int iHeight );
bool					CreateControlEx			( int iMode, int iControlID, HWND hwndParent, LPCTSTR pWindowType, DWORD dwExtendedStyle, DWORD dwWindowStyle, LPSTR pText, int iX, int iY, int iWidth, int iHeight );
bool					UpdateControls			( HWND hwndParent, int iScreenWidth, int iScreenHeight );
void					FreeControl				( HWND hwndParent, int iControlID );
void					HideControl				( int iControlID );
void					ShowControl				( int iControlID );

void					Move					( int iControlID, int iX, int iY );
void					Size					( int iControlID, int iW, int iH );

void					SetFont					( int iControlID, LPSTR pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode );
void					SetBackColour			( int iControlID, DWORD dwBack );
void					SetForeColour			( int iControlID, DWORD dwFore );
void					SetProgressRange		( int iControlID, int nMinRange, int nMaxRange );
void					SetProgressPosition		( int iControlID, int iPosition );
void					SetSliderRange			( int iControlID, int nMinRange, int nMaxRange );
void					SetSliderPosition		( int iControlID, int iPosition );
int						GetSliderPosition		( int iControlID );
void					SetUpDownRange			( int iControlID, int nMinRange, int nMaxRange );
void					SetUpDownPosition		( int iControlID, int iPosition );
void					SetScrollBarRange		( int iControlID, int nMinRange, int nMaxRange );
void					SetScrollBarPosition	( int iControlID, int iPosition );
void					SetScrollBarPageSize	( int iControlID, int iPageSize );
void					SetText					( int iControlID, LPSTR pText );
void					SetIcon					( HINSTANCE hInstance, int iControlID, DWORD dwIconHandle );
void					SetIcon					( HINSTANCE hInstance, int iControlID, int iIndex, DWORD dwIconHandle );
void					SetAlignment			( int iControlID, int iAlignCode );
void					SetSelection			( int iControlID, DWORD dwSelection );
DWORD					GetSelection			( int iControlID );
int						GetControlCount			( int iControlID );
void					GetString				( int iControlID, LPSTR pString );

void					InsertItem				( int iControlID, int iIndex, LPSTR lpText );
void					RemoveItem				( int iControlID, int iIndex );
void					RemoveAllItems			( int iControlID );
void					SetItem					( int iControlID, int iIndex, LPSTR lpText );
void					SetItemIcon				( HINSTANCE hInstance, int iControlID, int iIndex, DWORD dwIconHandle );
void					Link					( int iChildControlID, int iParentControlID, int iParentIndex );
int						GetControlItemValue		( int iControlID );
int						GetControlItemIcon		( int iControlID );

int						GetControlMouseX		( int iControlID );
int						GetControlMouseY		( int iControlID );

// Windows GUI System Commands

void					CaptureSystemColors		( void );
void					SetSystemColors			( int iSystemColorIndex, DWORD dwRGB );
void					RestoreSystemColors		( void );