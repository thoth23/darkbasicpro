//
// WinGUI Header File
//

// Defined for use of OEM bitmaps
#define OEMRESOURCE 

// Common Includes
#include <windows.h>

// Include Controls\Windows\Menus
#include "GUIControls.h"
#include "GUIMenus.h"
#include "GUIDialogs.h"
#include "macros.h"

// Define macro
#define MYCOMMAND __declspec ( dllexport )

// Constructor Functions (optional)
MYCOMMAND void	Constructor ( void );
MYCOMMAND void	Destructor ( void );
MYCOMMAND void	ReceiveCoreDataPtr ( LPVOID pCore );
MYCOMMAND int	GetDLLVersion ( void );

// Do not decorate DLL-function-names
extern "C"
{
	// My Maintenance Commands
	MYCOMMAND void		UpdateWinGUI			( void );

	// My Control Commands
	MYCOMMAND int		ClearControlArea		( int iControlID, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlPopup		( int iControlID, DWORD pName, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND void		DeleteControl			( int iControlID );
	MYCOMMAND int		MakeControlText			( int iControlID, int iStyle, int iExtendedStyle, DWORD pText, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlButton		( int iControlID, int iStyle, int iExtendedStyle, DWORD pText, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlProgress		( int iControlID, int iStyle, int iExtendedStyle, int iMin, int iMax, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlTabs			( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlListView		( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlTreeView		( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlScrollbar	( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlComboBox		( int iControlID, int iStyle, int iExtendedStyle, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlSlider		( int iControlID, int iStyle, int iExtendedStyle, int iMin, int iMax, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlEdit			( int iControlID, int iStyle, int iExtendedStyle, DWORD pText, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlUpDown		( int iControlID, int iStyle, int iExtendedStyle, int iMin, int iMax, int iX, int iY, int iWidth, int iHeight );
	MYCOMMAND int		MakeControlToolBar		( int iControlID, int iStyle, int iExtendedStyle, DWORD pFilename, int iNumberOfToolImages );

	// My Control Setting Commands
	MYCOMMAND void		SetControlFont			( int iControlID, DWORD pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode );
	MYCOMMAND void		SetControlBackColor		( int iControlID, int iRed, int iGreen, int iBlue );
	MYCOMMAND void		SetControlForeColor		( int iControlID, int iRed, int iGreen, int iBlue );
	MYCOMMAND void		SetControlRange			( int iControlID, int iMin, int iMax );
	MYCOMMAND void		SetControlPosition		( int iControlID, int iPosition );
	MYCOMMAND void		SetControlPageSize		( int iControlID, int iPageSize );
	MYCOMMAND void		SetControlText			( int iControlID, DWORD pText );
	MYCOMMAND void		SetControlIcon			( int iControlID, DWORD dwIconHandle );
	MYCOMMAND void		SetControlIconEx		( int iControlID, int iIndex, DWORD dwIconHandle );
	MYCOMMAND void		SetControlAlignment		( int iControlID, int iAlignCode );
	MYCOMMAND void		SetControlSelection		( int iControlID, DWORD dwSelection );

	// My Placement Commands
	MYCOMMAND void		MoveControl				( int iControlID, int iX, int iY );
	MYCOMMAND void		SizeControl				( int iControlID, int iW, int iH );

	// My Control Construction Commands
	MYCOMMAND void		AddControlItem			( int iControlID, int iIndex, DWORD pText );
	MYCOMMAND void		DeleteControlItem		( int iControlID, int iIndex );
	MYCOMMAND void		SetControlItem			( int iControlID, int iIndex, DWORD pText );
	MYCOMMAND void		SetControlItemIcon		( int iControlID, int iIndex, DWORD pIconHandle );
	MYCOMMAND void		LinkControl				( int iChildControlID, int iParentControlID, int iParentIndex );
	MYCOMMAND void		DeleteAllControlItems	( int iControlID );
	MYCOMMAND void		HideTheControl			( int iControlID );
	MYCOMMAND void		ShowTheControl			( int iControlID );
	MYCOMMAND int		GetTheControlItemValue	( int iControlID );
	MYCOMMAND int		GetTheControlItemIcon	( int iControlID );
	MYCOMMAND int		GetTheControlMouseX		( int iControlID );
	MYCOMMAND int		GetTheControlMouseY		( int iControlID );

	// My Default Dialog Control Commands
	MYCOMMAND void		CallFileRequester		( void );

	// My Menu Commands
	MYCOMMAND void		AddMenuItem				( int iMenuID, int iMenuTargetID, int iStyle, LPSTR pText );
	MYCOMMAND void		InsertAMenuItem			( int iMenuID, int iMenuTargetID, int iInsertPosition, int iStyle, LPSTR pText );
	MYCOMMAND void		DeleteMenuItem			( int iMenuID );
	MYCOMMAND void		SetMenuFont				( LPSTR pFontname, int iFontSize, int iWeight, int iItalic, int iInternationalCode );
	MYCOMMAND void		SetAMenuItemState		( int iMenuID, int iPosition, int iState );

	// My Dialog Commands - Printing
	MYCOMMAND int		DialogPrintBox			( void );
	MYCOMMAND void		DialogAddTextToPage		( LPSTR pText, int x, int y, int centered );
	MYCOMMAND void		DialogEndPrinting		( void );

	// My Dialog Commands - File Requester
	MYCOMMAND DWORD		DialogOpenFileBox		( DWORD pDestStr );
	MYCOMMAND DWORD		DialogSaveFileBox		( DWORD pDestStr );

	// My Dialog Commands - Browser
	MYCOMMAND void		DialogBrowserBox		( DWORD pURLString );
	MYCOMMAND void		DialogBrowserBoxClose	( void );

	// My Windows GUI System Commands
	MYCOMMAND void		SetWinGUIColors			( int iSystemColorIndex, int iRed, int iGreen, int iBlue );
	MYCOMMAND void		RestoreWinGUIColors		( void );

	// My Control Expressions
	MYCOMMAND int		GetControlClick			( int iControlID );
	MYCOMMAND int		GetControlValue			( int iControlID );
	MYCOMMAND DWORD		GetControlString		( DWORD pDestStr, int iControlID );
	MYCOMMAND DWORD		GetControlSelected		( int iControlID );
	MYCOMMAND int		GetControlSelectionCount( int iControlID );

	// My Menu Expressions
	MYCOMMAND int		GetMenuSelected			( int iMenuID );

}
