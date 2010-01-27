//
// WinGUI
//

// Standard Include
#include "WinGUI.h"
#include "resource.h"

// Include Core
#include ".\..\..\Shared\Core\globstruct.h"
GlobStruct*				g_pGlob = NULL;
PTR_FuncCreateStr		g_pCreateDeleteStringFunction	= NULL;

// for now a single window structure to hold stuffs
HWND					hwndOriginalParent				= NULL;
LONG					lOriginalWinProc				= 0;

// Work variables
int						m_iOnlyOneControlWithMouseAt	= -1;
DWORD					m_dwWorkStringSize				= 0;
LPSTR					m_pWorkString					= NULL;

//
// TPC error reporting structures
//
struct CErrorData
{
	DWORD dwErrorCode;
	DWORD dwErrorLine;
};
CErrorData* g_pErrorHandlerPtr = NULL;

//
// TPC Error Code Required
//
// Get Error Ptr from GLOBSTRUCT
// g_pErrorHandlerPtr = (CErrorData*)g_pGlob->g_pErrorHandlerRef;
// // Get the runtime error line number
// DWORD dwLine = g_pErrorHandlerPtr->dwErrorLine;
// // Send an error back
// g_pErrorHandlerPtr->dwErrorCode=101;


//
// Internal Subclass WinProc Function
//

LRESULT CALLBACK SubClassWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Process messages through various GUI parts
	if ( SubClassControlsWinProc ( hWnd, msg, wParam, lParam )==TRUE ) return TRUE;
	if ( SubClassMenusWinProc ( hWnd, msg, wParam, lParam )==TRUE ) return TRUE;
	return CallWindowProc ( (WNDPROC)lOriginalWinProc, hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK SubClassControlWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// find control in question
	WinControlData* pCtrl = NULL;
	for ( int c=0; c<g_iControlsMax; c++ )
	{
		// must clear cycle-dynamic values
		if ( c!=m_iOnlyOneControlWithMouseAt )
		{
			g_pControls [ c ].iMouseAtX = 0;
			g_pControls [ c ].iMouseAtY = 0;
		}

		// select this HWND on route
		if ( g_pControls [ c ].hWnd==hWnd )
		{
			m_iOnlyOneControlWithMouseAt = c;
			pCtrl = &g_pControls [ c ];
		}
	}

	// can intercept all messages to toolbar now
	if ( pCtrl )
	{
		// update any feedback values
		switch ( msg )
		{
			case WM_MOUSEMOVE:
			{
				pCtrl->iMouseAtX = LOWORD(lParam);  // horizontal position of cursor 
				pCtrl->iMouseAtY = HIWORD(lParam);  // vertical position of cursor 
			}
			break;
		}
	}
	return CallWindowProc ( (WNDPROC)pCtrl->lOriginalWinProc, hWnd, msg, wParam, lParam );
}

//
// Constructors
//

void Constructor ( void )
{
	// Clear pointers
	g_iControlsMax = 0;
	g_pControls = NULL;

	// Initialise newer controls
	InitCommonControls();
//	INITCOMMONCONTROLSEX controlsex;
//	controlsex.dwSize = sizeof(controlsex);
//	controlsex.dwICC = ICC_WIN95_CLASSES ;
//	InitCommonControlsEx ( &controlsex );

	// Init GUIControls
	InitGUIControls();
}

void Destructor ( void )
{
	// Free components within array first
	FreeAllControls( g_pGlob->hWnd );
	FreeAllMenus();
	FreeAllDialogs();

	// Restore subclass before leave
	if ( lOriginalWinProc )
	{
		// Return correct win proc before closing
		SetWindowLong ( hwndOriginalParent, GWL_WNDPROC, (LONG)lOriginalWinProc );
	}

	// Free allocations
	SAFE_DELETE ( g_pControls );
}

void ReceiveCoreDataPtr ( LPVOID pCore )
{
	// Get Core Data Pointer here
	g_pGlob = (GlobStruct*)pCore;

	// Held in Core, used here..
	g_pCreateDeleteStringFunction = g_pGlob->CreateDeleteString;

	// LEEMOD - 150803 - Get Error Ptr from GLOBSTRUCT
	g_pErrorHandlerPtr = (CErrorData*)g_pGlob->g_pErrorHandlerRef;
}

