//
// GUIDialogs
//

// Includes
#include "GUIDialogs.h"
#include "direct.h"
#include "exdisp.h"

// Include Core
#include ".\..\..\Shared\Core\globstruct.h"
extern GlobStruct* g_pGlob;

// Printer Settings
HWND				hwnd;
PRINTDLG			pd;
DOCINFO				di;

// Browser Settings
HWND				hWndBrowser = NULL;
IWebBrowser2*		pBrowser = NULL;

// Maintenance

void FreeAllDialogs ( void )
{
	if ( pBrowser ) 
	{
		pBrowser->Release();
		pBrowser=NULL;
	}
}

// Printer Functions

int PrintBox ( void )
{
	// Gain HWND if can
	hwnd = NULL;
	if ( g_pGlob )
		hwnd = g_pGlob->hWnd;

	// Initialize PRINTDLG
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof(PRINTDLG);
	pd.hwndOwner   = hwnd;
	pd.hDevMode    = NULL;      // Don't forget to free or store hDevMode.
	pd.hDevNames   = NULL;      // Don't forget to free or store hDevNames.
	pd.Flags       = PD_ALLPAGES | PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC | PD_HIDEPRINTTOFILE | PD_NOPAGENUMS; 
	pd.nCopies     = 1;
	pd.nFromPage   = 0; 
	pd.nToPage     = 0; 
	pd.nMinPage    = 1; 
	pd.nMaxPage    = 1; 

	// Print Setup
	if (PrintDlg(&pd)==TRUE)
	{
		// Free unused data
		if (pd.hDevMode) GlobalFree (pd.hDevMode);
		if (pd.hDevNames) GlobalFree (pd.hDevNames); 

		// Prepare Document
		di.cbSize = sizeof(DOCINFO); 
		di.lpszDocName = "Printing Document"; 
		di.lpszDatatype = (LPTSTR) NULL; 
		di.lpszOutput = (LPTSTR) NULL; 
		di.fwType = 0; 

		// Begin a print job by calling the StartDoc function. 
		int nError = StartDoc(pd.hDC, &di); 
		if (nError == SP_ERROR) 
		{
			DeleteDC(pd.hDC); 
			pd.hDC = NULL;
			return 0;
		}
		
		// Inform the driver that the application is about to begin 
		nError = StartPage(pd.hDC); 
		if (nError <= 0) 
		{
			DeleteDC(pd.hDC); 
			pd.hDC = NULL;
			return 0;
		}
	}
	else
		return 0;

	// success - begin sending text
	return 1;
}

void AddTextToPage ( LPSTR pText, int x, int y, int centered )
{
	// Dimension of print area
    int cWidthPels = GetDeviceCaps(pd.hDC, HORZRES); 
    int cHeightPels = GetDeviceCaps(pd.hDC, VERTRES); 

	// Adjustment for center
	switch ( centered )
	{
		case 0 : SetTextAlign ( pd.hDC, TA_LEFT );		break;
		case 1 : SetTextAlign ( pd.hDC, TA_CENTER );	break;
		case 2 : SetTextAlign ( pd.hDC, TA_RIGHT );		break;
	}

	// Print to scaled location
	double dx = (x/100.0)*cWidthPels;
	double dy = (y/100.0)*cHeightPels;

	// put text to device context
	TextOut (pd.hDC, (int)dx, (int)dy, (LPCTSTR) pText, strlen(pText)); 
}

void EndPrinting ( void )
{
	if ( pd.hDC )
	{
		int nError = EndPage(pd.hDC); 
		if (nError <= 0)
		{
			DeleteDC ( pd.hDC ); 
			pd.hDC = NULL;
			return;
		}

		// end printing
		EndDoc(pd.hDC);

		// Delete DC when done.
		DeleteDC ( pd.hDC ); 
		pd.hDC = NULL;
	}
}

// File Requester Functions

bool FileOpenBox ( LPSTR pFilename )
{
	char initdir[_MAX_PATH];
	getcwd(initdir, _MAX_PATH);
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for filename
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
//	ofn.hwndOwner = hwnd;
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
//	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initdir;
	ofn.Flags = OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Default is empty
	strcpy ( szFile, "" );
	strcpy ( pFilename, "" );

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn)!=0) 
	{
		// Copy result to filename passed in
		if ( ofn.lpstrFile )
		{
			strcpy ( pFilename, ofn.lpstrFile );
		}
		return true;
	}

	// failure
	return false;
}

bool FileSaveBox ( LPSTR pFilename )
{
	char initdir[_MAX_PATH];
	getcwd(initdir, _MAX_PATH);
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for filename
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
//	ofn.hwndOwner = hwnd;
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
//	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initdir;
	ofn.Flags = OFN_LONGNAMES | OFN_PATHMUSTEXIST;

	// Default is empty
	strcpy ( szFile, "" );
	strcpy ( pFilename, "" );

	// Display the Open dialog box. 
	if (GetSaveFileName(&ofn)!=0) 
	{
		// Copy result to filename passed in
		if ( ofn.lpstrFile )
		{
			strcpy ( pFilename, ofn.lpstrFile );
		}
		return true;
	}

	// failure
	return false;
}

// Browser Functions

void BrowserBox ( LPSTR pURL )
{
	/* redundant

	// Create webbrowser instance
	HRESULT hr = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_SERVER, IID_IWebBrowser2, (LPVOID*)&pBrowser );

	// If browser present, set url
	if ( pBrowser )
	{
		// set url
		CComVariant ve;
		DWORD dwSize = (strlen(pURL)+1)*2;
		BSTR bstrurl = (BSTR)new char[dwSize+4];
		memset ( bstrurl, 0, dwSize+4 );
		*(DWORD*)bstrurl = dwSize;
		for ( DWORD n=0; n<strlen(pURL); n++)
		{
			*((unsigned char*)bstrurl+4+(n*2)) = pURL[n];
		}
		hr = pBrowser->Navigate(bstrurl+2, &ve, &ve, &ve, &ve);

		// show as full screen browser
		pBrowser->put_FullScreen(VARIANT_TRUE);
		pBrowser->put_Visible(VARIANT_TRUE);

		// set default size of area
//		hr = pBrowser->put_Left(0);
//		hr = pBrowser->put_Width(800);
//		hr = pBrowser->put_Top(0);
//		hr = pBrowser->put_Height(600);

		// set default visual properties
//		pBrowser->put_AddressBar(VARIANT_FALSE);
//		pBrowser->put_MenuBar(VARIANT_FALSE);
//		pBrowser->put_ToolBar(VARIANT_FALSE);
//		pBrowser->put_StatusBar(VARIANT_FALSE);
//		pBrowser->put_Resizable(VARIANT_FALSE);

		// by default remove systemmenu from browser
		pBrowser->get_HWND ( (long*)&hWndBrowser );
		SetWindowLong ( hWndBrowser, GWL_STYLE, WS_POPUP | WS_BORDER );
		SetWindowPos ( hWndBrowser, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
		ShowWindow ( hWndBrowser, SW_SHOW);
		InvalidateRect ( hWndBrowser, NULL, FALSE);
		UpdateWindow ( hWndBrowser );
	}
	*/
}

void BrowserBoxClose ( void )
{
	/*
	if ( hWndBrowser && pBrowser ) 
	{
		PostMessage ( hWndBrowser, WM_CLOSE, 0, 0 );
		pBrowser->Release();
		hWndBrowser=NULL;
		pBrowser=NULL;
	}
	*/
}