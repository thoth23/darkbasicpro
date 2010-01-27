//
// GUIControls
//

// Includes
#include "GUIControls.h"
#include "atlbase.h"

// Global Data Pointers
int					g_iControlsMax		= 0;
WinControlData*		g_pControls			= NULL;

// General Font for GUI
HFONT				hfontCurrentFont	= NULL;

// WinGUI System Globals
DWORD aDefaultSystemColors[30]; 
DWORD aCurrentSystemColors[30]; 

//
// Internal Controls SubClass Functions
//

void OnlyShowOneAreaOfTabControl ( HWND hTabWnd, int iPage )
{
	for ( int c=0; c<g_iControlsMax; c++ )
	{
		if ( g_pControls [ c ].hWndMasterControl==hTabWnd )
		{
			if ( g_pControls [ c ].iMasterIndexID==iPage )
			{
				ShowWindow ( g_pControls [ c ].hWnd, SW_SHOW );
				g_pControls [ c ].bVisible=true;
			}
			else
			{
				ShowWindow ( g_pControls [ c ].hWnd, SW_HIDE );
				g_pControls [ c ].bVisible=false;
			}
		}
	}
}

LRESULT SubClassControlsWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
    {
		case WM_COMMAND:
		{
			// GET CLICK
			for ( int c=0; c<g_iControlsMax; c++ )
			{
				if ( g_pControls [ c ].hWnd )
				{
					if ( g_pControls [ c ].sType==WINCONTROL_POPUP 
					||	 g_pControls [ c ].sType==WINCONTROL_BUTTON 
					||	 g_pControls [ c ].sType==WINCONTROL_COMBOBOX
					||	 g_pControls [ c ].sType==WINCONTROL_TOOLBAR  )
					{
						if ( (HWND)lParam == g_pControls [ c ].hWnd )
						{
							// set click flag
							g_pControls [ c ].iClicked = 1;

							// optionally set selected value if tookbar
							if ( g_pControls [ c ].sType==WINCONTROL_TOOLBAR )
								g_pControls [ c ].iLastSelected = wParam;

							return TRUE; 
						}
					}
				}
			}
			break;
		}
		
        case WM_NOTIFY: 
		{
			LPNMHDR pnmh = (LPNMHDR) lParam;
            switch (pnmh->code)
			{ 
                case TCN_SELCHANGE :
				{ 
					HWND hTabWnd = pnmh->hwndFrom;
                    int iPage = TabCtrl_GetCurSel(hTabWnd); 
					OnlyShowOneAreaOfTabControl ( hTabWnd, iPage );
                } 
                break; 

                case NM_CLICK :
				{
					HWND hWnd = pnmh->hwndFrom;
					for ( int c=0; c<g_iControlsMax; c++ )
					{
						if ( g_pControls [ c ].hWnd==hWnd )
						{
							g_pControls [ c ].iClicked = 1;
							break;
						}
					}
                } 
                break; 
			}
            break; 
		}

		case WM_VSCROLL:
		{
			// message from a scrollbar
			int nScrollCode = (int) LOWORD(wParam); // scroll bar value 
			int nPos = (short int) HIWORD(wParam);  // scroll box position 
			HWND hwndScrollBar = (HWND) lParam;      // handle to scroll bar 
			for ( int c=0; c<g_iControlsMax; c++ )
			{
				if ( g_pControls [ c ].hWnd==hwndScrollBar )
				{
					// get additional values
					SCROLLINFO scrollinfo;
					memset ( &scrollinfo, 0, sizeof(scrollinfo) );
					scrollinfo.cbSize=sizeof(scrollinfo);
					scrollinfo.fMask=SIF_RANGE | SIF_PAGE | SIF_POS;
					GetScrollInfo ( hwndScrollBar, SB_CTL, &scrollinfo );

					// handle position change
					int newPos = -123;
					if ( nScrollCode==SB_TOP )				newPos=0;
					if ( nScrollCode==SB_BOTTOM )			newPos=scrollinfo.nMax-(scrollinfo.nPage-1);
					if ( nScrollCode==SB_LINEDOWN )			newPos=scrollinfo.nPos+1;
					if ( nScrollCode==SB_LINEUP )			newPos=scrollinfo.nPos-1;
					if ( nScrollCode==SB_PAGEDOWN )			newPos=scrollinfo.nPos+scrollinfo.nPage;
					if ( nScrollCode==SB_PAGEUP )			newPos=scrollinfo.nPos-scrollinfo.nPage;
					if ( nScrollCode==SB_THUMBPOSITION )	newPos=nPos;

					// only perform if change required
					if ( newPos!=-123 )
					{
						// update control data
						g_pControls [ c ].iClicked = 1;

						// update scrollbar
						memset ( &scrollinfo, 0, sizeof(scrollinfo) );
						scrollinfo.cbSize=sizeof(scrollinfo);
						scrollinfo.fMask=SIF_POS;
						scrollinfo.nPos=newPos;
						SetScrollInfo ( hwndScrollBar, SB_CTL, &scrollinfo, TRUE );
					}
					break;
				}
			}
		}
		break;
    }
	return FALSE;
}

//
// Internal Functions
//