int GetDLLVersion ( void )
{
	// DBPro V1 - Patch 4
	return 14;
}

void MaintainGlobalHWND ( void )
{
	// Subclass the parent so can intercept the messages (from the controls)
	if ( g_pGlob->hWnd )
	{
		if ( lOriginalWinProc==0 )
		{
			// Only do this once per new-window
			hwndOriginalParent = g_pGlob->hWnd;
			lOriginalWinProc = SetWindowLong ( g_pGlob->hWnd, GWL_WNDPROC, (LONG)SubClassWinProc );

			// Initialise menu when get window handle
			InitGUIMenus ( hwndOriginalParent );
		}
	}
}

//
// Internal Global Functions
//

void UpdateSafeRectsArray ( void )
{
	// Clear out old safe array
	if ( g_pGlob->pSafeRects )
	{
		g_pGlob->CreateDeleteString ( (DWORD*)&g_pGlob->pSafeRects, 0 );
		g_pGlob->pSafeRects = NULL;
	}

	// Ensure all guis have safe-rects (for later primary surface rendering)
	g_pGlob->dwSafeRectMax = 0;
	for ( int c=0; c<g_iControlsMax; c++ )
		if ( g_pControls [ c ].hWndMasterControl == NULL )
			if ( g_pControls [ c ].hWnd && g_pControls [ c ].bVisible )
				g_pGlob->dwSafeRectMax++;

	// Create rect array from core data pool
	BYTE* pMemPtr = NULL;
	DWORD dwSize = sizeof(RECT) * g_pGlob->dwSafeRectMax;
	g_pGlob->CreateDeleteString ( (DWORD*)&pMemPtr, dwSize );

	// Build safe rects
	DWORD dwSafeIndex=0;
	g_pGlob->pSafeRects	= (RECT*)pMemPtr;
	for ( c=0; c<g_iControlsMax; c++ )
	{
		if ( g_pControls [ c ].hWndMasterControl == NULL )
		{
			if ( g_pControls [ c ].hWnd && g_pControls [ c ].bVisible )
			{
				g_pGlob->pSafeRects[dwSafeIndex].left	= g_pControls [ c ].iX;
				g_pGlob->pSafeRects[dwSafeIndex].right	= g_pControls [ c ].iX + g_pControls [ c ].iWidth;
				g_pGlob->pSafeRects[dwSafeIndex].top	= g_pControls [ c ].iY;
				g_pGlob->pSafeRects[dwSafeIndex].bottom	= g_pControls [ c ].iY + g_pControls [ c ].iHeight;
				dwSafeIndex++;
			}
		}
	}
}

LPSTR GetReturnStringFromWorkString(void)
{
	LPSTR pReturnString=NULL;
	if(m_pWorkString)
	{
		DWORD dwSize=strlen(m_pWorkString);
		g_pCreateDeleteStringFunction((DWORD*)&pReturnString, dwSize+1);
		strcpy(pReturnString, m_pWorkString);
	}
	return pReturnString;
}

void ValidateWorkString(DWORD dwSize)
{
	// free string that is too small
	if ( m_pWorkString )
		if ( m_dwWorkStringSize<dwSize )
			SAFE_DELETE(m_pWorkString);

	// create new work string of good size
	if ( m_pWorkString==NULL )
	{
		m_dwWorkStringSize = dwSize+1;
		m_pWorkString = new char[m_dwWorkStringSize];
		memset ( m_pWorkString, 0, m_dwWorkStringSize );
	}
}

void ValidateWorkString(LPSTR pString)
{
	// Size from string
	if ( pString ) ValidateWorkString ( strlen(pString)+1 );
}

//
// Maintenance Commands
//

void UpdateWinGUI ( void )
{
	// Ensure HWND is captured
	MaintainGlobalHWND();

	// Update GUI controls
	if ( !UpdateControls ( g_pGlob->hWnd, g_pGlob->iScreenWidth, g_pGlob->iScreenHeight ) )
	{
		// GUI has forced a quit - close the window associated with it
	}

	// Update GUI menus
	UpdateMenus();

	// Update safe rects after new addition - performance warning!
	UpdateSafeRectsArray();
}

