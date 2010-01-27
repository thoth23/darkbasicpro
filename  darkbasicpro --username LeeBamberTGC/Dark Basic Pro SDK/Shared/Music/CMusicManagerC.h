#ifndef _CMUSICMANAGER_H_
#define _CMUSICMANAGER_H_

#include ".\..\data\cdatac.h"
#include "CMusicDataC.h"

#define SAFE_DELETE( p )       { if ( p ) { delete ( p );       ( p ) = NULL; } }
#define SAFE_RELEASE( p )      { if ( p ) { ( p )->Release ( ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if ( p ) { delete [ ] ( p );   ( p ) = NULL; } }

class CMusicManager : private CData
{
	public:
		CData	m_List;	

	public:
		CMusicManager  ( );			// constructor
		~CMusicManager ( );			// destructor

		bool Add    ( tagMusicData* pData, int iID );
		bool Delete ( int iID );

		tagMusicData* GetData ( int iID );

		void Update ( void );

		void ShutDown ( void );
};

#endif _CMUSICMANAGER_H_