void InitGUIControls ( void )
{
	// Initlialise array ptr
	g_pControls=NULL;

	// Capture all system colors
	CaptureSystemColors();

	// Create default controls font (cool Tahoma)
	hfontCurrentFont = CreateFont (	16, 0, 0, 0,
									400, 0, FALSE, FALSE,
									ANSI_CHARSET,
									OUT_DEFAULT_PRECIS,
									CLIP_DEFAULT_PRECIS,
									NONANTIALIASED_QUALITY,
									VARIABLE_PITCH,
									"Tahoma" );
}

void FreeAllControls ( HWND hwndParent )
{
	// Delete current GUI font
	DeleteObject ( hfontCurrentFont );

	// Free all componetnts of controls
	for ( int c=0; c<g_iControlsMax; c++ )
		FreeControl ( hwndParent, c );

	// Delete control data
	SAFE_DELETE(g_pControls);
}

//
// Internal Functions
//

bool ConfirmControlIndex ( int iControlID )
{
	if ( iControlID < g_iControlsMax )
	{
		if ( g_pControls [ iControlID ].hWnd )
		{
			return true;
		}
	}
	return false;
}

bool ConfirmNewControlIndex ( int iControlID )
{
	if ( iControlID < g_iControlsMax )
	{
		if ( g_pControls [ iControlID ].hWnd )
		{
			return false;
		}
	}
	return true;
}

WinControlData* AllocateControlIndex ( int iControlID )
{
	if ( g_pControls==NULL )
	{
		// new array
		g_iControlsMax = 256;
		g_pControls = new WinControlData [ g_iControlsMax ];
		memset ( g_pControls, 0, sizeof(WinControlData) * g_iControlsMax );
	}
	else
	{
		// check if array needs expanding
		if ( iControlID >= g_iControlsMax )
		{
			// new array
			int iNewControlsMax = iControlID+32;
			WinControlData* pNewControls = new WinControlData [ iNewControlsMax ];
			memset ( pNewControls, 0, sizeof(WinControlData) * iNewControlsMax );

			// copy old data over
			memcpy ( pNewControls, g_pControls, sizeof(WinControlData) * g_iControlsMax );

			// delete old array
			SAFE_DELETE(g_pControls);

			// assign new ptrs
			g_iControlsMax = iNewControlsMax;
			g_pControls = pNewControls;
		}
	}

	// return control array ptr
	return &(g_pControls [ iControlID ]);
}

DWORD GetExtendedStyle ( int iExtendedStyle )
{
	DWORD dwExtendedStyle = 0;
	switch ( iExtendedStyle )
	{
		case 1 : dwExtendedStyle = WS_EX_CLIENTEDGE;				break;
		case 2 : dwExtendedStyle = WS_EX_LEFT;						break;
		case 3 : dwExtendedStyle = WS_EX_OVERLAPPEDWINDOW;			break;
		case 4 : dwExtendedStyle = WS_EX_PALETTEWINDOW;				break;
		case 5 : dwExtendedStyle = WS_EX_RIGHT;						break;
		case 6 : dwExtendedStyle = WS_EX_STATICEDGE;				break;
		case 7 : dwExtendedStyle = WS_EX_TOOLWINDOW;				break;
		case 8 : dwExtendedStyle = WS_EX_TOPMOST;					break;
		case 9 : dwExtendedStyle = WS_EX_WINDOWEDGE;				break;
	}
	return dwExtendedStyle;
}

//
// Internal Control Creation Functions
//

