
#include "DarkSDK.h"

void DarkSDK ( void )
{
	dbSyncOn   ( );
	dbSyncRate ( 0 );

	while ( LoopSDK ( ) )
	{
		if ( dbEscapeKey ( ) )
			return;

		dbSync ( );
	}
}