//
// Control Commands
//

int ClearControlArea ( int iControlID, int iX, int iY, int iWidth, int iHeight )
{
	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, 0, 0, 0, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlPopup ( int iControlID, DWORD pName, int iX, int iY, int iWidth, int iHeight )
{
	// Create control
	MaintainGlobalHWND();
	if ( !CreateControlEx ( 1, iControlID, g_pGlob->hWnd, 0, 0, 0, (LPSTR)pName, iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

void DeleteControl ( int iControlID )
{
	// Delete control
	FreeControl ( g_pGlob->hWnd, iControlID );
}

int MakeControlText ( int iControlID, int iStyle, int iExtendedStyle, DWORD pText, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = SS_CENTER;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = SS_LEFTNOWORDWRAP;		break;
		case 2 : dwSelectStyle = SS_LEFT;				break;
		case 3 : dwSelectStyle = SS_CENTER;				break;
		case 4 : dwSelectStyle = SS_RIGHT;				break;
		case 5 : dwSelectStyle = SS_SIMPLE;				break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, "STATIC", dwExtendedStyle, dwSelectStyle, (LPSTR)pText, iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlButton ( int iControlID, int iStyle, int iExtendedStyle, DWORD pText, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = BS_DEFPUSHBUTTON;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = BS_PUSHBUTTON;			break;
		case 2 : dwSelectStyle = BS_RADIOBUTTON;		break;
		case 3 : dwSelectStyle = BS_AUTOCHECKBOX;		break;
		case 4 : dwSelectStyle = BS_3STATE;				break;
		case 5 : dwSelectStyle = BS_DEFPUSHBUTTON;		break;
		case 6 : dwSelectStyle = BS_AUTORADIOBUTTON;	break;
		case 7 : dwSelectStyle = BS_AUTOCHECKBOX;		break;
		case 8 : dwSelectStyle = BS_AUTO3STATE;			break;
		case 9 : dwSelectStyle = BS_ICON;				break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, "BUTTON", dwExtendedStyle, dwSelectStyle, (LPSTR)pText, iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlProgress ( int iControlID, int iStyle, int iExtendedStyle, int iMin, int iMax, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = 0;

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, PROGRESS_CLASS, dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Setup control
	SetProgressRange ( iControlID, iMin, iMax );
	SetProgressPosition ( iControlID, iMin+((iMax-iMin)/2) );

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlTabs ( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = WS_CLIPSIBLINGS;

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, WC_TABCONTROL, dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlListView ( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = LVS_LIST;
	DWORD dwIconType = LVSIL_SMALL;
	DWORD dwIconSize = 16;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = LVS_ICON | LVS_ALIGNTOP;	dwIconSize=16;	dwIconType=LVSIL_NORMAL;	break;
		case 2 : dwSelectStyle = LVS_ICON | LVS_ALIGNTOP;	dwIconSize=32;	dwIconType=LVSIL_NORMAL;	break;
		case 3 : dwSelectStyle = LVS_ICON | LVS_ALIGNTOP;	dwIconSize=48;	dwIconType=LVSIL_NORMAL;	break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, WC_LISTVIEW, dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Create image list
	g_pControls [ iControlID ].himlImageList = ImageList_Create(dwIconSize, dwIconSize, ILC_COLOR24, 0, 65535); 

	// Assign the image lists to the list view control. 
	HWND hwndLV = g_pControls [ iControlID ].hWnd;
	ListView_SetImageList(hwndLV, g_pControls [ iControlID ].himlImageList, dwIconType); 
	
	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlTreeView ( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = TVS_HASLINES;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = TVS_HASLINES;	break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, WC_TREEVIEW, dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Create the image list. 
	HIMAGELIST himl = ImageList_Create(16, 16, TRUE, 3, 0);
	g_pControls [ iControlID ].himlImageList = himl;

	// Load the icon resources, and add the icons to the image list. 
	HICON hicon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ERROR)); 
	ImageList_AddIcon(himl, hicon); 
	hicon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ERROR)); 
	ImageList_AddIcon(himl, hicon); 

	// Assign the image lists to the list view control. 
	HWND hwndLV = g_pControls [ iControlID ].hWnd;
	TreeView_SetImageList(hwndLV, g_pControls [ iControlID ].himlImageList, LVSIL_NORMAL); 
	
	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlScrollbar ( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = SBS_VERT;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = SBS_HORZ;			break;
		case 2 : dwSelectStyle = SBS_VERT;			break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, "SCROLLBAR", dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Setup control
	SetScrollBarRange ( iControlID, 0, 0 );
	SetScrollBarPosition ( iControlID, 0 );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlComboBox ( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = CBS_DROPDOWNLIST;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = CBS_SIMPLE;			break;
		case 2 : dwSelectStyle = CBS_DROPDOWN;			break;
		case 3 : dwSelectStyle = CBS_DROPDOWNLIST;		break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, "COMBOBOX", dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlSlider ( int iControlID, int iStyle, int iExtendedStyle, int iMin, int iMax, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = TBS_HORZ | TBS_NOTICKS | TBS_BOTH;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = TBS_HORZ | TBS_NOTICKS | TBS_BOTH;	break;
		case 2 : dwSelectStyle = TBS_VERT | TBS_NOTICKS | TBS_BOTH;	break;
		case 3 : dwSelectStyle = TBS_HORZ;							break;
		case 4 : dwSelectStyle = TBS_VERT;							break;
		case 5 : dwSelectStyle = TBS_HORZ | TBS_AUTOTICKS;			break;
		case 6 : dwSelectStyle = TBS_VERT | TBS_AUTOTICKS;			break;
		case 7 : dwSelectStyle = TBS_ENABLESELRANGE;				break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, TRACKBAR_CLASS, dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Ticks always represented by a line
	SendMessage ( g_pControls[iControlID].hWnd, TBM_SETTICFREQ, (WPARAM)1, 0 );

	// Setup control
	SetSliderRange ( iControlID, iMin, iMax );
	SetSliderPosition ( iControlID, iMin+((iMax-iMin)/2) );

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlEdit ( int iControlID, int iStyle, int iExtendedStyle, DWORD pText, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = ES_LEFT;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle = ES_CENTER;										break;
		case 2 : dwSelectStyle = WS_VSCROLL | ES_MULTILINE | ES_READONLY;		break;
		case 3 : dwSelectStyle = WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL;	break;
		case 4 : dwSelectStyle = ES_NUMBER | ES_CENTER;							break;
		case 5 : dwSelectStyle = ES_NUMBER | ES_CENTER | ES_READONLY;			break;
		case 6 : dwSelectStyle = ES_LOWERCASE;									break;
		case 7 : dwSelectStyle = ES_UPPERCASE;									break;
		case 8 : dwSelectStyle = ES_PASSWORD;									break;
		case 9 : dwSelectStyle = ES_MULTILINE | ES_WANTRETURN;					break;
		case 10 : dwSelectStyle = ES_MULTILINE | ES_READONLY;					break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, "EDIT", dwExtendedStyle, dwSelectStyle, (LPSTR)pText, iX, iY, iWidth, iHeight ) )
		return 0;

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlUpDown ( int iControlID, int iStyle, int iExtendedStyle, int iMin, int iMax, int iX, int iY, int iWidth, int iHeight )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = UDS_SETBUDDYINT | UDS_ARROWKEYS;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle |= 0;							break;
		case 2 : dwSelectStyle |= UDS_ALIGNLEFT;				break;
		case 3 : dwSelectStyle |= UDS_ALIGNRIGHT;				break;
		case 4 : dwSelectStyle |= UDS_HORZ;						break;
		case 5 : dwSelectStyle |= UDS_HORZ | UDS_ALIGNLEFT;		break;
		case 6 : dwSelectStyle |= UDS_HORZ | UDS_ALIGNRIGHT;	break;
		case 7 : dwSelectStyle |= UDS_WRAP;						break;
		case 8 : dwSelectStyle |= UDS_HORZ | UDS_WRAP;			break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, UPDOWN_CLASS, dwExtendedStyle, dwSelectStyle, "", iX, iY, iWidth, iHeight ) )
		return 0;

	// Setup control
	SetUpDownRange ( iControlID, iMin, iMax );
	SetUpDownPosition ( iControlID, iMin+((iMax-iMin)/2) );

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

int MakeControlToolBar ( int iControlID, int iStyle, int iExtendedStyle, DWORD pFilename, int iNumberOfToolImages )
{
	// Assign the style
	DWORD dwExtendedStyle = GetExtendedStyle ( iExtendedStyle );
	DWORD dwSelectStyle = CCS_TOP;
	switch ( iStyle )
	{
		case 1 : dwSelectStyle |= 0;							break;
	}

	// Create control
	MaintainGlobalHWND();
	if ( !CreateControl ( iControlID, g_pGlob->hWnd, TOOLBARCLASSNAME, dwExtendedStyle, dwSelectStyle, "", 0, 0, 0, 0 ) )
		return 0;

	// Create image list
	g_pControls [ iControlID ].himlImageList = ImageList_Create(16, 16, ILC_COLOR24, 0, 65535); 

	// Assign the image lists to the list view control. 
	HWND hwndTB = g_pControls [ iControlID ].hWnd;

 	// Sets the size of the TBBUTTON structure.
	SendMessage(hwndTB, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

	// Set the maximum number of text rows and bitmap size.
	SendMessage(hwndTB, TB_SETBITMAPSIZE,  0, (LPARAM)MAKELONG(16, 16));

	// Load bitmap for all buttons
	for ( int iI=0; iI<iNumberOfToolImages; iI++ )
	{
		char pFile[256];
		wsprintf ( pFile, "%s%d.bmp", pFilename, iI );
		HBITMAP hbmp = (HBITMAP)LoadImage(NULL, pFile,IMAGE_BITMAP,16,16,LR_LOADFROMFILE);
		ImageList_Add(g_pControls [ iControlID ].himlImageList,hbmp,NULL);
	}
	SendMessage(hwndTB, TB_SETIMAGELIST, 0, (LPARAM)g_pControls [ iControlID ].himlImageList);
	SendMessage(hwndTB, TB_SETHOTIMAGELIST, 0, (LPARAM)g_pControls [ iControlID ].himlImageList);

	// Subclass to get access to all control messages directly
	g_pControls [ iControlID ].lOriginalWinProc = SetWindowLong ( hwndTB, GWL_WNDPROC, (LONG)SubClassControlWinProc );

	// Update safe rects after new addition
	UpdateSafeRectsArray();

	// Success
	return 1;
}

//
// Control Setting Commands
//

void SetControlFont ( int iControlID, DWORD pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode )
{
	// set control font 
	SetFont ( iControlID, (LPSTR)pFontname, iFontSize, iWeight, iItalic, iInternationalCode );
}

void SetControlBackColor ( int iControlID, int iRed, int iGreen, int iBlue )
{
	// set control background colour
	SetBackColour ( iControlID, RGB ( iRed, iGreen, iBlue ) );
}

void SetControlForeColor ( int iControlID, int iRed, int iGreen, int iBlue )
{
	// set control foreground colour
	SetForeColour ( iControlID, RGB ( iRed, iGreen, iBlue ) );
}

void SetControlRange ( int iControlID, int iMin, int iMax )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_PROGRESS :	SetProgressRange ( iControlID, iMin, iMax );	break;
		case WINCONTROL_SLIDER :	SetSliderRange ( iControlID, iMin, iMax );		break;
		case WINCONTROL_SCROLLBAR :	SetScrollBarRange ( iControlID, iMin, iMax );	break;
	}
}

void SetControlPosition ( int iControlID, int iPosition )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_PROGRESS :	SetProgressPosition ( iControlID, iPosition );	break;
		case WINCONTROL_SLIDER :	SetSliderPosition ( iControlID, iPosition );	break;
		case WINCONTROL_SCROLLBAR :	SetScrollBarPosition ( iControlID, iPosition );	break;
	}
}

void SetControlPageSize ( int iControlID, int iPageSize )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_SCROLLBAR :	SetScrollBarPageSize ( iControlID, iPageSize );	break;
	}	
}

void SetControlText ( int iControlID, DWORD pText )
{
	SetText ( iControlID, (LPSTR) pText);
}

void SetControlIcon	( int iControlID, DWORD dwIconHandle )
{
	SetIcon ( g_pGlob->hInstance, iControlID, dwIconHandle );
}

void SetControlIconEx ( int iControlID, int iIndex, DWORD dwIconHandle )
{
	SetIcon ( g_pGlob->hInstance, iControlID, iIndex, dwIconHandle );
}

void SetControlAlignment ( int iControlID, int iAlignCode )
{
	SetAlignment ( iControlID, iAlignCode );
}

//
// Placement
//

void MoveControl ( int iControlID, int iX, int iY )
{
	Move ( iControlID, iX, iY );
	UpdateSafeRectsArray();
}

void SizeControl ( int iControlID, int iW, int iH )
{
	Size ( iControlID, iW, iH );
	UpdateSafeRectsArray();
}

//
// Control Construction Commands
//

void AddControlItem ( int iControlID, int iIndex, DWORD pText )
{
	InsertItem ( iControlID, iIndex, (LPSTR) pText );
}

void DeleteControlItem ( int iControlID, int iIndex )
{
	RemoveItem ( iControlID, iIndex );
}

void DeleteAllControlItems ( int iControlID )
{
	RemoveAllItems ( iControlID );
}

void SetControlItem ( int iControlID, int iIndex, DWORD pText )
{
	SetItem ( iControlID, iIndex, (LPSTR) pText );
}

void SetControlItemIcon ( int iControlID, int iIndex, DWORD dwIconHandle )
{
	SetItemIcon ( g_pGlob->hInstance, iControlID, iIndex, dwIconHandle );
}

void LinkControl ( int iChildControlID, int iParentControlID, int iParentIndex )
{
	Link ( iChildControlID, iParentControlID, iParentIndex );
}

void SetControlSelection ( int iControlID, DWORD dwSelection )
{
	SetSelection ( iControlID, dwSelection );
}

void HideTheControl ( int iControlID )
{
	HideControl ( iControlID );
	SetFocus ( hwndOriginalParent );
}

void ShowTheControl ( int iControlID )
{
	ShowControl ( iControlID );
}

int GetTheControlItemValue ( int iControlID )
{
	return GetControlItemValue ( iControlID );
}

int GetTheControlItemIcon ( int iControlID )
{
	return GetControlItemIcon ( iControlID );
}

int GetTheControlMouseX ( int iControlID )
{
	return GetControlMouseX ( iControlID );
}

int GetTheControlMouseY ( int iControlID )
{
	return GetControlMouseY ( iControlID );
}

//
// Default Dialog Control Commands
//

void CallFileRequester ( void )
{
	// Aquire HWND from core
	MaintainGlobalHWND();

	// Create space for return filename
	char pFile[2048];

	// Create special dialogue control
	OPENFILENAME ofn;
	memset ( &ofn, 0, sizeof(OPENFILENAME) );
	ofn.lStructSize			= sizeof(OPENFILENAME);
//	ofn.hwndOwner			= hwndOriginalParent;
//	ofn.hInstance			= (HINSTANCE) GetWindowLong(hwndOriginalParent, GWL_HINSTANCE),

	ofn.Flags			= 0;
	ofn.lpstrFilter		= "All Files (*.*)|*.*|Text Files (*.txt)|*.txt||";

	ofn.lpstrFile			= pFile;
	ofn.nMaxFile			= sizeof(pFile);

	int iResult = GetOpenFileName ( &ofn );
	DWORD dwErr = CommDlgExtendedError();
}

//
// My Menu Commands
//

void AddMenuItem ( int iMenuID, int iMenuTargetID, int iStyle, LPSTR pText )
{
	DWORD dwStyleFlag = GetStyleFlag ( iStyle );
	MaintainGlobalHWND();
	AddItem ( iMenuID, iMenuTargetID, -1, dwStyleFlag, pText );
	DrawMenuBar ( hwndOriginalParent );
}

void InsertAMenuItem ( int iMenuID, int iMenuTargetID, int iInsertPosition, int iStyle, LPSTR pText )
{
	DWORD dwStyleFlag = GetStyleFlag ( iStyle );
	MaintainGlobalHWND();
	AddItem ( iMenuID, iMenuTargetID, iInsertPosition, dwStyleFlag, pText );
	DrawMenuBar ( hwndOriginalParent );
}

void DeleteMenuItem ( int iMenuID )
{
	DeleteItem ( iMenuID );
	DrawMenuBar ( hwndOriginalParent );
}

void SetMenuFont ( LPSTR pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode )
{
	SetTheFont ( hwndOriginalParent, pFontname, iFontSize, iWeight, iItalic, iInternationalCode );
}

void SetAMenuItemState ( int iMenuID, int iPosition, int iState )
{
	SetTheState ( iMenuID, iPosition, iState );
}

//
// My Dialog Commands - Print
//

int	DialogPrintBox ( void )
{
	return PrintBox();
}

void DialogAddTextToPage ( LPSTR pText, int x, int y, int centered )
{
	AddTextToPage ( pText, x, y, centered );
}

void DialogEndPrinting ( void )
{
	EndPrinting();
}

//
// My Dialog Commands - File
//

DWORD DialogCoreFileBox ( DWORD pDestStr, int iFileType )
{
	// Get filename for open
	char m_pWorkString[256];
	switch ( iFileType )
	{
		case 1 : FileOpenBox ( m_pWorkString );	break;
		case 2 : FileSaveBox ( m_pWorkString );	break;
	}

	// Create and return string
	LPSTR pReturnString;
	if(pDestStr) g_pCreateDeleteStringFunction((DWORD*)&pDestStr, 0);
	DWORD dwSize=strlen(m_pWorkString);
	g_pCreateDeleteStringFunction((DWORD*)&pReturnString, dwSize+1);
	strcpy(pReturnString, m_pWorkString);

	// Return new string
	return (DWORD)pReturnString;
}

DWORD DialogOpenFileBox ( DWORD pDestStr )
{
	return DialogCoreFileBox ( pDestStr, 1 );
}


DWORD DialogSaveFileBox ( DWORD pDestStr )
{
	return DialogCoreFileBox ( pDestStr, 2 );
}

//
// My Dialog Commands - File
//

void DialogBrowserBox ( DWORD pURLString )
{
	BrowserBox ( (LPSTR)pURLString );
}

void DialogBrowserBoxClose ( void )
{
	BrowserBoxClose();
}


//
// My Windows GUI System Commands
//

void SetWinGUIColors ( int iSystemColorIndex, int iRed, int iGreen, int iBlue )
{
	SetSystemColors ( iSystemColorIndex, RGB(iRed,iGreen,iBlue) );
}

void RestoreWinGUIColors ( void )
{
	RestoreSystemColors();
}

//
// Control Expressions
//

int GetControlClick ( int iControlID )
{
	// confirm control exists
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// return control data
	return g_pControls [ iControlID ].iClicked;
}

int GetControlValue ( int iControlID )
{
	// confirm control exists
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// return control data
	return GetSliderPosition ( iControlID );
}

DWORD GetControlString ( DWORD pDestStr, int iControlID )
{
	// confirm control exists
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// fill string with path
	ValidateWorkString ( 512 );
	GetString ( iControlID, m_pWorkString );

	// Create and return string
	if(pDestStr) g_pCreateDeleteStringFunction((DWORD*)&pDestStr, 0);
	LPSTR pReturnString=GetReturnStringFromWorkString();
	return (DWORD)pReturnString;
}

DWORD GetControlSelected ( int iControlID )
{
	// confirm control exists
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// return control data
	return GetSelection ( iControlID );
}

int GetControlSelectionCount ( int iControlID )
{
	// confirm control exists
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// return control data
	return GetControlCount ( iControlID );
}

//
// Menu Expressions
//

int GetMenuSelected ( int iMenuID )
{
	// Check if number valid
	if ( !ConfirmMenuIndex ( iMenuID ) )
		return false;

	// return menu selected state
	return g_pMenus [ iMenuID ].iSelected;
}