LRESULT SubClassPopupWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CreateControlEx ( int iMode, int iControlID, HWND hwndParent, LPCTSTR pWindowType, DWORD dwExtendedStyle, DWORD dwWindowStyle, LPSTR pText, int iX, int iY, int iWidth, int iHeight )
{
	// Check if contol number valid
	if ( !ConfirmNewControlIndex ( iControlID ) )
		return false;

	// Allocate control and get ptr
	WinControlData* pCtrl = AllocateControlIndex ( iControlID );
	if ( pCtrl==NULL )
		return false;

	// Create control window
	if ( iMode==1 )
	{
		// Create a popup window
		WNDCLASS wndClass;
		memset ( &wndClass, 0, sizeof ( WNDCLASS ) );
		wndClass.lpfnWndProc   = (WNDPROC)SubClassPopupWinProc;
		wndClass.hInstance     = (HINSTANCE) GetWindowLong(hwndParent, GWL_HINSTANCE);
		wndClass.lpszClassName = pText; 
		RegisterClass(&wndClass);
		pCtrl->hWnd =  CreateWindow(	pText,
										pText,
										WS_CAPTION | WS_THICKFRAME | WS_CHILD | WS_VISIBLE,
										iX, iY, iWidth, iHeight,
										hwndParent, NULL,
										(HINSTANCE) GetWindowLong(hwndParent, GWL_HINSTANCE),
										NULL );

		// For holding control only (for now)
		EnableWindow ( pCtrl->hWnd, FALSE );
	}
	else
	{
		if ( pWindowType != NULL )
		{
			pCtrl->hWnd =  CreateWindowEx(	dwExtendedStyle,
											pWindowType,
											pText,
											WS_VISIBLE | WS_CHILD | dwWindowStyle,
											iX, iY, iWidth, iHeight,
											hwndParent, NULL,
											(HINSTANCE) GetWindowLong(hwndParent, GWL_HINSTANCE),
											NULL );
		}
		else
		{
			pCtrl->hWnd =  CreateWindowEx(	0,
											"STATIC",
											"",
											WS_CHILD,
											0, 0, 32, 32,
											hwndParent, NULL,
											(HINSTANCE) GetWindowLong(hwndParent, GWL_HINSTANCE),
											NULL );
		}
	}

	// Fill control data
	pCtrl->bVisible		= true;
	pCtrl->iX			= iX;
	pCtrl->iY			= iY;
	pCtrl->iWidth		= iWidth;
	pCtrl->iHeight		= iHeight;

	// Determine control with type value
	pCtrl->sType = WINCONTROL_CLEARAREA;
	if ( iMode==1 )
	{
		pCtrl->sType = WINCONTROL_POPUP;
	}
	else
	{
		if ( pCtrl->hWnd && pWindowType!=NULL )
		{
			if ( stricmp ( pWindowType, "STATIC" )==NULL )		pCtrl->sType = WINCONTROL_TEXT;
			if ( stricmp ( pWindowType, "BUTTON" )==NULL )		pCtrl->sType = WINCONTROL_BUTTON;
			if ( pWindowType==PROGRESS_CLASS )					pCtrl->sType = WINCONTROL_PROGRESS;
			if ( pWindowType==WC_TABCONTROL )					pCtrl->sType = WINCONTROL_TAB;
			if ( pWindowType==WC_LISTVIEW )						pCtrl->sType = WINCONTROL_LISTVIEW;
			if ( pWindowType==WC_TREEVIEW )						pCtrl->sType = WINCONTROL_TREEVIEW;
			if ( stricmp ( pWindowType, "SCROLLBAR" )==NULL )	pCtrl->sType = WINCONTROL_SCROLLBAR;
			if ( stricmp ( pWindowType, "COMBOBOX" )==NULL )	pCtrl->sType = WINCONTROL_COMBOBOX;
			if ( pWindowType==TRACKBAR_CLASS )					pCtrl->sType = WINCONTROL_SLIDER;
			if ( stricmp ( pWindowType, "EDIT" )==NULL )		pCtrl->sType = WINCONTROL_EDIT;
			if ( pWindowType==UPDOWN_CLASS )					pCtrl->sType = WINCONTROL_UPDOWN;
			if ( pWindowType==TOOLBARCLASSNAME )				pCtrl->sType = WINCONTROL_TOOLBAR;
		}
	}

	// Assign the window our current GUI font
	if ( hfontCurrentFont )
	{
		// set new font for control
		SendMessage ( pCtrl->hWnd, WM_SETFONT, (WPARAM)hfontCurrentFont, MAKELPARAM(TRUE, 0) );
	}

	// success
	return true;
}

bool CreateControl ( int iControlID, HWND hwndParent, LPCTSTR pWindowType, DWORD dwExtendedStyle, DWORD dwWindowStyle, LPSTR pText, int iX, int iY, int iWidth, int iHeight )
{
	return CreateControlEx ( 0, iControlID, hwndParent, pWindowType, dwExtendedStyle, dwWindowStyle, pText, iX, iY, iWidth, iHeight );
}

bool UpdateControls ( HWND hwndParent, int iScreenWidth, int iScreenHeight )
{
	// Wuit flag
	bool bProceedNoQuit=true;

	// Clear all once-only flags
	for ( int c=0; c<g_iControlsMax; c++ )
		g_pControls [ c ].iClicked=0;

	// Ensure all controls are adjusted based on client area and screen size
	RECT clientrc = { 0,0,0,0 };
	GetClientRect(hwndParent, &clientrc);

	// calculate scaling between src and dest
	float fX = (float)clientrc.right / (float)iScreenWidth;
	float fY = (float)clientrc.bottom / (float)iScreenHeight;

	// check if refresh required
	bool bRefreshRequired = false;
	for ( c=0; c<g_iControlsMax; c++ )
	{
		if ( g_pControls [ c ].hWnd )
		{
			// Calculate scaled position of control
			int iX		= (int)( g_pControls [ c ].iX								* fX);
			int iY		= (int)( g_pControls [ c ].iY								* fY);
			int iX2		= (int)((g_pControls [ c ].iX + g_pControls [ c ].iWidth )	* fX);
			int iY2		= (int)((g_pControls [ c ].iY + g_pControls [ c ].iHeight)	* fY);
			int iUpSum	= iX+iY+iX2+iY2+g_pControls [ c ].bVisible;

			// Update only if position changes
			if ( iUpSum != g_pControls [ c ].iUpdateSum )
				bRefreshRequired=true;
		}
	}

	// update the controls
	if ( bRefreshRequired==true )
	{
		// Update controls of window
		for ( c=0; c<g_iControlsMax; c++ )
		{
			if ( g_pControls [ c ].hWnd )
			{
				// Calculate scaled position of control
				int iX		= (int)( g_pControls [ c ].iX								* fX);
				int iY		= (int)( g_pControls [ c ].iY								* fY);
				int iX2		= (int)((g_pControls [ c ].iX + g_pControls [ c ].iWidth )	* fX);
				int iY2		= (int)((g_pControls [ c ].iY + g_pControls [ c ].iHeight)	* fY);
				int iUpSum	= iX+iY+iX2+iY2+g_pControls [ c ].bVisible;
				g_pControls [ c ].iUpdateSum = iUpSum;

				// Control type
				if ( g_pControls [ c ].sType==WINCONTROL_CLEARAREA )
				{
					// Clear parent window
					RECT rc = { iX, iY, iX2, iY2 };
					InvalidateRect ( hwndParent, &rc, FALSE );
				}
				else
				{
					// Update control
					SetWindowPos ( g_pControls [ c ].hWnd, NULL, iX, iY, iX2-iX, iY2-iY, SWP_NOZORDER );
					InvalidateRect ( g_pControls [ c ].hWnd, NULL, FALSE );
				}
			}
		}
	}

	// return proceed flag
	return bProceedNoQuit;
}

