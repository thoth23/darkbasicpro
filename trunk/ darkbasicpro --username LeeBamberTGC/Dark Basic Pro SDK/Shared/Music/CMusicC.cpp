#include ".\..\core\globstruct.h"
#include "cmusicc.h"
#include ".\..\error\cerror.h"

// Global Shared Data Pointer (passed in from core)
DBPRO_GLOBAL GlobStruct*				g_pGlob							= NULL;

//#define GENLOGFILE

#pragma comment ( lib, "winmm.lib" )

#pragma comment ( lib, "quartz.lib" )
#pragma comment ( lib, "msacm32.lib" )

//#pragma comment ( lib, "strmiids.lib" )

//quartz.lib winmm.lib msacm32.lib olepro32.lib strmiids.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 

 //winmm.lib 

#include <direct.h>
#include <dshow.h>

// global variables
DBPRO_GLOBAL CMusicManager				m_MusicManager;
DBPRO_GLOBAL tagMusicData*				m_ptr;

DBPRO_GLOBAL IGraphBuilder*	pGB = NULL;
DBPRO_GLOBAL IMediaControl*	pMC = NULL;
DBPRO_GLOBAL IMediaEventEx*	pME = NULL;
DBPRO_GLOBAL IBasicAudio*	pBA = NULL;
DBPRO_GLOBAL IMediaSeeking*	pMS = NULL;


DARKSDK void Constructor ( HWND hWnd )
{
	/* done in core
	// setup com
	if ( FAILED ( CoInitialize ( NULL ) ) )
    {
        Error ( "Failed to initialize COM for music DLL" );
		return;
    }
	*/
}

DARKSDK void Destructor ( void )
{
	// close everything down

	// stop any cd audio playing
	mciSendString ( "stop cdaudio", NULL, NULL, NULL );
	mciSendString ( "close cdaudio", NULL, NULL, NULL );

	// shut down the manager
	m_MusicManager.ShutDown ( );

//	// shut down com
//	CoUninitialize ( );
}

DARKSDK bool UpdatePtr ( int iID )
{
	// update internal data

	m_ptr = NULL;
	m_ptr = m_MusicManager.GetData ( iID );

	if ( m_ptr == NULL )
		return false;

	return true;
}

DARKSDK void Update ( void )
{
	// update, we need to call this so we can check the status of any playing music

	// get a pointer to the start of the list
	link* check = m_MusicManager.m_List.m_start;

	// run through all elements in the list
//	for ( int temp = 0; temp < m_MusicManager.m_List.m_count; temp++ )
    while (check)
	{
		// get object data
		tagMusicData* ptr = (tagMusicData*)(check->data);

		// return if it's invalid
		if ( ptr == NULL )
			continue;

		// move onto next if we're paused
		if ( ptr->bPause )
			continue;

		if ( ptr->eID == CD )
		{
			// used to get string information
			char szCommand [ 256 ];
			char szReturn  [ 256 ];
		
			memset ( &szCommand, 0, sizeof ( szCommand ) );
			memset ( &szReturn,  0, sizeof ( szReturn ) );

			memset ( &szCommand, 0, sizeof ( szCommand ) );
			wsprintf ( szCommand, "status cdaudio mode" );

			// send the command
			mciSendString ( szCommand, szReturn, sizeof ( szReturn ), NULL );

			// check the statuc
			if ( strcmp ( szReturn, "stopped" ) == 0 )
			{
				// loop if flagged
				if ( ptr->bLoop )
				{
					// if the track is stopped restart
					PlayMusic ( check->id );
				}
				else
				{
					// end play state
					ptr->bPlay = false;
				}
			}
		}
		else
		{
			LONG	evCode,
					evParam1,
					evParam2;
			HRESULT hr;

			if ( !ptr->pMediaEvent )
			{
				Error ( "Update - media event interface invalid" );
				return;
			}

			while ( SUCCEEDED ( ptr->pMediaEvent->GetEvent ( &evCode, &evParam1, &evParam2, 0 ) ) )
			{
				// run through events
				ptr->pMediaEvent->FreeEventParams ( evCode, evParam1, evParam2 );

				if ( evCode == EC_COMPLETE )
				{
					// loop if flagged
					if ( ptr->bLoop )
					{
						// reset to start
						LONGLONG pos = 0;
						if ( FAILED ( hr = ptr->pMediaSeeking->SetPositions ( &pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning ) ) )
						{
							ptr->pMediaControl->Stop ( );
							ptr->pMediaControl->Run  ( );
						}
					}
					else
					{
						// end play state
						ptr->bPlay = false;
					}
				}
			}
		}
		
		// move to next item in the list
		check = check->next;
	}
}

