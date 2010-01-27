//
// TPC Prototype
//

// A Common Include
#include "windows.h"

// My Includes
#include "ActivexpressionHeader.h"

// Include Core (optional)
#include ".\..\..\Shared\core\globstruct.h"
GlobStruct* g_pGlob;

// Actual DLL Instance
HINSTANCE			g_hDLLInstance		= NULL;

// ACTI Globals
ax_handle_t			g_hubHandle			= AX_HANDLE_BAD;
void AX_CALLCONV	MultichoiceSessionCallbackProc( ax_handle_t handle, ax_int32_t serialNumber, ax_int32_t questionId, ax_int32_t responseSize, const ax_char16_t* pResponse, void* pContext );
void AX_CALLCONV	YesnoSessionCallbackProc( ax_handle_t handle, ax_int32_t serialNumber, ax_int32_t questionId, ax_yes_no_response_t response, void* pContext );
void AX_CALLCONV    TruefalseSessionCallbackProc(ax_handle_t handle, ax_int32_t serialNumber, ax_int32_t questionId, ax_true_false_response_t response, void* pContext );
void AX_CALLCONV    LikertSessionCallbackProc(ax_handle_t handle, ax_int32_t serialNumber, ax_int32_t questionId, ax_int32_t likertValue, void* pContext );
void AX_CALLCONV    NumericSessionCallbackProc( ax_handle_t handle, ax_int32_t serialNumber, ax_int32_t questionId, ax_int32_t responseSize, const ax_char16_t* pResponse, void* pContext );
void AX_CALLCONV	TextSessionCallbackProc( ax_handle_t, ax_int32_t, ax_int32_t, ax_int32_t, const ax_char16_t*, void* );
void AX_CALLCONV    SortInOrderSessionCallbackProc( ax_handle_t handle, ax_int32_t serialNumber, ax_int32_t questionId, ax_int32_t responseSize, const ax_char16_t* pResponse, void* pContext );
void AX_CALLCONV    MessageSessionCallbackProc( ax_handle_t handle, ax_int32_t serialNumber, ax_int32_t questionId, void* pContext );

// Manage list of serials
DWORD				g_dwSerialNumberListMax	= 0;
int*				g_pSerialNumberList		= NULL;
LPSTR*				g_pszSessionData		= NULL;

// Manage internal error
char				g_pLastError [ 512 ];

//
// Constructors (optional)
//
void Constructor ( void )
{
	// Create memory here
	strcpy ( g_pLastError, "" );
}

void Destructor ( void )
{
	// Free memory here
}

void ReceiveCoreDataPtr ( LPVOID pCore )
{
	// Get Core Data Pointer here
	g_pGlob = (GlobStruct*)pCore;
}

int GetAssociatedDLLs ( void )
{
	// none
	return 0;
}

//
// dependency num and string function for Activote.dll, add later
//

int ACTInit ( void )
{
	// error return number
    int exitValue = 0;

	// detect library 
    if( Ax_GetInterfaceVersion() < ACTIVEXPRESSION_SDK_INTERFACE_VERSION )
    {
        strcpy ( g_pLastError, "Failed. Incompatible Activexpression library loaded.");
        return 1;
    }

	// detect driver
    if( !Ax_IsDriverInstalled() )
    {
        strcpy ( g_pLastError, "Failed. Activboard driver is not installed.");
        return 2;
    }

	// initialise system
    ax_result_t res = Ax_Initialize();
    if( AX_FAILED( res ) )
    {
        strcpy ( g_pLastError, "Failed. Could not initialize the Activexpression library.");
        return 3;
    }

	// count hubs
    ax_int32_t hubCount;
    res = Ax_GetHubCount( &hubCount );
    if( AX_FAILED( res ) )
    {
        strcpy ( g_pLastError, "Failed. Could not count Activhubs.");
        exitValue = 5;
        goto tidy_exit;
    }

	// for each hub, test worthyness
    bool bFound = false;
    for( int index = 0; index < hubCount; ++index )
    {
		// get hub instance
        ax_handle_t hubHandle;
        res = Ax_GetHubInstance( index, &hubHandle );
        if( AX_FAILED( res ) )
        {
            strcpy ( g_pLastError, "Failed. Could not get instance handle.");
            exitValue = 6;
            goto tidy_exit;
        }

		// get hub type
        ax_hub_type_t hubType;
        res = Ax_GetHubType( hubHandle, &hubType );
        if( AX_FAILED( res ) )
        {
            strcpy ( g_pLastError, "Failed. Could not get Activhub type.");
            exitValue = 7;
            goto tidy_exit;
        }

		// only use 2.4ghz hubs
        if( hubType == AX_HUB_TYPE_2_4GHZ )
        {
            g_hubHandle = hubHandle;
            bFound = true;
            break;
        }
    }

	// if no 2.4ghz hubs, leave
    if( !bFound )
    {
        strcpy ( g_pLastError, "Terminating. Could not find a 2.4GHz Activhub.");
        exitValue = 8;
        goto tidy_exit;
    }

	// success
	return exitValue;

	// clean exit
	tidy_exit:
    res = Ax_Uninitialize();
    if( AX_FAILED( res ) )
    {
        strcpy ( g_pLastError, "Failed. Could not uninitialize the Activexpression library.");
        exitValue = 9;
    }

	// success
	return exitValue;
}

