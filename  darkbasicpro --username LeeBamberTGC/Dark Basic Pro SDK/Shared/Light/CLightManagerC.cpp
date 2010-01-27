#include "CLightManagerC.h"

CLightManager::CLightManager ( void )
{
	
}

CLightManager::~CLightManager ( void )
{
}

bool CLightManager::Add ( tagLightData* pData, int iID )
{
	///////////////////////////////////////
	// check if an object already exists //
	// with the same id, if it does then //
	// delete it                         //
	///////////////////////////////////////
	tagLightData* ptr = NULL;
	ptr = ( tagLightData* ) m_List.Get ( iID );
			
	if ( ptr != NULL )
		m_List.Delete ( iID );
	///////////////////////////////////////

	///////////////////////////////////////
	// create a new object and insert in //
	// the list                          //
	///////////////////////////////////////
	tagLightData* test;
	test = new tagLightData;

	memset ( test,     0, sizeof ( test          ) );
	memcpy ( test, pData, sizeof ( tagLightData ) );

	m_List.Add ( iID, ( VOID* ) test, 0, 1 );
	///////////////////////////////////////

	return false;
}

bool CLightManager::Delete ( int iID )
{
	tagLightData* ptr = NULL;
	ptr = ( tagLightData* ) m_List.Get ( iID );
	if ( ptr != NULL ) delete ptr;
	m_List.Delete ( iID );
	return false;
}


 
tagLightData* CLightManager::GetData ( int iID )
{
	// get a pointer to the sprite info and update data
	return ( tagLightData* ) m_List.Get ( iID );
}
