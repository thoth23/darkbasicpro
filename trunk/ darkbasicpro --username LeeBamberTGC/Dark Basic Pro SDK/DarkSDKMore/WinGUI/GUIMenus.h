//
// Menus Header
//

// Common Includes
#include <windows.h>
#include "macros.h"

// Internal Control Structure
struct WinMenuData
{
	HMENU				hMenu;
	HMENU				hPopupMenu;
	int					iSelected;
};

// External Global Data Pointers
extern int				g_iMenusMax;
extern WinMenuData*		g_pMenus;
extern HFONT			g_hFont;

// Internal Menus Functions

LRESULT					SubClassMenusWinProc	( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void					InitGUIMenus			( HWND hWnd );
void					FreeAllMenus			( void );
bool					ConfirmMenuIndex		( int iMenuID );
bool					ConfirmNewMenuIndex		( int iMenuID );
DWORD					GetStyleFlag			( int iStyle );

// Menu Functions

bool					AddItem					( int iMenuID, int iMenuTargetID, int iInsertA, DWORD dwFlags, LPSTR pText );
bool					DeleteItem				( int iMenuID );
void					UpdateMenus				( void );
void					SetTheFont				( HWND hWnd, LPSTR pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode );
void					SetTheState				( int iMenuID, int iAt, int iState );