void FreeControl ( HWND hwndParent, int iControlID )
{
	// free linkage
	if ( g_pControls[iControlID].hWndMasterControl )
	{
		SetParent ( g_pControls[iControlID].hWnd, hwndParent );
		g_pControls[iControlID].hWndMasterControl=NULL;
	}

	// return correct win proc before closing
	if ( g_pControls [ iControlID ].lOriginalWinProc )
		SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_WNDPROC, (LONG)g_pControls [ iControlID ].lOriginalWinProc );

	// delete font object when done
	if ( g_pControls [ iControlID ].hfont )
		DeleteObject ( g_pControls [ iControlID ].hfont );

	// free control window
	DestroyWindow ( g_pControls [ iControlID ].hWnd );

	// clear structure
	memset ( &g_pControls[iControlID], 0, sizeof(g_pControls[iControlID]) );
}

void HideControl ( int iControlID )
{
	ShowWindow ( g_pControls [ iControlID ].hWnd, SW_HIDE );
}

void ShowControl ( int iControlID )
{
	ShowWindow ( g_pControls [ iControlID ].hWnd, SW_SHOW );
}

void Move ( int iControlID, int iX, int iY )
{
	g_pControls [ iControlID ].iX = iX;
	g_pControls [ iControlID ].iY = iY;
	SetWindowPos ( g_pControls [ iControlID ].hWnd, NULL, iX, iY, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
}

void Size ( int iControlID, int iW, int iH )
{
	g_pControls [ iControlID ].iWidth = iW;
	g_pControls [ iControlID ].iHeight = iH;
	SetWindowPos ( g_pControls [ iControlID ].hWnd, NULL, 0, 0, iW, iH, SWP_NOMOVE | SWP_NOZORDER );
}

//
// Internal Control Setting Functions
//

void SetFont ( int iControlID, LPSTR pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// Default inter code is ansicharset
	if ( iInternationalCode==0 )
		iInternationalCode = ANSI_CHARSET;

	// Create correct point size
	HDC hDC = CreateCompatibleDC ( NULL );
	DWORD nHeight = -MulDiv(iFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	DeleteDC ( hDC );

	// Create font
	g_pControls[iControlID].hfont = CreateFont (	nHeight, 0, 0, 0,
													iWeight, iItalic, FALSE, FALSE,
													iInternationalCode,
													OUT_DEFAULT_PRECIS,
													CLIP_DEFAULT_PRECIS,
													NONANTIALIASED_QUALITY,
													VARIABLE_PITCH,
													pFontname );

	// set control property
	SendMessage ( g_pControls[iControlID].hWnd, WM_SETFONT, (WPARAM)g_pControls[iControlID].hfont, MAKELPARAM(TRUE, 0) );
}

void SetBackColour ( int iControlID, DWORD dwBack )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	HWND hWnd = g_pControls[iControlID].hWnd;
	LPARAM lParam = (LPARAM)(COLORREF)dwBack;
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_PROGRESS : SendMessage ( hWnd, PBM_SETBKCOLOR , 0, lParam ); break;
	}
}

void SetForeColour ( int iControlID, DWORD dwFore )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	HWND hWnd = g_pControls[iControlID].hWnd;
	LPARAM lParam = (LPARAM)(COLORREF)dwFore;
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_PROGRESS : SendMessage ( hWnd, PBM_SETBARCOLOR , 0, lParam ); break;
	}
}

void SetProgressRange ( int iControlID, int nMinRange, int nMaxRange )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SendMessage ( g_pControls[iControlID].hWnd, PBM_SETRANGE, 0, MAKELPARAM(nMinRange, nMaxRange) );
}

void SetProgressPosition ( int iControlID, int iPosition )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SendMessage ( g_pControls[iControlID].hWnd, PBM_SETPOS, (WPARAM)iPosition, 0 );
}

void SetSliderRange ( int iControlID, int nMinRange, int nMaxRange )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SendMessage ( g_pControls[iControlID].hWnd, TBM_SETRANGE, (WPARAM)TRUE, MAKELONG(nMinRange, nMaxRange) );
}

void SetSliderPosition ( int iControlID, int iPosition )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SendMessage ( g_pControls[iControlID].hWnd, TBM_SETPOS, (WPARAM) TRUE, (LPARAM)iPosition );
}

int GetSliderPosition ( int iControlID )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// get control property
	return SendMessage ( g_pControls[iControlID].hWnd, TBM_GETPOS, (WPARAM)0, (LPARAM)0 );
}

void SetUpDownRange ( int iControlID, int nMinRange, int nMaxRange )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SendMessage ( g_pControls[iControlID].hWnd, UDM_SETRANGE , 0, MAKELONG((short)nMaxRange, (short)nMinRange) );
}

void SetUpDownPosition ( int iControlID, int iPosition )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SendMessage ( g_pControls[iControlID].hWnd, UDM_SETPOS, 0, (LPARAM)(short)iPosition );
}