DARKSDK void SetErrorHandler ( LPVOID pErrorHandlerPtr )
{
	// Update error handler pointer
	g_pErrorHandler = (CRuntimeErrorHandler*)pErrorHandlerPtr;
}

DARKSDK void PassCoreData( LPVOID pGlobPtr )
{
	// Held in Core, used here..
	g_pGlob = (GlobStruct*)pGlobPtr;
}

DARKSDK void RefreshD3D ( int iMode )
{
}

DARKSDK void LoadMusicCore ( LPSTR szFilename, int iID )
{
	#ifdef GENLOGFILE
	char dir[256];
	getcwd(dir, 256);
	strcat(dir, "\\musiclog.txt");
	WritePrivateProfileString("LOG", "STATE2", "BeforeCheck", dir);
	#endif

	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICALREADYEXISTS);
		return;
	}

	HRESULT	hr;

	tagMusicData m_Data;
	memset ( &m_Data, 0, sizeof ( m_Data ) );
	#ifdef GENLOGFILE
	WritePrivateProfileString("LOG", "STATE2", "CreatedMusicData", dir);
	#endif

	// Creates a FilterGraph
	hr = CoCreateInstance(	CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
						IID_IGraphBuilder, (void **)&m_Data.pGraphBuilder);
	if (FAILED(hr))
	{
		RunTimeError(RUNTIMEERROR_MUSICLOADFAILED);
		return;
	}

	#ifdef GENLOGFILE
	WritePrivateProfileString("LOG", "STATE3", "CreatedCoCreateInstance", dir);
	#endif

	// Construct WideCharacter Filename
	DWORD dwLength = strlen(szFilename)+1;
	LPOLESTR pWideStr = (LPOLESTR)new WORD[dwLength];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFilename, -1, pWideStr, dwLength*2);

	// setup the graph
	if ( FAILED ( hr = m_Data.pGraphBuilder->RenderFile ( pWideStr, NULL ) ) ) 
	{
		SAFE_DELETE(pWideStr);
		SAFE_RELEASE ( m_Data.pGraphBuilder );
		RunTimeError(RUNTIMEERROR_MUSICLOADFAILED);
		return;
	}
	SAFE_DELETE(pWideStr);

	// query interface for DirectShow interfaces
	if ( FAILED ( hr = m_Data.pGraphBuilder->QueryInterface ( IID_IMediaControl, ( void** ) &m_Data.pMediaControl ) ) )
	{
		SAFE_RELEASE ( m_Data.pGraphBuilder );
		RunTimeError(RUNTIMEERROR_MUSICLOADFAILED);
		return;
	}

	#ifdef GENLOGFILE
	WritePrivateProfileString("LOG", "STATE4", "QueriedMediaControl", dir);
	#endif

	// get the media event interface
	if ( FAILED ( hr = m_Data.pGraphBuilder->QueryInterface ( IID_IMediaEventEx, ( void** ) &m_Data.pMediaEvent ) ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICLOADFAILED);
		return;
	}

	// get the media seeking interface
	if ( FAILED ( hr = m_Data.pGraphBuilder->QueryInterface ( IID_IMediaSeeking, ( void** ) &m_Data.pMediaSeeking ) ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICLOADFAILED);
		return;
	}

	// get the basic audio interface
	if ( FAILED ( hr = m_Data.pGraphBuilder->QueryInterface ( IID_IBasicAudio, ( void** ) &m_Data.pBasicAudio ) ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICLOADFAILED);
		return;
	}

	#ifdef GENLOGFILE
	WritePrivateProfileString("LOG", "STATE5", "IID_IBasicAudio", dir);
	#endif

	// get the MediaPosition interface
	if ( FAILED ( hr = m_Data.pGraphBuilder->QueryInterface ( IID_IMediaPosition, ( void** ) &m_Data.pMediaPosition ) ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICLOADFAILED);
		return;
	}

	#ifdef GENLOGFILE
	WritePrivateProfileString("LOG", "STATE6", "IID_IMediaPosition", dir);
	#endif

	// setup properties
	m_Data.eID		= MUSIC;
	m_Data.bPause	= false;
	m_Data.bPlay	= false;
	m_Data.bLoop	= false;
	m_Data.iState   = 1;

	m_Data.iVolume   = 100;
	m_Data.iSpeed   = 100;

	// add to the list
	m_MusicManager.Add ( &m_Data, iID );

	#ifdef GENLOGFILE
	WritePrivateProfileString("LOG", "STATE7", "AdddedMusicToList", dir);
	#endif
}

