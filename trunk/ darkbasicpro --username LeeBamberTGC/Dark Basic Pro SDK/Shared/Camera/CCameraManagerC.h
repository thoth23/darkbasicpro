#ifndef _CCAMERAMANAGER_H_
#define _CCAMERAMANAGER_H_

#include ".\..\data\cdatac.h"
#include "CCameraDatac.h"

class CCameraManager : private CData
{
	public:
		CData	m_List;	

	public:
		CCameraManager  ( );
		~CCameraManager ( );

		bool Add    ( tagCameraData* pData, int iID );
		bool Delete ( int iID );
		void Sort	( void );

		tagCameraData* GetData ( int iID );

		void Update ( void );

		int GetNextID ( int iIDHave );
};

#endif _CCAMERAMANAGER_H_