void SetScrollBarRange ( int iControlID, int nMinRange, int nMaxRange )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SCROLLINFO scrollinfo;
	memset ( &scrollinfo, 0, sizeof(scrollinfo) );
	scrollinfo.cbSize=sizeof(scrollinfo);
	scrollinfo.fMask=SIF_RANGE;
	scrollinfo.nMin=nMinRange;
	scrollinfo.nMax=nMaxRange;
	SetScrollInfo ( g_pControls[iControlID].hWnd, SB_CTL, &scrollinfo, TRUE );
}

void SetScrollBarPosition ( int iControlID, int iPosition )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SCROLLINFO scrollinfo;
	memset ( &scrollinfo, 0, sizeof(scrollinfo) );
	scrollinfo.cbSize=sizeof(scrollinfo);
	scrollinfo.fMask=SIF_POS;
	scrollinfo.nPos=iPosition;
	SetScrollInfo ( g_pControls[iControlID].hWnd, SB_CTL, &scrollinfo, TRUE );
}

void SetScrollBarPageSize ( int iControlID, int iPageSize )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SCROLLINFO scrollinfo;
	memset ( &scrollinfo, 0, sizeof(scrollinfo) );
	scrollinfo.cbSize=sizeof(scrollinfo);
	scrollinfo.fMask=SIF_PAGE;
	scrollinfo.nPage=iPageSize;
	SetScrollInfo ( g_pControls[iControlID].hWnd, SB_CTL, &scrollinfo, TRUE );
}

void SetText ( int iControlID, LPSTR lpText )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// set control property
	SetWindowText ( g_pControls[iControlID].hWnd, lpText );
}

void SetIcon ( HINSTANCE hInstance, int iControlID, DWORD dwIconHandle )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_BUTTON :
		{
			SendMessage ( g_pControls[iControlID].hWnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)dwIconHandle );
			InvalidateRect ( g_pControls[iControlID].hWnd, NULL, FALSE );
			UpdateWindow ( g_pControls[iControlID].hWnd );
			break;
		}
	}
}

void SetIcon ( HINSTANCE hInstance, int iControlID, int iIndex, DWORD dwIconHandle )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TOOLBAR :
		{
			SendMessage ( g_pControls[iControlID].hWnd, TB_CHANGEBITMAP, (WPARAM)1+iIndex, (LPARAM) MAKELPARAM(dwIconHandle, 0) );
			break;
		}
	}
}

void SetAlignment ( int iControlID, int iAlignCode )
{
	// capture old long
	DWORD dwMaskLong = 0;
	DWORD dwLong = GetWindowLong ( g_pControls[iControlID].hWnd, GWL_STYLE );

	// adjust alignment of control
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TEXT :	dwMaskLong = SS_LEFT | SS_CENTER | SS_RIGHT;
								dwMaskLong = 0xFFFFFFFF - dwMaskLong;
								dwLong &= dwMaskLong;
								switch ( iAlignCode )
								{
									case 1 :	dwLong |= SS_LEFT;		break;
									case 2 :	dwLong |= SS_CENTER;	break;
									case 3 :	dwLong |= SS_RIGHT;		break;
								}
								break;
	}

	// Update control long
	SetWindowLong ( g_pControls [ iControlID ].hWnd, GWL_STYLE, dwLong );	
}

void SetSelection ( int iControlID, DWORD dwSelection )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			break;
		}

		case WINCONTROL_LISTVIEW :
		{
			ListView_SetSelectionMark( g_pControls[iControlID].hWnd, dwSelection );
			SetFocus ( g_pControls[iControlID].hWnd );
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			TreeView_Select ( g_pControls[iControlID].hWnd, dwSelection, TVGN_FIRSTVISIBLE );
			TreeView_SelectItem ( g_pControls[iControlID].hWnd, dwSelection );
			SetFocus ( g_pControls[iControlID].hWnd );
			break;
		}

		case WINCONTROL_COMBOBOX :
		{
            SendMessage ( g_pControls[iControlID].hWnd, CB_SETCURSEL, dwSelection, 0 ); 
			break;
		}
	}
}

DWORD GetSelection ( int iControlID )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// get control property
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_LISTVIEW :
		{
			return SendMessage ( g_pControls[iControlID].hWnd, LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0 );
		}

		case WINCONTROL_TREEVIEW :
		{
			return (DWORD)TreeView_GetSelection ( g_pControls[iControlID].hWnd );
		}

		case WINCONTROL_COMBOBOX :
		{
			return SendMessage ( g_pControls[iControlID].hWnd, CB_GETCURSEL, (WPARAM)0, (LPARAM)0 );
		}

		case WINCONTROL_SCROLLBAR :
		{
			SCROLLINFO scrollinfo;
			memset ( &scrollinfo, 0, sizeof(scrollinfo) );
			scrollinfo.cbSize=sizeof(scrollinfo);
			scrollinfo.fMask=SIF_POS;
			GetScrollInfo ( g_pControls[iControlID].hWnd, SB_CTL, &scrollinfo );
			return scrollinfo.nPos;
		}
		break;

		case WINCONTROL_TOOLBAR :
		{
			return g_pControls[iControlID].iLastSelected;
		}
		break;
	}

	return 0;
}

int GetControlCount ( int iControlID )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return 0;

	// get control property
	return SendMessage ( g_pControls[iControlID].hWnd, LVM_GETITEMCOUNT, (WPARAM)0, (LPARAM)0 );
}