DARKSDK void LoadMusic ( LPSTR szFilename, int iID )
{
	#ifdef GENLOGFILE
	char dir[256];
	getcwd(dir, 256);
	strcat(dir, "\\musiclog.txt");
	WritePrivateProfileString("LOG", "STATE1", "VeryStart", dir);
	#endif

	// Uses actual or virtual file..
	char VirtualFilename[_MAX_PATH];
	strcpy(VirtualFilename, szFilename);
	g_pGlob->UpdateFilenameFromVirtualTable( (DWORD)VirtualFilename);

	// Decrypt and use media, re-encrypt
	g_pGlob->Decrypt( (DWORD)VirtualFilename );
	LoadMusicCore ( VirtualFilename, iID );
	g_pGlob->Encrypt( (DWORD)VirtualFilename );
}

DARKSDK void LoadCDMusic ( int iTrackNumber, int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICALREADYEXISTS);
		return;
	}
	if ( iTrackNumber < 1 || iTrackNumber > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICTRACKILLEGAL);
		return;
	}

	// load cd music

	// setup properties
	tagMusicData m_Data;
	memset ( &m_Data, 0, sizeof ( m_Data ) );

	m_Data.eID				= CD;
	m_Data.bPause			= false;
	m_Data.bPlay			= false;
	m_Data.bLoop			= false;
	m_Data.iCurrentTrack	= iTrackNumber;
	m_Data.iState			= 1;

	// open the audio and set the time format
	mciSendString ( "open cdaudio", NULL, NULL, NULL );
	mciSendString ( "set cdaudio time format tmsf", NULL, NULL, NULL );

	// add to the list
	m_MusicManager.Add ( &m_Data, iID );
}

