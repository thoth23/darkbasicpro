#ifndef _CVECTORMANAGER_H_
#define _CVECTORMANAGER_H_

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

#include <map>

#define TYPE_VECTOR2	1
#define TYPE_VECTOR3	2
#define TYPE_VECTOR4	3
#define TYPE_MATRIX		4

class BaseVector
{
private:
    int Type;
public:
    BaseVector(int Type_) : Type( Type_ ) {}
    int GetType() const { return Type; }
};

class Vector2 : public BaseVector
{
private:
    D3DXVECTOR2 V;
public:
    Vector2() : BaseVector(TYPE_VECTOR2), V(0.0f, 0.0f) { }
    D3DXVECTOR2& Get() { return V; }
};

class Vector3 : public BaseVector
{
private:
    D3DXVECTOR3 V;
public:
    Vector3() : BaseVector(TYPE_VECTOR3), V(0.0f, 0.0f, 0.0f) { }
    D3DXVECTOR3& Get() { return V; }
};

class Vector4 : public BaseVector
{
private:
    D3DXVECTOR4 V;
public:
    Vector4() : BaseVector(TYPE_VECTOR4), V(0.0f, 0.0f, 0.0f, 0.0f) { }
    D3DXVECTOR4& Get() { return V; }
};

class Matrix : public BaseVector
{
private:
    D3DXMATRIX V;
public:
    Matrix() : BaseVector(TYPE_MATRIX), V()
    {
        memset( &V, 0, sizeof(V) );
    }
    D3DXMATRIX& Get() { return V; }
};

class cDataManager
{
	private:
        typedef std::map<int, BaseVector*>	List_t;
        typedef List_t::iterator			ListPtr;
        typedef std::pair<ListPtr, bool>    ListInsertStatus;

        List_t	m_List;

	public:
		cDataManager  		( );
		~cDataManager 		( );
		void ClearAll		( );

		bool Add    		( BaseVector* pData, int iID );
		bool Delete 		( int iID );

		BaseVector* GetData ( int iID );
};

#endif _CVECTORMANAGER_H_