void GetString ( int iControlID, LPSTR pString )
{
	// Check if contol number valid
	if ( !ConfirmControlIndex ( iControlID ) )
		return;

	// get control string
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_EDIT :
		{
			// get text from control
			SendMessage ( g_pControls[iControlID].hWnd, WM_GETTEXT, (WPARAM)256, (LPARAM)pString );
		}
		break;

		case WINCONTROL_LISTVIEW :
		{
			// get current selection
			int iCurrent = ListView_GetSelectionMark ( g_pControls[iControlID].hWnd );

			// get text from item
			LVITEM currentlvi;
			memset ( &currentlvi, 0, sizeof(currentlvi) );
			currentlvi.pszText = new char[512];
			currentlvi.cchTextMax = 512;
			currentlvi.mask = LVIF_TEXT;
			currentlvi.iItem = iCurrent;
			currentlvi.iSubItem = 0; 
			ListView_GetItem(g_pControls[iControlID].hWnd, &currentlvi); 

			// Copy text to string
			strcpy(pString, currentlvi.pszText);

			// free usages
			SAFE_DELETE ( currentlvi.pszText );
		}
		break;

		case WINCONTROL_TREEVIEW :
		{
			// get current selection
			HTREEITEM hCurrent = TreeView_GetSelection ( g_pControls[iControlID].hWnd );

			// must be adding back to root
			strcpy(pString, "");
			HTREEITEM pFindItem = hCurrent;
			while(pFindItem) 
			{
				// get text from item
				TVITEM currenttvi;
				memset ( &currenttvi, 0, sizeof(currenttvi));
				currenttvi.pszText = new char[512];
				currenttvi.cchTextMax = 512;
				currenttvi.mask = TVIF_TEXT;
				currenttvi.hItem = pFindItem;
				TreeView_GetItem(g_pControls[iControlID].hWnd, &currenttvi); 

				// add it to start of string chain
				char pNewString[512];
				strcpy ( pNewString, currenttvi.pszText );
				strcat ( pNewString, "\\" );
				strcat ( pNewString, pString );
				strcpy ( pString, pNewString );

				// free usages
				SAFE_DELETE ( currenttvi.pszText );

				// next one
				pFindItem = TreeView_GetParent ( g_pControls[iControlID].hWnd, pFindItem );
			}
		}
		break;
	}
}

//
// Control Construction Functions
//

