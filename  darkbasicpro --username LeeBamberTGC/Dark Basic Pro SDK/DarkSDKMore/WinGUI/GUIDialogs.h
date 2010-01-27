//
// GUIDialogs Header
//

// Common Includes
#include <windows.h>
#include "macros.h"

// Maintenance
void	FreeAllDialogs		( void );

// Printer Functions

int		PrintBox			( void );
void	AddTextToPage		( LPSTR pText, int x, int y, int centered );
void	EndPrinting			( void );

// File Requester Functions

bool	FileOpenBox			( LPSTR pFilename );
bool	FileSaveBox			( LPSTR pFilename );

// Browser Functions

void	BrowserBox			( LPSTR pURL );
void	BrowserBoxClose		( void );