void ACTUpdate ( void )
{
	// monitor callback information easily
	if ( g_hubHandle != AX_HANDLE_BAD )
	{
		if( Ax_IsHubConnected ( g_hubHandle ) )
		{
			// Hub is connected
			ax_int32_t count = 0;
			Ax_GetActivexpressionCount ( g_hubHandle, &count );
			for( ax_int32_t index = 0; index<count; index++ )
			{
				ax_int32_t serial_number = 0;
				Ax_GetActivexpressionSerialNumber ( g_hubHandle, index, &serial_number );
				serial_number = serial_number;
				ax_int32_t name_buffer_size = 256;
				ax_char16_t name_buffer[256];
				ax_int32_t name_buffer_out = 0;
				Ax_GetActivexpressionName ( g_hubHandle, serial_number, name_buffer_size, name_buffer, &name_buffer_out );
				name_buffer_out = name_buffer_out;
			}
		}
		else
		{
		  // Hub is disconnected
		}
	}
}

void ACTClose ( void )
{
	// stop session before close
	ACTSessionStop();

	// free serial number table
	if ( g_pSerialNumberList ) { delete g_pSerialNumberList; g_pSerialNumberList=0; }

	// close hub when finished
    ax_result_t res = Ax_Uninitialize();
    if( AX_FAILED( res ) )
    {
        strcpy ( g_pLastError, "Failed. Could not uninitialize the Activexpression library.");
    }
}

MYDLL void ACTSessionStop ( void )
{
	// delete any session memory usage
	if ( g_pszSessionData )
	{
		for ( int n=0; n<(int)g_dwSerialNumberListMax; n++ )
			if ( g_pszSessionData [ n ] ) { delete g_pszSessionData [ n ]; g_pszSessionData [ n ]=0; }
		delete g_pszSessionData;
		g_pszSessionData=0;
	}

	// stop session
    ax_result_t res = Ax_StopSession( g_hubHandle );
    if( AX_FAILED( res ) )
    {
        strcpy ( g_pLastError, "Failed to stop text session.");
    }
}

MYDLL DWORD ACTGetErrorMessage ( DWORD pDestStr )
{
	if(pDestStr) g_pGlob->CreateDeleteString((DWORD*)&pDestStr, 0);
	LPSTR pReturnString=NULL;
	DWORD dwSize=strlen(g_pLastError);
	g_pGlob->CreateDeleteString((DWORD*)&pReturnString, dwSize+1);
	strcpy(pReturnString, g_pLastError);
	return (DWORD)pReturnString;
}

