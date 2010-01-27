// include definition
#include "cmusicmanagerc.h"

CMusicManager::CMusicManager ( )
{
	
}

CMusicManager::~CMusicManager ( )
{
	
}

bool CMusicManager::Add ( tagMusicData* pData, int iID )
{
		// check if an object already exists //
	// with the same id, if it does then //
	// delete it                         //
	///////////////////////////////////////
	tagMusicData* ptr = NULL;
	ptr = ( tagMusicData* ) m_List.Get ( iID );
			
	if ( ptr != NULL )
		m_List.Delete ( iID );
	///////////////////////////////////////

	///////////////////////////////////////
	// create a new object and insert in //
	// the list                          //
	///////////////////////////////////////
	tagMusicData* test;
	test = new tagMusicData;

	memset ( test,     0, sizeof ( test         ) );
	memcpy ( test, pData, sizeof ( tagMusicData ) );

	m_List.Add ( iID, ( VOID* ) test, 0, 1 );
	///////////////////////////////////////


	return true;
}

bool CMusicManager::Delete ( int iID )
{
	tagMusicData* ptr = NULL;
	ptr = this->GetData ( iID );

	SAFE_RELEASE ( ptr->pMediaPosition );
	SAFE_RELEASE ( ptr->pBasicAudio );
	SAFE_RELEASE ( ptr->pMediaSeeking );
	SAFE_RELEASE ( ptr->pMediaEvent );
	SAFE_RELEASE ( ptr->pMediaControl );
	SAFE_RELEASE ( ptr->pGraphBuilder );

	delete ptr;

	m_List.Delete ( iID );

	return true;
}

tagMusicData* CMusicManager::GetData ( int iID )
{		
	return ( tagMusicData* ) m_List.Get ( iID );
}

void CMusicManager::ShutDown ( void )
{
	link* check = m_List.m_start;

	for ( int temp = 0; temp < m_List.m_count; temp++ )
	{
		tagMusicData* ptr = NULL;
		ptr = this->GetData ( check->id );

		if ( ptr == NULL )
			return;

		SAFE_RELEASE ( ptr->pMediaPosition );
		SAFE_RELEASE ( ptr->pBasicAudio );
		SAFE_RELEASE ( ptr->pMediaSeeking );
		SAFE_RELEASE ( ptr->pMediaEvent );
		SAFE_RELEASE ( ptr->pMediaControl );
		SAFE_RELEASE ( ptr->pGraphBuilder );

		delete ptr;

		check = check->next;
	}
}

void CMusicManager::Update ( void )
{
	link* check = m_List.m_start;

	for ( int temp = 0; temp < m_List.m_count; temp++ )
	{
		tagMusicData* ptr = NULL;
		ptr = this->GetData ( check->id );

		if ( ptr == NULL )
			return;

		
		check = check->next;
	}
}