DARKSDK void PlayMusic ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}

	// play previously loaded music

    // u74b7 - Duplicate call to UpdatePtr
	// get data
	//if ( !UpdatePtr ( iID ) )
	//	return;

	// check type
	if ( m_ptr->eID == CD )
	{
		char	szCommand [ 256 ];
		char	szReturn  [ 256 ];
		
		memset ( &szCommand, 0, sizeof ( szCommand ) );
		memset ( &szReturn,  0, sizeof ( szReturn ) );

		memset ( &szCommand, 0, sizeof ( szCommand ) );
		wsprintf ( szCommand, "status cdaudio number of tracks" );

		mciSendString ( szCommand, szReturn, sizeof ( szReturn ), NULL );

		int iMaxTrack = atoi ( szReturn );

		char	szNum1 [ 4 ];
		char	szNum2 [ 4 ];
		
		memset ( &szCommand, 0, sizeof ( szCommand ) );
		memset ( &szReturn,  0, sizeof ( szReturn ) );
		memset ( &szNum1,    0, sizeof ( szNum1 ) );
		memset ( &szNum2,    0, sizeof ( szNum2 ) );
		
		if ( m_ptr->iCurrentTrack < iMaxTrack )
		{
			_itoa ( m_ptr->iCurrentTrack, szNum1, 10 );
			_itoa ( m_ptr->iCurrentTrack + 1, szNum2, 10 );
			wsprintf ( szCommand, "play cdaudio from %s to %s", szNum1, szNum2 );
		}
		else
		{
			_itoa ( m_ptr->iCurrentTrack, szNum1, 10 );
			wsprintf ( szCommand, "play cdaudio from %s", szNum1 );
		}

		mciSendString ( szCommand, NULL, NULL, NULL );
	}
	else
	{
		if ( !m_ptr->pMediaControl )
		{
			Error ( "PlayMusic - invalid media control interface" );
			return;
		}

		m_ptr->pMediaControl->Run ( );
	}

	m_ptr->bPlay = true;
	m_ptr->bLoop = false;
}

DARKSDK int GetNumberOfCDTracks ( void )
{
	char	szCommand [ 256 ];
	char	szReturn  [ 256 ];
	
	memset ( &szCommand, 0, sizeof ( szCommand ) );
	memset ( &szReturn,  0, sizeof ( szReturn ) );

	memset ( &szCommand, 0, sizeof ( szCommand ) );
	wsprintf ( szCommand, "status cdaudio number of tracks" );

	mciSendString ( szCommand, szReturn, sizeof ( szReturn ), NULL );

	return atoi ( szReturn );
}

DARKSDK void LoopMusic ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}

	if ( m_ptr->eID == CD )
	{
		PlayMusic ( iID );
	}
	else
	{
		if ( !m_ptr->pMediaControl )
		{
			Error ( "PlayMusic - invalid media control interface" );
			return;
		}

		m_ptr->pMediaControl->Run ( );
	}

	m_ptr->bPlay = true;
	m_ptr->bLoop = true;
}

DARKSDK void StopMusic ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}

	m_ptr->bPlay = false;
	m_ptr->bLoop = false;

	if ( m_ptr->eID == CD )
	{
		mciSendString ( "stop cdaudio", NULL, NULL, NULL );
	}
	else
	{
		if ( !m_ptr->pMediaControl )
		{
			Error ( "StopMusic - invalid media control interface" );
			return;
		}
		if ( !m_ptr->pMediaSeeking )
		{
			Error ( "StopMusic - invalid media control interface" );
		}

		// reset to start
		__int64 pos = 0;
		m_ptr->pMediaSeeking->SetPositions ( &pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning );
		m_ptr->pMediaControl->Stop ( );
	}
}

DARKSDK void PauseMusic ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}

	m_ptr->bPlay = false;
	m_ptr->bPause = true;

	if ( m_ptr->eID == CD )
	{
		mciSendString ( "pause cdaudio", NULL, NULL, NULL );
	}
	else
	{
		if ( !m_ptr->pMediaControl )
		{
			Error ( "PauseMusic - invalid media control interface" );
			return;
		}

		m_ptr->pMediaControl->Pause ( );
	}
}

DARKSDK void ResumeMusic ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}

	m_ptr->bPlay = true;
	m_ptr->bPause = false;

	if ( m_ptr->eID == CD )
	{
		mciSendString ( "resume cdaudio", NULL, NULL, NULL );
	}
	else
	{
		if ( !m_ptr->pMediaControl )
		{
			Error ( "ResumeMusic - invalid media control interface" );
			return;
		}

		m_ptr->pMediaControl->Run ( );
	}
}

DARKSDK void DeleteMusic ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}

	m_MusicManager.Delete ( iID );
}

