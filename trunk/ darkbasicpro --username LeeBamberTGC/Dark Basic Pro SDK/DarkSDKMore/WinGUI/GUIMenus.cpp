//
// GUIMenus
//

// Includes
#include "GUIMenus.h"

// Global Data Pointers
HMENU				g_hMainMenu			= NULL;
int					g_iMenusMax			= 0;
WinMenuData*		g_pMenus			= NULL;
HFONT				g_hFont				= NULL;

//
// Internal Menus SubClass Functions
//

LRESULT SubClassMenusWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
    {
		case WM_COMMAND:
		{
			WORD wMenuID = LOWORD(wParam);
			if ( wMenuID > 0 )
			{
				if ( g_pMenus [ wMenuID ].hMenu )
				{
					g_pMenus [ wMenuID ].iSelected=1;
					return TRUE;
				}
			}
			break;
		}
    }
	return FALSE;
}

//
// Internal Functions
//

void InitGUIMenus ( HWND hWnd )
{
	// Initlialise array ptr
	g_pMenus=NULL;

	// Create MainMenu and set is to parent
	g_hMainMenu = CreateMenu ();
	SetMenu ( hWnd, g_hMainMenu );
}

void FreeAllMenus ( void )
{
	// Delete control data
	SAFE_DELETE(g_pMenus);
	if(g_hFont) { DeleteObject(g_hFont); g_hFont=NULL; }
}

bool ConfirmMenuIndex ( int iMenuID )
{
	if ( iMenuID > 0 && iMenuID <= 65535 )
	{
		if ( iMenuID < g_iMenusMax )
		{
			if ( g_pMenus [ iMenuID ].hMenu )
			{
				return true;
			}
		}
	}
	return false;
}

