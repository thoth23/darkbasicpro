#ifndef _CMUSICDATA_H_
#define _CMUSICDATA_H_

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#pragma warning(disable : 4995)
#pragma warning(disable : 4996)

#include <direct.h>
#include <dshow.h>

enum enumType
{
	MUSIC,
	CD,
	MIDI,
};

struct tagMusicData
{
	enumType					eID;				// what type of music is it

	// direct show interfaces ( not used for cd audio )
	IGraphBuilder*				pGraphBuilder;
	IMediaControl*				pMediaControl;
	IMediaEventEx*				pMediaEvent;
	IBasicAudio*				pBasicAudio;
	IMediaSeeking*				pMediaSeeking;
	IMediaPosition*				pMediaPosition;

	// cd properties
	int							iCurrentTrack;		// current track number

	// shared properties
	int                         iState;				// state of music
	bool						bPause;				// should music pause?
	bool						bPlay;				// is music playing?
	bool						bLoop;				// should music loop?

	int							iVolume;
	int							iSpeed;
};

#endif _CMUSICDATA_H_