DARKSDK int GetMusicExist ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return 0;
	}
	if ( !UpdatePtr ( iID ) )
		return 0;
	
	return 1;
}

DARKSDK int GetMusicPlaying ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return 0;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return 0;
	}

	if ( m_ptr->bPlay )
		return 1;
	else
		return 0;
}

DARKSDK int GetMusicLooping ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return 0;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return 0;
	}

	if ( m_ptr->bLoop )
		return 1;
	else
		return 0;
}

DARKSDK int GetMusicPaused ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return 0;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return 0;
	}

	if ( m_ptr->bPause )
		return 1;
	else
		return 0;
}

DARKSDK int GetMusicVolume ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return 0;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return 0;
	}
	return m_ptr->iVolume;
}

DARKSDK int GetMusicSpeed ( int iID )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return 0;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return 0;
	}
	return m_ptr->iSpeed;
}

DARKSDK void SetMusicVolume ( int iID, int iVolume )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}

	// set volume value
	m_ptr->iVolume = iVolume;

	// if CD music, volume is through MCI
	if ( m_ptr->eID == CD )
	{
		// cannot adjust CD volume directly, must change master volume via mixer
		DWORD m_nNumMixers = ::mixerGetNumDevs();
		HMIXER m_hMixer = NULL;
		MIXERCAPS m_mxcaps;
		::ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));
		if (m_nNumMixers != 0)
		{
			if (::mixerOpen(&m_hMixer,
							0,
							reinterpret_cast<DWORD>(g_pGlob->hWnd),
							NULL,
							MIXER_OBJECTF_MIXER | CALLBACK_WINDOW)
				!= MMSYSERR_NOERROR)
				return;

			if (::mixerGetDevCaps(reinterpret_cast<UINT>(m_hMixer),
								  &m_mxcaps, sizeof(MIXERCAPS))
				!= MMSYSERR_NOERROR)
				return;
		}

		// get dwLineID
		DWORD m_dwVolumeControlID = 0;
		MIXERLINE mxl;
		mxl.cbStruct = sizeof(MIXERLINE);
		mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		if (::mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(m_hMixer),
							   &mxl,
							   MIXER_OBJECTF_HMIXER |
							   MIXER_GETLINEINFOF_COMPONENTTYPE)
			!= MMSYSERR_NOERROR)
			return;

		// get dwControlID
		MIXERCONTROL mxc;
		MIXERLINECONTROLS mxlc;
		mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = &mxc;
		if (::mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(m_hMixer),
								   &mxlc,
								   MIXER_OBJECTF_HMIXER |
								   MIXER_GETLINECONTROLSF_ONEBYTYPE)
			!= MMSYSERR_NOERROR)
			return;

		// store dwControlID
		DWORD m_dwMinimum = mxc.Bounds.dwMinimum;
		DWORD m_dwMaximum = mxc.Bounds.dwMaximum;
		m_dwVolumeControlID = mxc.dwControlID;

		// set volume
		DWORD dwVal = (DWORD)(((m_dwMaximum - m_dwMinimum)/100.0f) * iVolume);
		MIXERCONTROLDETAILS_UNSIGNED mxcdVolume = { dwVal };
		MIXERCONTROLDETAILS mxcd;
		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = m_dwVolumeControlID;
		mxcd.cChannels = 1;
		mxcd.cMultipleItems = 0;
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		mxcd.paDetails = &mxcdVolume;
		if (::mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
									 &mxcd,
									 MIXER_OBJECTF_HMIXER |
									 MIXER_SETCONTROLDETAILSF_VALUE)
			!= MMSYSERR_NOERROR)
				return;

		if (m_hMixer != NULL)
		{
			BOOL bSucc = (::mixerClose(m_hMixer) == MMSYSERR_NOERROR);
			m_hMixer = NULL;
		}
	}
	else
	{
		// basic audio interface required for music volume change
		if ( !m_ptr->pBasicAudio )
		{
			Error ( "SetMusicVolume - invalid basic audio interface" );
			return;
		}
		long lVolume = -2500 - (iVolume * -25);
		if(iVolume>0)
			m_ptr->pBasicAudio->put_Volume ( lVolume );
		else
			m_ptr->pBasicAudio->put_Volume ( -10000 );
	}
}