void InsertItem ( int iControlID, int iIndex, LPSTR lpText )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			TCITEM tie;
			memset ( &tie, 0, sizeof(tie) );
			tie.mask = TCIF_TEXT | TCIF_IMAGE; 
			tie.iImage = -1; 
			tie.pszText = lpText; 
			TabCtrl_InsertItem ( g_pControls[iControlID].hWnd, iIndex, &tie );
			break;
		}

		case WINCONTROL_LISTVIEW :
		{
			// Add an icon to each image list
			HICON hiconItem = LoadIcon(NULL, IDI_WINLOGO);
			int iImage = ImageList_AddIcon(g_pControls[iControlID].himlImageList, hiconItem); 
			DeleteObject(hiconItem); 

			// Add a list item 
			LVITEM lvi; 
			memset ( &lvi, 0, sizeof(lvi) );
			lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
			lvi.state = 0; 
			lvi.stateMask = 0; 
			lvi.iItem = iIndex; 
			lvi.iSubItem = 0; 
			lvi.lParam = (LPARAM) 0;
			lvi.iImage = iImage; 
			lvi.pszText = lpText;
			ListView_InsertItem( g_pControls[iControlID].hWnd, &lvi);

			// done
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			// Add an item 
			TVITEM tvi; 
			memset ( &tvi, 0, sizeof(tvi) );
			tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
			tvi.pszText = lpText;
			tvi.cchTextMax = lstrlen(lpText); 
			tvi.iImage = 0;
			tvi.iSelectedImage = 0;
			tvi.lParam = (LPARAM) iIndex;

			// Assign place in tree
			TVINSERTSTRUCT tvins; 
			memset ( &tvins, 0, sizeof(tvins) );
			tvins.item = tvi; 

			// Index refers to the level of hierarchy
			// 0-root
			// 1-1st nest
			// 2-2nd nest, etc
			bool bAddedAsChild = false;
			HTREEITEM hCurrent = TreeView_GetSelection ( g_pControls[iControlID].hWnd );

			// find current hierarchy
			TVITEM currenttvi;
			memset ( &currenttvi, 0, sizeof(currenttvi) );
			if ( hCurrent )
			{
				currenttvi.mask = TVIF_PARAM;
				currenttvi.hItem = hCurrent;
				TreeView_GetItem(g_pControls[iControlID].hWnd, &currenttvi); 
			}
			else
				memset ( &currenttvi, 0, sizeof(currenttvi) );
			
			// if same, add as symbling
			if ( iIndex==currenttvi.lParam )
			{
				tvins.hParent = TreeView_GetParent ( g_pControls[iControlID].hWnd, hCurrent ); 
				tvins.hInsertAfter = hCurrent; 
			}
			else
			{
				// if index greater, add as child
				if ( iIndex>currenttvi.lParam )
				{
					tvins.hParent = hCurrent; 
					tvins.hInsertAfter = TVI_LAST; 
					bAddedAsChild = true;
				}
				else
				{
					// must be adding back to earlier nest, find last item with hierarchy match
					HTREEITEM pFindItem = TreeView_GetParent ( g_pControls[iControlID].hWnd, hCurrent );
					while(pFindItem) 
					{
						currenttvi.mask = TVIF_PARAM;
						currenttvi.hItem = pFindItem;
						TreeView_GetItem(g_pControls[iControlID].hWnd, &currenttvi); 
						if ( iIndex==currenttvi.lParam )
						{
							// found hierarchy to add to
							break;
						}
						pFindItem = TreeView_GetParent ( g_pControls[iControlID].hWnd, pFindItem );
					}
					tvins.hParent = TreeView_GetParent ( g_pControls[iControlID].hWnd, pFindItem );
					tvins.hInsertAfter = TVI_LAST; 

					// Collapse opened hierarchy when done
					SendMessage ( g_pControls[iControlID].hWnd, TVM_EXPAND, (WPARAM)(UINT)TVE_COLLAPSE, (LPARAM)(HTREEITEM)pFindItem ); 
				}
			}

			// Add the item to the tree view control. 
			HTREEITEM nTheItem = (HTREEITEM) SendMessage(g_pControls[iControlID].hWnd, TVM_INSERTITEM, 0, (LPARAM) (LPTVINSERTSTRUCT) &tvins); 

			// ensure parent image changes when add as a child item
			if ( bAddedAsChild==true )
			{
				HTREEITEM hti = TreeView_GetParent(g_pControls[iControlID].hWnd, nTheItem); 
				tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
				tvi.hItem = hti; 
				tvi.iImage = 1; 
				tvi.iSelectedImage = 1; 
				TreeView_SetItem(g_pControls[iControlID].hWnd, &tvi); 
			}

			// Last action is to set to last inserted item
			TreeView_Select ( g_pControls[iControlID].hWnd, nTheItem, TVGN_CARET );

			// done
			break;
		}

		case WINCONTROL_COMBOBOX :
		{
			SendMessage ( g_pControls[iControlID].hWnd, CB_INSERTSTRING, (WPARAM)iIndex, (LPARAM)lpText );
			break;
		}

		case WINCONTROL_TOOLBAR :
		{
			// Get numnber of buttons currently
			DWORD dwButtonCount = SendMessage ( g_pControls[iControlID].hWnd, TB_BUTTONCOUNT, 0, 0 );

			// Add a button 
			TBBUTTON tbutton; 
			memset ( &tbutton, 0, sizeof(tbutton) );
			tbutton.idCommand = 1+dwButtonCount;
			tbutton.dwData = 0; 
			tbutton.iString = 0; 
			if ( iIndex==0 )
			{
				tbutton.iBitmap = 0;
				tbutton.fsStyle = TBSTYLE_SEP;
			}
			else
			{
				tbutton.iBitmap = iIndex - 1;
				tbutton.fsStyle = TBSTYLE_BUTTON;
				tbutton.fsState = TBSTATE_ENABLED; 

			}
			SendMessage ( g_pControls[iControlID].hWnd, TB_ADDBUTTONS, (WPARAM)(UINT)1, (LPARAM)(LPTBBUTTON)&tbutton );
			break;
		}
	}
}

void RemoveItem ( int iControlID, int iIndex )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			break;
		}

		case WINCONTROL_LISTVIEW :
		{
			ListView_DeleteItem( g_pControls[iControlID].hWnd, iIndex );
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			TreeView_DeleteItem( g_pControls[iControlID].hWnd, iIndex );
			break;
		}

		case WINCONTROL_COMBOBOX :
		{
			break;
		}
	}
}

void RemoveAllItems ( int iControlID )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			break;
		}

		case WINCONTROL_LISTVIEW :
		{
			// erase list
			ListView_DeleteAllItems( g_pControls[iControlID].hWnd );
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			// erase list
			TreeView_DeleteAllItems( g_pControls[iControlID].hWnd );
			break;
		}

		case WINCONTROL_COMBOBOX :
		{
			break;
		}
	}
}

void SetItem ( int iControlID, int iIndex, LPSTR lpText )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			TCITEM tie;
			memset ( &tie, 0, sizeof(tie) );
			tie.mask = TCIF_TEXT; 
			tie.pszText = lpText; 
			TabCtrl_SetItem( g_pControls[iControlID].hWnd, iIndex, &tie );
			break;
		}

		case WINCONTROL_LISTVIEW :
		{
			ListView_SetItemText( g_pControls[iControlID].hWnd, iIndex, 0, lpText );
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			break;
		}

		case WINCONTROL_COMBOBOX :
		{
			break;
		}
	}
}

void SetItemIcon ( HINSTANCE hInstance, int iControlID, int iIndex, DWORD dwIconHandle )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_LISTVIEW :
		{
			LVITEM lvi; 
			memset ( &lvi, 0, sizeof(lvi) );
			lvi.mask = LVIF_IMAGE; 
			lvi.iItem = iIndex; 
			lvi.iSubItem = 0; 
			ListView_GetItem( g_pControls[iControlID].hWnd, &lvi);
			int iReplaced = ImageList_ReplaceIcon ( g_pControls [ iControlID ].himlImageList, lvi.iImage, (HICON)dwIconHandle);
			InvalidateRect ( g_pControls[iControlID].hWnd, NULL, FALSE );
			UpdateWindow ( g_pControls[iControlID].hWnd );
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			ImageList_ReplaceIcon ( g_pControls [ iControlID ].himlImageList, iIndex, (HICON)dwIconHandle );
			InvalidateRect ( g_pControls[iControlID].hWnd, NULL, FALSE );
			UpdateWindow ( g_pControls[iControlID].hWnd );
			break;
		}
	}
}