bool ConfirmNewMenuIndex ( int iMenuID )
{
	if ( iMenuID > 0 && iMenuID <= 65535 )
	{
		if ( iMenuID < g_iMenusMax )
		{
			if ( g_pMenus [ iMenuID ].hMenu )
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

WinMenuData* AllocateMenuIndex ( int iMenuID )
{
	if ( g_pMenus==NULL )
	{
		// new array
		g_iMenusMax = 256;
		g_pMenus = new WinMenuData [ g_iMenusMax ];
		memset ( g_pMenus, 0, sizeof(WinMenuData) * g_iMenusMax );
	}
	else
	{
		// check if array needs expanding
		if ( iMenuID >= g_iMenusMax )
		{
			// new array
			int iNewMenusMax = iMenuID+32;
			WinMenuData* pNewMenus = new WinMenuData [ iNewMenusMax ];
			memset ( pNewMenus, 0, sizeof(WinMenuData) * iNewMenusMax );

			// copy old data over
			memcpy ( pNewMenus, g_pMenus, sizeof(WinMenuData) * g_iMenusMax );

			// delete old array
			SAFE_DELETE(g_pMenus);

			// assign new ptrs
			g_iMenusMax = iNewMenusMax;
			g_pMenus = pNewMenus;
		}
	}

	// return Menu array ptr
	return &(g_pMenus [ iMenuID ]);
}

DWORD GetStyleFlag ( int iStyle )
{
	DWORD dwStyleFlag = MF_STRING;
	switch ( iStyle )
	{
		case 1 : dwStyleFlag = MF_SEPARATOR;		break;
		case 2 : dwStyleFlag = MF_MENUBARBREAK;		break;
	}
	return dwStyleFlag;
}

//
// Menu Functions
//

bool AddItem ( int iMenuID, int iMenuTargetID, int iInsertAt, DWORD dwFlags, LPSTR pText )
{
	// Check if number valid
	if ( !ConfirmNewMenuIndex ( iMenuID ) )
		return false;

	// Allocate pMenu and get ptr
	WinMenuData* pMenu = AllocateMenuIndex ( iMenuID );
	if ( pMenu==NULL )
		return false;

	// Main Menu or SubMenu
	HMENU hThisMenu = g_hMainMenu;
	if ( iMenuTargetID > 0 )
	{
		// Check if number valid
		if ( !ConfirmMenuIndex ( iMenuTargetID ) )
			return false;

		// Get handle to parent menu
		char pParentText[256];
		HMENU hParentMenu = g_pMenus [ iMenuTargetID ].hMenu;
		GetMenuString( hParentMenu, iMenuTargetID, pParentText, 256, MF_BYCOMMAND );

		// Create popup menu if not available
		HMENU hPopupMenu = NULL;
		if ( g_pMenus [ iMenuTargetID ].hPopupMenu==NULL ) 
		{
			// Create popup
			hPopupMenu = CreatePopupMenu();

			// Modify item to add popup to target menu item
			ModifyMenu ( hParentMenu, iMenuTargetID, MF_BYCOMMAND | MF_STRING | MF_POPUP, (UINT)hPopupMenu, pParentText );

			// Store popup menu in target menu item (for future adds)
			g_pMenus [ iMenuTargetID ].hPopupMenu = hPopupMenu;
		}
		else
		{
			// Popup of the target menu
			hPopupMenu = g_pMenus [ iMenuTargetID ].hPopupMenu;
		}

		// Set new or existing popup menu as menu to add to
		hThisMenu = hPopupMenu;
	}

	// Add the menu or insert
	if ( iInsertAt!=-1 )
		InsertMenu ( hThisMenu, iInsertAt, MF_BYPOSITION | dwFlags, (UINT)iMenuID, pText );
	else
		AppendMenu ( hThisMenu, dwFlags, (UINT)iMenuID, pText );

	// Fill control data
	pMenu->hMenu = hThisMenu;

	// success
	return true;
}

bool DeleteItem ( int iMenuID )
{
	// Check if number valid
	if ( !ConfirmMenuIndex ( iMenuID ) )
		return false;

	// Delete menu
	DeleteMenu ( g_pMenus [ iMenuID ].hMenu, iMenuID, MF_BYCOMMAND );

	// Delete menu array from list
	g_pMenus [ iMenuID ].hMenu = NULL;
	g_pMenus [ iMenuID ].hPopupMenu = NULL;

	// success
	return true;
}

void UpdateMenus ( void )
{
	// Clear all once-only flags
	for ( int m=0; m<g_iMenusMax; m++ )
		g_pMenus [ m ].iSelected=0;
}

void SetTheFont ( HWND hWnd, LPSTR pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode )
{
	// Default inter code is ansicharset
	if ( iInternationalCode==0 )
		iInternationalCode = ANSI_CHARSET;

	// Create correct point size
	HDC hDC = CreateCompatibleDC ( NULL );
	DWORD nHeight = -MulDiv(iFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	DeleteDC ( hDC );

	// Create font
	g_hFont = CreateFont (	nHeight, 0, 0, 0,
							iWeight, iItalic, FALSE, FALSE,
							iInternationalCode,
							OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							NONANTIALIASED_QUALITY,
							VARIABLE_PITCH,
							pFontname );

	// set font
	SendMessage ( hWnd, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0) );
}

void SetTheState ( int iMenuID, int iAt, int iState )
{
	// Check if number valid
	if ( !ConfirmMenuIndex ( iMenuID ) )
		return;

	// Set State
	MENUITEMINFO menuitem;
	memset ( &menuitem, 0, sizeof ( MENUITEMINFO ) );
	menuitem.cbSize = sizeof( MENUITEMINFO );
	menuitem.fMask = MIIM_STATE;
	if ( iState==0 )
		menuitem.fState = MFS_DISABLED;
	else
		menuitem.fState = MFS_ENABLED;

	// apply change
	SetMenuItemInfo ( g_pMenus [ iMenuID ].hMenu, iAt, FALSE, &menuitem );

	// success
	return;
}