DARKSDK void SetMusicSpeed ( int iID, int iSpeed )
{
	if ( iID < 1 || iID > MAXIMUMVALUE )
	{
		RunTimeError(RUNTIMEERROR_MUSICNUMBERILLEGAL);
		return;
	}
	if ( !UpdatePtr ( iID ) )
	{
		RunTimeError(RUNTIMEERROR_MUSICNOTEXIST);
		return;
	}
	if ( iSpeed < 0 )
	{
		RunTimeError(RUNTIMEERROR_MUSICSPEEDILLEGAL);
		return;
	}

	if ( !m_ptr->pMediaPosition )
	{
		Error ( "SetMusicSpeed - invalid pMediaPosition interface" );
		return;
	}

	// lee - 310306 - u6rc4 - added pause and resume around speed change
	m_ptr->iSpeed = iSpeed;
	m_ptr->pMediaControl->Pause();
	m_ptr->pMediaSeeking->SetRate( (double)iSpeed/100.0 );
	m_ptr->pMediaPosition->put_Rate ( (double)iSpeed/100.0 );
	m_ptr->pMediaControl->Run();
}

//////////////////////////////////////////////////////////////////////////////////
// DARK SDK SECTION //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifdef DARKSDK_COMPILE

void ConstructorMusic ( HWND hWnd )
{
	Constructor ( hWnd );
}

void DestructorMusic ( void )
{
	Destructor ( );
}

void SetErrorHandlerMusic	( LPVOID pErrorHandlerPtr )
{
	SetErrorHandler (pErrorHandlerPtr  );
}

void PassCoreDataMusic ( LPVOID pGlobPtr )
{
	PassCoreData ( pGlobPtr );
}

void RefreshD3DMusic ( int iMode )
{
	RefreshD3D ( iMode );
}

void UpdateMusic ( void )
{
	Update ( );
}

void dbLoadMusic ( LPSTR szFilename, int iID )
{
	LoadMusic ( szFilename, iID );
}

void dbLoadCDMusic ( int iTrackNumber, int iID )
{
	LoadCDMusic ( iTrackNumber, iID );
}

void dbDeleteMusic ( int iID )
{
	DeleteMusic ( iID );
}

void dbPlayMusic ( int iID )
{
	PlayMusic ( iID );
}
 
void dbLoopMusic ( int iID )
{
	LoopMusic ( iID );
}
 
void dbStopMusic ( int iID )
{
	StopMusic ( iID );
}
 
void dbPauseMusic ( int iID )
{
	PauseMusic ( iID );
}
 
void dbResumeMusic ( int iID )
{
	ResumeMusic ( iID );
}

void dbSetMusicVolume ( int iID, int iVolume )
{
	SetMusicVolume ( iID, iVolume );
}
	
void dbSetMusicSpeed ( int iID, int iSpeed )
{
	SetMusicSpeed ( iID, iSpeed );
}

int dbMusicExist ( int iID )
{
	return GetMusicExist ( iID );
}
	
int dbMusicPlaying ( int iID )
{
	return GetMusicPlaying ( iID );
}
	
int dbMusicLooping ( int iID )
{
	return GetMusicLooping ( iID );
}
	
int dbMusicPaused ( int iID )
{
	return GetMusicPaused ( iID );
}
	
int dbMusicVolume ( int iID )
{
	return GetMusicVolume ( iID );
}

int dbMusicSpeed ( int iID )
{
	return GetMusicSpeed ( iID );
}

int dbGetNumberOfCDTracks ( void )
{
	return GetNumberOfCDTracks ( );
}

#endif

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////