void ACTPerformChecklistForSession ( void )
{
	// prepare checklist
	g_pGlob->checklistexists = true;
	g_pGlob->checklisthasstrings = true;
	g_pGlob->checklisthasvalues = true;
	g_pGlob->checklistqty = g_dwSerialNumberListMax;

	// go through each item in the checklist
	for ( int i = 0; i < g_pGlob->checklistqty; i++ )
	{
		// expand the checklist so we can add a string
		GlobExpandChecklist ( i, 512 );
		strcpy ( g_pGlob->checklist [ i ].string, "" );
		if ( g_pszSessionData )
		{
			if ( g_pszSessionData [ i ] )
			{
				g_pGlob->checklist [ i ].valuea = 1;
				strcpy ( g_pGlob->checklist [ i ].string, g_pszSessionData [ i ] );
			}
		}
	}
}

void CopyInQuestion ( DWORD dwQuestion, ax_char16_t* g_question )
{
	LPSTR pQuestion = (LPSTR)dwQuestion;
	for ( int n=0; n<(int)strlen(pQuestion); n++ )
	{
		unsigned int ui =_mbbtombc ( pQuestion[n] );
		g_question[n] = (ax_char16_t)ui;
	}
}

void CopyOutResponse ( LPSTR pQuestion, const ax_char16_t* pResponse, DWORD dwResponseSize )
{
    const std::wstring response( reinterpret_cast<const wchar_t*>(pResponse), dwResponseSize );
	DWORD n = 0;
	for(;n<dwResponseSize;n++)
		pQuestion[n] = (unsigned char)_mbctombb ( response[n] );
	pQuestion[n]=0;
}

int HandleSerialTable ( ax_int32_t serialNumber )
{
	int iSerialTableIndex = 0;
	if ( g_dwSerialNumberListMax==0 )
	{
		g_pSerialNumberList = new int [ 1 ];
		g_pszSessionData = new LPSTR [ 1 ];
		g_pSerialNumberList [ iSerialTableIndex ] = serialNumber;
		g_pszSessionData [ iSerialTableIndex ] = NULL;
		g_dwSerialNumberListMax = 1;
	}
	else
	{
		// look for matching serial
		int found=-1;
		for ( int n=0; n<(int)g_dwSerialNumberListMax; n++ )
			if ( serialNumber==g_pSerialNumberList [ n ] )
				found=n;

		// if not found, make, else use found index
		if ( found==-1 )
		{
			// create an extra entry in serial table
			int* pNewSerialNumberList = new int [ g_dwSerialNumberListMax+1 ];
			LPSTR* pNewSessionData = new LPSTR [ g_dwSerialNumberListMax+1 ];
			memcpy ( pNewSerialNumberList, g_pSerialNumberList, sizeof(int)*g_dwSerialNumberListMax );
			memcpy ( pNewSessionData, g_pszSessionData, sizeof(LPSTR)*g_dwSerialNumberListMax );
			pNewSerialNumberList [ g_dwSerialNumberListMax ] = serialNumber;
			pNewSessionData [ g_dwSerialNumberListMax ] = NULL;
			iSerialTableIndex = g_dwSerialNumberListMax;
			g_dwSerialNumberListMax++;
			if ( g_pSerialNumberList ) { delete g_pSerialNumberList; g_pSerialNumberList=0; }
			if ( g_pszSessionData ) { delete g_pszSessionData; g_pszSessionData=0; }
			g_pSerialNumberList = pNewSerialNumberList;
			g_pszSessionData = pNewSessionData;
		}
		else
			iSerialTableIndex = found;
	}

	// copy session answer into data array
	if ( g_pszSessionData [ iSerialTableIndex ] ) { delete g_pszSessionData [ iSerialTableIndex ]; g_pszSessionData [ iSerialTableIndex ]=0; }
	g_pszSessionData [ iSerialTableIndex ] = new char [ 512 ];
	return iSerialTableIndex;
}

