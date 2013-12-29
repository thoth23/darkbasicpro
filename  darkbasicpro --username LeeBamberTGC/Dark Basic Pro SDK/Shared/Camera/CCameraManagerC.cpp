#include "CCameraManagerc.h"

CCameraManager::CCameraManager ( )
{
	
}

CCameraManager::~CCameraManager ( )
{
	link* pCheck = m_List.m_start;
	while(pCheck)
	{
		tagCameraData* ptr = NULL;
		ptr = this->GetData ( pCheck->id );
		if ( ptr == NULL ) continue;

		delete ptr;

		pCheck = pCheck->next;
	}
}

bool CCameraManager::Add ( tagCameraData* pData, int iID )
{
	///////////////////////////////////////
	// check if an object already exists //
	// with the same id, if it does then //
	// delete it                         //
	///////////////////////////////////////
	tagCameraData* ptr = NULL;
	ptr = ( tagCameraData* ) m_List.Get ( iID );
			
	if ( ptr != NULL )
		m_List.Delete ( iID );
	///////////////////////////////////////

	///////////////////////////////////////
	// create a new object and insert in //
	// the list                          //
	///////////////////////////////////////
	tagCameraData* test;
	test = new tagCameraData;

	memset ( test,     0, sizeof ( test          ) );
	memcpy ( test, pData, sizeof ( tagCameraData ) );

	m_List.Add ( iID, ( VOID* ) test, 0, 1 );
	///////////////////////////////////////

	return true;
}

bool CCameraManager::Delete ( int iID )
{
	m_List.Delete ( iID );

	return true;
}

void CCameraManager::Sort ( void )
{
	// leeadd - 140906 - u63 - added so cameras can be in index-order
	link* checka = m_List.m_start;
	for ( int a = 0; a < m_List.m_count; a++ )
	{
		tagCameraData* ptra = this->GetData ( checka->id );
		if ( ptra != NULL )
		{
			link* checkb = m_List.m_start;
			for ( int b = 0; b < m_List.m_count; b++ )
			{
				tagCameraData* ptrb = this->GetData ( checkb->id );
				if ( ptrb != NULL )
				{
					if ( a!=b )
					{
						if ( checka->id < checkb->id )
						{
							// swap 
							int iStore = checka->id;
							void* iStoreData = checka->data;
							checka->id = checkb->id;
							checka->data = checkb->data;
							checkb->id = iStore;
							checkb->data = iStoreData;
						}
					}
				}
				checkb = checkb->next;
			}
		}
		checka = checka->next;
	}
}

tagCameraData* CCameraManager::GetData ( int iID )
{		
	return ( tagCameraData* ) m_List.Get ( iID );
}

void CCameraManager::Update ( void )
{
	link* check = m_List.m_start;

	for ( int temp = 0; temp < m_List.m_count; temp++ )
	{
		tagCameraData* ptr = NULL;
		ptr = this->GetData ( check->id );

		if ( ptr == NULL )
			return;

		check = check->next;
	}
}

int CCameraManager::GetNextID ( int iIDHave )
{
	int iTemp;
	link* pCheck = m_List.m_start;
	for ( iTemp = 0; iTemp < m_List.m_count; iTemp++ )
	{
		tagCameraData* ptr = NULL;
		ptr = this->GetData ( pCheck->id );
		if ( ptr == NULL ) continue;
		if ( pCheck->id == iIDHave )
		{
			if(pCheck->next)
				return pCheck->next->id;
			else
				return -1;
		}
		pCheck = pCheck->next;
	}
	return -1;
}
