#include "cVectorManagerC.h"


cDataManager::cDataManager ( )
{
	
}

cDataManager::~cDataManager ( )
{
	int iTemp;

	link* pCheck = m_List.m_start;

	for ( iTemp = 0; iTemp < m_List.m_count; iTemp++ )
	{
		void* ptr = NULL;
		ptr = this->GetData ( pCheck->id );

		if ( ptr == NULL )
			continue;

		delete ptr;

		pCheck = pCheck->next;
	}
}

bool cDataManager::Add ( void* pData, int iID )
{
	///////////////////////////////////////
	// check if an object already exists //
	// with the same id, if it does then //
	// delete it                         //
	///////////////////////////////////////
	void* ptr = NULL;
	ptr = ( void* ) m_List.Get ( iID );
			
	if ( ptr != NULL )
		m_List.Delete ( iID );
	///////////////////////////////////////

	///////////////////////////////////////
	// create a new object and insert in //
	// the list                          //
	///////////////////////////////////////
	//void* test;
	//test = new void*;

	//memset ( test,     0, sizeof ( test  ) );
	//memcpy ( test, pData, sizeof ( sData ) );

	//m_List.Add ( iID, ( VOID* ) test, 0, 1 );
	m_List.Add ( iID, ( VOID* ) pData, 0, 1 );
	///////////////////////////////////////

	return true;
}

bool cDataManager::Delete ( int iID )
{
	void* ptr = NULL;
	ptr = ( void* ) m_List.Get ( iID );
	if(ptr) delete ptr;
	m_List.Delete ( iID );
	return true;
}

void* cDataManager::GetData ( int iID )
{		
	return ( void* ) m_List.Get ( iID );
}

void cDataManager::Update ( void )
{
	link* check = m_List.m_start;

	for ( int temp = 0; temp < m_List.m_count; temp++ )
	{
		void* ptr = NULL;
		ptr = this->GetData ( check->id );

		if ( ptr == NULL )
			return;

		check = check->next;
	}
}