void Link ( int iChildControlID, int iParentControlID, int iParentIndex )
{
	// Parent controls type of linkage
	switch ( g_pControls[iParentControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			// Assign dependence of child to parent
			g_pControls[iChildControlID].hWndMasterControl = g_pControls[iParentControlID].hWnd;
			g_pControls[iChildControlID].iMasterIndexID = iParentIndex;
			g_pControls[iChildControlID].bVisible = false;

			// Trigger parent to select chosen index
			TabCtrl_SetCurSel ( g_pControls[iParentControlID].hWnd, iParentIndex );
			OnlyShowOneAreaOfTabControl ( g_pControls[iParentControlID].hWnd, iParentIndex );
			break;
		}

		case WINCONTROL_POPUP :
		{
			g_pControls[iChildControlID].hWndMasterControl = g_pControls[iParentControlID].hWnd;
			SetParent ( g_pControls[iChildControlID].hWnd, g_pControls[iParentControlID].hWnd );
			break;
		}

		case WINCONTROL_UPDOWN :
		{
			// Sets the UpDown Buddy to point to child who is linking on
			SendMessage ( g_pControls[iParentControlID].hWnd, UDM_SETBUDDY, (WPARAM)(HWND)g_pControls[iChildControlID].hWnd, 0 );
			break;
		}
	}
}

int GetControlItemValue ( int iControlID )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			break;
		}

		case WINCONTROL_LISTVIEW :
		{
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			TVITEM tvi; 
			memset ( &tvi, 0, sizeof(tvi));
			tvi.mask = TVIF_PARAM; 
			tvi.hItem = TreeView_GetSelection ( g_pControls[iControlID].hWnd );
			TreeView_GetItem( g_pControls[iControlID].hWnd, &tvi );
			return tvi.lParam;
		}
	}

	// failure
	return 0;
}

int GetControlItemIcon ( int iControlID )
{
	switch ( g_pControls[iControlID].sType )
	{
		case WINCONTROL_TAB :
		{
			break;
		}

		case WINCONTROL_LISTVIEW :
		{
			break;
		}

		case WINCONTROL_TREEVIEW :
		{
			TVITEM tvi; 
			memset ( &tvi, 0, sizeof(tvi));
			tvi.mask = TVIF_IMAGE; 
			tvi.hItem = TreeView_GetSelection ( g_pControls[iControlID].hWnd );
			TreeView_GetItem( g_pControls[iControlID].hWnd, &tvi );
			return tvi.iImage;
		}
	}

	// failure
	return 0;
}

int	GetControlMouseX ( int iControlID )
{
	return g_pControls[iControlID].iMouseAtX;
}

int	GetControlMouseY ( int iControlID )
{
	return g_pControls[iControlID].iMouseAtY;
}


//
// Windows GUI System Commands
//

void CaptureSystemColors ( void )
{
	// capture all system colors
	for ( int i=0; i<30; i++)
	{
		aDefaultSystemColors[i] = GetSysColor(i); 
		aCurrentSystemColors[i] = GetSysColor(i); 
	}
}

void SetSystemColors ( int iSystemColorIndex, DWORD dwRGB )
{
	// element array
	int aiSysElements[30];
	for ( int i=0; i<30; i++)
		aiSysElements[i]=i;

	// SystemColorIndex meanings:
	// COLOR_SCROLLBAR         0
	// COLOR_BACKGROUND        1
	// COLOR_ACTIVECAPTION     2
	// COLOR_INACTIVECAPTION   3
	// COLOR_MENU              4
	// COLOR_WINDOW            5
	// COLOR_WINDOWFRAME       6
	// COLOR_MENUTEXT          7
	// COLOR_WINDOWTEXT        8
	// COLOR_CAPTIONTEXT       9
	// COLOR_ACTIVEBORDER      10
	// COLOR_INACTIVEBORDER    11
	// COLOR_APPWORKSPACE      12
	// COLOR_HIGHLIGHT         13
	// COLOR_HIGHLIGHTTEXT     14
	// COLOR_BTNFACE           15
	// COLOR_BTNSHADOW         16
	// COLOR_GRAYTEXT          17
	// COLOR_BTNTEXT           18
	// COLOR_INACTIVECAPTIONTEXT 19
	// COLOR_BTNHIGHLIGHT      20
	// COLOR_3DDKSHADOW        21
	// COLOR_3DLIGHT           22
	// COLOR_INFOTEXT          23
	// COLOR_INFOBK            24
	// COLOR_HOTLIGHT                  26
	// COLOR_GRADIENTACTIVECAPTION     27
	// COLOR_GRADIENTINACTIVECAPTION   28

	// set specific system color
	aCurrentSystemColors[iSystemColorIndex] = dwRGB;
  
	// refresh all system colors
	SetSysColors(28, aiSysElements, aCurrentSystemColors); 
}

void RestoreSystemColors ( void )
{
	// element array
	int aiSysElements[30];
	for ( int i=0; i<30; i++)
		aiSysElements[i]=i;

	// restore all system colors
	SetSysColors(28, aiSysElements, aDefaultSystemColors); 
}