MYDLL int ACTSessionStartMultichoice ( int iQuestionID, int iDisplayOption, int iOptions, int iResponses )
{
	ax_display_option_t displayOption;
	switch ( iDisplayOption  )
	{
		default:	displayOption = AX_DISPLAY_OPTION_NUMERIC;			break;
		case 1:		displayOption = AX_DISPLAY_OPTION_ALPHA_LOWER;		break;
		case 2:		displayOption = AX_DISPLAY_OPTION_ALPHA_UPPER;		break;
	}
	ax_result_t res = Ax_StartMultichoiceSession(	g_hubHandle,
													(ax_int32_t)iQuestionID,
													(ax_int32_t)iOptions,//numOptions Number of options (2 to 6)
													(ax_int32_t)iResponses,//numResponses Number of responses (2 to 6)
													(ax_display_option_t)displayOption,
													MultichoiceSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start multichoice session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV MultichoiceSessionCallbackProc( ax_handle_t handle,
                                          ax_int32_t serialNumber,
                                          ax_int32_t questionId,
                                          ax_int32_t responseSize,
                                          const ax_char16_t* pResponse,
                                          void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	char mess[612];
	CopyOutResponse ( mess, pResponse, responseSize );
	strcpy ( g_pszSessionData [ iSerialTableIndex ], mess );
}

MYDLL int ACTSessionStartYesNo ( int iQuestionID, int iDontKnowRequired )
{
	ax_bool_t bDontKnowRequired = false;
	if ( iDontKnowRequired==1 ) bDontKnowRequired = true;
	ax_result_t res = Ax_StartYesNoSession(	g_hubHandle,
											(ax_int32_t)iQuestionID,
											(ax_bool_t)bDontKnowRequired,
											YesnoSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start yesno session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV YesnoSessionCallbackProc( ax_handle_t handle,
                                          ax_int32_t serialNumber,
                                          ax_int32_t questionId,
                                          ax_yes_no_response_t response,
                                          void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	char mess[256];
	strcpy ( mess, "" );
	if ( response==AX_YES_NO_RESPONSE_YES  ) strcpy ( mess, "yes" );
	if ( response==AX_YES_NO_RESPONSE_NO ) strcpy ( mess, "no" );
	if ( response==AX_YES_NO_RESPONSE_DONT_KNOW ) strcpy ( mess, "don't know" );
	strcpy ( g_pszSessionData [ iSerialTableIndex ], mess );
}

MYDLL int ACTSessionStartTrueFalse ( int iQuestionID, int iDontKnowRequired )
{
	ax_bool_t bDontKnowRequired = false;
	if ( iDontKnowRequired==1 ) bDontKnowRequired = true;
	ax_result_t res = Ax_StartTrueFalseSession(	g_hubHandle,
												(ax_int32_t)iQuestionID,
												(ax_bool_t)bDontKnowRequired,
												TruefalseSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start truefalse session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV TruefalseSessionCallbackProc(ax_handle_t handle,
									          ax_int32_t serialNumber,
											  ax_int32_t questionId,
										      ax_true_false_response_t response,
											  void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	char mess[256];
	strcpy ( mess, "" );
	if ( response==AX_TRUE_FALSE_RESPONSE_TRUE  ) strcpy ( mess, "true" );
	if ( response==AX_TRUE_FALSE_RESPONSE_FALSE ) strcpy ( mess, "false" );
	if ( response==AX_TRUE_FALSE_RESPONSE_DONT_KNOW ) strcpy ( mess, "don't know" );
	strcpy ( g_pszSessionData [ iSerialTableIndex ], mess );
}

MYDLL int ACTSessionStartLikert ( int iQuestionID, int iMaxScaleValue )
{
	ax_result_t res = Ax_StartLikertSession(	g_hubHandle,
												(ax_int32_t)iQuestionID,
												(ax_int32_t)iMaxScaleValue,//The maximum value in the Likert scale (4 to 7)
												LikertSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start likert session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV LikertSessionCallbackProc(ax_handle_t handle,
									          ax_int32_t serialNumber,
											  ax_int32_t questionId,
											  ax_int32_t likertValue,
											  void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	char mess[256];
	wsprintf ( mess, "%d", (int)likertValue );
	strcpy ( g_pszSessionData [ iSerialTableIndex ], mess );
}

MYDLL int ACTSessionStartNumeric ( int iQuestionID, DWORD dwQuestion )
{
	static ax_char16_t g_question [ 256 ];
	CopyInQuestion ( dwQuestion, g_question );
	static const ax_int32_t g_maxResponseSize = 16;
	ax_result_t res = Ax_StartNumericSession(	g_hubHandle,
											(ax_int32_t)iQuestionID,
											static_cast<ax_int32_t>(strlen((LPSTR)dwQuestion)),
											reinterpret_cast<ax_char16_t*>( g_question ),
											g_maxResponseSize,
											NumericSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start numeric session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV NumericSessionCallbackProc(  ax_handle_t handle,
							                  ax_int32_t serialNumber,
								              ax_int32_t questionId,
									          ax_int32_t responseSize,
										      const ax_char16_t* pResponse,
											  void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	char mess[612];
	CopyOutResponse ( mess, pResponse, responseSize );
	strcpy ( g_pszSessionData [ iSerialTableIndex ], mess );
}

MYDLL int ACTSessionStartText ( int iQuestionID, DWORD dwQuestion )
{
	static ax_char16_t g_question [ 256 ];
	CopyInQuestion ( dwQuestion, g_question );
	static const ax_int32_t g_maxResponseSize = 16;
	ax_result_t res = Ax_StartTextSession(	g_hubHandle,
											(ax_int32_t)iQuestionID,
											static_cast<ax_int32_t>(strlen((LPSTR)dwQuestion)),
											reinterpret_cast<ax_char16_t*>( g_question ),
											g_maxResponseSize,
											TextSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start text session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV TextSessionCallbackProc( ax_handle_t handle,
                                          ax_int32_t serialNumber,
                                          ax_int32_t questionId,
                                          ax_int32_t responseSize,
                                          const ax_char16_t* pResponse,
                                          void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	char mess[612];
	CopyOutResponse ( mess, pResponse, responseSize );
	strcpy ( g_pszSessionData [ iSerialTableIndex ], mess );
}

MYDLL int ACTSessionStartSortInOrder ( int iQuestionID, int iDisplayOption, int iOptions, int iResponses )
{
	ax_display_option_t displayOption;
	switch ( iDisplayOption  )
	{
		default:	displayOption = AX_DISPLAY_OPTION_NUMERIC;			break;
		case 1:		displayOption = AX_DISPLAY_OPTION_ALPHA_LOWER;		break;
		case 2:		displayOption = AX_DISPLAY_OPTION_ALPHA_UPPER;		break;
	}
	ax_result_t res = Ax_StartSortInOrderSession(	g_hubHandle,
													(ax_int32_t)iQuestionID,
													(ax_int32_t)iOptions,//numOptions Number of options (2 to 6)
													(ax_int32_t)iResponses,//numResponses Number of responses (2 to 6)
													(ax_display_option_t)displayOption,
													SortInOrderSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start sortinorder session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV SortInOrderSessionCallbackProc( ax_handle_t handle,
                                          ax_int32_t serialNumber,
                                          ax_int32_t questionId,
                                          ax_int32_t responseSize,
                                          const ax_char16_t* pResponse,
                                          void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	char mess[612];
	CopyOutResponse ( mess, pResponse, responseSize );
	strcpy ( g_pszSessionData [ iSerialTableIndex ], mess );
}

MYDLL int ACTSessionStartMessage ( int iQuestionID, DWORD dwMessage )
{
	static ax_char16_t g_message [ 512 ];
	CopyInQuestion ( dwMessage, g_message );
	DWORD dwMessageSize = strlen( (LPSTR)dwMessage );
	if ( dwMessageSize > 48 ) dwMessageSize = 48;
	ax_result_t res = Ax_StartMessageSession(	g_hubHandle,
												(ax_int32_t)iQuestionID,
												(ax_int32_t)dwMessageSize,
												reinterpret_cast<ax_char16_t*>( g_message ),
												MessageSessionCallbackProc, 0, 0, 0 );
	if( AX_FAILED( res ) )
	{
		strcpy ( g_pLastError, "Failed to start message session.");
		return 1;
	}
	return 0;
}

void AX_CALLCONV MessageSessionCallbackProc(  ax_handle_t handle,
							                  ax_int32_t serialNumber,
								              ax_int32_t questionId,
											  void* pContext )
{
	int iSerialTableIndex = HandleSerialTable(serialNumber);
	strcpy ( g_pszSessionData [ iSerialTableIndex ], "responded" );
}
