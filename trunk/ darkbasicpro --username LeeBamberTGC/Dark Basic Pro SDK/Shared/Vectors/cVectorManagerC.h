#ifndef _CVECTORMANAGER_H_
#define _CVECTORMANAGER_H_

#include ".\..\data\cdatac.h"
#include "cVectorDataC.h"

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "d3d9.lib"   )
#pragma comment ( lib, "d3dx9.lib"  )
#pragma comment ( lib, "dxerr9.lib" )

#include <d3d9.h>
#include <Dxerr9.h>
#include <D3dx9tex.h>
#include <D3dx9core.h>
#include <basetsd.h>
#include <stdio.h>
#include <math.h>
#include <D3DX9.h>
#include <d3d9types.h>

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>   
#include <windowsx.h> 

class cDataManager
{
	private:
		CData	m_List;

	public:
		cDataManager  ( );
		~cDataManager ( );

		bool Add    ( void* pData, int iID );
		bool Delete ( int iID );

		void* GetData ( int iID );

		void Update ( void );
};

#endif _CVECTORMANAGER_H_